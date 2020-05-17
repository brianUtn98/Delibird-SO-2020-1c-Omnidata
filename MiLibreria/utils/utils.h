#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>

//typedef enum {
//	SUSCRIPCION = 1, MENSAJE
//} t_opCode;

typedef enum {
	MENSAJE_NEW_POKEMON = 1,
	MENSAJE_APPEARED_POKEMON,
	MENSAJE_CATCH_POKEMON,
	MENSAJE_CAUGHT_POKEMON,
	MENSAJE_GET_POKEMON,
	MENSAJE_LOCALIZED_POKEMON,
	SUSCRIBIRSE_NEW_POKEMON,
	SUSCRIBIRSE_APPEARED_POKEMON,
	SUSCRIBIRSE_CATCH_POKEMON,
	SUSCRIBIRSE_CAUGHT_POKEMON,
	SUSCRIBIRSE_GET_POKEMON,
	SUSCRIBIRSELOCALIZED_POKEMON

} t_header;

//por ahi se puede unificar t_colas y t_mensajes por ahora estan separados
// esto se usa en broker,pero se puede usar en cualquier lugar para enumerar el switch y tener un protocolo comun
typedef enum t_colaMensaje {
	tNEW_POKEMON = 1,
	tAPPEARED_POKEMON,
	tCATCH_POKEMON,
	tCAUGHT_POKEMON,
	tGET_POKEMON,
	tLOCALIZED_POKEMON,

	tFinDeProtocolo //NO SACAR Y DEJAR A LO ULTIMO!!!
} t_colaMensaje;

//typedef struct {
//	uint32_t id;
//	uint32_t idCorrelacional;
//} t_mensaje;

//typedef struct {
//	int size;
//	void* stream;
//} t_buffer;

//typedef struct {
//	uint32_t size;
//	void* stream;
//} t_buffer;

typedef struct {
	int x;
	int y;
} t_posicion;
//typedef struct {
//	t_buffer* buffer;
//	int header;
//} t_paquete;

typedef struct {
	bool booleano;
	uint32_t posX;
	uint32_t posY;
	uint32_t cantidadPokemons;
	uint32_t largoNombre;
	uint32_t cantidadDePares;
	t_list *paresDeCoordenadas;
	char *nombrePokemon;

}t_mensaje;

typedef struct{
	uint32_t size;
	t_mensaje *mensaje;
}t_buffer;

typedef struct {
	pid_t pid;
	t_header codigoOperacion;
	t_buffer* buffer;
}__attribute__((packed)) t_paquete;



//Funciones generales
int crearConexion(char *ip, int puerto, int tiempo_reconexion);
void liberarConexion(int socket);
void* serializarPaquete(t_paquete* paquete, int *bytes);
t_paquete *recibirMensaje(int socketCliente);
void liberarPaquete(t_paquete *paquete);
void liberarMensaje(t_mensaje *mensaje);
//void devolverMensajeConfirmacion(void* layout, int socket_cliente);
//char* recibirConfirmacion(int socket_cliente);


//Mensaje NEW_POKEMON
void *serializarMensajeNewPokemon(t_mensaje *mensaje,int *bytes);
void crearMensajeNewPokemon(pid_t pid, char* nombrePokemon, uint32_t posX,
		uint32_t posY, uint32_t cantidadPokemons, int socketCliente);

//Mensaje GET_POKEMON
void *serializarMensajeGetPokemon(t_mensaje *mensaje,int *bytes);
void crearMensajeGetPokemon(int pid, char* nombrePokemon, int socket_cliente);

//Mensaje APPEARED_POKEMON
void *serializarMensajeAppearedPokemon(t_mensaje *mensaje,int *bytes);
void crearMensajeAppearedPokemon(int pid, char* nombrePokemon, int posX,
		int posY, int socket_cliente);

//Mensaje CATCH_POKEMON
void* serializarMensajeCatchPokemon(t_paquete* paquete, int *bytes);
void crearMensajeCatchPokemon(int pid, char* nombrePokemon, int posX, int posY,
		int socket_cliente);

//Mensaje CAUGTH_POKEMON
void* serializarPaqueteCaughtPokemon(t_paquete* paquete, int *bytes);
void crearMensajeCaughtPokemon(int pid, int booleano, int socket_cliente);

//Mensaje LOCALIZED_POKEMON
void* serializarPaqueteLocalizedPokemon(t_paquete* paquete, int *bytes);
void crearMensajeLocalizedPokemon(int pid, char* nombrePokemon, int posX,
		int posY, int cantidadPokemons, int socket_cliente);//Pensando este mensaje todavía





#endif/*UTILS_UTILS_H*/
