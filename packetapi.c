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

int setPacket(Byte *packet , Byte type, Byte  *data,Word size){
    int i;
	Word packetLength = size +1;
		
	if( (size >0) && (data == NULL)) {
		
		SetError("Data buffer is NULL and size is greater than 0 ");
		return -1;
		}
	
	/* Start of data */
	#ifdef BIG_ENDIAN
	*((Word *)packet) = __AdaptWord((PACK_START1) | (PACK_START2 << 8));
	#else
	*((Word *)packet) = (PACK_START1) | (PACK_START2 << 8);
	#endif
	
	/* Packet Length */
	#ifdef BIG_ENDIAN
	*((Word *)(packet+=2)) = __AdaptWord(packetLength);
	#else
	*((Word *)(packet+=2)) = packetLength;
	#endif
	
	/*Data and Type*/
	payload[0] = type;
	Byte *ptr = payload + 1;
	for(i=0 ;i < size;i++)  *ptr++ = *data++;
	
	packet+=2;
	ptr=payload;
	for(i=0 ;i < packetLength;i++)  *packet++ = *ptr++;
		
	/*Checksum */
	#ifdef BIG_ENDIAN
	*((Word *)packet) = __AdaptWord(getChecksum(payload,packetLength));
	#else
	*((Word *)packet) = getChecksum(payload,packetLength);
	#endif
	
	/*End word*/
	#ifdef BIG_ENDIAN
	*((Word *)(packet+=2)) = __AdaptWord((PACK_END1) | (PACK_END2 << 8));
	#else
	*((Word *)(packet+=2)) =(PACK_END1) | (PACK_END2 << 8);
	#endif
	
	return packetLength + 8;
	
}

int readPacket(Byte *packet, Word* Lengthofpacket, Byte *databuffer){

	Word i,checksum = 0,length;
	Byte *ptr = &payload[0];
	if((packet == NULL)){
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
