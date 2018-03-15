#include "navilink_packet.h"
#include "navilink_endianness.h"
#include "navilink_errors.h"
#include "navilink_internal.h"

#include <string.h>

uint16_t navilink_compute_checksum(const uint8_t* payloaddata, uint16_t size)
{
  uint16_t index = 0;
  uint16_t checksum = 0;

  while (index < size) {
    checksum += payloaddata[index];
    index++;
  }

  checksum &= 0x7FFFUL;
  return checksum;
}

int navilink_create_packet(uint8_t* dst, uint8_t type, const uint8_t* payload, uint16_t payload_length)
{
  // Compute:
  // The first byte of the payload is always the packet id (aka type) -> +1
  // Start bytes (2) + payload length (2) + crc(2) + footer(2) -> + 8
  size_t packet_length = payload_length + 9;
  uint16_t startBytes = (NAVILINK_PACK_START2 << 8) | NAVILINK_PACK_START1;
  uint16_t endBytes = (NAVILINK_PACK_END2 << 8) | NAVILINK_PACK_END1;

  // If packet is too big, skip and fail.
  if (packet_length > NAVILINK_MAX_PAYLOAD_SIZE) {
    return -1;
  }

  // Copy start bytes
  memcpy(dst, &startBytes, 2);

  // Copy size
  uint16_t size = _host16_to_le(payload_length + 1);
  memcpy(dst + 2, &size, 2);

  // Copy packet type
  memcpy(dst + 4, &type, type);

  uint8_t* payload_packet = NULL;
  if (payload != NULL) {
    memcpy(dst + 5, &payload[0], payload_length);
  }

  uint16_t checksum = navilink_compute_checksum(dst + 4, payload_length + 1);
  memcpy(dst + payload_length + 5, &checksum, 2);
  // Copy start bytes
  memcpy(dst + payload_length + 7, &endBytes, 2);
  return packet_length;
}

int navilink_read_packet(NavilinkPacket* packet, const uint8_t* src)
{
  uint16_t startBytes = (NAVILINK_PACK_START2 << 8) | NAVILINK_PACK_START1;
  uint16_t endBytes = (NAVILINK_PACK_END2 << 8) | NAVILINK_PACK_END1;

  // Check the first bytes
  uint16_t start = 0;
  memcpy(&start, src, 2);

  if (start != startBytes) {
    return -1;
  }

  // Get size
  memcpy(&packet->payload_length, src + 2, 2);

  // Check End
  uint16_t end = 0;
  memcpy(&end, src + 6 + packet->payload_length, 2);
  if (end != endBytes) {
    return -1;
  }

  // Get payload
  memcpy(&packet->payload[0], src + 4, packet->payload_length);

  // Get checksum
  uint16_t checksum = 0;
  memcpy(&checksum, src + 4 + packet->payload_length, 2);

  uint16_t computed_checksum = navilink_compute_checksum(packet->payload, packet->payload_length);
  if (checksum != computed_checksum) {
    return -1;
  }

  packet->type = packet->payload[0];
  memcpy(&packet->payload[0], &packet->payload[1], packet->payload_length - 1);

  return 0;
}