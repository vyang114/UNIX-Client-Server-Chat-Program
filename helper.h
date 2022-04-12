#ifndef _HELPER_H_
#define _HELPER_H_

#include "list.h"

#define MSG_MAX_LEN 1024

extern int end;

struct thread_args{
	char *myPort;
	char *remoteMachine;
	char *remotePort;
	int mySocket;
	struct sockaddr_in sinRemote; 
	List *toSendList; 
	List *toPrintList; 

};


#endif
