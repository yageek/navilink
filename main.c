#include <stdio.h>
#include <stdlib.h>

#include "api.h"
#include "endianness.h"
Byte data[MAX_DATA_SIZE] = {0};
Byte Packet[MAX_PACKET_SIZE] = {0};

Byte test[32] = {0x0d, 0x00, 0x00, 0x01, 0x00, 0x00, 0x12, 0x40, 0x2f, 0x96, 0xa6, 0x31, 0x00, 0x00, 0x15, 0x00, 0x59, 0x61, 0x47, 0x65, 0x65, 0x6b, 0x00, 0x40, 0x0c, 0x08, 0x01, 0x60, 0x88, 0xd8, 0x00, 0x00};

#define PRINT_SIZE(x) printf("Size of a "#x": %d\n",sizeof(x))
#define PRINT_STRING(x) printf(#x":%s\n",x);
#define PRINT_NUMBER(x) printf(#x":%s\n",x);

int main()
{
	

	
	if(setPacket(Packet,PID_SYNC,NULL,0) < 0) 
		printf("Error : %s\n",NavilinkError());
	else
		printf("Packet correctly set\n");
	
	
	T_INFORMATION  info;
	info = *((T_INFORMATION*)test);
	__AdaptT_INFORMATION(&info);

	
	
	



    return 0;
}

