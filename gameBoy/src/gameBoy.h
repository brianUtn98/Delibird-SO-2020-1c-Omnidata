#ifndef GAME_BOY_GAMEBOY_H_
#define GAME_BOY_GAMEBOY_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/string.h>
#include <string.h>

#include "../MiLibreria/utils/servidor.h"
#include "../MiLibreria/utils/utils.h"

#define GAMEBOY_CONFIG_PATH "gameBoy.config"

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

} t_GAMEBOYConfig;

t_log *logger;
t_config *GAMEBOYTConfig;
t_GAMEBOYConfig *gameBoyConf;

void inicializarLoggerGameBoy(void);
void cargarConfigGameBoy(void);
void iniciarServidor(void);
void manejarMensajes(int argc, char *argv[]);

void liberarGameBoyConfig();

#endif /* GAME_BOY_GAMEBOY_H_ */
