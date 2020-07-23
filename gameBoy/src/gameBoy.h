#ifndef GAME_BOY_GAMEBOY_H_
#define GAME_BOY_GAMEBOY_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/process.h>
#include <commons/string.h>
#include <string.h>
#include "../MiLibreria/utils/cliente.h"
#include "../MiLibreria/utils/utils.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <pthread.h>
#include <semaphore.h>

#define GAMEBOY_CONFIG_PATH "gameBoy.config"
#define MAX_CONEXIONES 100

typedef enum t_protocolo {
	tBrokerNewPokemon = 1,
	tBrokerAppearedPokemon,
	tBrokerCatchPokemon,
	tBrokerCaughtPokemon,
	tBrokerGetPokemon,
	tTeamAppearedPokemon,
	tGameCardNewPokemon,
	tGameCardCatchPokemon,
	tGameCardGetPokemon,
	tModoSuscriptor,
	tFinDeProtocolo //NO SACAR Y DEJAR A LO ULTIMO!!!
} t_protocolo;

typedef struct {
	char *ipBroker;
	char *ipTeam;
	char *ipGameCard;
	int puertoBroker;
	int puertoTeam;
	int puertoGameCard;
	char* nombre;
	char* LOG_FILE;
} t_GAMEBOYConfig;

t_log *logger;
t_config *GAMEBOYTConfig;
t_GAMEBOYConfig *gameBoyConf;
int flagNew, flagAppeared, flagGet, flagCatch, flagCaught, flagLocalized;
t_queue* bandejaDeMensajes;

sem_t contadorBandeja;
t_log *logEntrega;

pthread_mutex_t mutex_bandeja;

int socketBroker;
int socketGameCard;
int socketTeam;

void inicializarLoggerGameBoy(void);
void cargarConfigGameBoy(void);
void iniciarServidor(void);
void manejarMensajes(int argc, char *argv[]);
void mostrarListaChar(t_list* lista);
void mostrarChar(void *elemento);
void liberarGameBoyConfig();
void *iniciarConexionGameCard(void *arg);
void *iniciarConexionTeam(void *arg);
void *iniciarConexionBroker(void *arg);

void* recvMensajes(void* socketCliente);
void* procesarMensaje();
void inicializarLoggerEntregable();

void *suscribirseBrokerNew();
void *suscribirseBrokerGet();
void *suscribirseBrokerAppeared();
void *suscribirseBrokerCatch();
void *suscribirseBrokerCaught();
void *suscribirseBrokerLocalized();

void* matarHiloSuscriptorGet(void* tiempo);
void* matarHiloSuscriptorAppeared(void* tiempo);
void* matarHiloSuscriptorNew(void* tiempo);
void* matarHiloSuscriptorCatch(void* tiempo);
void* matarHiloSuscriptorCaught(void* tiempo);
void* matarHiloSuscriptorLocalized(void* tiempo);
#endif /* GAME_BOY_GAMEBOY_H_ */
