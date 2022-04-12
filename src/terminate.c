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
#include <unistd.h>

#include "helper.h"
#include "terminate.h"
#include "sender.h"
#include "receiver.h"
#include "list.h"

static pthread_t threadPID;


void free_item(void * n){

	if(n != NULL) {
		free(n);
		n = NULL;
	}

/*	if(n == NULL) {
       printf("item freed\n");
    }*/
}


void* terminateThread(void* args){

	struct thread_args *myargs = (struct thread_args *)args;

	void (*free_ptr)(void*);	// Function pointer declaration
    free_ptr = &free_item;	// Assign function address to pointer

	while(1){

		if(end == 1){

//			printf("Inside terminate\n");

			Input_shutdown();
			Sender_shutdown();
			Receiver_shutdown();
			Printer_shutdown();

			//printf("after shutdown of all 4 threads\n");
			List_free(myargs->toSendList, free_ptr);
			List_free(myargs->toPrintList, free_ptr);
			//printf("afterr freeing the lists!!!!!!!!\n");

			return NULL;
		}
	}
	
	return NULL;
}


void Terminate_init(void * arguments){

	pthread_create(
		&threadPID,
		NULL,
		terminateThread, (void *)arguments);
}


void Terminate_shutdown(void){

	pthread_join(threadPID, NULL);
}
