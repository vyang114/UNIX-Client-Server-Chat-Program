#ifndef _TERMINATE_H_
#define _TERMINATE_H_

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

#include "list.h"

void Terminate_init(void* args);
void Terminate_shutdown(void);


#endif
