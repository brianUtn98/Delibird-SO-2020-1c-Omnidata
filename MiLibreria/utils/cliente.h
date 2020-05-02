#ifndef UTILS_CLIENTE_H_
#define UTILS_CLIENTE_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<unistd.h>

void conectarseAServidor(char *ip,int puerto,int tiempoReconexion);

#endif /* UTILS_CLIENTE_H_ */
