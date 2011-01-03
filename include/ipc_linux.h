#ifndef IPC_LINUX_INCLUDED
#define IPC_LINUX_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

void initIPC();
void handle_IO(int signum,siginfo_t *info, void *ptr);
void handle_INT(int signum,siginfo_t *info, void *ptr);







#endif
