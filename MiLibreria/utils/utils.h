#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/process.h>
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

typedef struct {
	int puerto;
	char* ip;
} t_suscriptor;

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
	SUSCRIBIRSE_LOCALIZED_POKEMON

} t_header;

typedef struct {
	int x;
	int y;
} t_posicion;
typedef struct {
	//int size;

	bool boolean;
	t_list* listaCoordenadas;
	int posX;
	int posY;
	int cantidadPokemons;
	int largoNombre;
	char* nombrePokemon;
	int idMensaje;

}__attribute__((packed)) t_bufferOmnidata;
typedef struct {

	t_header codigoOperacion;

	t_bufferOmnidata* buffer;
}__attribute__((packed)) t_paquete;

int crearConexion(char *ip, int puerto, int tiempo_reconexion);

void liberarConexion(int socket);

void enviarMensajeNew(char* nombrePokemon, int posX, int posY,
		int cantidadPokemons, int socketCliente);
void enviarMensajeGet(char* nombrePokemon, int socketCliente);
void enviarMensajeAppeared(char* nombrePokemon, int posX, int posY,
		int idMensaje, int socketCliente);
void enviarMensajeCatch(char* nombrePokemon, int posX, int posY,
		int socketCliente);
void enviarMensajeCaught(int idMensaje, bool booleano, int socketCliente);
void enviarMensajeLocalized(char* nombrePokemon, t_list coordenadas,
		int socketCliente);

void* serializarPaquete(t_paquete* paquete, int *bytes);

void suscribirseNew(int socketCliente);
void suscribirseGet(int socketCliente);
void suscribirseCatch(int socketCliente);
void suscribirseCaught(int socketCliente);
void suscribirseLocalized(int socketCliente);
void suscribirseAppered(int socketCliente);
t_paquete* recibirMensaje(int socketCliente);

int *aplanarLista(t_list* lista);

#endif/*UTILS_UTILS_H*/
