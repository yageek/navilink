/** \file stdpackets.h
 *	\brief This header descibes the protocol used by the device.
 *
 *	Most of the code is extracted from the documentation.pdf file
 *	extracted from splitbrain.org.
 *
 */
#ifndef NAVILINK_PROTOCOL_H_INCLUDED
#define NAVILINK_PROTOCOL_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>

typedef enum {
  NavilinkOK = 0,
  NavilinkErr = -1
} NavilinkResult;

/* Define all the predefined packets
-> a0 a2 01 00  d6 d6 00 b0  b3
<- a0 a2 01 00  0c 0c 00 b0  b3
-> a0 a2 01 00  20 20 00 b0  b3
<- a0 a2 21 00  03 0d 00 00  01 00 00 12  40 2f 96 a6  31 00 00 15  00 59 61 47
65 65 6b 00  40 0c 08 01  60 88 d8 00  00 5f 06 b0  b3
-> a0 a2 01 00  fe fe 00 b0  b3
<- a0 a2 13 00  03 56 31 2e  32 28 42 30  39 32 32 29  20 2c 30 30  31 35 00 5c
03 b0 b3
*/

/** @defgroup packetID Packets ID
 *	This part describe all the packet IDs. This represent the first byte in
 *the payload od each packet.
 * @{
 */

/** The first byte of the first two byte. */
#define NAVILINK_PACK_START1 0xa0U
/** The second byte of the first two byte. */
#define NAVILINK_PACK_START2 0xa2U
/** The first byte of the last two  byte. */
#define NAVILINK_PACK_END1 0xb0U
/** The second byte of the last two byte. */
#define NAVILINK_PACK_END2 0xb3U
/**The beginning packet to check if NAVi GPS device is ready or not. */
#define NAVILINK_PID_SYNC 0xd6U
/** General acknowledge packet */
#define NAVILINK_PID_ACK 0x0cU
/** General none-acknowledge packet*/
#define NAVILINK_PID_NAK 0x00U
/** Packet to query NAViGPS information*/
#define NAVILINK_PID_QRY_INFORMATION 0x20U
/** Packet to query the firmware version of the device*/
#define NAVILINK_PID_QRY_FW_VERSION 0xfeU
/** General data packet*/
#define NAVILINK_PID_DATA 0x03U
/** Packet to add a route to NAViGPS*/
#define NAVILINK_PD_ADD_A_WAYPOINT 0x3cU
/** Packet to read 1 to 32 waypoints from NAVIGPS*/
#define NAVILINK_PID_QRY_WAYPOINTS 0x28U
/** Packet to query a route from NAViGPS*/
#define NAVILINK_PID_QRY_ROUTE 0x24U
/** Packet to delete one waypoint*/
#define NAVILINK_PID_DEL_WAYPOINT 0x36U
/** Packet request deleting all waypoints*/
#define NAVILINK_PID_DEL_ALL_WAYPOINT 0x37U
/** Packet to delete one route*/
#define NAVILINK_PID_DEL_ROUTE 0x34U
/** Packet request deleting all routes*/
#define NAVILINK_PID_DEL_ALL_ROUTE 0x35U
/** Packet to add a route to NAViGPS*/
#define NAVILINK_PID_ADD_A_ROUTE 0x3dU
/** Packet request deleting track*/
#define NAVILINK_PID_ERASE_TRACK 0x11U
/** Packet request reading track logs from NAViGPS*/
#define NAVILINK_PID_READ_TRACKPOINTS 0x14U
/** Packet request to write track points to NAVI*/
#define NAVILINK_PID_WRITE_TRACKPOINTS 0x16U
/** Packet to indicate command is OK*/
#define NAVILINK_PID_CMD_OK 0xf3U
/** Packet to indicate command failed*/
#define NAVILINK_PID_CMD_FAIL 0xf4U
/** Packet to end connection*/
#define NAVILINK_PID_QUIT 0xf2U
/** @} */
#define NAVILINK_MAX_WAYPOINT_QUERY_LENGTH 32
#define NAVILINK_MAX_WAYPOINT_ID 499

#define NAVILINK_MAX_ROUTE_QUERY_LENGTH 19

/* Not defined in documentation from splitbrain*/
#define NAVILINK_PID_LOG_DATA_ADDR 0x1cU /* send 1 and after get Data */
#define NAVILINK_PID_READ_LOG_HEADER 0x50U

#define NAVILINK_MAX_PAYLOAD_SIZE 4096 /** Maximal value for the payload*/
#define NAVILINK_MAX_DATA_SIZE \
  NAVILINK_MAX_PAYLOAD_SIZE - 1 /** Maximal value for the data*/
#define NAVILINK_MAX_PACKET_SIZE \
  NAVILINK_MAX_PAYLOAD_SIZE + 8 /** Maximal value for the packet*/

#define NAVILINK_INFORMATION_PAYLOAD_LENGTH 32
#define NAVILINK_WAYPOINT_PAYLOAD_LENGTH 32
#define NAVILINK_POSITION_PAYLOAD_LENGTH 6
#define NAVILINK_DATETIME_PAYLOAD_LENGTH 10

