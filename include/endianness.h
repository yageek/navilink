#ifndef ENDIANNESS_H_INCLUDED
#define ENDIANNESS_H_INCLUDED

#include "stdpackets.h"
#include "stdlib.h"

#define __AdaptWord(val) ((Word)( \
		(((Word)(val) & (Word)(0x00ffUL)) << 8) | \
		(((Word)(val) & (Word)(0xff00UL)) >> 8)))

#define __AdaptDoubleWord(x) ((DoubleWord)(				\
	(((DoubleWord)(x) & (DoubleWord)0x000000ffUL) << 24) |		\
	(((DoubleWord)(x) & (DoubleWord)0x0000ff00UL) <<  8) |		\
	(((DoubleWord)(x) & (DoubleWord)0x00ff0000UL) >>  8) |		\
	(((DoubleWord)(x) & (DoubleWord)0xff000000UL) >> 24)))

#define __AdaptNible(val) ((Byte)( \
		((Byte)(val) & (Byte)(0x0F)) | \
		((Byte)(val) & (Byte)(0xF0)))) 

typedef enum {
    YGBigEndian,
    YGLittleEndian,
    YGUnknownEndian
} YGEndianness;

YGEndianness localEndianness();

#define YGHostToLEWord(WORD) localEndianness() == YGLittleEndian ? WORD : __AdaptWord(WORD);


void returnbuffer(Byte  * buffer,int size);
void __AdaptT_INFORMATION(T_INFORMATION *x);
void __AdaptT_POSITION(T_POSITION *x);
#endif 
