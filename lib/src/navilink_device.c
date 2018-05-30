
#include "navilink_device.h"
#include "navilink_endianness.h"
#include "navilink_errors.h"
#include "navilink_internal.h"
#include "navilink_packet.h"
#include <assert.h>

#define __DEBUG_CMD 0
/// INTERNAL
void DumpHex(const void* data, size_t size)
{
  char ascii[17];
  size_t i, j;
  ascii[16] = '\0';
  for (i = 0; i < size; ++i) {
    printf("%02X ", ((unsigned char*)data)[i]);
    if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
      ascii[i % 16] = ((unsigned char*)data)[i];
    }
    else {
      ascii[i % 16] = '.';
    }
    if ((i + 1) % 8 == 0 || i + 1 == size) {
      printf(" ");
      if ((i + 1) % 16 == 0) {
        printf("|  %s \n", ascii);
      }
      else if (i + 1 == size) {
        ascii[(i + 1) % 16] = '\0';
        if ((i + 1) % 16 <= 8) {
          printf(" ");
        }
        for (j = (i + 1) % 16; j < 16; ++j) {
          printf("   ");
        }
        printf("|  %s \n", ascii);
      }
    }
  }
}

int navilink_write_device(NavilinkDevice* device, uint8_t packet_type, const uint8_t* payload, size_t payload_length);
int navilink_read_device(NavilinkDevice* device, NavilinkPacket* packet);
int navilink_query_device(NavilinkDevice* device, uint8_t packet_type, const uint8_t* payload, size_t payload_length, NavilinkPacket* response);

int navilink_check_protocol(NavilinkDevice* device)
{
  NavilinkPacket packet;
  int result = navilink_query_device(device, NAVILINK_PID_SYNC, NULL, 0, &packet);
  if (result < 0) {
    return result;
  }

  if (packet.type != NAVILINK_PID_ACK) {
    return -1;
  }
  return 0;
}

int navilink_open_sp_port(struct sp_port* port, NavilinkDevice* device)
{
  enum sp_return result = sp_open(port, SP_MODE_READ_WRITE);
  if (result != SP_OK) {
    set_current_error(device, NAVILINK_ERROR_OPEN_DEVICE_ERROR);
    goto error_cleanup_port;
  }

  struct sp_port_config* config = NULL;
  result = sp_new_config(&config);
  if (result != SP_OK) {
    goto error_clean_config;
  }

  // Set the config
  sp_set_baudrate(port, 115200);
  sp_set_bits(port, 8);
  sp_set_parity(port, SP_PARITY_NONE);
  sp_set_stopbits(port, 1);
  sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE);

  sp_set_config(port, config);

  // Allocate events buffer
  struct sp_event_set* event_set = NULL;
  sp_new_event_set(&event_set);
  sp_add_port_events(event_set, port, SP_EVENT_TX_READY);

  //Wait for the port to be ready
  result = sp_wait(event_set, 5000);
  if (result != SP_OK) {
    set_current_error(device, NAVILINK_ERROR_OPEN_DEVICE_ERROR);
    goto error_clean_event_set;
  }

  device->serial_port = port;
  device->event_set = event_set;

  //Check that this is a Navilink device
  result = navilink_check_protocol(device);
  if (result < 0) {
    goto error_clean_event_set;
  }

  // Retrieve the informations about the device
  result = navilink_query_information(device, &device->informations);
  if (result < 0) {
    goto error_clean_event_set;
  }
  // Retrieve the firmware version
  result = navilink_query_firmware_version(device, &device->firmware_version);

  if (result < 0) {
    goto error_clean_event_set;
  }
  return 0;
error_clean_event_set:
  sp_free_event_set(event_set);
error_clean_config:
  sp_free_config(config);
error_cleanup_port:
  sp_free_port(port);
  return -1;
}
/// PUBLIC

int navilink_open_device(NavilinkSerialPort* serial_port, NavilinkDevice* device)
{
  assert(serial_port != NULL);
  assert(device != NULL);

  return navilink_open_sp_port(serial_port->serial_port, device);
}

