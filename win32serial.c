#include<stdio.h>
#include<string.h>

#include<windows.h>

#include "NaviGPSapi.h"

#define CLR_RX_BUFFER() memset(recievingbuffer,0,MAX_PACKET_SIZE)
#define CLR_TX_BUFFER() memset(transmitbuffer,0,MAX_PACKET_SIZE)
static Byte transmitbuffer[MAX_PACKET_SIZE];
static Byte recievingbuffer[MAX_PACKET_SIZE];


int init_gps_serial_link(NaviGPS * dev){
 
	unsigned long confSize = sizeof(COMMCONFIG);




}
