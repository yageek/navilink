#include "navilink_errors.h"
#include <string.h>

static char message[1024];
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
  case NAVILINK_ERROR_LIBSERIALPORT_ERROR:
    return "Libserial port error";
  case NAVILINK_ERROR_QUERY_LENGTH_TOO_BIG:
    return "Query length is too big (max 32)";
  case NAVILINK_DEVICE_UNACKNOWLEDGE:
    return "The device did not acknowledge";
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

void navilink_set_current_error(NavilinkDevice* device, int code, const char* txt)
{
  // Address code number
  device->last_error_code = code;

  // Address description
  const char* desc = txt ?: description_from_error(code);
  strcpy(device->last_message_str, desc);
}