int navilink_open_device_from_name(const char* serial_port_name, NavilinkDevice* device)
{
  struct sp_port* port = NULL;
  enum sp_return result = sp_get_port_by_name(serial_port_name, &port);
  if (result < 0) {
    return result;
  }

  return navilink_open_sp_port(port, device);
}

int navilink_close_device(NavilinkDevice* device)
{
  return sp_close(device->serial_port);
}

int navilink_get_serial_list(NavilinkSerialPort*** list)
{
  struct sp_port** sp_list = NULL;
  sp_list_ports(&sp_list);

  size_t realloc_size = 5;
  NavilinkSerialPort** local_list = (NavilinkSerialPort**)malloc(realloc_size * sizeof(NavilinkSerialPort*));
  NavilinkSerialPort* port = NULL;
  int i = 0;
  struct sp_port* iter = sp_list[i];
  while (iter) {

    if (i > realloc_size - 1) {
      realloc_size += realloc_size;
      list = realloc(list, realloc_size);
    }

    port = (NavilinkSerialPort*)malloc(sizeof(NavilinkSerialPort));

    sp_copy_port(iter, &port->serial_port);
    strcpy(&port->name[0], sp_get_port_name(iter));

    local_list[i] = port;
    iter = sp_list[++i];
  }

  sp_free_port_list(sp_list);
  if (i > realloc_size - 1) {
    realloc_size += 1;
    list = realloc(list, realloc_size);
  }

  local_list[i] = NULL;
  *list = local_list;

  return 0;
}

int navilink_free_serial_list(NavilinkSerialPort** list)
{
  NavilinkSerialPort** iter = &list[0];
  while (*iter) {
    sp_free_port((*iter)->serial_port);
    iter++;
  }
  free(list);
  return 0;
}

int navilink_write_device(NavilinkDevice* device, uint8_t packet_type, const uint8_t* payload, size_t payload_length)
{
  assert(device != NULL);
  uint8_t* buff = (void*)(&device->buffer[0]);
  memset(buff, 0, NAVILINK_MAX_PACKET_SIZE);

  int bytes = navilink_create_packet(buff, packet_type, payload, payload_length);
  if (bytes < 0) {
    return -1;
  }

#if (__DEBUG_CMD)
  printf("WRITE:");
  DumpHex(buff, bytes);
#endif
  enum sp_return result = sp_blocking_write(device->serial_port, &device->buffer[0], bytes, 1000);
  if (result < 0) {
    return result;
  }

  return sp_drain(device->serial_port);
}

int navilink_read_device(NavilinkDevice* device, NavilinkPacket* packet)
{
  assert(device != NULL);
  uint8_t* buff = (void*)(&device->buffer[0]);
  memset(buff, 0, NAVILINK_MAX_PACKET_SIZE);

  enum sp_return result = sp_blocking_read(device->serial_port, buff, 4, 1000);
  if (result < 0) {
    return result;
  }

  if (buff[0] != NAVILINK_PACK_START1 && buff[1] != NAVILINK_PACK_START2) {
    return -1;
  }

  uint16_t packet_length = 0;
  memcpy(&packet_length, &buff[2], 2);
  result = sp_blocking_read(device->serial_port, &buff[4], packet_length + 4, 1000);
  if (result < 0) {
    return result;
  }
#if (__DEBUG_CMD)
  printf("READ:");
  DumpHex(buff, packet_length + 8);
#endif

  return navilink_read_packet(packet, device->buffer);
}

int navilink_query_device(NavilinkDevice* device, uint8_t packet_type, const uint8_t* payload, size_t payload_length, NavilinkPacket* response)
{
  assert(device != NULL);

  int result = navilink_write_device(device, packet_type, payload, payload_length);

  if (result < 0) {
    return result;
  }
  return navilink_read_device(device, response);
}

int navilink_query_device_expect_result(NavilinkDevice* device, uint8_t packet_type, const uint8_t* payload, size_t payload_length, uint8_t type)
{
  int result = navilink_query_device(device, packet_type, payload, payload_length, &device->response_packet);
  if (result < 0) {
    return result;
  }

  if (device->response_packet.type != type) {
    return -1;
  }
  return 0;
}

