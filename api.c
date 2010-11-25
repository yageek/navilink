#include "api.h"




void PrintPacket(const Byte* packet, int size){
    int i;
    for(i=0; i < size; i++){
        printf("%02x ",packet[i]);

    }
    printf("\n");
}

Word getChecksum(const Byte *payloaddata, Word size){
    Word index = 0;
    Word checksum =0;

    while(index < size){
     checksum += payloaddata[index];
     index++;
	 }
    checksum &= 0x7FFFUL;
    return checksum ;
}

Byte* setPacket(Byte type, Byte  *data,Word size){
    int i;
	Word packetLength = size +1;
	Byte *packet = malloc(8 + packetLength),*org = packet;
	Byte payload[MAX_PAYLOAD_SIZE] = {0};
		
	if( (size >0) && (data == NULL)) {
	
		printf("The data provided to the function are not accessible!\n");
		return NULL;
		}
	
	/* Start of data */
	*((Word *)packet) = (PACK_START1) | (PACK_START2 << 8);

	/* Packet Length */
	*((Word *)(packet+=2)) = packetLength;
		
	
	/*Data and Type*/
	payload[0] = type;
	Byte *ptr = payload + 1;
	for(i=0 ;i < size;i++)  *ptr++ = *data++;
	
	packet+=2;
	ptr=payload;
	for(i=0 ;i < packetLength;i++)  *packet++ = *ptr++;
	
	
	/*Checksum */
	*((Word *)(packet)) = getChecksum(payload,packetLength);
		
	/*End word*/
	*((Word *)(packet+=2)) =(PACK_END1) | (PACK_END2 << 8);
	
	
	return org;
}

int validPacket(Byte * packet){
	
	Word checksum = 0,packetLength = 0;
	Byte *data;
	
	if((*packet!=PACK_START1) && (*(packet+1) != PACK_START2)){
		printf("The two start bytes are not correct\n");
		return -1;
	}
	packet+=2;
	
	/*Length of the data*/
	memcpy(&packetLength, (Word *)packet,2);
			
	printf("Size of packet:%d\n",packetLength);
	packet+=2;
	
	/*Look for end bytes first */
	if((*(packet+packetLength + 2 )!=PACK_END1) && ((*(packet + packetLength + 3 )!=PACK_END2))){
	
		printf("No end bytes after the specified size\n");
		return -1;
		
	}
	/*Compute checksum if it is correct*/
	
	memcpy(&checksum,packet + packetLength,2);
	
	
	if(checksum != getChecksum(packet,packetLength)){
		printf("The checksum is not correct\n");
		return -1;
	}
	
	printf("Packet Valid\n");
	return 1;
}
