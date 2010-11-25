#ifndef ENDIANNESS_H_INCLUDED
#define ENDIANNESS_H_INCLUDED

#include "stdlib.h"
/* Big Endian <--> Little Endian  */
typedef unsigned char Byte;
typedef unsigned short Word;

#define __AdaptWord(val) ((Word)( \
		(((Word)(val) & (Word)(0x00ffUL)) << 8) | \
		(((Word)(val) & (Word)(0xff00UL)) >> 8)))

#define __AdaptNible(val) ((Byte)( \
		((Byte)(val) & (Byte)(0x0F)) | \
		((Byte)(val) & (Byte)(0xF0))) 

void buffer_to_little_endian(Byte  * buffer,int size);

#endif 
