#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

typedef enum
{
	MENSAJE = 1,
}op_code;

typedef struct {
int size;
void* stream;
}t_buffer;

typedef struct {
t_buffer* buffer;
op_code codigo_operacion;
}t_paquete;

void* serializar_paquete(t_paquete *paquete,int *bytes);
int crear_conexion(char *ip,char *puerto);
void enviar_mensaje(char *mensaje,int socket);
char* recibir_mensaje(int socket);
void liberar_conexion(int socket);



#endif/*UTILS_UTILS_H*/
