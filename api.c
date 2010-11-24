#include "api.h"

Word packetLength = 0, checksum = 0, payload = 0, end =0;
Byte packet[10] = {0xFF};


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
     checksum =+ payloaddata[index];
     index++;
	 }
    checksum &= 0x7FFF;
    return checksum ;
}


int setPacket(Byte type, Byte  *data,Word size){
    /* 4 octets and 30 bits equals 62 bits -> (64 bits >> 2) = ( 8 * sizeof(char) >> 2)*/
	if( (size >0) && (data == NULL)) {
	
		printf("The data provided to the function are not accessible!\n");
		return -1;
		}
	
	int offset=0;
	Byte byte=0;
	Byte * ptr = packet;
	
	packetLength = size + 1 ;
	/* First two bytes */
	packet[0] = PACK_START1;
	packet[1] = PACK_START2;
	ptr+=2;
	offset+=2;
	
	/* Length */
	memcpy(ptr,&packetLength ,2);
	ptr+=2;
	offset+=2;
	
	/*Data*/
	byte = type;
	memcpy(ptr,&byte,1);
	ptr+=1;
	offset+=1;
	
	memcpy(ptr,data,size);
	ptr+=size;
	offset+=size;
	
	/*Checksum*/
	checksum = getChecksum((const Byte *)&payload,packetLength);
	memcpy(ptr,&checksum,packetLength);
	ptr+=2;
	offset+=2;
	
	/*end*/
	end = PACK_END1;
	memcpy(ptr,&end,1);
	ptr+=1;
	offset+=1;
	
	end = PACK_END2;
	memcpy(ptr,&end,1);
	offset+=1;
	
	printf("Length of bytes : %d\n",offset);
	return offset;


}


