#ifndef __NAVILINK_INTERNAL
#define __NAVILINK_INTERNAL
struct NavilinkDevice;
/// Errors
void set_current_error(NavilinkDevice* device, int code);
void DumpHex(const void* data, size_t size);
/// Serialization

#endif