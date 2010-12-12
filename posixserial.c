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

	
	dev->fd = open(dev->deviceName, O_NOCTTY | O_RDWR);
	
	if(dev->fd < 0){
		perror("Unable to open the serial file");
		return -1;
	}
	
	//fcntl(dev->fd,F_SETFL,0); /* Change configuration to block until a chracters is on the line */
	//
	tcgetattr(dev->fd,&options); /*Get the current options of the current file */
	
	/*Baudrate option*/
	//cfsetispeed(&options, B115200);
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
	
	/*Local options */
	//options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	
	/*Hardware flow control off*/
	//options.c_cflag &= ~CRTSCTS;

	/*software Flow control ON*/
	//options.c_iflag |= (IXON | IXOFF | IXANY);
	//options.c_iflag &= ~(IXON | IXOFF | IXANY);
	
	/*Output options*/
	//options.c_oflag &= ~OPOST;/* Raw Output*/
	/*Control timeout and cahracters (test)*/
	//options.c_cc[VMIN] = 0;	
	//options.c_cc[VTIME] = 20;
	tcflush(dev->fd,TCIFLUSH);
	tcsetattr(dev->fd, TCSANOW, &options);
	
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


int read_packet_from_gps(NaviGPS *dev){
	
	CLR_RX_BUFFER();
	Byte *ptr = recievingbuffer;
	int nbytes;
	Word length;
	/*
	while( nbytes = read(dev->fd,recievingbuffer,recievingbuffer + sizeof(recievingbuffer) - ptr)){
		if((ptr[-1] == PACK_START2) && ((ptr[-2] == PACK_START1))) break;
		ptr+=nbytes;
	}
	ptr = recievingbuffer;
	while( nbytes = read(dev->fd,recievingbuffer,recievingbuffer + sizeof(recievingbuffer) - ptr)){
		if((ptr[-1] == PACK_END2) && ((ptr[-2] == PACK_END1))) break;
		ptr+=nbytes;
		
	}
	*/
	int n = read(dev->fd,recievingbuffer,1);
	printf("Number of bytes getted : %d\n",n);
	for (n=0; n <9;n++)  printf("%#x ", recievingbuffer[n]);
	
	
	
	
	


}

int write_packet_to_gps(NaviGPS *dev, Byte type, Byte *data, Word size){

	int n = setPacket(&transmitbuffer[0],type,data,size);
	
	n = write(dev->fd,&transmitbuffer[0],1);
	
	return n;

}

int close_gps_serial_link(NaviGPS *dev){

	close(dev->fd);

}


