#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>

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

void* serializar_paquete(t_paquete*,int*);
int crear_conexion(char*,char*);
void enviar_mensaje(char*,int);
char* recibir_mensaje(int);
void liberar_conexion(int)



#endif
