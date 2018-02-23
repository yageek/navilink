#include "navilink_errors.h"
#include <string.h>

#define ERROR_MESS_LENGTH 256

static char navilinkError[ERROR_MESS_LENGTH] = "";
static int navilinkErrorCode = 0;

int navilink_get_error_code()
{
  return navilinkErrorCode;
}

const char* navilink_get_error_description()
{
  return navilinkError;
}

const char* description_from_error(int error)
{

  switch (error) {
  case NAVILINK_ERROR_PAYLOAD_SIZE:
    return "The provided payload of too big";
  default:
    return "Unknown error code";
  }
}

void set_current_error(int code)
{
  navilinkErrorCode = code;
  strcpy(navilinkError, description_from_error(code));
}