#ifndef API_H_INCLUDED
#define API_H_INCLUDED
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "stdpackets.h"
#include "endianness.h"

#define MAX_PAYLOAD_SIZE 4096
#define MAX_DATA_SIZE MAX_PAYLOAD_SIZE - 1
#define MAX_PACKET_SIZE MAX_PAYLOAD_SIZE + 8
#define CR() printf("\n")



Word getChecksum(const Byte *data, Word size);
Byte* setPacket( Byte type,  Byte *data, Word size); /* For setting up a packet */
void PrintPacket(const Byte* packet, int size);
int validPacket(Byte * packet);


#endif // API_H_INCLUDED
