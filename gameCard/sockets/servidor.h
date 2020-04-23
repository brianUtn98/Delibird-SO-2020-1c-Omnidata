#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include "../src/gameCard.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

//#define MY_PORT "PUERTO_BROKER"
// Puerto al que conectarán los usuarios

#define BACKLOG 10
// Cuántas conexiones pendientes se mantienen en cola

struct sockaddr_in my_addr;

void iniciarServidor(void);

#endif /* SERVIDOR_H_ */
