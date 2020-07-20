#include "gameBoy.h"

int main(int argc, char *argv[]) {
	//int i = 0;
	socketBroker = 0;
	socketTeam = 0;
	socketGameCard = 0;
	inicializarLoggerGameBoy();
	cargarConfigGameBoy();
	char* nombreProceso = getNombreProceso(gameBoyConf->nombre);
//	socketBroker = crearConexion(gameBoyConf->ipBroker,
//			gameBoyConf->puertoBroker, 30);

//socketTeam=crearConexion(gameBoyConf->ipTeam,gameBoyConf->puertoTeam,30);
//socketGameCard=crearConexion(gameBoyConf->ipGameCard,gameBoyConf->puertoGameCard,30);

	pthread_t conexionBroker;
	pthread_t conexionTeam;
	pthread_t conexionGameCard;

	if (argc == 1) {
		if (pthread_create(&conexionBroker, NULL, iniciarConexionBroker, NULL)
				< 0) {
			printf("No se pudo crear el hilo para conectar el broker\n");
		} else {
			printf("Se creo un hilo para conectar el broker\n");
		}
		if (pthread_create(&conexionTeam, NULL, iniciarConexionTeam, NULL)
				< 0) {
			printf("No se pudo crear el hilo para conectar el team\n");
		} else {
			printf("Se creo un hilo para conectar el team\n");
		}

		if (pthread_create(&conexionGameCard, NULL, iniciarConexionGameCard,
		NULL) < 0) {
			printf("No se pudo crear el hilo para conectar el game card\n");
		} else {
			printf("Se creo un hilo para conectar el game card\n");
		}
	} else {
		printf("Entro en el else\n");
		if (strcmp(argv[1], "BROKER") == 0) {
			pthread_create(&conexionBroker, NULL, iniciarConexionBroker, NULL);
			pthread_join(conexionBroker, NULL);
		}
		if (strcmp(argv[1], "TEAM") == 0) {
			pthread_create(&conexionTeam, NULL, iniciarConexionTeam, NULL);
			pthread_join(conexionTeam, NULL);
		}
		if (strcmp(argv[1], "GAMECARD") == 0) {
			pthread_create(&conexionGameCard, NULL, iniciarConexionGameCard,
			NULL);
			pthread_join(conexionGameCard, NULL);
		}
	}
	//sleep(2);

//	printf("El socket del team es %d\n",socketTeam);
//	printf("El socket del broker es %d\n",socketBroker);
//	printf("El socket del gamecard es %d\n",socketGameCard);

	int cantidadArgumentos = 0;
	int ciclo = 1;
	char *consoleLineAux;
	char *consoleLine;
	while (ciclo) {
		t_list *argumentos = list_create();

		if (argc == 1) {
			consoleLineAux = readline(">");
			consoleLine = malloc((strlen(consoleLineAux) + 1) * sizeof(char));
			strcpy(consoleLine, consoleLineAux);
			//printf("Leí por consola: %s\n", consoleLine);

			//char *token, *tokenAux;
			char *token;

			//tokenAux = strtok(consoleLineAux, " ");

			token = strtok(consoleLine, " ");

			while (token != NULL) {
				list_add(argumentos, (void*) token);
				//printf("Token: %s\n", token);
				token = strtok(NULL, " ");
			}
			cantidadArgumentos = argumentos->elements_count;
//		printf("Cantidad de argumentos: %d\n", cantidadArgumentos);

//		printf("Mostrando lista\n");
//		mostrarLista(argumentos);

		} else {
			ciclo = 0;
			int i;
			for (i = 0; i < (argc - 1); i++) {
				cantidadArgumentos = argc - 1;
				list_add(argumentos, (void*) argv[i + 1]);
				printf("Arg %d: %s ", i + 1, argv[i + 1]);
			}

		}

		char* proceso = (char*) list_get(argumentos, 0);
		char *mensaje = (char*) list_get(argumentos, 1);

		if ((strcmp(proceso, "exit")) == 0 || (strcmp(proceso, "EXIT")) == 0) {
			printf("Saliendo...\n");
			return 0;
		}

		if ((strcmp(proceso, "BROKER") == 0)
				&& (strcmp(mensaje, "NEW_POKEMON") == 0)) { //ok

			if ((cantidadArgumentos == 6)) {
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
			if (cantidadArgumentos == 6) {
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
			if (cantidadArgumentos == 5) {
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
			if (cantidadArgumentos == 4) {
				printf("Voy a enviar CAUGHT_POKEMON\n");

				int idMensaje = atoi((char*) list_get(argumentos, 2));

				//agregue esto para el ok o fail desde acá
				int booleanoAux = 0;
				char* auxiliar = (char*) list_get(argumentos, 3);

				if (strcmp(auxiliar, "OK")) {
					booleanoAux = 1;
				}
				if (strcmp(auxiliar, "FAIL")) {
					booleanoAux = 0;
				}//hasta acá , aviso por si rompe.

				//int booleano = atoi((char*) list_get(argumentos, 3));
				enviarMensajeBrokerCaught(idMensaje, booleanoAux, socketBroker);//cambie booleano por booleanoAux
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf(
						"Formato válido ./gameboy BROKER CAUGHT_POKEMON [ID_MENSAJE_CORRELATIVO] [OK/FAIL]\n");
			}
		}

		if ((strcmp(proceso, "BROKER") == 0)
				&& (strcmp(mensaje, "GET_POKEMON") == 0)) { //NO OK
			if (cantidadArgumentos == 3) {
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
			if (cantidadArgumentos == 5) {
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

		if ((strcmp(proceso, "GAMECARD") == 0)
				&& (strcmp(mensaje, "NEW_POKEMON") == 0)) { //ok
			if (cantidadArgumentos == 7) {
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

		if ((strcmp(proceso, "GAMECARD") == 0)
				&& (strcmp(mensaje, "CATCH_POKEMON") == 0)) { //ok
			if (cantidadArgumentos == 6) {
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
		if ((strcmp(proceso, "GAMECARD") == 0)
				&& (strcmp(mensaje, "GET_POKEMON") == 0)) {	//ok
			if (cantidadArgumentos == 4) {
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
			if (cantidadArgumentos == 3) {
				printf("Rompo aca?\n");
				printf("%s\n", (char*) list_get(argumentos, 2));
				int tiempo = atoi((char*) list_get(argumentos, 2));
				printf("Rompo aca?\n");
				suscribirseNew(nombreProceso, tiempo, socketBroker);
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
			}

		}

		if ((strcmp(proceso, "SUSCRIPTOR") == 0)
				&& (strcmp(mensaje, "APPEARED_POKEMON") == 0)) { //ok
			if (cantidadArgumentos == 3) {
				printf("Voy a enviar SUBS_APPEARED_POKEMON\n");
				int tiempo = atoi((char*) list_get(argumentos, 2));
				suscribirseAppeared(nombreProceso, tiempo, socketBroker);
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
			}

		}

		if ((strcmp(proceso, "SUSCRIPTOR") == 0)
				&& (strcmp(mensaje, "CATCH_POKEMON") == 0)) { //ok
			if (cantidadArgumentos == 3) {
				printf("Voy a enviar SUBS_CATCH_POKEMON\n");
				int tiempo = atoi((char*) list_get(argumentos, 2));
				suscribirseCatch(nombreProceso, tiempo, socketBroker);
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
			}

		}

		if ((strcmp(proceso, "SUSCRIPTOR") == 0)
				&& (strcmp(mensaje, "CAUGHT_POKEMON") == 0)) { //ok
			if (cantidadArgumentos == 3) {
				printf("Voy a enviar SUBS_CAUGHT_POKEMON\n");
				int tiempo = atoi((char*) list_get(argumentos, 2));
				suscribirseCaught(nombreProceso, tiempo, socketBroker);
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
			}

		}

		if ((strcmp(proceso, "SUSCRIPTOR") == 0)
				&& (strcmp(mensaje, "GET_POKEMON") == 0)) { //ok
			if (cantidadArgumentos == 3) {
				printf("Voy a enviar SUBS_GET_POKEMON\n");
				int tiempo = atoi((char*) list_get(argumentos, 2));

				suscribirseGet(nombreProceso, tiempo, socketBroker);
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
			}

		}
		if ((strcmp(proceso, "SUSCRIPTOR") == 0)
				&& (strcmp(mensaje, "LOCALIZED_POKEMON") == 0)) { //ok
			if (cantidadArgumentos == 3) {
				printf("Voy a enviar SUBS_LOCALIZED_POKEMON\n");
				int tiempo = atoi((char*) list_get(argumentos, 2));
				suscribirseLocalized(nombreProceso, tiempo, socketBroker);
			} else {
				printf("Cantidad de argumentos incorrectos.\n");
				printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
			}

		}

//	free(consoleLineAux);
//	free(consoleLine);
		if (ciclo == 1) {
			free(consoleLine);
			free(consoleLineAux);
		}
		list_destroy(argumentos);
	}
	//liberarConexion(socketBroker);
	liberarGameBoyConfig();

	return EXIT_SUCCESS;
}
