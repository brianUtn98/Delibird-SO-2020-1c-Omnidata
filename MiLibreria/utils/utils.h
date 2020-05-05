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
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>

typedef enum {
	SUSCRIPCION = 1,
	MENSAJE
} t_opCode;


//por ahi se puede unificar t_colas y t_mensajes por ahora estan separados
// esto se usa en broker,pero se puede usar en cualquier lugar para enumerar el switch y tener un protocolo comun
typedef enum t_colaMensaje {
	tNEW_POKEMON = 1,
	tAPPEARED_POKEMON,
	tCATCH_POKEMON,
	tCAUGTH_POKEMON,
	tGET_POKEMON,
	tLOCALIZED_POKEMON,

	tFinDeProtocolo //NO SACAR Y DEJAR A LO ULTIMO!!!
} t_colaMensaje;

typedef struct {
	uint32_t id;
    uint32_t idCorrelacional;
} t_mensaje;


typedef struct {
	int size;
	void* stream;
} t_buffer;

//typedef struct {
//	t_buffer* buffer;
//	op_code codigo_operacion;
//} t_paquete;

typedef struct {
	t_buffer* buffer;
	t_colaMensaje colaMensaje;
	t_opCode codigoOperacion;
} t_paquete;

typedef struct {
	char *ip;
	int puerto;
} t_suscriptor;

void* serializarPaquete(t_paquete* paquete, int bytes);
int crearConexion(char *ip, int puerto, int tiempo_reconexion);
void enviarMensaje(char *mensaje, int socket);
void liberarConexion(int socket);
t_paquete *recibirMensaje(int socket_cliente, int* size);
void crearMensaje(void* payload, int size, int socket_cliente);


#endif/*UTILS_UTILS_H*/
