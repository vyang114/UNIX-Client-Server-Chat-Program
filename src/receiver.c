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

static pthread_t threadPIDReceiver;
static pthread_t threadPIDPrinter;

static pthread_cond_t s_syncOkToPrintCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_syncOkToPrintMutex = PTHREAD_MUTEX_INITIALIZER;

static char * ptr;
static int bytesRx;

void Printer_signalPrinterThread(){

	//printf("Inside Printer signaller\n");

	pthread_mutex_lock(&s_syncOkToPrintMutex);
	{
		pthread_cond_signal(&s_syncOkToPrintCondVar);
	}
	pthread_mutex_unlock(&s_syncOkToPrintMutex);
}


void* receiverThread(void * args){

	struct thread_args *myargs = (struct thread_args *)args;
	struct sockaddr_in remote = myargs->sinRemote; 

	while(1){

		//printf("Listening on: %s  Waiting for message: \n", myargs->myPort);

		ptr = malloc(MSG_MAX_LEN*sizeof(char));
		unsigned int sin_len = sizeof(remote);  

		//int bytesRx;
		bytesRx = recvfrom(myargs->mySocket, ptr, MSG_MAX_LEN, 0, (struct sockaddr *)&remote, &sin_len);
		
		if (bytesRx > 0) {
			//messageRx[bytesRx] = 0;
			//printf("Receiving and adding to printList: %s", ptr);
			List_append(myargs->toPrintList, ptr);
			Printer_signalPrinterThread();
			//puts(messageRx);
		}
	}

	return NULL;
}


void* printerThread(void* args){

	struct thread_args *myargs = (struct thread_args *)args;

	while(1){

		// wait until signalled that there's a message to be output
		pthread_mutex_lock(&s_syncOkToPrintMutex);
		{
			pthread_cond_wait(&s_syncOkToPrintCondVar, &s_syncOkToPrintMutex);
		}
		pthread_mutex_unlock(&s_syncOkToPrintMutex);

		List_first(myargs->toPrintList);
		char * temp = List_remove(myargs->toPrintList);
		
		//printf("Receiving: ");
		write(1, temp, bytesRx);

		if(strcmp(temp, "!\n") == 0){

			pthread_mutex_destroy(&s_syncOkToPrintMutex);
			pthread_cond_destroy(&s_syncOkToPrintCondVar);
		
			free(temp);
			close(myargs->mySocket);
			end = 1;
			//printf("Exclamation mark! Value of end: %d\n", end);
			return NULL;
		}

		free(temp);
		temp = NULL;
		//printf("after freeing in printer\n");
	}
	
	return NULL;
}


void Receiver_init(void * arguments){

	pthread_create(
		&threadPIDReceiver,
		NULL,
		receiverThread, (void *)arguments);
}


void Receiver_shutdown(void){

	pthread_cancel(threadPIDReceiver);
	pthread_join(threadPIDReceiver, NULL);

	if(ptr){
		free(ptr);
		ptr = NULL;
	}
}


void Printer_init(void * arguments){

	pthread_create(
		&threadPIDPrinter,
		NULL,
		printerThread, (void *)arguments);
}


void Printer_shutdown(void){

	pthread_cancel(threadPIDPrinter);
	pthread_join(threadPIDPrinter, NULL);
}


