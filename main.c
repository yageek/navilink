#include <stdio.h>
#include <stdlib.h>


#include "NaviGPSapi.h"




#define PRINT_SIZE(x) printf("Size of a "#x": %d\n",sizeof(x))
#define PRINT_STRING(x) printf(#x":%s\n",x);
#define PRINT_NUMBER(x) printf(#x":%d\n",x);

Byte packet[MAX_PACKET_SIZE] = {0};
Byte cpause;

NaviGPS *bgt31 = NULL;
int main()
{
	
	
	bgt31 = get_new_GPS("/dev/tty.usbserial");
		if(bgt31 == NULL){
		printf("Not possible to initialize\n");
		return -1;
		
	}
	
	printf("Trying to connect .....\n");
	if(init_gps_serial_link(bgt31) < 0){
		 printf("Error not connected\n"); 
		 return -1;
	 }
	 printf("Great connection .... :-)\n");
	sleep(1);
    
	Byte * data = YGCreatePacket(PID_SYNC, 0, 0);
	
	
	//read_packet_from_gps(bgt31); 
	
	while(1);
	
	
	
	
	
	return 0;
}

