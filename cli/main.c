#include "navilink.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static struct option long_options[] = {
  { "port", required_argument, NULL, 'p' },
  { "output", required_argument, NULL, 'o' },
  { NULL, 0, NULL, 0 }
};

void usage(void)
{
  puts("Usage: navilink-cli -p [PORT_NAME] -o [GPX_NAME]");
}
int main(int argc, char** argv)
{

  int ch;
  char* port_name = NULL;
  char* output_gpx = NULL;
  // loop over all of the options
  static const char* opt_string = "p:o:";
  ch = getopt_long(argc, argv, opt_string, long_options, NULL);

  while (ch != -1) {
    // check to see if a single character or long option came through
    switch (ch) {
    // short option 't'
    case 'o':
      output_gpx = optarg;
      break;
    // short option 'a'
    case 'p':
      port_name = optarg;
      break;
    case '?':
    default:
      usage();
      return -1;
    }

    ch = getopt_long(argc, argv, opt_string, long_options, NULL);
  }

  if (port_name == NULL || output_gpx == NULL) {
    puts("No arguments provided");
    usage();
    return -1;
  }

  printf("Opening navilink on port: %s \n", port_name);
  NavilinkDevice device;
  NavilinkResult result = navilink_open_device_from_name(port_name, &device);
  if (result < 0) {
    perror(navilink_get_error_description(&device));
    return -1;
  }

  printf("A device has been found: \n");
  NavilinkInformation infos = device.informations;
  printf("\tDevice name: %s \n", infos.username);
  printf("\tDevice serial: %i \n", infos.deviceSerialNum);
  printf("\tDevice protocol version: %i \n", infos.protocolVersion);
  printf("\tTotal waypoints: %i \n", infos.totalWaypoint);
  printf("\tTotal routes: %i \n", infos.totalRoute);
  printf("\tTotal track: %i \n\n", infos.totalTrack);

  if (infos.totalWaypoint < 1) {
    printf("No waypoints to download!");
    return 0;
  }

  // Slots download
  int number_download = infos.totalWaypoint / NAVILINK_MAX_WAYPOINT_QUERY_LENGTH;
  int rest = infos.totalWaypoint % NAVILINK_MAX_WAYPOINT_QUERY_LENGTH;
  if (rest > 0) {
    number_download += 1;
  }

  NavilinkGPXFile file;
  result = navilink_gpx_create(output_gpx, &file);
  if (result) {
    perror("Can not create file");
    return -1;
  }

  NavilinkWaypoint waypoint_array[NAVILINK_MAX_WAYPOINT_QUERY_LENGTH * sizeof(NavilinkWaypoint)] = { 0 };
  for (unsigned int i = 0; i < number_download; i++) {

    printf("Download pass %i/%i \n", i + 1, number_download);
    // Format outputs
    NavilinkWaypoint* waypoint
        = &waypoint_array[i * NAVILINK_MAX_WAYPOINT_QUERY_LENGTH];
    result = navilink_query_waypoint(&device, i * NAVILINK_MAX_WAYPOINT_QUERY_LENGTH, NAVILINK_MAX_WAYPOINT_QUERY_LENGTH, waypoint);

    if (result < 0) {
       perror(navilink_get_error_description(&device));
      continue;
    }

    for (unsigned int cursor = i * NAVILINK_MAX_WAYPOINT_QUERY_LENGTH; cursor < i * NAVILINK_MAX_WAYPOINT_QUERY_LENGTH + result; cursor++) {
      navilink_gpx_write(&file, &waypoint_array[cursor]);
    }
  }

  navilink_gpx_close(&file);
  navilink_close_device(&device);
  printf("Successfully created file at: %s \n", output_gpx);
  return 0;
}