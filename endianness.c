#include "endianness.h"

#ifdef __APPLE__

#include <CoreFoundation/CFByteOrder.h>
static YGEndianness endiannessVar = YGUnknownEndian;

YGEndianness localEndianness()
{
    if(endiannessVar != YGUnknownEndian)
        return endiannessVar;
    
    CFByteOrder order = CFByteOrderGetCurrent();
    if(order == CFByteOrderLittleEndian)
        endiannessVar = YGLittleEndian;
    else if(order == CFByteOrderBigEndian)
        endiannessVar = YGBigEndian;
        return endiannessVar;
}
#endif


static Byte tempBuffer[MAX_DATA_SIZE];

void returnbuffer(Byte * buffer,int size){
	if( buffer == NULL ||  size < 2 ){
		return;
	}
	int i;
	Byte * temp = tempBuffer,*org=buffer;
	for(i = 0; i < size; i++) *temp++ = *buffer++;
	for(i = 0; i < size; i++) *org++ = *--temp;
}
void __AdaptT_INFORMATION(T_INFORMATION *x){

	x->totalWaypoint = __AdaptWord(x->totalWaypoint);
	x->startAdrOfTrackBuffer = __AdaptDoubleWord(x->startAdrOfTrackBuffer);
	x->deviceSerialNum = __AdaptDoubleWord(x->deviceSerialNum);
	x->numOfTrackpoints = __AdaptWord(x->numOfTrackpoints);
	x->protocolVersion = __AdaptWord(x->protocolVersion);
	returnbuffer((Byte *)(&x->username),16);
	
}

void __AdaptT_POSITION(T_POSITION *x){

	x->latitude = __AdaptDoubleWord(x->latitude);
	x->longitude = __AdaptDoubleWord(x->longitude);
	x->altitude = __AdaptWord(x->altitude);
	
}
/* To continue */
