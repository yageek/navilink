#ifndef __NAVILINK_ERRORS
#define __NAVILINK_ERRORS

#include "navilink_device.h"
#define NAVILINK_ERROR_INVALID_START_BYTE -5
#define NAVILINK_ERROR_INVALID_END_BYTE -7
#define NAVILINK_ERROR_PAYLOAD -10
#define NAVILINK_ERROR_CHECKSUM -15
#define NAVILINK_ERROR_OPEN_DEVICE_ERROR -20

/**
 * @brief Get the description about the error
 * 
 * @return const char* The description of the error
 */
const char* navilink_get_error_description(NavilinkDevice* device);
const char* navilink_error_description(int error);
/**
 * @brief Get the error code of the error.
 * 
 * @return int The error code of the last error.
 */
int navilink_get_error_code(NavilinkDevice* device);

#endif //__NAVILINK_ERRORS
