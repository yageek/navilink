#ifndef _NAVILINK_GPX
#define _NAVILINK_GPX

#include <stdio.h>

#include "navilink_protocol.h"

typedef struct {
  FILE* fd;
} NavilinkGPXFile;

int navilink_gpx_create(const char* path, NavilinkGPXFile* file);
int navilink_gpx_write(NavilinkGPXFile* file, NavilinkWaypoint* waypoint);
int navilink_gpx_close(NavilinkGPXFile* file);
#endif