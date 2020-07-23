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
#include <netinet/in.h>
#include <arpa/inet.h>

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
	SUSCRIBIRSE_LOCALIZED_POKEMON,
	CONFIRMACION_ACK,
	ENVIAR_ID_MENSAJE

} t_header;

typedef struct {
	uint32_t x;
	uint32_t y;
} t_posicion;
typedef struct {
	//int size;
	t_header ack;
	uint32_t boolean;
	t_list* listaCoordenadas;
	uint32_t posX;
	uint32_t posY;
	uint32_t cantidadPokemons;
	uint32_t largoNombre;
	char* nombrePokemon;
	uint32_t idMensaje;
	uint32_t idMensajeCorrelativo;
	uint32_t socket;
	uint32_t largoNombreProceso;
	char* nombreProceso;

}__attribute__((packed)) t_bufferOmnidata;
typedef struct {
	t_header codigoOperacion;
	t_bufferOmnidata* buffer;
}__attribute__((packed)) t_paquete;

int crearConexion(char *ip, int puerto, int tiempo_reconexion);
int crearConexionSinReintento(char *ip, int puerto);
void liberarConexion(int socket);

void enviarMensajeBrokerNew(char* nombrePokemon, int posX, int posY,
		int cantidadPokemons, int socketCliente);
void enviarMensajeBrokerGet(char* nombrePokemon, int socketCliente);

void enviarMensajeGamecardGet(char* nombrePokemon,int idMensaje, int socketCliente);

void enviarMensajeBrokerAppeared(char* nombrePokemon, int posX, int posY,
		int idMensajeCorrelativo, int socketCliente);
void enviarMensajeBrokerCatch(char* nombrePokemon, int posX, int posY,
		int socketCliente);
void enviarMensajeBrokerCaught(int idMensajeCorrelativo, int booleano,
		int socketCliente);
void enviarMensajeLocalized(char* nombrePokemon, t_list* coordenadas,
		int socketCliente);

void enviarMensajeLocalizedId(char* nombrePokemon, t_list* coordenadas, int idMensaje,
		int socketCliente);

void enviarMensajeTeamAppeared(char* nombrePokemon, int posX, int posY,
		int socketCliente);
void enviarMensajeGameCardNewPokemon(char* nombrePokemon, int posX, int posY,
		int cantidadPokemons, int idMensajeCorrelativo, int socketCliente);
void enviarMensajeGameCardCatchPokemon(char* nombrePokemon, int posX, int posY,
		int idMensaje, int socketCliente);
void enviarMensajeGameCardCatch(char* nombrePokemon, int posX, int posY,int idMensaje,
		int socketCliente);
void enviarMensajeGameCardGetPokemon(char* nombrePokemon, int idMensaje,
		int socketCliente);

void* serializarPaquete(t_paquete* paquete, int *bytes);

void suscribirseNew(char* nombreProceso, int tiempo, int socketCliente);
void suscribirseGet(char* nombreProceso, int tiempo, int socketCliente);
void suscribirseCatch(char* nombreProceso, int tiempo, int socketCliente);
void suscribirseCaught(char* nombreProceso, int tiempo, int socketCliente);
void suscribirseLocalized(char* nombreProceso, int tiempo, int socketCliente);
void suscribirseAppeared(char* nombreProceso, int tiempo, int socketCliente);
t_paquete* recibirMensaje(int socketCliente);
void enviarAck(char* nombreProceso, t_paquete* mensaje, int socketCliente);
void enviarIdMensaje(int idMensaje, int socketCliente);

void mostrarCoordenada(void *data);
void mostrarListaCoordenadas(t_list *lista);
void mostrarListaPaquetes(t_list *lista);
void mostrarPaquete(t_paquete *paquete);

char* getIp();
char* getNombreProceso(char* nombre);

#endif/*UTILS_UTILS_H*/
