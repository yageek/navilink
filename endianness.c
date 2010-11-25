#include "endianness.h"

void buffer_to_little_endian(Byte * buffer,int size){
	if( buffer == NULL ||  size < 2 ){
		printf("Nothing to do");
	}
	int i;
	Byte * temp = malloc(size);
	for(i = 0; i < size; i++) *(temp + size - 1 - i) = *(buffer + i);
	
	memcpy(buffer,temp,size);


}
