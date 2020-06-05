#include "gameBoy.h"

int main() {
	//int i = 0;
	int socketBroker = 0;
	int socketTeam = 0;
	int socketGameCard = 0;
	inicializarLoggerGameBoy();
	cargarConfigGameBoy();
	log_info(logger, "Conectando a PUERTO=%d en IP=%s",
			gameBoyConf->puertoBroker, gameBoyConf->ipBroker);
	socketBroker = crearConexion(gameBoyConf->ipBroker,
			gameBoyConf->puertoBroker, 30);
//	socketTeam=crearConexion(gameBoyConf->ipTeam,gameBoyConf->puertoTeam,30);
//	socketGameCard=crearConexion(gameBoyConf->ipGameCard,gameBoyConf->puertoGameCard,30);
	int argc = 0;
	while (1) {

		char *consoleLineAux = readline(">");
		char *consoleLine = malloc((strlen(consoleLineAux) + 1) * sizeof(char));
		strcpy(consoleLine, consoleLineAux);
		printf("Leí por consola: %s\n", consoleLine);

		//char *token, *tokenAux;
		char *token;
		t_list *argumentos = list_create();
		//tokenAux = strtok(consoleLineAux, " ");

		token = strtok(consoleLine, " ");

		while (token != NULL) {
			list_add(argumentos, (void*) token);
			printf("Token: %s\n", token);
			token = strtok(NULL, " ");
		}
		argc = argumentos->elements_count;
		printf("Cantidad de argumentos: %d\n", argc);

		printf("Mostrando lista\n");
		mostrarLista(argumentos);

		//sleep(100);

		char* proceso = (char*) list_get(argumentos, 0);
		char *mensaje = (char*) list_get(argumentos, 1);
		if ((strcmp(proceso, "BROKER") == 0)
				&& (strcmp(mensaje, "NEW_POKEMON") == 0)) { //ok

			if ((argc == 6)) {
				printf("Voy a enviar NEW_POKEMON\n");
				char *nombrePokemon = (char*) list_get(argumentos, 2);
				int posX = atoi((char*) list_get(argumentos, 3));
				int posY = atoi((char*) list_get(argumentos, 4));
				int cantidadPokemons = atoi((char*) list_get(argumentos, 5));
				enviarMensajeBrokerNew(nombrePokemon, posX, posY,
						cantidadPokemons, socketBroker);

			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf(
						"Formato válido ./gameboy BROKER NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]\n");
			}
		}

		if ((strcmp(proceso, "BROKER") == 0)
				&& (strcmp(mensaje, "APPEARED_POKEMON") == 0)) { //ok
			if (argc == 6) {
				printf("Voy a enviar APPEARED_POKEMON\n");
				char *nombrePokemon = (char*) list_get(argumentos, 2);
				int posX = atoi((char*) list_get(argumentos, 3));
				int posY = atoi((char*) list_get(argumentos, 4));
				int idMensaje = atoi((char*) list_get(argumentos, 5));
				enviarMensajeBrokerAppeared(nombrePokemon, posX, posY,
						idMensaje, socketBroker);
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf(
						"Formato válido ./gameboy BROKER APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE_CORRELATIVO]\n");
			}

		}

		if ((strcmp(proceso, "BROKER") == 0)
				&& (strcmp(mensaje, "CATCH_POKEMON") == 0)) { //ok
			if (argc == 5) {
				printf("Voy a enviar CATCH_POKEMON\n");
				char *nombrePokemon = (char*) list_get(argumentos, 2);
				int posX = atoi((char*) list_get(argumentos, 3));
				int posY = atoi((char*) list_get(argumentos, 4));
				enviarMensajeBrokerCatch(nombrePokemon, posX, posY,
						socketBroker);
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf(
						"Formato válido ./gameboy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY]\n");
			}
		}

		if ((strcmp(proceso, "BROKER") == 0)
				&& (strcmp(mensaje, "CAUGHT_POKEMON") == 0)) { //NO ok
			if (argc == 4) {
				printf("Voy a enviar CAUGHT_POKEMON\n");
				int idMensaje = atoi((char*) list_get(argumentos, 2));
				int booleano = atoi((char*) list_get(argumentos, 3));
				enviarMensajeBrokerCaught(idMensaje, booleano, socketBroker);
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf(
						"Formato válido ./gameboy BROKER CAUGHT_POKEMON [ID_MENSAJE_CORRELATIVO] [OK/FAIL]\n");
			}
		}

		if ((strcmp(proceso, "BROKER") == 0)
				&& (strcmp(mensaje, "GET_POKEMON") == 0)) { //NO OK
			if (argc == 3) {
				printf("Voy a enviar GET_POKEMON\n");
				char *nombrePokemon = (char*) list_get(argumentos, 2);
				enviarMensajeBrokerGet(nombrePokemon, socketBroker);
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf(
						"Formato válido ./gameboy BROKER GET_POKEMON [POKEMON]\n");
			}

		}

		if ((strcmp(proceso, "TEAM") == 0)
				&& (strcmp(mensaje, "APPEARED_POKEMON") == 0)) { //ok
			if (argc == 5) {
				printf("Voy a enviar APPEARED_POKEMON\n");
				char *nombrePokemon = (char*) list_get(argumentos, 2);
				int posX = atoi((char*) list_get(argumentos, 3));
				int posY = atoi((char*) list_get(argumentos, 4));
				//int idMensaje = atoi((char*) list_get(argumentos, 5));
				enviarMensajeTeamAppeared(nombrePokemon, posX, posY,
						socketTeam);
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf(
						"Formato válido ./gameboy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY]\n");
			}
		}

		if ((strcmp(proceso, "GAME_CARD") == 0)
				&& (strcmp(mensaje, "NEW_POKEMON") == 0)) { //ok
			if (argc == 7) {
				printf("Voy a enviar NEW_POKEMON\n");
				char *nombrePokemon = (char*) list_get(argumentos, 2);
				int posX = atoi((char*) list_get(argumentos, 3));
				int posY = atoi((char*) list_get(argumentos, 4));
				int cantidadPokemons = atoi((char*) list_get(argumentos, 5));
				int idCorrelativo = atoi((char*) list_get(argumentos, 6));
				enviarMensajeGameCardNewPokemon(nombrePokemon, posX, posY,
						cantidadPokemons, idCorrelativo, socketGameCard);
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf(
						"Formato válido ./gameboy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD] [ID_MENSAJE_CORRELATIVO]\n");
			}
		}

		if ((strcmp(proceso, "GAME_CARD") == 0)
				&& (strcmp(mensaje, "CATCH_POKEMON") == 0)) { //ok
			if (argc == 6) {
				printf("Voy a enviar CATCH_POKEMON\n");
				char *nombrePokemon = (char*) list_get(argumentos, 2);
				int posX = atoi((char*) list_get(argumentos, 3));
				int posY = atoi((char*) list_get(argumentos, 4));
				int idCorrelativo = atoi((char*) list_get(argumentos, 5));
				enviarMensajeGameCardCatchPokemon(nombrePokemon, posX, posY,
						idCorrelativo, socketGameCard);
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf(
						"Formato válido ./gameboy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE_CORRELATIVO]\n");
			}

		}
		if ((strcmp(proceso, "GAME_CARD") == 0)
				&& (strcmp(mensaje, "GET_POKEMON") == 0)) {	//ok
			if (argc == 4) {
				printf("Voy a enviar GET_POKEMON\n");
				char *nombrePokemon = (char*) list_get(argumentos, 2);
				int idCorrelativo = atoi((char*) list_get(argumentos, 3));
				enviarMensajeGameCardGetPokemon(nombrePokemon, idCorrelativo,
						socketGameCard);
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf(
						"Formato válido ./gameboy GAMECARD GET_POKEMON [POKEMON] [ID_MENSAJE_CORRELATIVO]\n");
			}

		}

		if ((strcmp(proceso, "SUSCRIPTOR") == 0)
				&& (strcmp("NEW_POKEMON", mensaje) == 0)) { //ok
			if (argc == 3) {
				int tiempo = atoi((char*) list_get(argumentos, 2));
				suscribirseNew(tiempo, socketBroker);
			} else {

			}
			printf("Cantidad de argumentos incorrectos.\n");
			printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");

		}

		if ((strcmp(proceso, "SUSCRIPTOR") == 0)
				&& (strcmp(mensaje, "APPEARED_POKEMON") == 0)) { //ok
			if (argc == 3) {
				printf("Voy a enviar SUBS_APPEARED_POKEMON\n");
				int tiempo = atoi((char*) list_get(argumentos, 2));
				suscribirseAppeared(tiempo, socketBroker);
			} else {

			}
			printf("Cantidad de argumentos incorrectos.\n");
			printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
		}

		if ((strcmp(proceso, "SUSCRIPTOR") == 0)
				&& (strcmp(mensaje, "CATCH_POKEMON") == 0)) { //ok
			if (argc == 3) {
				printf("Voy a enviar SUBS_CATCH_POKEMON\n");
				int tiempo = atoi((char*) list_get(argumentos, 2));
				suscribirseCatch(tiempo, socketBroker);
			} else {

			}
			printf("Cantidad de argumentos incorrectos.\n");
			printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
		}

		if ((strcmp(proceso, "SUSCRIPTOR") == 0)
				&& (strcmp(mensaje, "CAUGHT_POKEMON") == 0)) { //ok
			if (argc == 3) {
				printf("Voy a enviar SUBS_CAUGHT_POKEMON\n");
				int tiempo = atoi((char*) list_get(argumentos, 2));
				suscribirseCaught(tiempo, socketBroker);
			} else {

			}
			printf("Cantidad de argumentos incorrectos.\n");
			printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
		}

		if ((strcmp(proceso, "SUSCRIPTOR") == 0)
				&& (strcmp(mensaje, "GET_POKEMON") == 0)) { //ok
			if (argc == 3) {
				printf("Voy a enviar SUBS_GET_POKEMON\n");
				int tiempo = atoi((char*) list_get(argumentos, 2));
				suscribirseGet(tiempo, socketBroker);
			} else {

			}
			printf("Cantidad de argumentos incorrectos.\n");
			printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");

		}
		if ((strcmp(proceso, "SUSCRIPTOR") == 0)
				&& (strcmp(mensaje, "LOCALIZED_POKEMON") == 0)) { //ok
			if (argc == 3) {
				printf("Voy a enviar SUBS_LOCALIZED_POKEMON\n");
				int tiempo = atoi((char*) list_get(argumentos, 2));
				suscribirseLocalized(tiempo, socketBroker);
			} else {

			}
			printf("Cantidad de argumentos incorrectos.\n");
			printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");

		}

//	free(consoleLineAux);
//	free(consoleLine);

		free(consoleLine);
		free(consoleLineAux);

	}
	liberarConexion(socketBroker);
	liberarGameBoyConfig();

	return EXIT_SUCCESS;
}
