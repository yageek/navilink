#include "navilink_errors.h"
#include <string.h>

const char* description_from_error(int error)
{

  switch (error) {
  case NAVILINK_ERROR_PAYLOAD:
    return "The provided payload is invalid";
  default:
    return "Unknown error code";
  }
}

int navilink_get_error_code(NavilinkDevice* device)
{
  return device->last_error_code;
}

const char* navilink_get_error_description(NavilinkDevice* device)
{
  return description_from_error(device->last_error_code);
}

void set_current_error(NavilinkDevice* device, int code)
{
  device->last_error_code = code;
}