int navilink_query_information(NavilinkDevice* device, NavilinkInformation* information)
{
  int result = navilink_query_device_expect_result(device, NAVILINK_PID_QRY_INFORMATION, NULL, 0, NAVILINK_PID_DATA);
  if (result < 0) {
    return result;
  }

  return navilink_read_informations(information, &device->response_packet.payload[0], device->response_packet.payload_length);
}

int navilink_query_firmware_version(NavilinkDevice* device, int* firmware_version)
{
  int result = navilink_query_device_expect_result(device, NAVILINK_PID_QRY_FW_VERSION, NULL, 0, NAVILINK_PID_DATA);
  if (result < 0) {
    return result;
  }

  if (device->response_packet.payload_length < 1) {
    return -1;
  }

  uint8_t value = 0;
  memcpy(&value, &device->response_packet.payload[0], 1);
  *firmware_version = value;
  return 0;
}

int navilink_query_waypoint(NavilinkDevice* device, int waypoint_index, int query_length, NavilinkWaypoint* waypoint)
{
  if (query_length > NAVILINK_MAX_WAYPOINT_QUERY_LENGTH) {
    printf("query_length can not be greater than %i \n", NAVILINK_MAX_WAYPOINT_QUERY_LENGTH);
    return -1;
  }

  uint8_t payload[7] = { 0 };
  uint64_t index = _le64_to_host(waypoint_index);
  memcpy(&payload[0], &index, 4);
  query_length = _le16_to_host(query_length);
  memcpy(&payload[4], &query_length, 2);
  payload[6] = 0x01;

  int result = navilink_query_device(device, NAVILINK_PID_QRY_WAYPOINTS, payload, 7, &device->response_packet);
  if (result < 0) {
    return result;
  }

  if (device->response_packet.type == NAVILINK_PID_DATA) {
    for (unsigned int i = 0; i < query_length; i++) {

      // Copy buffer to waypoint
   
      uint8_t *buff = &device->response_packet.payload[0] + i*NAVILINK_WAYPOINT_PAYLOAD_LENGTH;
      navilink_read_waypoint(&waypoint[i], buff, NAVILINK_WAYPOINT_PAYLOAD_LENGTH);
    }
    return 0;
  } else if (device->response_packet.type == NAVILINK_PID_NAK) {
    waypoint = NULL;
    return 0;
  }
  return -1;
}

int navilink_delete_waypoint(NavilinkDevice* device, int waypoint_id)
{
  uint8_t payload[4] = { 0 };
  uint16_t id = waypoint_id;
  memcpy(&payload[2], &id, 2);

  int result = navilink_query_device(device, NAVILINK_PID_DEL_WAYPOINT, payload, 4, &device->response_packet);
  if (result < 0) {
    return result;
  }

  if (device->response_packet.type == NAVILINK_PID_ACK) {
    return 0;
  }

  return -1;
}

int navilink_query_route(NavilinkDevice* device, int route_index, NavilinkRoute* route)
{
  if (route_index > NAVILINK_MAX_ROUTE_QUERY_LENGTH) {
    printf("query_length can not be greater than %i \n", NAVILINK_MAX_ROUTE_QUERY_LENGTH);
    return -1;
  }

  uint8_t payload[7] = { 0 };
  uint64_t route_ = _le64_to_host(route_index);
  memcpy(&payload[0], &route_, 4);
  payload[6] = 0x01;

  int result = navilink_query_device(device, NAVILINK_PID_QRY_ROUTE, payload, 8, &device->response_packet);
  if (result < 0) {
    return result;
  }

  if (device->response_packet.type == NAVILINK_PID_DATA) {
    memcpy(route, &device->response_packet.payload[0], device->response_packet.payload_length);
    return 0;
  }
  else if (device->response_packet.type == NAVILINK_PID_NAK) {
    route = NULL;
    return 0;
  }
  return -1;
}