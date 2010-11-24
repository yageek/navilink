#ifndef API_H_INCLUDED
#define API_H_INCLUDED
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "stdpackets.h"

#define CR() printf("\n")

typedef unsigned char Byte;
typedef unsigned short Word;

Word getChecksum(const Byte *data, Word size);
int setPacket( Byte type,  Byte *data, Word size);
void PrintPacket(const Byte* packet, int size);

#endif // API_H_INCLUDED
