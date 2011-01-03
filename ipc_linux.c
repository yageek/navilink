#include "ipc_linux.h"
#include "NaviGPSapi.h"
extern NaviGPS *bgt31;
extern Byte recievingbuffer[MAX_PACKET_SIZE];
struct sigaction sigaction_IOSignal;
struct sigaction sigaction_INTSignal;

void initIPC(){
	
	sigaction_INTSignal.sa_sigaction = handle_INT;
	sigaction_INTSignal.sa_flags = SA_SIGINFO;
	
	sigaction_IOSignal.sa_sigaction = handle_IO;
	sigaction_IOSignal.sa_flags = SA_SIGINFO;
	
	
	sigaction(SIGINT,&sigaction_INTSignal,NULL);	
	sigaction(SIGIO,&sigaction_IOSignal,NULL);	
	
}

void handle_IO(int signum,siginfo_t *info, void *ptr){
	printf("Signal : IO\n");
	printf("Received signal %d\n", signum);
    printf("Signal originates from process %lu\n",(unsigned long)info->si_pid);
    Byte car;
	read(bgt31->fd,&car,1);
	printf("Retour : %c\n",car);

	
}

void handle_INT(int signum,siginfo_t *info, void *ptr){
	printf("Signal : INT\n");
	printf("Received signal %d\n", signum);
    printf("Signal originates from process %lu\n",(unsigned long)info->si_pid);
    exit(0);

	
}

