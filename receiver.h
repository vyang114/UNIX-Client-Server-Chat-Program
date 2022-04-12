#ifndef _RECEIVER_H_
#define _RECEIVER_H_

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
#include <pthread.h>


//void* receiverThread(void * args, int s, struct sockaddr_in remote);
void Receiver_init(void* args);
//void Receiver_init(void* arg, pthread_mutex_t *psyncOkToSendMutex, pthread_cond_t *psyncOkToSendCondVar);
void Receiver_cancel(void);
void Receiver_shutdown(void);

void Printer_init(void* args);
void Printer_signalPrinterThread();
//void Sender_init(void* arg, pthread_mutex_t *psyncOkToSendMutex, pthread_cond_t *psyncOkToSendCondVar);
void Printer_cancel(void);
void Printer_shutdown(void);

#endif