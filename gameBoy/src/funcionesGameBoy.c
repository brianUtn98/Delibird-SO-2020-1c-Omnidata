#include "gameBoy.h"

void inicializarLogger() {
	//Crea el logger
	logger = log_create("logs/GAMEBOY.log", "GAMEBOY", 1, LOG_LEVEL_TRACE);
}

void cargarConfigGameBoy() {
	//printf("pude cargar la configuracion correctamente");
	//Carga la configuracion del txt de config al struct de config

	GAMEBOYTConfig = config_create(GAMEBOY_CONFIG_PATH);
	//inicializarLogger();
	if (GAMEBOYTConfig == NULL) {
		perror("error archivo de configuracion");
		log_error(logger, "- NO SE PUDO IMPORTAR LA CONFIGURACION");
		exit(1);
	}

	log_info(logger, "- CONFIGURACION IMPORTADA\n");

	gameBoyConf = malloc(sizeof(t_GAMEBOYConfig));

	gameBoyConf->ipBroker = config_get_string_value(GAMEBOYTConfig,
			"IP_BROKER");
	gameBoyConf->ipTeam = config_get_string_value(GAMEBOYTConfig, "IP_TEAM");
	gameBoyConf->ipGameCard = config_get_string_value(GAMEBOYTConfig,
			"IP_GAMECARD");

	gameBoyConf->puertoBroker = config_get_int_value(GAMEBOYTConfig,
			"PUERTO_BROKER");
	gameBoyConf->puertoTeam = config_get_int_value(GAMEBOYTConfig,
			"PUERTO_TEAM");
	gameBoyConf->puertoGameCard = config_get_int_value(GAMEBOYTConfig,
			"PUERTO_GAMECARD");

	//brokerConf->logFile = config_get_string_value(GAMEBOYTConfig, "LOG_FILE");

	printf(" puerto broker usado: %d \n", gameBoyConf->puertoBroker);
	printf(" Puerto team usado: %d \n", gameBoyConf->puertoTeam);
	printf(" ip broker usado: %s \n", gameBoyConf->ipBroker);

	log_info(logger, "· Puerto escucha = %d", gameBoyConf->puertoBroker);
	log_info(logger, "· IP  = %s", gameBoyConf->ipBroker);

	config_destroy(GAMEBOYTConfig);
	//ver cuando liberar el brokerConf , si lo hacemos acá no se va a poder usar en el servidor por ej,
	//estariamos cargando una estructura y liberandola sin darle uso.
	// talvez podriamos devolver la estructura y que la función no sea void.
	//hacer...
	//return brokerConf;
	//free(brokerConf);
}
//Hago esta función para acordarnos que debemos liberar el malloc de Sofi
void liberarBrokerConf() {
	free(gameBoyConf);
}

void manejarMensajes(int argc, char *argv[]) {



	switch (strcmp(argv[1],"BROKER")) {
		case tBrokerNewPokemon: {
			break;
		}
		case tBrokerAppearedPokemon: {
			break;
		}
		case tBrokerCatchPokemon: {
			break;
		}
		case tBrokerCaughtPokemon: {
			break;
		}
		case tBrokerGetPokemon: {
			break;
		}
		case tTeamAppearedPokemon: {
			break;
		}
		case tGameCardNewPokemon: {
			break;
		}
		case tGameCardCatchPokemon: {
			break;
		}
		case tGameCardGetPokemon: {
			break;
		}
		case tModoSuscriptor: {
			break;
		}
		default: {
			printf("Mensaje invalido, ingrese escriba nuevamente.Gracias.");
			break;
		}
		}

	}
