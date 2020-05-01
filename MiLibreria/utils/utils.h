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
// esto se usa en broker,pero se puede usar en cualquier lugar para enumerar el switch y tener un protocolo comun
typedef enum t_colas {
	tNEW_POKEMON = 1,
	tAPPEARED_POKEMON,
	tCATCH_POKEMON,
	tCAUGTH_POKEMON,
	tGET_POKEMON,
	tLOCALIZED_POKEMON,

	tFinDeProtocolo //NO SACAR Y DEJAR A LO ULTIMO!!!
} t_colas;

/*
//esto todavia no lo usamos pero no borrar
typedef enum t_protocolo {
	tAdministrativo = 1,
	tPath,
	tWrite,
	tRead,
	tDIR,
	tFinDeProtocolo //NO SACAR Y DEJAR A LO ULTIMO!!!
} t_protocolo;
typedef enum t_tipoOperacion{
	tGetattr = 1,
	tMkdir,
	tRmdir,
	tOpen,
	tRelease,
	tFlush,
	tReleaseDir,
	tOpenDir,
	tFinTipoOperacion //NO SACAR Y DEJAR A LO ULTIMO!!!
} t_tipoOperacion;

*/


typedef struct {
int size;
void* stream;
}t_buffer;

typedef struct {
t_buffer* buffer;
op_code codigo_operacion;
}t_paquete;

void* serializar_paquete(t_paquete *paquete,int *bytes);
int crear_conexion(char *ip,int  puerto, int tiempo_reconexion);
void enviar_mensaje(char *mensaje,int socket);
char* recibir_mensaje(int socket,int *buffer_size);
void liberar_conexion(int socket);



#endif/*UTILS_UTILS_H*/
