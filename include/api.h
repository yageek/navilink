#ifndef API_H_INCLUDED
#define API_H_INCLUDED
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "stdpackets.h"
#include "endianness.h"

#define ERROR_MESS_LENGTH 256
#define CR() printf("\n")

Word getChecksum(const Byte *data, Word size);
int setPacket(Byte *packet , Byte type, Byte  *data,Word size); /* For setting up a packet */
char * NavilinkError();


#endif // API_H_INCLUDED
