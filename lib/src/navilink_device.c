
#include "navilink_device.h"
#include "navilink_endianness.h"
#include "navilink_errors.h"
#include "navilink_internal.h"
#include "navilink_packet.h"
#include <assert.h>

#define CATCH_LIBSERIAL_ERROR(device)                                                \
  do {                                                                               \
    char* message = sp_last_error_message();                                         \
    navilink_set_current_error(device, NAVILINK_ERROR_LIBSERIALPORT_ERROR, message); \
    sp_free_error_message(message);                                                  \
  } while (0);

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

NavilinkResult navilink_write_device(NavilinkDevice* device, uint8_t packet_type, const uint8_t* payload, size_t payload_length);
NavilinkResult navilink_read_device(NavilinkDevice* device, NavilinkPacket* packet);
NavilinkResult navilink_query_device(NavilinkDevice* device, uint8_t packet_type, const uint8_t* payload, size_t payload_length, NavilinkPacket* response);

NavilinkResult navilink_check_protocol(NavilinkDevice* device)
{
  NavilinkPacket packet;
  int result = navilink_query_device(device, NAVILINK_PID_SYNC, NULL, 0, &packet);
  if (result < 0) {
    return NavilinkErr;
  }

  if (packet.type != NAVILINK_PID_ACK) {
    return NavilinkErr;
  }
  return NavilinkOK;
}

int navilink_open_sp_port(struct sp_port* port, NavilinkDevice* device)
{
  enum sp_return result = sp_open(port, SP_MODE_READ_WRITE);
  if (result != SP_OK) {

    CATCH_LIBSERIAL_ERROR(device);
    goto error_cleanup_port;
  }

  struct sp_port_config* config = NULL;
  result = sp_new_config(&config);
  if (result != SP_OK) {
    CATCH_LIBSERIAL_ERROR(device);
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

    CATCH_LIBSERIAL_ERROR(device);

    goto error_clean_event_set;
  }

  device->serial_port = port;
  device->event_set = event_set;

  //Check that this is a Navilink device
  NavilinkResult res = navilink_check_protocol(device);
  if (res < 0) {
    goto error_clean_event_set;
  }

  // Retrieve the informations about the device
  res = navilink_query_information(device, &device->informations);
  if (res < 0) {
    goto error_clean_event_set;
  }
  // Retrieve the firmware version
  res = navilink_query_firmware_version(device, &device->firmware_version);

  if (res < 0) {
    goto error_clean_event_set;
  }
  return NavilinkOK;
error_clean_event_set:
  sp_free_event_set(event_set);
error_clean_config:
  sp_free_config(config);
error_cleanup_port:
  sp_free_port(port);
  return NavilinkErr;
}

/// PUBLIC
NavilinkResult navilink_open_device(NavilinkSerialPort* serial_port, NavilinkDevice* device)
{
  assert(serial_port != NULL);
  assert(device != NULL);

  int result = navilink_open_sp_port(serial_port->serial_port, device);
  if (result < 0) {
    CATCH_LIBSERIAL_ERROR(device);
    return NavilinkErr;
  }
  return NavilinkOK;
}

NavilinkResult navilink_open_device_from_name(const char* serial_port_name, NavilinkDevice* device)
{
  assert(serial_port_name != NULL);
  assert(serial_port_name != NULL);

  struct sp_port* port = NULL;
  enum sp_return result = sp_get_port_by_name(serial_port_name, &port);
  if (result < 0) {
    goto serial_error;
  }

  result = navilink_open_sp_port(port, device);
  if (result < 0) {
    goto serial_error;
  }
  return NavilinkOK;
serial_error:
  CATCH_LIBSERIAL_ERROR(device);
  return NavilinkErr;
}

NavilinkResult navilink_close_device(NavilinkDevice* device)
{
  int result = sp_close(device->serial_port);
  if (result < 0) {
    CATCH_LIBSERIAL_ERROR(device);
    return NavilinkErr;
  }
  return NavilinkErr;
}

