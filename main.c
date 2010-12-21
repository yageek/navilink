#include <stdio.h>
#include <stdlib.h>


#include "NaviGPSapi.h"

#define PRINT_SIZE(x) printf("Size of a "#x": %d\n",sizeof(x))
#define PRINT_STRING(x) printf(#x":%s\n",x);
#define PRINT_NUMBER(x) printf(#x":%d\n",x);

Byte packet[MAX_PACKET_SIZE] = {0};
Byte cpause;
int main()
{
	NaviGPS *bgt31 = get_new_GPS("/dev/ttyUSB0");
	printf("Trying to connect .....\n");
	if(init_gps_serial_link(bgt31) < 0){
		 printf("Error not connected\n"); 
		 return -1;
	 }
	 printf("Great connection .... :-)\n");
	//sleep(1);
	int n = write_packet_to_gps(bgt31,PID_SYNC,0,0);
	printf("Number of bytes send :%d\n",n);
	
	read_packet_from_gps(bgt31); 
	
	
	
	
	
	
	close_gps_serial_link(bgt31);
	free_GPS(bgt31);
	
	return 0;
}

