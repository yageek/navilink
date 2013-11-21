#include "packetapi.h"

#define SetError(s) strcpy(navilinkError,s)

static Byte payload[MAX_PAYLOAD_SIZE] = {0};
static char navilinkError[ERROR_MESS_LENGTH] = "";

char * NavilinkError(){
	return navilinkError;
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

Byte * YGCreatePacket(Byte type, Byte  *payload,Word packetLength)
{
    Byte * buffer = malloc((8+packetLength+1) * sizeof(Byte));
    
    *buffer = PACK_START1;
    *(buffer+1) = PACK_START2;
    
    *((Word*)(buffer+2)) = YGHostToLEWord(packetLength+1);
    
    *(buffer+4) = type;
    
    memcpy(buffer+5, payload, packetLength);
    
    *((Word*)(buffer+5+packetLength)) = YGHostToLEWord(getChecksum(buffer+4, packetLength+1));

    *(buffer+7+packetLength) = PACK_END1;
     *(buffer+8+packetLength) = PACK_END2;
    
    return buffer;
}

int readPacket(Byte *packet, Word* Lengthofpacket, Byte *databuffer){

	Word i,checksum = 0,length;
	Byte *ptr = &payload[0];
	if(packet == NULL){
		SetError("The provided packet pointer is NULL. ");
		return -1;
	}
	
	if((*packet != 0xA0) || (*(packet+1)!=0xA2)){
		SetError("The beginning bytes are wrong.");
		return -1;
	}
	
	length = *((Word *)(packet+=2));
	packet+=2;
	
	for(i = 0; i < length; i++) *ptr++ = *packet++;
		
	checksum = *((Word*)packet);
	
	if(checksum!=getChecksum(&payload[0],length)){
		SetError("Unexpected checksum");	
		return -1;
	}
	
	packet+=2;
	if((*packet != 0xB0) || (*(packet+1)!=0xB3)){
		SetError("The ending bytes are wrong.");
		return -1;
	}
		
	if(databuffer!=NULL){
		ptr = &payload[1];
		for(i=0;i< length - 1;i++) *databuffer++ = *ptr++;
		}
		
	if(Lengthofpacket != NULL) *Lengthofpacket = length;
	return (Word)(payload[0]);
		
			
}
