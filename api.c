#include "api.h"

#define SetError(s) strcpy(navilinkError,s)

static Byte payload[MAX_PAYLOAD_SIZE] = {0};
static char navilinkError[ERROR_MESS_LENGTH] = "";
static Byte unityPacket[9];
Byte GET_INFO[41] = {0xa0, 0xa2, 0x21, 0x00, 0x03, 0x0d, 0x00, 0x00, 0x01, 0x00, 0x00, 0x12, 0x40, 0x2f, 0x96, 0xa6, 0x31, 0x00, 0x00, 0x15, 0x00, 0x59, 0x61, 0x47, 0x65, 0x65, 0x6b, 0x00, 0x40, 0x0c, 0x08, 0x01, 0x60, 0x88, 0xd8, 0x00, 0x00, 0x5f, 0x06, 0xb0, 0xb3};

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

int syncGPS(NaviGPS* dev){

	/* Some space for initialising the serial device to Full Duplex, serial Data,115200 8N1S*/
	if(setPacket(unityPacket,PID_SYNC,NULL,0) < 0) return -1;
	
	//Wait for recieving data
	if(readPacket(unityPacket,NULL,NULL) != PID_ACK){
	
		SetError("GPS don't answer");
		return -1; 
	}
		return 1;
}

NaviGPS *initGPS(const char* dev){
	
	NaviGPS * ptr = malloc(sizeof(NaviGPS));
	
	ptr->informations = malloc(sizeof(T_INFORMATION));
	ptr->waypoints = malloc(sizeof(T_WAYPOINT));
	ptr->routes = malloc(sizeof(T_ROUTE));
	ptr->tracks = malloc(sizeof(T_TRACKPOINT));
	
	strcpy(ptr->deviceName,dev);

}

void freeNaviGPS(NaviGPS * dev){
	free(dev);
}

int getInfo(NaviGPS * dev){

	if(setPacket(unityPacket,PID_QRY_INFORMATION,0,0) < 0) return -1;
	
	if(readPacket(GET_INFO,&(dev->packetLength),dev->buffer) != PID_DATA){
	SetError("No data recieved from the GPS");
	return -1;
	
	}
	
	*(dev->informations) = *((T_INFORMATION*)(dev->buffer));
	return 1;

}


void displayInfo(NaviGPS *dev){
	printf(" ===== Informations of %s ====\n",dev->deviceName);
	printf("\tTotal waypoints   : %20d\n",dev->informations->totalWaypoint);
	printf("\tTotal route       : %20d\n",dev->informations->totalRoute);
	printf("\tTotal track       : %20d\n",dev->informations->totalTrack);
	printf("\tAdressTrack (hex) : %20X\n",dev->informations->startAdrOfTrackBuffer);
	printf("\tDevice serial num : %20d\n",dev->informations->deviceSerialNum);
	printf("\tProtocol version  : %20d\n",dev->informations->protocolVersion);
	printf("\tUser name         : %20s\n",dev->informations->username);
}


void queryWaypoints(NaviGPS *dev,DoubleWord first, Word size ){

	Byte data[7];
	int i;
	
	
	*((DoubleWord *)data) = first;
	*((Word*)&data[5]) = size;
	*(&data[7]) = 0x01U;
	
	setPacket(dev->buffer,PID_QRY_WAYPOINTS,data,7);
	
		
}