NavilinkResult navilink_get_serial_list(NavilinkSerialPort*** list)
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

    int result = sp_copy_port(iter, &port->serial_port);
    if (result < 0) {
      goto last_error;
    }
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

last_error:
  return NavilinkErr;
}

NavilinkResult navilink_free_serial_list(NavilinkSerialPort** list)
{
  NavilinkSerialPort** iter = &list[0];
  while (*iter) {
    sp_free_port((*iter)->serial_port);
    iter++;
  }
  free(list);
  return 0;
}

NavilinkResult navilink_write_device(NavilinkDevice* device, uint8_t packet_type, const uint8_t* payload, size_t payload_length)
{
  assert(device != NULL);
  uint8_t* buff = (void*)(&device->buffer[0]);
  memset(buff, 0, NAVILINK_MAX_PACKET_SIZE);

  int bytes = navilink_create_packet(buff, packet_type, payload, payload_length);
  if (bytes < 0) {
    navilink_set_current_error(device, bytes, NULL);
    return NavilinkErr;
  }

#if (__DEBUG_CMD)
  printf("WRITE:");
  DumpHex(buff, bytes);
#endif
  enum sp_return result = sp_blocking_write(device->serial_port, &device->buffer[0], bytes, 1000);
  if (result < 0) {
    goto manage_serial_error;
  }

  result = sp_drain(device->serial_port);
  if (result < 0) {
    goto manage_serial_error;
  }
  return NavilinkOK;

manage_serial_error:
  CATCH_LIBSERIAL_ERROR(device);
  return NavilinkErr;
}

NavilinkResult navilink_read_device(NavilinkDevice* device, NavilinkPacket* packet)
{
  assert(device != NULL);
  uint8_t* buff = (void*)(&device->buffer[0]);
  memset(buff, 0, NAVILINK_MAX_PACKET_SIZE);

  enum sp_return result = sp_blocking_read(device->serial_port, buff, 4, 1000);
  if (result < 0) {
    goto manage_serial_error;
  }

  if (buff[0] != NAVILINK_PACK_START1 && buff[1] != NAVILINK_PACK_START2) {
    navilink_set_current_error(device, NAVILINK_ERROR_INVALID_START_BYTE, NULL);
    return NavilinkErr;
  }

  uint16_t packet_length = 0;
  memcpy(&packet_length, &buff[2], 2);
  packet_length = _le16_to_host(packet_length);

  result = sp_blocking_read(device->serial_port, &buff[4], packet_length + 4, 1000);
  if (result < 0) {
    goto manage_serial_error;
  }
#if (__DEBUG_CMD)
  printf("READ:");
  DumpHex(buff, packet_length + 8);
#endif

  return navilink_read_packet(packet, device->buffer);

manage_serial_error:
  CATCH_LIBSERIAL_ERROR(device);
  return NavilinkErr;
}

NavilinkResult navilink_query_device(NavilinkDevice* device, uint8_t packet_type, const uint8_t* payload, size_t payload_length, NavilinkPacket* response)
{
  assert(device != NULL);

  NavilinkResult result = navilink_write_device(device, packet_type, payload, payload_length);
  if (result < 0) {
    return result;
  }
  return navilink_read_device(device, response);
}

NavilinkResult navilink_query_device_expect_result(NavilinkDevice* device, uint8_t packet_type, const uint8_t* payload, size_t payload_length, uint8_t type)
{
  int result = navilink_query_device(device, packet_type, payload, payload_length, &device->response_packet);
  if (result < 0) {
    return result;
  }

  if (device->response_packet.type != type) {
    char message[250];
    sprintf(message, "Unexpected Packet Type: %i \n", device->response_packet.type);
    navilink_set_current_error(device, NAVILINK_UNEXPECTED_PACKET_TYPE, message);
    return NavilinkErr;
  }
  return NavilinkOK;
}

