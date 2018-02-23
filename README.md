# Navilink

A simple C library for the [Navilink protocol](http://notes.splitbrain.org/navilink). This can be used to communicate
with some Locosys GPS device.

# Installation

You will require the following to build the library:

- CMake (>= 3.2)
- [libserialport](https://sigrok.org/wiki/Libserialport) (0.1.1)

To build the library:
```shell
git clone git@github.com:yageek/navilink.git
cd navilink
mkdir build && cd build
cmake ..
make 
make install
```
# Usage

Some simple example:
```c
#include <navilink/navilink.h>

#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv)
{

  NavilinkDevice device;
  int result = navilink_open_device_from_name("/dev/tty.usbserial", &device);
  if (result < 0) {
    perror("Can not open the serial port");
    exit(-1);
  }

  NavilinkInformation infos;
  result = navilink_query_information(&device, &infos);
  if (result < 0) {
    perror("Can not query the device informations");
  }
  else {
    printf("Device username: %s \n", infos.username);
    printf("Device serial: %i \n", infos.deviceSerialNum);
    printf("Device protocol version: %i \n", infos.protocolVersion);
    printf("Device number of trackspoints: %i \n", infos.numOfTrackpoints);
  }

  navilink_close_device(&device);
  return 0;
}
```