typedef struct NavilinkRoute NavilinkRoute;
typedef struct NavilinkInformation NavilinkInformation;
typedef struct NavilinkPosition NavilinkPosition;
typedef struct NavilinkDateTime NavilinkDateTime;
typedef struct NavilinkWaypoint NavilinkWaypoint;
typedef struct NavilinkSubroute NavilinkSubroute;
typedef struct NavilinkTrackpoint NavilinkTrackpoint;

/** \struct NavilinkInformation
 *	\brief Describe the informations about the data logger.
 */
struct NavilinkInformation {
  uint16_t totalWaypoint; /**< Range 0..1000 */
  uint8_t totalRoute; /**< Range 0..20 */
  uint8_t totalTrack; /**< Always 1 for NAViGPS */
  uint32_t startAdrOfTrackBuffer;
  uint32_t deviceSerialNum;
  uint16_t numOfTrackpoints; /**< Range 0..8191*/
  uint16_t protocolVersion;
  // char unknown[16]; /* 16 bytes with unknown info */
  uint8_t username[16]; /**< Username  */
};

/** \struct NavilinkPosition
 *	\brief Describe the position of one waypoint.
 */
struct NavilinkPosition {
  int32_t latitude; /**< +-900000000,in 1/10000000 degree */
  int32_t longitude; /**< +-1800000000,in 1/10000000 degree*/
  uint16_t altitude; /**< range 0..65535,in feet*/
};

/** \struct NavilinkDateTime
 *	\brief Describe the timestamp of one waypoint.
 */
struct NavilinkDateTime {
  uint8_t year; /**< actual year= year+2000 */
  uint8_t month; /**< Range 1..12 */
  uint8_t day; /**< Range 1..31 */
  uint8_t hour; /**< Range 0..23 */
  uint8_t minute; /**< Range 0..59 */
  uint8_t second; /**< Range 0..59 */
};

/** \struct NavilinkWaypoint
 *	\brief Describe the waypoint element contained in the data logger.
 */
struct NavilinkWaypoint {
  //uint16_t recordType; /**< reserved. default 0x00 0x40*/
  uint16_t waypointID; /**< Range 0..999*/
  uint8_t waypointName[7]; /**< null-terminated- string,[ 0 .. 9 , , A .. Z ]*/
  //uint8_t reserved;
  NavilinkPosition position; /**< position based on WGS84 datum*/
  NavilinkDateTime datetime; /**< time, date in UTC */
  uint8_t symbolType; /**< Range 0..31*/
  //uint8_t reserved_2;
  //uint8_t tag1; /**< reserved, default 0x00 */
  //uint8_t tag2; /**< reserved, default 0x7e */
};

/** \struct NavilinkSubroute
 *	\brief To determine
 */
struct NavilinkSubroute {
  uint16_t recordType; /**< reserved, default 0x2010 */
  uint16_t waypointID[14]; /**< Range 0..999,0xffff:NULL waypoint ID */
  uint8_t tag1; /**< 0x7f for last subroute*/
  uint8_t tag2; /**< reserved , default 0x77*/
};

/** \struct NavilinkRoute
 *	\brief To determine
 */
struct NavilinkRoute {
  uint16_t recordType; /**< reserved, default 0x2000*/
  uint8_t routeID; /**< route ID:0..19,0xffff:null route ID*/
  // uint8_t reserved_1; /**< default 0x20 */
  uint8_t routeName[14]; /**< string,[ 0 .. 9 , A .. Z , ]*/
  // uint8_t reserved[2];
  // uint32_t reserved_2;
  // uint32_t reserved_3;
  // uint16_t reserved_4;
  uint8_t flag; /**< reserved, default 0x7b */
  uint8_t mark; /**< reserved, default 0x77 */
  NavilinkSubroute subRoutes[9];
};

/** \struct NavilinkTrackpoint
 *	\brief Seems to be not used with the BGT31
 */
struct NavilinkTrackpoint {
  uint16_t serialNum; /**< unique serial number,0..8191*/
  uint16_t headingOfPoint; /**< Range 0..360 degree*/
  int32_t x; /**< UTM x in WGS84 */
  int32_t y; /**< UTM y in WGS84 */
  NavilinkPosition position; /**< position in WGS84 datum*/
  NavilinkDateTime datetime; /**< time, date in UTC*/
  uint8_t zone; /**< UTM zone, 1..60*/
  uint8_t halfspeed; /**< in KMH, actual speed=halfspeed*2 */
  uint8_t tag1; /**< reserved, default 0x5a */
  uint8_t tag2; /**< reserved, default 0x7e */
};

int navilink_read_informations(NavilinkInformation* information, uint8_t* buffer, size_t buffer_len);
int navilink_read_datetime(NavilinkDateTime* datetime, uint8_t* buffer, size_t buffer_len);
int navilink_read_position(NavilinkPosition* position, uint8_t* buffer, size_t buffer_len);
int navilink_read_waypoint(NavilinkWaypoint* waypoint, uint8_t* buffer, size_t buffer_len);
#endif // NAVILINK_PROTOCOL_H_INCLUDED
