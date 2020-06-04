#include "gameBoy.h"

int main() {
	int i;
	int socketBroker;
	int socketTeam;
	int socketGameCard;
	inicializarLoggerGameBoy();
	cargarConfigGameBoy();
//	log_info(logger, "Conectando a PUERTO=%d en IP=%s",
//									gameBoyConf->puertoBroker,gameBoyConf->ipBroker);
//	socketBroker=crearConexion(gameBoyConf->ipBroker,gameBoyConf->puertoBroker,30);
//	socketTeam=crearConexion(gameBoyConf->ipTeam,gameBoyConf->puertoTeam,30);
//	socketGameCard=crearConexion(gameBoyConf->ipGameCard,gameBoyConf->puertoGameCard,30);
	int argc;
	char *argv;
	while(1){

	char *consoleLineAux=readline(">");
	char *consoleLine=malloc((strlen(consoleLineAux)+1)*sizeof(char));
	strcpy(consoleLine,consoleLineAux);
	printf("Leí por consola: %s\n",consoleLine);

	char *token,*tokenAux;

	tokenAux=strtok(consoleLineAux," ");
	while(tokenAux != NULL){
		argc++;
		tokenAux=strtok(NULL," ");
	}
	argv=malloc(sizeof(char*)*argc);
	printf("Cantidad de argumentos: %d\n",argc);
	token=strtok(consoleLine," ");
	int i=0;

	while(token != NULL){
		argv[i]=malloc(strlen(token)*sizeof(char));
		strcpy(argv[i],token);

		printf("Token: %s\n",token);
		printf("Argv[%d]%s\n",i,argv[i]);
		token=strtok(NULL," ");
		i++;
	}
	for(i=0;i<argc;i++){
			printf("%s\n",argv[i]);
		}


	//sleep(100);


	free(consoleLine);
	free(consoleLineAux);
	}





	if ((strcmp(argv[1], "BROKER") == 0)
			&& (strcmp(argv[2], "NEW_POKEMON") == 0)) { //ok

		if ((argc == 7)) {
			printf("Voy a enviar NEW_POKEMON\n");

			enviarMensajeBrokerNew(argv[3], atoi(argv[4]), atoi(argv[5]),
					atoi(argv[6]), socketBroker);

		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy BROKER NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD]\n");
		}
	}

	if ((strcmp(argv[1], "BROKER") == 0)
			&& (strcmp(argv[2], "APPEARED_POKEMON") == 0)) { //ok
		if (argc == 7) {
			printf("Voy a enviar APPEARED_POKEMON\n");

			enviarMensajeBrokerAppeared(argv[3], atoi(argv[4]), atoi(argv[5]),
					atoi(argv[6]), socketBroker);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy BROKER APPEARED_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE_CORRELATIVO]\n");
		}

	}

	if ((strcmp(argv[1], "BROKER") == 0)
			&& (strcmp(argv[2], "CATCH_POKEMON") == 0)) { //ok
		if (argc == 6) {
			printf("Voy a enviar CATCH_POKEMON\n");
			enviarMensajeBrokerCatch(argv[3], atoi(argv[4]), atoi(argv[5]),
					socketBroker);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy BROKER CATCH_POKEMON [POKEMON] [POSX] [POSY]\n");
		}
	}

	if ((strcmp(argv[1], "BROKER") == 0)
			&& (strcmp(argv[2], "CAUGHT_POKEMON") == 0)) { //NO ok
		if (argc == 5) {
			printf("Voy a enviar CAUGHT_POKEMON\n");
			enviarMensajeBrokerCaught( atoi(argv[3]),atoi(argv[4]),
					socketBroker);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy BROKER CAUGHT_POKEMON [ID_MENSAJE_CORRELATIVO] [OK/FAIL]\n");
		}
	}

	if ((strcmp(argv[1], "BROKER") == 0)
			&& (strcmp(argv[2], "GET_POKEMON") == 0)) { //NO OK
		if (argc == 4) {
			printf("Voy a enviar GET_POKEMON\n");
			enviarMensajeBrokerGet(argv[3], socketBroker);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf("Formato válido ./gameboy BROKER GET_POKEMON [POKEMON]\n");
		}

	}

	if ((strcmp(argv[1], "TEAM") == 0)
			&& (strcmp(argv[2], "APPEARED_POKEMON") == 0)) { //ok
		if (argc == 6) {
			printf("Voy a enviar APPEARED_POKEMON\n");
			enviarMensajeTeamAppeared(argv[3], atoi(argv[4]), atoi(argv[5]),
					socketTeam);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy TEAM APPEARED_POKEMON [POKEMON] [POSX] [POSY]\n");
		}
	}

	if ((strcmp(argv[1], "GAME_CARD") == 0)
			&& (strcmp(argv[2], "NEW_POKEMON") == 0)) { //ok
		if (argc == 8) {
			printf("Voy a enviar NEW_POKEMON\n");

			enviarMensajeGameCardNewPokemon(argv[3], atoi(argv[4]),
					atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), socketGameCard);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy GAMECARD NEW_POKEMON [POKEMON] [POSX] [POSY] [CANTIDAD] [ID_MENSAJE_CORRELATIVO]\n");
		}
	}

	if ((strcmp(argv[1], "GAME_CARD") == 0)
			&& (strcmp(argv[2], "CATCH_POKEMON") == 0)) { //ok
		if (argc == 7) {
			printf("Voy a enviar CATCH_POKEMON\n");
			enviarMensajeGameCardCatchPokemon(argv[3], atoi(argv[4]),
					atoi(argv[5]), atoi(argv[6]), socketGameCard);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy GAMECARD CATCH_POKEMON [POKEMON] [POSX] [POSY] [ID_MENSAJE_CORRELATIVO]\n");
		}

	}
	if ((strcmp(argv[1], "GAME_CARD") == 0)
			&& (strcmp(argv[2], "GET_POKEMON") == 0)) {	//ok
		if (argc == 5) {
			printf("Voy a enviar GET_POKEMON\n");
			enviarMensajeGameCardGetPokemon(argv[3], atoi(argv[4]),
					socketGameCard);
		} else {
			printf("Cantidad de argumentos incorrectos.\n");
			printf(
					"Formato válido ./gameboy GAMECARD GET_POKEMON [POKEMON] [ID_MENSAJE_CORRELATIVO]\n");
		}

	}

	if ((strcmp(argv[1], "SUSCRIPTOR") == 0)
			&& (strcmp("NEW_POKEMON", argv[2]) == 0)) { //ok
		if (argc == 4) {
			suscribirseNew(atoi(argv[3]), socketBroker);
		} else {

		}
		printf("Cantidad de argumentos incorrectos.\n");
		printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");

	}

	if ((strcmp(argv[1], "SUSCRIPTOR") == 0)
			&& (strcmp(argv[2], "APPEARED_POKEMON") == 0)) { //ok
		if (argc == 4) {
			printf("Voy a enviar SUBS_APPEARED_POKEMON\n");
			suscribirseAppeared(atoi(argv[3]), socketBroker);
		} else {

		}
		printf("Cantidad de argumentos incorrectos.\n");
		printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
	}

	if ((strcmp(argv[1], "SUSCRIPTOR") == 0)
			&& (strcmp(argv[2], "CATCH_POKEMON") == 0)) { //ok
		if (argc == 4) {
			printf("Voy a enviar SUBS_CATCH_POKEMON\n");

			suscribirseCatch(atoi(argv[3]), socketBroker);
		} else {

		}
		printf("Cantidad de argumentos incorrectos.\n");
		printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
	}

	if ((strcmp(argv[1], "SUSCRIPTOR") == 0)
			&& (strcmp(argv[2], "CAUGHT_POKEMON") == 0)) { //ok
		if (argc == 4) {
			printf("Voy a enviar SUBS_CAUGHT_POKEMON\n");
			suscribirseCaught(atoi(argv[3]), socketBroker);
		} else {

		}
		printf("Cantidad de argumentos incorrectos.\n");
		printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");
	}

	if ((strcmp(argv[1], "SUSCRIPTOR") == 0)
			&& (strcmp(argv[2], "GET_POKEMON") == 0)) { //ok
		if (argc == 4) {
			printf("Voy a enviar SUBS_GET_POKEMON\n");
			suscribirseGet(atoi(argv[3]), socketBroker);
		} else {

		}
		printf("Cantidad de argumentos incorrectos.\n");
		printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");

	}
	if ((strcmp(argv[1], "SUSCRIPTOR") == 0)
			&& (strcmp(argv[2], "LOCALIZED_POKEMON") == 0)) { //ok
		if (argc == 4) {
			printf("Voy a enviar SUBS_LOCALIZED_POKEMON\n");
			suscribirseLocalized(atoi(argv[3]), socketBroker);
		} else {

		}
		printf("Cantidad de argumentos incorrectos.\n");
		printf("Formato válido ./gameboy SUSCRIPTOR COLA [TIEMPO]\n");

	}

//	free(consoleLineAux);
//	free(consoleLine);


	liberarConexion(socketBroker);
	liberarGameBoyConfig();

	return EXIT_SUCCESS;
}
