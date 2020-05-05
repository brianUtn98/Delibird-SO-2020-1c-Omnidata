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


int iniciarServidor(char *ip,int puerto);
int createSocket(void);
//void esperarCliente(int);
//void processRequest(int cod_op, int clienteFd);
//void serveClient(int *socket);
void crearMensaje(void* payload, int size, int socketCliente);
void* recibirMensaje(int socket, int *bufferSize);

int init_server(int port);
int create_socket(void);

#endif //SERVIDOR_H
