#include "navilink_errors.h"
#include <string.h>

static char message[256];
const char* description_from_error(int error)
{
 
  switch (error) {
  case NAVILINK_ERROR_PAYLOAD:
    return "The provided payload is invalid";
  case NAVILINK_ERROR_INVALID_START_BYTE:
    return "The start byte is invalid";
  case NAVILINK_ERROR_INVALID_END_BYTE:
    return "Then end byte is invalid";
  case NAVILINK_ERROR_CHECKSUM:
    return "The checksum is invalid";
  case NAVILINK_ERROR_OPEN_DEVICE_ERROR:
    return  "Impossible to create a connection to the device";
  default:
    sprintf(message, "Unknown error code: %i", error);
    return message;
  }
}

int navilink_get_error_code(NavilinkDevice* device)
{
  return device->last_error_code;
}

const char* navilink_error_description(int error)
{
  return description_from_error(error);
}
const char* navilink_get_error_description(NavilinkDevice* device)
{
  return description_from_error(device->last_error_code);
}

void set_current_error(NavilinkDevice* device, int code)
{
  device->last_error_code = code;
}