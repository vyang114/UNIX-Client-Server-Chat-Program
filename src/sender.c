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
#include "sender.h"
#include "receiver.h"
#include "list.h"

static pthread_t threadPIDInput;
static pthread_t threadPIDSend;

static pthread_cond_t s_syncOkToSendCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_syncOkToSendMutex = PTHREAD_MUTEX_INITIALIZER;

static char * ptr = NULL;


void Sender_signalSenderThread(){

	//printf("Inside signaller\n");

	pthread_mutex_lock(&s_syncOkToSendMutex);
	{
		pthread_cond_signal(&s_syncOkToSendCondVar);
	}
	pthread_mutex_unlock(&s_syncOkToSendMutex);
}


void* inputThread(void* args){

	struct thread_args *myargs = (struct thread_args *)args;
	
	while(1){

		ptr = malloc(MSG_MAX_LEN*sizeof(char));		// malloc a new message
		fgets(ptr, MSG_MAX_LEN, stdin);				// fgets the input and store in ptr
		//printf("next count: %d\n", count);

		List_append(myargs->toSendList, ptr);		// add the message to the list
		//printf("Ptr to send: %d\n", ptr);

		// signal the Sender Thread
		Sender_signalSenderThread();				// signal senderThread that there's a message to be sent
	}
	
	return NULL;
}


void* senderThread(void * args){

	struct thread_args *myargs = (struct thread_args *)args;
	struct sockaddr_in remote = myargs->sinRemote; 

	while(1){

		// wait until signalled that there's a message to be sent
		pthread_mutex_lock(&s_syncOkToSendMutex);
		{
			pthread_cond_wait(&s_syncOkToSendCondVar, &s_syncOkToSendMutex);
		}
		pthread_mutex_unlock(&s_syncOkToSendMutex);

		List_first(myargs->toSendList);					// get the first item on the list

		//printf("First item to send: %s\n", *(char*)List_first(myargs->toSendList));
		
		char * temp = List_remove(myargs->toSendList);	// remove it
		//printf("current item to send: %s\n", temp);
		
		if (sendto(myargs->mySocket, temp, strlen(temp), 0, (struct sockaddr *)&remote, sizeof(remote)) < 0) {	// send it
			perror("sendto failed");
			return 0;
		}

		if(strcmp(temp, "!\n") == 0){		// terminate the thread if input is "!"

			pthread_mutex_destroy(&s_syncOkToSendMutex);
			pthread_cond_destroy(&s_syncOkToSendCondVar);
		
			free(temp);
			end = 1;
			//printf("Exclamation mark! Value of end: %d\n", end);

			return NULL;
		}

		//printf("Message sent!\n");
		free(temp);
		temp = NULL;
	}

	return NULL;
}


void Input_init(void * arguments){

	pthread_create(
		&threadPIDInput,
		NULL,
		inputThread, (void *)arguments);
}


void Input_shutdown(void){

	pthread_cancel(threadPIDInput);
	pthread_join(threadPIDInput, NULL);
	
	if(ptr){
		free(ptr);
		ptr = NULL;
	}	
}


void Sender_init(void * arguments){
	
	pthread_create(
		&threadPIDSend,
		NULL,
		senderThread, (void *)arguments);
}


void Sender_shutdown(void){

	pthread_cancel(threadPIDSend);
	pthread_join(threadPIDSend, NULL);
}


