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

#ifndef NAVILINK_API_H_INCLUDED
#define NAVILINK_API_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "navilink_protocol.h"
typedef struct {
  uint8_t type;
  uint8_t payload[NAVILINK_MAX_PAYLOAD_SIZE];
  uint16_t payload_length;
} NavilinkPacket;

int navilink_create_packet(uint8_t* dst, uint8_t type, const uint8_t* payload, uint16_t payload_length);

int navilink_read_packet(NavilinkPacket* packet, const uint8_t* src);
#endif // NAVILINK_API_H_INCLUDED
