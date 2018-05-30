#include "navilink_protocol.h"
#include <string.h>
#include "navilink_endianness.h"

void _read16(uint16_t *dst, const void *src) {
    uint16_t shortBuff = 0;
    memcpy(&shortBuff, src, 2);
    *dst = _le16_to_host(shortBuff);
}

void _read32(uint32_t *dst, const void *src) {
    uint32_t shortBuff = 0;
    memcpy(&shortBuff, src, 4);
    *dst = _le32_to_host(shortBuff);
}

int navilink_read_informations(NavilinkInformation *information, uint8_t *buffer, size_t buffer_len) {
    if (buffer_len < 32) {
        return -1;
    }
    
    _read16(&information->totalWaypoint, &buffer[0]);
    memcpy(&information->totalRoute, &buffer[2], 1);
    memcpy(&information->totalTrack, &buffer[3], 1);
    _read32(&information->startAdrOfTrackBuffer, &buffer[4]);
    _read32(&information->deviceSerialNum, &buffer[8]);
    _read16(&information->numOfTrackpoints, &buffer[12]);
    _read16(&information->protocolVersion, &buffer[14]);
    memcpy(&information->username, &buffer[16], 16);

    return 0;
}

int navilink_read_datetime(NavilinkDateTime *datetime, uint8_t *buffer, size_t buffer_len) {

    if (buffer_len < NAVILINK_POSITION_PAYLOAD_LENGTH ) {
        return -1;
    }

    memcpy(&datetime->year, &buffer[0], 1);
    memcpy(&datetime->month, &buffer[1], 1);
    memcpy(&datetime->day, &buffer[2], 1);
    memcpy(&datetime->hour, &buffer[3], 1);
    memcpy(&datetime->minute, &buffer[4], 1);
    memcpy(&datetime->second, &buffer[5], 1);

    return 0;
}

int navilink_read_position(NavilinkPosition *position, uint8_t *buffer, size_t buffer_len) {
    if (buffer_len < NAVILINK_DATETIME_PAYLOAD_LENGTH ) {
        return -1;
    }

    _read32((uint32_t*) &position->latitude, &buffer[0]);
    _read32((uint32_t*) &position->longitude, &buffer[4]);
    _read16(&position->altitude, &buffer[8]);

    return 0;
}

int navilink_read_waypoint(NavilinkWaypoint *waypoint, uint8_t *buffer, size_t buffer_len) {

    if (buffer_len < NAVILINK_WAYPOINT_PAYLOAD_LENGTH) {
        return -1;
    }

    _read16(&waypoint->waypointID, &buffer[2]);
    memcpy(&waypoint->waypointName, &buffer[4], 7);
    // Gap for reserved

    int result = navilink_read_position(&waypoint->position, &buffer[12], buffer_len - 12);
    if ( result < 0) {
        return result;
    }

    result = navilink_read_datetime(&waypoint->datetime, &buffer[22], buffer_len - 22);
    if ( result < 0) {
        return result;
    }

    memcpy(&waypoint->symbolType, &buffer[28], 1);
    return 0;
}