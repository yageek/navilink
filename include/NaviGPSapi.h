/** \file NaviGPSapi.h 
 *	\brief Main api header
 */

#ifndef NAVIGPSAPI_H_INCLUDED
#define NAVIGPSAPI_H_INCLUDED

#include "packetapi.h"

typedef struct NaviGPS NaviGPS;
/** \struct NaviGPS
 *	\brief Structure describing a naviGPD device
 */
struct NaviGPS{
	char deviceName[20]; /**< COM name of the device (/dev/ttyUSB0 for exemple ) */
	int fd; /**< File descriptor of the device */
	T_INFORMATION *informations;/**< Informations about the device */
	T_WAYPOINT *waypoints;/**< List of waypoints in the device */
	T_ROUTE *routes;/**< List of routes in the device */
	T_TRACKPOINT *tracks;/**< List of routes in the device (seems to be not used) */
};

NaviGPS* get_new_GPS(const char* dev);
void display_gps_info(NaviGPS *dev);
void queryWaypoints(NaviGPS *dev,DoubleWord first, Word size );
void free_GPS(NaviGPS * dev);

/* Serial Function */
/** \todo implement on WIN32 */
int write_packet_to_gps(NaviGPS *dev, Byte type, Byte *data, Word size); 
int write_test(NaviGPS *dev);
int read_test(NaviGPS*dev);
/** \todo implement on WIN32 */
int close_gps_serial_link(NaviGPS *dev);
/** \todo implement on WIN32 and POSIX*/
int read_packet_from_gps(NaviGPS *dev);
/** \todo implement on WIN32 */
int init_gps_serial_link(NaviGPS * dev); 

#endif
