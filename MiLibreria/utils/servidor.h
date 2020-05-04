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


pthread_t thread;

//void* recibirBuffer(int*, int); Esta funcion no tiene implementacion, no recuerdo que hace aca.
//Así que la comento

int iniciarServidor(char *ip,int puerto);
int createSocket(void);
void esperarCliente(int);
void processRequest(int cod_op, int clienteFd);
void serveClient(int *socket);
void devolverMensaje(void* payload, int size, int socketCliente);
void* recibirUnMensaje(int socket, int *bufferSize);



#endif //SERVIDOR_H
