#ifndef NAVILINK_ENDIANNESS
#define NAVILINK_ENDIANNESS

//Dependencies
#ifdef __APPLE__
// Includes
#include <CoreFoundation/CFByteOrder.h>
// Endianness
#define _host16_to_le(x) CFSwapInt16HostToLittle(x)
#define _le16_to_host(x) CFSwapInt16LittleToHost(x)
#define _host64_to_le(x) CFSwapInt64HostToLittle(x)
#define _le64_to_host(x) CFSwapInt64LittleToHost(x)
#elif defined(__linux__)
#include <endian.h>
#define _host16_to_le(x) htole16(x)
#define _le16_to_host(x) le16toh(x)
#define _host64_to_le(x) htole64(x)
#define _le64_to_host(x) le64toh(x)
#endif

#endif // NAVILINK_ENDIANNESS