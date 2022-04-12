#ifndef _SENDER_H_
#define _SENDER_H_

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

void Input_init(void* args);
void Input_shutdown(void);

void Sender_init(void* args);
void Sender_signalSenderThread();
void Sender_shutdown(void);

#endif
