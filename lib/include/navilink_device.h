#ifndef __NAVILINK_DEVICE
#define __NAVILINK_DEVICE
#include "navilink_packet.h"
#include "navilink_protocol.h"

#include <libserialport.h>
#include <stdlib.h>

/**
 * @brief A serialport to connect to.
 * 
 */
typedef struct {
  struct sp_port* serial_port;
  char name[100];
} NavilinkSerialPort;

/**
 * @brief A Navilink Device
 * 
 */
typedef struct {
  struct sp_port* serial_port;
  struct sp_event_set* event_set;
  uint8_t buffer[NAVILINK_MAX_PACKET_SIZE];
  NavilinkInformation informations;
  int firmware_version;
  NavilinkPacket response_packet;
  int last_error_code;
  char last_message_str[255];
} NavilinkDevice;

/**
 * @brief Returns the list of detected serial port
 * 
 * @param list A pointer to retrieve the list. If an error occurs, the pointer will be set to NULL.
 * @return int A negative response is send in case of error.
 */
int navilink_get_serial_list(NavilinkSerialPort*** list);
/**
 * @brief Deallocates a list of `NavilinkSerialPort`
 * 
 * @param list The pointer to the list to deallocate
 * @return int A negative response is send in case of error.
 */
int navilink_free_serial_list(NavilinkSerialPort** list);

/**
 * @brief Open a device from a NavilinkSerialPort returned by `navilink_get_serial_list`.
 * 
 * @param serial_port The serial port to open. Can not be NULL.
 * @param device The device to inialize. Can not be NULL;
 * @return int A negative response is send in case of error.
 */
int navilink_open_device(NavilinkSerialPort* serial_port, NavilinkDevice* device);
/**
 * @brief Open a connection with a device from the serial port name.
 * 
 * @param serial_port The serial port to open. Can not be NULL.
 * @param device The device to inialize. Can not be NULL;
 * @return int A negative response is send in case of error.
 */
int navilink_open_device_from_name(const char* serial_port_name, NavilinkDevice* device);
/**
 * @brief Close the connection with the provided device.
 * 
 * @param device A pointer to a `NavilinkDevice`device
 * @return int A negative response is send in case of error.
 */
int navilink_close_device(NavilinkDevice* device);
/**
 * @brief Query the informations about the device
 * 
 * @param device A pointer to a `NavilinkDevice`device
 * @param information 
 * @return int 
 */
int navilink_query_information(NavilinkDevice* device, NavilinkInformation* information);
/**
 * @brief Get the firmware version.
 * 
 * @param device A pointer to a `NavilinkDevice`device
 * @param firmware_version
 * @return int 
 */
int navilink_query_firmware_version(NavilinkDevice* device, int* firmware_version);

/**
 * @brief Query one or more waypoint from the device
 * 
 * @param device A pointer to a `NavilinkDevice`device
 * @param waypoint_index The index of the waypoint to fetch.
 * @param query_length The number of waypoint to fetch. Can not be greater than 32.
 * @param waypoint A pointer to a an buffer of waypoint of query_length size.
 * @return int The number of filled waypoint inside the buffer;
 */
int navilink_query_waypoint(NavilinkDevice* device, int waypoint_index, int query_length, NavilinkWaypoint* waypoint);

int navilink_delete_waypoint(NavilinkDevice* device, int waypoint_id);

#endif
