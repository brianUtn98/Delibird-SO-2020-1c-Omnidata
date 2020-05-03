#include "gameBoy.h"

void inicializarLoggerGameBoy() {
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

	gameBoyConf->ipBroker = string_duplicate(
			config_get_string_value(GAMEBOYTConfig, "IP_BROKER"));

	gameBoyConf->ipTeam = string_duplicate(
			config_get_string_value(GAMEBOYTConfig, "IP_TEAM"));
	gameBoyConf->ipGameCard = string_duplicate(
			config_get_string_value(GAMEBOYTConfig, "IP_GAMECARD"));

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
void liberarGameBoyConfig() {
	free(gameBoyConf);
}

//no me acuerdo si arranca en 0 o en 1 el valor de los argumentos, hay que probar esto

void manejarMensajes(int argc, char *argv[]) {

	if (strcmp(argv[1], "BROKER") & strcmp(argv[2], "NEW_POKEMON")) {
		//case 0: {
		if (argc != 6) {
			printf(
					"faltan argumentos, son ./gameboy BROKER NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]");
		}
		//break;
	}
	if (strcmp(argv[1], "BROKER") & strcmp(argv[2], "APPEARED_POKEMON")) {
		if (argc != 6) {
			printf(
					"faltan argumentos, son ./gameboy BROKER APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]");
		}
		//break;
	}
	if (strcmp(argv[1], "BROKER") & strcmp(argv[2], "CATCH_POKEMON")) {
		if (argc != 6) {
			printf(
					"faltan argumentos, son ./gameboy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY]");
		}
		//break;
	}
	if (strcmp(argv[1], "BROKER") & strcmp(argv[2], "CAUGHT_POKEMON")) {
		if (argc != 5) {
			printf(
					"faltan argumentos, son ./gameboy BROKER CAUGHT_POKEMON [ID_MENSAJE] [OK/FAIL]");
		}
		//break;
	}
	if (strcmp(argv[1], "BROKER") & strcmp(argv[2], "GET_POKEMON")) {
		if (argc != 3) {
			printf(
					"faltan argumentos, son ./gameboy BROKER GET_POKEMON [POKEMON]");
		}
		//break;
	}
	if (strcmp(argv[1], "TEAM") & strcmp(argv[2], "APPEARED_POKEMON")) {
		if (argc != 5) {
			printf(
					"faltan argumentos, son ./gameboy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY]");
		}
		//break;
	}
	if (strcmp(argv[1], "GAME_CARD") & strcmp(argv[2], "NEW_POKEMON")) {
		if (argc != 6) {
			printf(
					"faltan argumentos, son ./gameboy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]");
		}
		//break;
	}
	if (strcmp(argv[1], "GAME_CARD") & strcmp(argv[2], "CATCH_POKEMON")) {
		if (argc != 5) {
			printf(
					"faltan argumentos, son ./gameboy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY]");
		}
		//break;
	}
	if (strcmp(argv[1], "GAME_CARD") & strcmp(argv[2], "GET_POKEMON")) {
		if (argc != 3) {
			printf(
					"faltan argumentos, son ./gameboy GAMECARD GET_POKEMON [POKEMON]");
		}
		//break;
	}
	if (strcmp(argv[1], "SUSCRIPTOR")) {

		if (argc != 3) {
			printf(
					" faltan argumentos,son ./gameboy SUSCRIPTOR [COLA_DE_MENSAJES] [TIEMPO]");
			//break;
		}
//		default:
//		{
//			printf(
//					"Mensaje a modulo inválido, ingrese datos nuevamente.Gracias.");
//			break;
//		}
	}

}

//switch (strcmp(argv[1], "BROKER") & strcmp(argv[2], "NEW_POKEMON")) {
//	case 0: {
//		if (argc != 6) {
//			printf(
//					"faltan argumentos, son ./gameboy BROKER NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]");
//		}
//		break;
//	}
//	case strcmp(argv[1], "BROKER") & strcmp(argv[2], "APPEARED_POKEMON"): {
//		if (argc != 6) {
//			printf(
//					"faltan argumentos, son ./gameboy BROKER APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE]");
//		}
//		break;
//	}
//	case strcmp(argv[1], "BROKER") & strcmp(argv[2], "CATCH_POKEMON"): {
//		if (argc != 6) {
//			printf(
//					"faltan argumentos, son ./gameboy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY]");
//		}
//		break;
//	}
//	case strcmp(argv[1], "BROKER") & strcmp(argv[2], "CAUGHT_POKEMON"): {
//		if (argc != 5) {
//			printf(
//					"faltan argumentos, son ./gameboy BROKER CAUGHT_POKEMON [ID_MENSAJE] [OK/FAIL]");
//		}
//		break;
//	}
//	case strcmp(argv[1], "BROKER") & strcmp(argv[2], "GET_POKEMON"): {
//		if (argc != 3) {
//			printf(
//					"faltan argumentos, son ./gameboy BROKER GET_POKEMON [POKEMON]");
//		}
//		break;
//	}
//	case strcmp(argv[1], "TEAM") & strcmp(argv[2], "APPEARED_POKEMON"): {
//		if (argc != 5) {
//			printf(
//					"faltan argumentos, son ./gameboy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY]");
//		}
//		break;
//	}
//	case strcmp(argv[1], "GAME_CARD") & strcmp(argv[2], "NEW_POKEMON"): {
//		if (argc != 6) {
//			printf(
//					"faltan argumentos, son ./gameboy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]");
//		}
//		break;
//	}
//	case strcmp(argv[1], "GAME_CARD") & strcmp(argv[2], "CATCH_POKEMON"): {
//		if (argc != 5) {
//			printf(
//					"faltan argumentos, son ./gameboy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY]");
//		}
//		break;
//	}
//	case strcmp(argv[1], "GAME_CARD") & strcmp(argv[2], "GET_POKEMON"): {
//		if (argc != 3) {
//			printf(
//					"faltan argumentos, son ./gameboy GAMECARD GET_POKEMON [POKEMON]");
//		}
//		break;
//	}
//	case strcmp(argv[1], "SUSCRIPTOR"): {
//
//		if (argc != 3) {
//			printf(
//					" faltan argumentos,son ./gameboy SUSCRIPTOR [COLA_DE_MENSAJES] [TIEMPO]");
//			break;
//		}
//		default:
//		{
//			printf(
//					"Mensaje a modulo inválido, ingrese datos nuevamente.Gracias.");
//			break;
//		}
//	}
//
//	}