NavilinkResult navilink_query_information(NavilinkDevice* device, NavilinkInformation* information)
{
  int result = navilink_query_device_expect_result(device, NAVILINK_PID_QRY_INFORMATION, NULL, 0, NAVILINK_PID_DATA);
  if (result < 0) {
    return result;
  }

  return navilink_read_informations(information, &device->response_packet.payload[0], device->response_packet.payload_length);
}

NavilinkResult navilink_query_firmware_version(NavilinkDevice* device, int* firmware_version)
{
  NavilinkResult result = navilink_query_device_expect_result(device, NAVILINK_PID_QRY_FW_VERSION, NULL, 0, NAVILINK_PID_DATA);
  if (result < 0) {
    return result;
  }

  if (device->response_packet.payload_length < 1) {
    navilink_set_current_error(device, NAVILINK_ERROR_PAYLOAD, NULL);
    return NavilinkErr;
  }

  uint8_t value = 0;
  memcpy(&value, &device->response_packet.payload[0], 1);
  *firmware_version = value;
  return NavilinkOK;
}

NavilinkResult navilink_query_waypoint(NavilinkDevice* device, int waypoint_index, int query_length, NavilinkWaypoint* waypoint)
{
  int num_wpt = 0;
  if (query_length > NAVILINK_MAX_WAYPOINT_QUERY_LENGTH) {
    navilink_set_current_error(device, NAVILINK_ERROR_QUERY_LENGTH_TOO_BIG, NULL);
    return NavilinkErr;
  }

  uint8_t payload[7] = { 0 };
  uint64_t index = _le64_to_host(waypoint_index);
  memcpy(&payload[0], &index, 4);
  query_length = _le16_to_host(query_length);
  memcpy(&payload[4], &query_length, 2);
  payload[6] = 0x01;

  NavilinkResult result = navilink_query_device(device, NAVILINK_PID_QRY_WAYPOINTS, payload, 7, &device->response_packet);
  if (result < 0) {
    return result;
  }

  if (device->response_packet.type == NAVILINK_PID_DATA) {

    num_wpt = (device->response_packet.payload_length - 1) / query_length;
    for (unsigned int i = 0; i < num_wpt; i++) {

      // Copy buffer to waypoint
      uint8_t* buff = &device->response_packet.payload[i * NAVILINK_WAYPOINT_PAYLOAD_LENGTH];
      navilink_read_waypoint(&waypoint[i], buff, NAVILINK_WAYPOINT_PAYLOAD_LENGTH);
    }
  }
  else if (device->response_packet.type == NAVILINK_PID_NAK) {
    waypoint = NULL;
    navilink_set_current_error(device, NAVILINK_DEVICE_UNACKNOWLEDGE, NULL);
    return NavilinkErr;
  }
  return num_wpt;
}

NavilinkResult navilink_delete_waypoint(NavilinkDevice* device, int waypoint_id)
{
  uint8_t payload[4] = { 0 };
  uint16_t id = waypoint_id;
  memcpy(&payload[2], &id, 2);

  int result = navilink_query_device(device, NAVILINK_PID_DEL_WAYPOINT, payload, 4, &device->response_packet);
  if (result < 0) {
    return result;
  }

  if (device->response_packet.type == NAVILINK_PID_ACK) {
    return NavilinkOK;
  }
  navilink_set_current_error(device, NAVILINK_DEVICE_UNACKNOWLEDGE, NULL);
  return NavilinkErr;
}

int navilink_query_route(NavilinkDevice* device, int route_index, NavilinkRoute* route)
{
  if (route_index > NAVILINK_MAX_ROUTE_QUERY_LENGTH) {
    navilink_set_current_error(device, NAVILINK_ERROR_QUERY_LENGTH_TOO_BIG, NULL);
    return NavilinkErr;
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
    return NavilinkOK;
  }
  else if (device->response_packet.type == NAVILINK_PID_NAK) {
    route = NULL;
    return NavilinkOK;
  }
  return NavilinkErr;
}