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

void* recibir_buffer(int*, int);

void iniciar_servidor(char *IP,int PUERTO);// el puerto es int
void esperar_cliente(int);
void process_request(int cod_op, int cliente_fd);
void serve_client(int *socket);
void devolver_mensaje(void* payload, int size, int socket_cliente);



#endif //SERVIDOR_H
