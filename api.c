#include "api.h"

#define SetError(s) strcpy(navilinkError,s)

static Byte payload[MAX_PAYLOAD_SIZE] = {0};
static char navilinkError[ERROR_MESS_LENGTH] = "";

char * NavilinkError(){
	return navilinkError;
}

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
	return 0;
	
}

