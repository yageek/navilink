/** \file stdpackets.h
 *	\brief This header descibes the protocol used by the device.
 *
 *	Most of the code is extracted from the documentation.pdf file
 *	extracted from splitbrain.org.
 *
 */
#ifndef NAVILINK_PROTOCOL_H_INCLUDED
#define NAVILINK_PROTOCOL_H_INCLUDED

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
  unsigned short totalWaypoint; /**< Range 0..1000 */
  unsigned char totalRoute; /**< Range 0..20 */
  unsigned char totalTrack; /**< Always 1 for NAViGPS */
  unsigned int startAdrOfTrackBuffer;
  unsigned int deviceSerialNum;
  unsigned short numOfTrackpoints; /**< Range 0..8191*/
  unsigned short protocolVersion;
  // char unknown[16]; /* 16 bytes with unknown info */
  char username[16]; /**< Username  */
} __attribute__((packed));

/** \struct NavilinkPosition
 *	\brief Describe the position of one waypoint.
 */
struct NavilinkPosition {
  int latitude; /**< +-900000000,in 1/10000000 degree */
  int longitude; /**< +-1800000000,in 1/10000000 degree*/
  unsigned short altitude; /**< range 0..65535,in feet*/
} __attribute__((packed));

/** \struct NavilinkDateTime
 *	\brief Describe the timestamp of one waypoint.
 */
struct NavilinkDateTime {
  unsigned char year; /**< actual year= year+2000 */
  unsigned char month; /**< Range 1..12 */
  unsigned char day; /**< Range 1..31 */
  unsigned char hour; /**< Range 0..23 */
  unsigned char minute; /**< Range 0..59 */
  unsigned char second; /**< Range 0..59 */
} __attribute__((packed));

/** \struct NavilinkWaypoint
 *	\brief Describe the waypoint element contained in the data logger.
 */
struct NavilinkWaypoint {
  unsigned short recordType; /**< reserved. default 0x00 0x40*/
  unsigned short waypointID; /**< Range 0..999*/
  char waypointName[7]; /**< null-terminated- string,[ 0 .. 9 , , A .. Z ]*/
  unsigned char reserved;
  NavilinkPosition position; /**< position based on WGS84 datum*/
  NavilinkDateTime datetime; /**< time, date in UTC */
  unsigned char symbolType; /**< Range 0..31*/
  unsigned char reserved_2;
  unsigned char tag1; /**< reserved, default 0x00 */
  unsigned char tag2; /**< reserved, default 0x7e */
} __attribute__((packed));

/** \struct NavilinkSubroute
 *	\brief To determine
 */
struct NavilinkSubroute {
  unsigned short recordType; /**< reserved, default 0x2010 */
  unsigned short waypointID[14]; /**< Range 0..999,0xffff:NULL waypoint ID */
  unsigned char tag1; /**< 0x7f for last subroute*/
  unsigned char tag2; /**< reserved , default 0x77*/
} __attribute__((packed));

/** \struct NavilinkRoute
 *	\brief To determine
 */
struct NavilinkRoute {
  unsigned short recordType; /**< reserved, default 0x2000*/
  unsigned char routeID; /**< route ID:0..19,0xffff:null route ID*/
  unsigned char reserved_1; /**< default 0x20 */
  char routeName[14]; /**< string,[ 0 .. 9 , A .. Z , ]*/
  char reserved[2];
  unsigned int reserved_2;
  unsigned int reserved_3;
  unsigned short reserved_4;
  unsigned char flag; /**< reserved, default 0x7b */
  unsigned char mark; /**< reserved, default 0x77 */
  NavilinkSubroute subRoutes[9];
} __attribute__((packed));

/** \struct NavilinkTrackpoint
 *	\brief Seems to be not used with the BGT31
 */
struct NavilinkTrackpoint {
  unsigned short serialNum; /**< unique serial number,0..8191*/
  unsigned short headingOfPoint; /**< Range 0..360 degree*/
  int x; /**< UTM x in WGS84 */
  int y; /**< UTM y in WGS84 */
  NavilinkPosition position; /**< position in WGS84 datum*/
  NavilinkDateTime datetime; /**< time, date in UTC*/
  unsigned char zone; /**< UTM zone, 1..60*/
  unsigned char halfspeed; /**< in KMH, actual speed=halfspeed*2 */
  unsigned char tag1; /**< reserved, default 0x5a */
  unsigned char tag2; /**< reserved, default 0x7e */
} __attribute__((packed));

#endif // NAVILINK_PROTOCOL_H_INCLUDED
