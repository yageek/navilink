#include "navilink_gpx.h"
#include <assert.h>

#define __GPX_PREFIX "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n  \
<gpx version=\"1.0\" creator=\"navilink\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.topografix.com/GPX/1/0\" \
xsi:schemaLocation=\"http://www.topografix.com/GPX/1/0 http://www.topografix.com/GPX/1/0/gpx.xsd\">\n"

#define __GPX_SUFFIX "</gpx>"

#define __GPX_WAYPOINT_TEMPLATE "<wpt lat=\"%f\" lon=\"%f\">\n \
  <name>%s</name>\n \
  <ele>%f</ele>\n \
  <sym>%s</sym>\n \
  <time>%04d-%02d-%02dT%02d:%02d:%02d</time>\n \
</wpt>"

// From https://github.com/gpsbabel/gpsbabel/blob/master/navilink.cc
static const char* const icon_table[] = {
  "Star",
  "Flag",
  "House",
  "Left Sign",
  "Telegraph Pole",
  "People",
  "Fuel",
  "Phone",
  "Pole",
  "Mountain",
  "Water",
  "Tree",
  "Road Narrows",
  "Crossroads",
  "Road Fork",
  "Turn Right",
  "Turn Left",
  "Bird",
  "3D House",
  "Trig Point",
  "Tower",
  "Cable Car",
  "Church",
  "Telegraph Pole",
  "Skier",
  "Anchor",
  "Fish",
  "Fishes",
  "Rain",
  "Fisherman",
  "Tower",
  "Boats",
  "Boat",
  "Bicycle",
  "Railway Track",
  "Dollar Sign",
  "Bus",
  "Camera",
  "Fuel Pump",
  "Cup",
  "Merging Road",
  "Plane",
  "Red Cross",
  "House",
  "Parking"
};

const char* description_from_symbol(char symbol)
{
  if (symbol > 31 || symbol < 0) {
    return "Unknown";
  }
  return icon_table[symbol];
}

NavilinkResult navilink_gpx_create(const char* path, NavilinkGPXFile* file)
{
  assert(path != NULL);

  FILE* fd = fopen(path, "w");
  if (fd == NULL) {
    return NavilinkErr;
  }

  // Prefix
  fprintf(fd, "%s", __GPX_PREFIX);

  file->fd = fd;
  return NavilinkOK;
}

NavilinkResult navilink_gpx_write(NavilinkGPXFile* file, NavilinkWaypoint* waypoint)
{
  FILE* fd = file->fd;
  double lat = (double)waypoint->position.latitude / 10000000;
  double lon = (double)waypoint->position.longitude / 10000000;
  double altitude = (double)waypoint->position.altitude * 0.3048;
  NavilinkDateTime date = waypoint->datetime;

  fprintf(fd, __GPX_WAYPOINT_TEMPLATE,
          lat, lon,
          waypoint->waypointName,
          altitude,
          description_from_symbol(waypoint->symbolType),
          date.year + 2000, date.month, date.day, date.hour, date.minute, date.second);

  return NavilinkOK;
}

NavilinkResult navilink_gpx_close(NavilinkGPXFile* file)
{
  // Suffix
  fprintf(file->fd, "%s", __GPX_SUFFIX);
  if (fclose(file->fd) < 1) {
    return NavilinkErr;
  }
  return NavilinkOK;
}