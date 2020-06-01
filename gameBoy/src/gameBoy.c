#include "gameBoy.h"

int main(int argc, char *argv[]) {

	inicializarLoggerGameBoy();
	cargarConfigGameBoy();

	if ((strcmp(argv[1], "BROKER") == 0)
			&& (strcmp(argv[2], "NEW_POKEMON") == 0)) { //ok

		if ((argc == 7)) {

			int socketCliente = crearConexion(gameBoyConf->ipBroker,
					gameBoyConf->puertoBroker, 30);
			enviarMensajeBrokerNew(argv[3], atoi(argv[4]), atoi(argv[5]),
					atoi(argv[6]), socketCliente);
			printf("Recibo del argumento 1 %s\n", argv[1]);
			printf("Recibo del argumento 2 %s\n", argv[2]);
			printf("Recibo del argumento 3 %s\n", argv[3]);
			printf("Recibo del argumento 4 %s\n", argv[4]);

			liberarConexion(socketCliente);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy BROKER NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]\n");
		}
	}

	if ((strcmp(argv[1], "BROKER") == 0)
			&& (strcmp(argv[2], "APPEARED_POKEMON") == 0)) { //ok
		if (argc == 7) {

			int socketCliente;
			log_info(logger, "Conectando a PUERTO=%d en IP=%s",
					gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
			socketCliente = crearConexion(gameBoyConf->ipBroker,
					gameBoyConf->puertoBroker, 30);
			enviarMensajeBrokerAppeared(argv[3], atoi(argv[4]), atoi(argv[5]),
					atoi(argv[6]), socketCliente);
			liberarConexion(socketCliente);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy BROKER APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE_CORRELATIVO]\n");
		}

	}

	if ((strcmp(argv[1], "BROKER") == 0)
			&& (strcmp(argv[2], "CATCH_POKEMON") == 0)) { //ok
		if (argc == 6) {
			int socketCliente;
			log_info(logger, "Conectando a PUERTO=%d en IP=%s",
					gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
			socketCliente = crearConexion(gameBoyConf->ipBroker,
					gameBoyConf->puertoBroker, 30);
			enviarMensajeBrokerCatch(argv[3], atoi(argv[4]), atoi(argv[5]),
					socketCliente);
			liberarConexion(socketCliente);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY]\n");
		}
	}

	if ((strcmp(argv[1], "BROKER") == 0)
			&& (strcmp(argv[2], "CAUGHT_POKEMON") == 0)) { //ok
		if (argc == 5) {
			int socketCliente;
			log_info(logger, "Conectando a PUERTO=%d en IP=%s",
					gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
			socketCliente = crearConexion(gameBoyConf->ipBroker,
					gameBoyConf->puertoBroker, 30);
			enviarMensajeBrokerCaught((int) argv[4], (int) argv[5],
					socketCliente);
			liberarConexion(socketCliente);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy BROKER CAUGHT_POKEMON [ID_MENSAJE_CORRELATIVO] [OK/FAIL]\n");
		}
	}

	if ((strcmp(argv[1], "BROKER") == 0)
			&& (strcmp(argv[2], "GET_POKEMON") == 0)) { //ok
		if (argc == 4) {

			int socketCliente;
			log_info(logger, "Conectando a PUERTO=%d en IP=%s",
					gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
			socketCliente = crearConexion(gameBoyConf->ipBroker,
					gameBoyConf->puertoBroker, 30);
			enviarMensajeBrokerGet(argv[3], socketCliente);
			liberarConexion(socketCliente);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf("Formato válido ./gameboy BROKER GET_POKEMON [POKEMON]\n");
		}

	}

	if ((strcmp(argv[1], "TEAM") == 0)
			&& (strcmp(argv[2], "APPEARED_POKEMON") == 0)) { //ok
		if (argc == 6) {

			int socketCliente;
			log_info(logger, "Conectando a PUERTO=%d en IP=%s",
					gameBoyConf->ipTeam, gameBoyConf->puertoTeam);
			socketCliente = crearConexion(gameBoyConf->ipTeam,
					gameBoyConf->puertoTeam, 30);
			enviarMensajeTeamAppeared(argv[3], atoi(argv[4]), atoi(argv[5]),
					socketCliente);
			liberarConexion(socketCliente);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY]\n");
		}
	}

	if ((strcmp(argv[1], "GAME_CARD") == 0)
			&& (strcmp(argv[2], "NEW_POKEMON") == 0)) { //ok
		if (argc == 8) {
//				printf(
//						"faltan argumentos, son ./gameboy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD] [ID_MENSAJE_CORRELATIVO]");
			int socketCliente;
			log_info(logger, "Conectando a PUERTO=%d en IP=%s",
					gameBoyConf->ipTeam, gameBoyConf->puertoTeam);
			socketCliente = crearConexion(gameBoyConf->ipTeam,
					gameBoyConf->puertoTeam, 30);
			enviarMensajeGameCardNewPokemon(argv[3], atoi(argv[4]),
					atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), socketCliente);
			liberarConexion(socketCliente);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD] [ID_MENSAJE_CORRELATIVO]\n");
		}
	}

	if ((strcmp(argv[1], "GAME_CARD") == 0)
			&& (strcmp(argv[2], "CATCH_POKEMON") == 0)) { //ok
		if (argc == 7) {

			int socketCliente;
			log_info(logger, "Conectando a PUERTO=%d en IP=%s",
					gameBoyConf->ipTeam, gameBoyConf->puertoTeam);
			socketCliente = crearConexion(gameBoyConf->ipTeam,
					gameBoyConf->puertoTeam, 30);
			enviarMensajeGameCardCatchPokemon(argv[3], atoi(argv[4]),
					atoi(argv[5]), atoi(argv[6]), socketCliente);
			liberarConexion(socketCliente);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE_CORRELATIVO]\n");
		}

	}
	if ((strcmp(argv[1], "GAME_CARD") == 0)
			&& (strcmp(argv[2], "GET_POKEMON") == 0)) {	//ok
		if (argc == 5) {

			int socketCliente;
			log_info(logger, "Conectando a PUERTO=%d en IP=%s",
					gameBoyConf->ipTeam, gameBoyConf->puertoTeam);
			socketCliente = crearConexion(gameBoyConf->ipTeam,
					gameBoyConf->puertoTeam, 30);
			enviarMensajeGameCardGetPokemon(argv[3], atoi(argv[4]),
					socketCliente);
			liberarConexion(socketCliente);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy GAMECARD GET_POKEMON [POKEMON] [ID_MENSAJE_CORRELATIVO]\n");
		}

	}

	if ((strcmp(argv[1], "SUSCRIPTOR") == 0)
			&& (strcmp("NEW_POKEMON", argv[2]) == 0)) { //ok
		if (argc == 4) {
			int socketCliente = crearConexion(gameBoyConf->ipBroker,
					gameBoyConf->puertoBroker, 30);
			suscribirseNew(atoi(argv[3]), socketCliente);
			liberarConexion(socketCliente);
		} else {

		}
		printf("Cantidad de argumentos incorrectos.\n");
		printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");

	}

	if ((strcmp(argv[1], "SUSCRIPTOR") == 0)
			&& (strcmp(argv[2], "APPEARED_POKEMON") == 0)) { //ok
		if (argc == 4) {
			int socketCliente;
			log_info(logger, "Conectando a PUERTO=%d en IP=%s",
					gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
			socketCliente = crearConexion(gameBoyConf->ipBroker,
					gameBoyConf->puertoBroker, 30);
			suscribirseAppeared(atoi(argv[3]), socketCliente);
			liberarConexion(socketCliente);
		} else {

		}
		printf("Cantidad de argumentos incorrectos.\n");
		printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
	}

	if ((strcmp(argv[1], "SUSCRIPTOR") == 0)
			&& (strcmp(argv[2], "CATCH_POKEMON") == 0)) { //ok
		if (argc == 4) {
			int socketCliente;
			log_info(logger, "Conectando a PUERTO=%d en IP=%s",
					gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
			socketCliente = crearConexion(gameBoyConf->ipBroker,
					gameBoyConf->puertoBroker, 30);
			suscribirseCatch(atoi(argv[3]), socketCliente);
			liberarConexion(socketCliente);
		} else {

		}
		printf("Cantidad de argumentos incorrectos.\n");
		printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
	}

	if ((strcmp(argv[1], "SUSCRIPTOR") == 0)
			&& (strcmp(argv[2], "CAUGHT_POKEMON") == 0)) { //ok
		if (argc == 4) {
			int socketCliente;
			log_info(logger, "Conectando a PUERTO=%d en IP=%s",
					gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
			socketCliente = crearConexion(gameBoyConf->ipBroker,
					gameBoyConf->puertoBroker, 30);
			suscribirseCaught(atoi(argv[3]), socketCliente);
			liberarConexion(socketCliente);
		} else {

		}
		printf("Cantidad de argumentos incorrectos.\n");
		printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
	}

	if ((strcmp(argv[1], "SUSCRIPTOR") == 0)
			&& (strcmp(argv[2], "GET_POKEMON") == 0)) { //ok
		if (argc == 4) {
			int socketCliente;
			log_info(logger, "Conectando a PUERTO=%d en IP=%s",
					gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
			socketCliente = crearConexion(gameBoyConf->ipBroker,
					gameBoyConf->puertoBroker, 30);
			suscribirseGet(atoi(argv[3]), socketCliente);
			liberarConexion(socketCliente);
		} else {

		}
		printf("Cantidad de argumentos incorrectos.\n");
		printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");

	}
	if ((strcmp(argv[1], "SUSCRIPTOR") == 0)
			&& (strcmp(argv[2], "LOCALIZED_POKEMON") == 0)) { //ok
		if (argc == 4) {
			int socketCliente;
			log_info(logger, "Conectando a PUERTO=%d en IP=%s",
					gameBoyConf->ipBroker, gameBoyConf->puertoBroker);
			socketCliente = crearConexion(gameBoyConf->ipBroker,
					gameBoyConf->puertoBroker, 30);
			suscribirseLocalized(atoi(argv[3]), socketCliente);
			liberarConexion(socketCliente);
		} else {

		}
		printf("Cantidad de argumentos incorrectos.\n");
		printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");

	}

	liberarGameBoyConfig();

	return EXIT_SUCCESS;
}
