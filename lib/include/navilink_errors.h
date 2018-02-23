#ifndef __NAVILINK_ERRORS
#define __NAVILINK_ERRORS

static const int NAVILINK_ERROR_PAYLOAD_SIZE = 10;
static const int NAVILINK_ERROR_OPEN_DEVICE_ERROR = 20;

/**
 * @brief Get the description about the error
 * 
 * @return const char* The description of the error
 */
const char* navilink_get_error_description();

/**
 * @brief Get the error code of the error.
 * 
 * @return int The error code of the last error.
 */
int navilink_get_error_code();

#endif //__NAVILINK_ERRORS
