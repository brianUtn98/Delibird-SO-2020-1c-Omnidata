#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include "utils.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <string.h>

int createSocket(void);
int initServer(char* ip, int port);

#endif //SERVIDOR_H
