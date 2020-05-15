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

//typedef enum {
//	SUSCRIPCION = 1, MENSAJE
//} t_opCode;

typedef struct t_administrativo {
	int16_t codigo;
	uint32_t valor;
}__attribute__((packed)) t_administrativo;

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

typedef struct {
	int size; // Tamaño del payload
	void* stream; // Payload
}__attribute__((packed)) t_buffer;

typedef struct {
	int x;
	int y;
} t_posicion;

typedef struct {
	int pid;
	t_header codigoOperacion;

	int posX;
	int posY;
	int cantidadPokemons;
	int largoNombre;
	char*nombrePokemon;

	t_buffer* buffer;
}__attribute__((packed)) t_paquete;

typedef struct {
	int size;
	int posX;
	int posY;
	int cantidadPokemons;
	int largoNombre;
	char* nombrePokemon;

}__attribute__((packed)) t_mensajeNew;

void* serializarPaqueteNewPokemon(t_paquete* paquete);
int crearConexion(char *ip, int puerto, int tiempo_reconexion);
void enviarMensaje(char *mensaje, int socket);
void liberarConexion(int socket);
t_paquete *recibirMensajeNewPokemon(int socket_cliente, int* bytes);

//void crearMensaje(void* payload, int socket_cliente);
//void crearMensajeNewPokemon(int pid, char* nombrePokemon, int posX, int posY,
//		int cantidadPokemons, int socket_cliente);

void devolverMensajeConfirmacion(void* layout, int socket_cliente);
char* recibirConfirmacion(int socket_cliente);

t_paquete *recibirSuscriptor(int socket_cliente, int* size);
void* serializarSuscriptor(t_paquete* suscriptor, int bytes);
void suscribirseAcola(int pid, char* nombreCola, int socket);

t_paquete *recibirMensajeGetPokemon(int socket_cliente, int* size);
void crearMensajeGetPokemon(int pid, char* nombrePokemon, int socket_cliente);
void* serializarPaqueteGetPokemon(t_paquete* paquete, int bytes);

t_paquete *recibirMensajeAppearedPokemon(int socket_cliente, int* size);
void* serializarPaqueteAppearedPokemon(t_paquete* paquete, int bytes);
void crearMensajeApperedPokemon(int pid, char* nombrePokemon, int posX,
		int posY, int socket_cliente);

t_paquete *recibirMensajeCatchPokemon(int socket_cliente, int* size);
void* serializarPaqueteCatchPokemon(t_paquete* paquete, int bytes);
void crearMensajeCatchPokemon(int pid, char* nombrePokemon, int posX, int posY,
		int socket_cliente);

t_paquete *recibirMensajeCaughtPokemon(int socket_cliente, int* size);
void* serializarPaqueteCaughtPokemon(t_paquete* paquete, int bytes);
void crearMensajeCaughtPokemon(int pid, int booleano, int socket_cliente);

t_paquete *recibirMensajeLocalizedPokemon(int socket_cliente, int* size);
void* serializarPaqueteLocalizedPokemon(t_paquete* paquete, int bytes);
void crearMensajeLocalizedPokemon(int pid, char* nombrePokemon, int posX,
		int posY, int cantidadPokemons, int socket_cliente);

void crearMensajeNewPokemon(pid_t pid, char* nombrePokemon, uint32_t posX,
		uint32_t posY, uint32_t cantidadPokemons, int socketCliente);

void* serializarPaqueteNew(t_paquete* paquete, int *bytes);

char *recibirMensaje(int socketCliente);

void* serializarMensajeNew2(t_paquete* paquete, int *bytes);
void crearMensajePokemon2(int pid, char* nombrePokemon, int posX, int posY,
		int cantidadPokemons, int socketCliente);
void recibirMensaje2(int socketCliente);

//void enviar2int(int pid, int codOp, int socketCliente);
void enviar2int(int pid, int codOp, char* nombrePokemon, int posX, int posY,
		int cantidadPokemons, int socketCliente);
void recibir2int(int socket);

int enviarCadena(int socketDestino, char *mensaje);
int recibirCadena(int socketOrigen, char *mensaje);

int enviarInt(int socketDestino, int numero);
int recibirInt(int socketDestino, int *intRecibido);

void enviarMensajeRecurso(int pid, int codOp, char* nombrePokemon, int posX,
		int posY, int cantidadPokemons, int socketCliente);


void recibirMensajeRecurso(int socketCliente);

#endif/*UTILS_UTILS_H*/
