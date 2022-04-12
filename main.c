#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <arpa/inet.h> 
#include <pthread.h> 

#include "helper.h"
#include "sender.h"
#include "receiver.h"
#include "list.h"
#include "terminate.h"


int createSocket(void * args){

	struct thread_args *myargs = (struct thread_args *)args;

	struct sockaddr_in sin;      // my address 
	//printf("Inside createSocket\n");
	int myPort = atoi(myargs -> myPort);

	int s;                         // my socket 
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("cannot create socket\n");
        return 0;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);	// any address
    sin.sin_port = htons(myPort);	// host to network short

    // bind the socket
    if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        printf("bind failed.\n");
        return 0;
    }

    //printf("My port:%d   ", myPort);

	return s;
}


struct sockaddr_in setUpRemote(void * args){

	struct thread_args *myargs = (struct thread_args *)args;

	struct sockaddr_in sinRemote;     // remote address 
	int remotePort = atoi(myargs -> remotePort);

	struct hostent *h;     // remote information 
	char * remoteMachine = myargs ->remoteMachine;	// name of the remote

	memset(&sinRemote, 0, sizeof(sinRemote));
	sinRemote.sin_family = AF_INET;
	sinRemote.sin_port = htons(remotePort);

	// get IP address for remote 
    h = gethostbyname(remoteMachine);
    if (!h) {
		printf("gethostbyname failed.\n");
	}

	memcpy((void *)&sinRemote.sin_addr, h->h_addr_list[0], h->h_length);
	//printf("RemoteMachine:%s   RemotePort:%d\n", hp->h_name, remotePort);

	return sinRemote;
}


int main(int argc, char **argv){

	end = 0;

	struct thread_args *args;
	args = malloc(sizeof(struct thread_args));
	args->myPort = argv[1];
	args->remoteMachine = argv[2];
	args->remotePort = argv[3];

	args->mySocket = createSocket((void *)args);
	args->sinRemote = setUpRemote((void *)args);

	args->toSendList = List_create();
	args->toPrintList = List_create();

	Input_init((void *)args);
	Terminate_init((void *)args);
	Sender_init((void *)args);
	Receiver_init((void *)args);
	Printer_init((void *)args);

	//printf("inside main value of end: %d\n", end);

	Terminate_shutdown();
	free(args);

	//printf("inside main after all shutdown()\n");
}

