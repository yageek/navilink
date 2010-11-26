#ifndef STDPACKETS_H_INCLUDED
#define STDPACKETS_H_INCLUDED

/* Define all the predefined packets
-> a0 a2 01 00  d6 d6 00 b0  b3
<- a0 a2 01 00  0c 0c 00 b0  b3
-> a0 a2 01 00  20 20 00 b0  b3
<- a0 a2 21 00  03 0d 00 00  01 00 00 12  40 2f 96 a6  31 00 00 15  00 59 61 47  65 65 6b 00  40 0c 08 01  60 88 d8 00  00 5f 06 b0  b3
-> a0 a2 01 00  fe fe 00 b0  b3
<- a0 a2 13 00  03 56 31 2e  32 28 42 30  39 32 32 29  20 2c 30 30  31 35 00 5c  03 b0 b3
*/

#define PACK_END1 0xb0U
#define PACK_END2 0xb3U
#define PACK_START1 0xa0U
#define PACK_START2 0xa2U
#define PID_SYNC 0xd6U
#define PID_ACK 0x0cU
#define PID_NAK 0x00U
#define PID_QRY_INFORMATION 0x20U
#define PID_QRY_FW_VERSION 0xfeU
#define PID_DATA 0x03U
#define PD_ADD_A_WAYPOINT 0x3cU
#define PID_QRY_WAYPOINTS 0x08U
#define PID_QRY_ROUTE 0x24U
#define PID_DEL_WAYPOINT 0x36U
#define PID_DEL_ALL_WAYPOINT 0x37U
#define PID_DEL_ROUTE 0x34U
#define PID_DEL_ALL_ROUTE 0x35U
#define PID_ADD_A_ROUTE 0x3dU
#define PID_ERASE_TRACK 0x11U
#define PID_READ_TRACKPOINTS 0x14U
#define PID_WRITE_TRACKPOINTS 0x16U
#define PID_CMD_OK 0xf3U
#define PID_CMD_FAIL 0xf4U
#define PID_QUIT 0xf2U

/*Not defined in documentation */
#define PID_LOG_DATA_ADDR 0x1cU /* send 1 and after get Data */
#define PID_READ_LOG_HEADER 0x50U

#define MAX_PAYLOAD_SIZE 4096
#define MAX_DATA_SIZE MAX_PAYLOAD_SIZE - 1
#define MAX_PACKET_SIZE MAX_PAYLOAD_SIZE + 8
#define MAX_FIELD_SIZE sizeof(short) * 14

typedef struct T_ROUTE T_ROUTE;
typedef struct T_INFORMATION T_INFORMATION;
typedef struct T_POSITION T_POSITION;
typedef struct T_DATETIME T_DATETIME;
typedef struct T_WAYPOINT T_WAYPOINT;
typedef struct T_SUBROUTE T_SUBROUTE;
typedef struct T_TRACKPOINT T_TRACKPOINT;


struct T_INFORMATION{
	unsigned short totalWaypoint; /* 0..1000 */
	unsigned char totalRoute; /*0..20 */
	unsigned char totalTrack; /* always 1 for NAViGPS */
	unsigned int startAdrOfTrackBuffer;
	unsigned int deviceSerialNum;
	unsigned short numOfTrackpoints; /* 0..8191*/
	unsigned short protocolVersion;
	//char unknown[16]; /* 16 bytes with unknown info */
	char username[16]; /* zero byte terminated */
} __attribute__((packed));


struct T_POSITION{
    int latitude;             /*+-900000000,in 1/10000000 degree */
    int longitude;            /*+-1800000000,in 1/10000000 degree*/
    unsigned short altitude;  /*0..65535,in feet*/
} __attribute__((packed));

struct T_DATETIME{
    unsigned char year;    /* actual year= year+2000 */
    unsigned char month;   /* 1..12 */
    unsigned char day;     /* 1..31 */
    unsigned char hour;    /* 0..23 */
    unsigned char minute;  /* 0..59 */
    unsigned char second;  /* 0..59 */
} __attribute__((packed));

struct T_WAYPOINT {
    unsigned short recordType; /* reserved. default 0x00 0x40*/
    unsigned short waypointID; /* 0..999*/
    char waypointName[7];      /* null-terminated- string,[ 0 .. 9 , , A .. Z ]*/
    unsigned char reserved;
    T_POSITION position;       /*position based on WGS84 datum*/
    T_DATETIME datetime;       /*time, date in UTC */
    unsigned char symbolType;  /*0..31*/
    unsigned char reserved_2;
    unsigned char tag1;        /*reserved, default 0x00 */
    unsigned char tag2;        /*reserved, default 0x7e */
} __attribute__((packed));

struct T_SUBROUTE{
    unsigned short recordType;     /*reserved, default 0x2010 */
    unsigned short waypointID[14]; /*0..999,0xffff:NULL waypoint ID */
    unsigned char tag1;            /*0x7f for last subroute*/
    unsigned char tag2;            /*reserved , default 0x77*/
} __attribute__((packed));

struct T_ROUTE{
    unsigned short recordType; /*reserved, default 0x2000*/
    unsigned char routeID;     /*route ID:0..19,0xffff:null route ID*/
    unsigned char reserved_1;    /*default 0x20 */
    char routeName[14];        /*c string,[ 0 .. 9 , A .. Z , ]*/
    char reserved[2];
    unsigned int reserved_2;
    unsigned int reserved_3;
    unsigned short reserved_4;
    unsigned char flag;        /* reserved, default 0x7b */
    unsigned char mark;        /* reserved, default 0x77 */
    T_SUBROUTE subRoutes[9];
} __attribute__((packed));

 struct T_TRACKPOINT{
    unsigned short serialNum;      /*unique serial number,0..8191*/
    unsigned short headingOfPoint; /*0..360 degree*/
    int x;                         /*UTM x in WGS84 */
    int y;                         /*UTM y in WGS84 */
    T_POSITION position;           /*position in WGS84 datum*/
    T_DATETIME datetime;           /*time, date in UTC*/
    unsigned char zone;            /*UTM zone, 1..60*/
    unsigned char halfspeed;       /*in KMH, actual speed=halfspeed*2 */
    unsigned char tag1;            /*reserved, default 0x5a */
    unsigned char tag2;            /*reserved, default 0x7e */
} __attribute__((packed));


#endif // STDPACKETS_H_INCLUDED
