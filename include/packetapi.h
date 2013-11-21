/*! \mainpage NAvilink API
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *  
 * etc...
 */
/** \file packetapi.h 
 *	\brief Headers for the packet manipulation functions.
 */

#ifndef API_H_INCLUDED
#define API_H_INCLUDED
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "stdpackets.h"
#include "endianness.h"

#define ERROR_MESS_LENGTH 256
#define CR() printf("\n")

/** \brief Compute the packet checksum 
*
*	This function use the foloowing pseudo algorithm:
*	\code
*	Index = first
*	checkSum = 0
*	while index < msgLen
*		checkSum = checkSum + Packet[index]
*	checkSum = checkSum AND (0x7FFF)
*	\endcode
*	\param data The data <b> with the packet ID </b>
*	\param size The size of the \p data buffer
*/
Word getChecksum(const Byte *data, Word size);

/** \brief Setting up a packet for sending 
*
*	This function is used to build packet before sending it 
*	on the serial bus.
*
*	\param packet The destination buffer of the packet
*	\param byte The packet ID of the packet
*	\param data The data to send in the packet <b> without the packet </b>
*	\param size The size of of the \p data buffer
* 	\return The size of the packet to send
*/

Byte * YGCreatePacket(Byte type, Byte  *payload,Word packetLength); /* For setting up a packet */

/** \brief Decode a packet 
*
*	This function is used to decode a packet and operate action in consequences. This function takes a
*	\p packet pointer, modify the providing \p Lengthofpacket and \p databuffer corresponding to 
*	the \p packet pointer.
*	
*	\param packet The buffer to read from
*	\param Lengthofpacket The size of the payload
*	\param databuffer The data contained in the payload
*	\return The packet ID of the packet
*/
int readPacket(Byte *packet, Word* Lengthofpacket, Byte *databuffer);

/** \brief Return the error message
*
*	When a error occurs, calling this functions return an error message.
*	\return The error message
*/
char * NavilinkError();
#endif // API_H_INCLUDED
