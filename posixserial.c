#include<stdio.h>
#include<string.h>

/*Unix headers part*/
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<termios.h>

#include "NaviGPSapi.h"

#define CLR_RX_BUFFER() memset(recievingbuffer,0,MAX_PACKET_SIZE)
#define CLR_TX_BUFFER() memset(transmitbuffer,0,MAX_PACKET_SIZE)

static Byte transmitbuffer[MAX_PACKET_SIZE];
static Byte recievingbuffer[MAX_PACKET_SIZE];
static struct termios options;

int init_gps_serial_link(NaviGPS * dev){

	
	dev->fd = open(dev->deviceName, O_NOCTTY | O_NDELAY | O_RDWR);
	
	if(dev->fd < 0){
		perror("Unable to open %s",device);
		return -1;
	}
	
	fcntl(dev->fd,F_SETFL,0); /* Change configuration to block until a chracters is on the line */
	
	tcgetattr(dev->fd,&options); /*Get the current options of the current file */
	
	/*Baudrate option*/
	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);
	
	/*Control options */
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~PARENB
	options.c_cflag &= ~CSTOPB
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	
	/*Local options */
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	
	/*Flow control*/
	/*options.c_iflag |= (IXON | IXOFF | IXANY);*/
	options.c_iflag &= ~(IXON | IXOFF | IXANY);
	
	/*Output options*/
	options.c_oflag &= ~OPOST;/* Raw Output*/
	/*Control timeout and cahracters (test)*/
	options.c_cc[VMIN] = 2;
	
	tcsetattr(dev->fd, TCSANOW, &options);
	
	return dev->fd;
}

int read_packet_from_gps(NaviGPS *dev){
	
	CLR_RX_BUFFER();
	Byte *ptr = recievingbuffer;
	int nbytes;
	Word length;
	
	while( nbytes = read(dev->fd,recievingbuffer,recievingbuffer + sizeof(recievingbuffer) - ptr)){
		if((ptr[-1] == PACK_START2) && ((ptr[-2] == PACK_START1))) break;
		ptr+=nbytes;
	}
	ptr = recievingbuffer;
	while( nbytes = read(dev->fd,recievingbuffer,recievingbuffer + sizeof(recievingbuffer) - ptr)){
		if((ptr[-1] == PACK_END2) && ((ptr[-2] == PACK_END1))) break;
		ptr+=nbytes;
		
	}
	
	
	
	
	


}

int write_packet_to_gps(NaviGPS *dev, Byte type, Byte *data, Word size){

	setPacket(transmitbuffer,type,data,size);
	
	return write(dev->fd,transmitbuffer,size);

}

int close_gps_serial_link(NaviGPS *dev){

	close(fd);

}


