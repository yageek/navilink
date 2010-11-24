#include "api.h"


Byte packet[4104]={0};
Byte payload[4096] = {0};

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


Byte* setPacket(Byte type, Byte  *data,Word size){
    /* 4 octets and 30 bits equals 62 bits -> (64 bits >> 2) = ( 8 * sizeof(char) >> 2)*/
	if( (size >0) && (data == NULL)) {
	
		printf("The data provided to the function are not accessible!\n");
		return NULL;
		}
	
	Word packetLength = size +1;
	Word word =0;
	
	if(!packet){
		printf("Could not alloc the required quantity of data!\n");
		return NULL;
		
	}
	/* Start of data */
	word = (PACK_START1) | (PACK_START2 << 8);
	memcpy(packet,(Byte *)&word,2);
	
	
	/* Packet Length */
	memcpy(&packet[2],(Byte *)&packetLength,2);
	
	
	/*Data and Type*/
	memcpy(payload,(Byte *) &type,1);
	memcpy(&payload[1],(Byte *) data,size);
	memcpy(&packet[3],(Byte *) payload, packetLength);
	
	
	
	/*Checksum */
	word = getChecksum(payload,packetLength);
	memcpy(&packet[3+packetLength],(Byte *)&word,2);
	
	/*End word*/
	word = (PACK_END1) | (PACK_END2 << 8);
	memcpy(&packet[5+packetLength],(Byte *)&word,2);
	
	return packet;


}


