#include<stdio.h>
#include<string.h>

/*Unix headers part*/

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>


#include "NaviGPSapi.h"

#define CLR_RX_BUFFER() memset(recievingbuffer,0,MAX_PACKET_SIZE)
#define CLR_TX_BUFFER() memset(transmitbuffer,0,MAX_PACKET_SIZE)

Byte transmitbuffer[MAX_PACKET_SIZE];
Byte recievingbuffer[MAX_PACKET_SIZE];

static struct termios options;


int init_gps_serial_link(NaviGPS * dev){

	
	dev->fd = open(dev->deviceName, O_NOCTTY | O_RDWR | O_NONBLOCK);
	
	if(dev->fd < 0){
		perror("Unable to open the serial file");
		return -1;
	}
    
       /* allow the process to receive SIGIO */
        fcntl(dev->fd, F_SETOWN, getpid());
        /* Make the file descriptor asynchronous (the manual page says only 
           O_APPEND and O_NONBLOCK, will work with F_SETFL...) */
        fcntl(dev->fd, F_SETFL, FASYNC);
	
	
	//fcntl(dev->fd,F_SETFL,0); /* Change configuration to block until a chracters is on the line */
	//
	tcgetattr(dev->fd,&options); /*Get the current options of the current file */
	
	/*Baudrate option*/
	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);
	
	/*Control options */
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	
	options.c_cflag|=CREAD|CLOCAL;
    options.c_lflag&=(~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|ISIG));
    options.c_iflag&=(~(INPCK|IGNPAR|PARMRK|ISTRIP|ICRNL|IXANY));
    options.c_oflag&=(~OPOST);
    options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 1;
    
	tcflush(dev->fd,TCIFLUSH);
	tcsetattr(dev->fd, TCSANOW, &options);
	
	initIPC();
	
	return dev->fd;
}
int read_test(NaviGPS *dev){
	Byte car;
	read(dev->fd,&car,1);
	printf("Retour : %c\n",car);
	return 0;
	
	
	
}
int write_test(NaviGPS*dev){
	Byte car ='a';
	return write(dev->fd,&car,1);
	
}

int close_gps_serial_link(NaviGPS *dev){

	return close(dev->fd);

}

