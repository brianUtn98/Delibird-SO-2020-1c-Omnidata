#include "gameBoy.h"

int main(int argc, char *argv[]) {

	inicializarLoggerGameBoy();
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	cargarConfigGameBoy();



	if (strcmp(argv[2], "BROKER") && strcmp("NEW_POKEMON", argv[3])) { //ok
		int socketCliente = crearConexion(gameBoyConf->ipBroker,
				gameBoyConf->puertoBroker, 30);
		enviarMensajeBrokerNew(argv[4], (int) argv[5], (int) argv[6],
				(int) argv[7], socketCliente);
		//printf("hola %s", argv[2]);

		//./gameboy BROKER NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]
		liberarConexion(socketCliente);
	}

	if (strcmp(argv[2], "BROKER") && strcmp(argv[3], "APPEARED_POKEMON")) { //ok
//		if (argc != 7) {
//			printf(
//					"faltan argumentos, son ./gameboy BROKER APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE_CORRELATIVO]");

		int socketCliente;
		log_info(logger, "Conectando a PUERTO=%d en IP=%s",
				gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
		socketCliente = crearConexion(gameBoyConf->ipBroker,
				gameBoyConf->puertoBroker, 30);
		enviarMensajeBrokerAppeared(argv[4], (int) argv[5], (int) argv[6],
				(int) argv[7], socketCliente);

	}

	if (strcmp(argv[2], "BROKER") && strcmp(argv[3], "CATCH_POKEMON")) { //ok
//			if (argc != 6) {
//				printf(
//						"faltan argumentos, son ./gameboy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY]");
//
		int socketCliente;
		log_info(logger, "Conectando a PUERTO=%d en IP=%s",
				gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
		socketCliente = crearConexion(gameBoyConf->ipBroker,
				gameBoyConf->puertoBroker, 30);
		enviarMensajeBrokerCatch(argv[4], (int) argv[5], (int) argv[6],
				socketCliente);

	}

	if (strcmp(argv[2], "BROKER") && strcmp(argv[3], "CAUGHT_POKEMON")) { //ok
//			if (argc != 5) {
//				printf(
//						"faltan argumentos, son ./gameboy BROKER CAUGHT_POKEMON [ID_MENSAJE_CORRELATIVO] [OK/FAIL]");

		int socketCliente;
		log_info(logger, "Conectando a PUERTO=%d en IP=%s",
				gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
		socketCliente = crearConexion(gameBoyConf->ipBroker,
				gameBoyConf->puertoBroker, 30);
		enviarMensajeBrokerCaught((int) argv[4], (int) argv[5], socketCliente);
	}

	if (strcmp(argv[2], "BROKER") && strcmp(argv[3], "GET_POKEMON")) { //ok
//			if (argc != 3) {
//				printf(
//						"faltan argumentos, son ./gameboy BROKER GET_POKEMON [POKEMON]");

		int socketCliente;
		log_info(logger, "Conectando a PUERTO=%d en IP=%s",
				gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
		socketCliente = crearConexion(gameBoyConf->ipBroker,
				gameBoyConf->puertoBroker, 30);
		enviarMensajeBrokerGet(argv[4], socketCliente);

	}
	if (strcmp(argv[2], "TEAM") & strcmp(argv[3], "APPEARED_POKEMON")) { //ok
//			if (argc != 5) {
//				printf(
//						"faltan argumentos, son ./gameboy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY]");

		int socketCliente;
		log_info(logger, "Conectando a PUERTO=%d en IP=%s", gameBoyConf->ipTeam,
				gameBoyConf->puertoTeam);
		socketCliente = crearConexion(gameBoyConf->ipTeam,
				gameBoyConf->puertoTeam, 30);
		enviarMensajeTeamAppeared(argv[4], (int) argv[5], (int) argv[6],
				socketCliente);
	}

	if (strcmp(argv[2], "GAME_CARD") && strcmp(argv[3], "NEW_POKEMON")) { //ok
//			if (argc != 6) {
//				printf(
//						"faltan argumentos, son ./gameboy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD] [ID_MENSAJE_CORRELATIVO]");
		int socketCliente;
		log_info(logger, "Conectando a PUERTO=%d en IP=%s", gameBoyConf->ipTeam,
				gameBoyConf->puertoTeam);
		socketCliente = crearConexion(gameBoyConf->ipTeam,
				gameBoyConf->puertoTeam, 30);
		enviarMensajeGameCardNewPokemon(argv[4], (int) argv[5], (int) argv[6],
				(int) argv[7], (int) argv[8], socketCliente);
	}

	if (strcmp(argv[2], "GAME_CARD") && strcmp(argv[3], "CATCH_POKEMON")) { //ok
	//		if (argc != 5) {
	//			printf(
	//					"faltan argumentos, son ./gameboy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE_CORRELATIVO]");
		int socketCliente;
		log_info(logger, "Conectando a PUERTO=%d en IP=%s", gameBoyConf->ipTeam,
				gameBoyConf->puertoTeam);
		socketCliente = crearConexion(gameBoyConf->ipTeam,
				gameBoyConf->puertoTeam, 30);
		enviarMensajeGameCardCatchPokemon(argv[4], (int) argv[5], (int) argv[6],
				(int) argv[7], socketCliente);

	}
	if (strcmp(argv[2], "GAME_CARD") && strcmp(argv[3], "GET_POKEMON")) {	//ok
	//		if (argc != 3) {
	//			printf(
	//					"faltan argumentos, son ./gameboy GAMECARD GET_POKEMON [POKEMON] [ID_MENSAJE_CORRELATIVO]");

		int socketCliente;
		log_info(logger, "Conectando a PUERTO=%d en IP=%s", gameBoyConf->ipTeam,
				gameBoyConf->puertoTeam);
		socketCliente = crearConexion(gameBoyConf->ipTeam,
				gameBoyConf->puertoTeam, 30);
		enviarMensajeGameCardGetPokemon(argv[4], (int) argv[5], socketCliente);

	}

	if (strcmp(argv[2], "SUSCRIPTOR") && strcmp("NEW_POKEMON", argv[3])) { //ok
		int socketCliente = crearConexion(gameBoyConf->ipBroker,
				gameBoyConf->puertoBroker, 30);
		suscribirseNew((int) argv[4], socketCliente);

	}

	if (strcmp(argv[2], "SUSCRIPTOR") && strcmp(argv[3], "APPEARED_POKEMON")) { //ok

		int socketCliente;
		log_info(logger, "Conectando a PUERTO=%d en IP=%s",
				gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
		socketCliente = crearConexion(gameBoyConf->ipBroker,
				gameBoyConf->puertoBroker, 30);
		suscribirseAppeared((int) argv[4], socketCliente);
	}

	if (strcmp(argv[2], "SUSCRIPTOR") && strcmp(argv[3], "CATCH_POKEMON")) { //ok

		int socketCliente;
		log_info(logger, "Conectando a PUERTO=%d en IP=%s",
				gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
		socketCliente = crearConexion(gameBoyConf->ipBroker,
				gameBoyConf->puertoBroker, 30);
		suscribirseCatch((int) argv[4], socketCliente);
	}

	if (strcmp(argv[2], "SUSCRIPTOR") && strcmp(argv[3], "CAUGHT_POKEMON")) { //ok

		int socketCliente;
		log_info(logger, "Conectando a PUERTO=%d en IP=%s",
				gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
		socketCliente = crearConexion(gameBoyConf->ipBroker,
				gameBoyConf->puertoBroker, 30);
		suscribirseCaught((int) argv[4], socketCliente);
	}

	if (strcmp(argv[2], "SUSCRIPTOR") && strcmp(argv[3], "GET_POKEMON")) { //ok

		int socketCliente;
		log_info(logger, "Conectando a PUERTO=%d en IP=%s",
				gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
		socketCliente = crearConexion(gameBoyConf->ipBroker,
				gameBoyConf->puertoBroker, 30);
		suscribirseGet((int) argv[4], socketCliente);

	}
	if (strcmp(argv[2], "SUSCRIPTOR") && strcmp(argv[3], "LOCALIZED_POKEMON")) { //ok

		int socketCliente;
		log_info(logger, "Conectando a PUERTO=%d en IP=%s",
				gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
		socketCliente = crearConexion(gameBoyConf->ipBroker,
				gameBoyConf->puertoBroker, 30);
		suscribirseLocalized((int) argv[4], socketCliente);

	}

	//liberarGameBoyConfig();
	//liberarConexion(socketCliente);
	return EXIT_SUCCESS;
}
