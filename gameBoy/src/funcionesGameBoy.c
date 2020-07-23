#include "gameBoy.h"

void inicializarLoggerGameBoy() {
	//Crea el logger
	logger = log_create("GAMEBOY.log", "GAMEBOY", 1, LOG_LEVEL_TRACE);
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
	gameBoyConf->nombre = string_duplicate(
			config_get_string_value(GAMEBOYTConfig, "NOMBRE_PROCESO"));
	gameBoyConf->LOG_FILE = string_duplicate(
			config_get_string_value(GAMEBOYTConfig, "LOG_FILE"));

	//brokerConf->logFile = config_get_string_value(GAMEBOYTConfig, "LOG_FILE");

	printf(" puerto broker usado: %d \n", gameBoyConf->puertoBroker);
	printf(" Puerto team usado: %d \n", gameBoyConf->puertoTeam);
	printf(" ip broker usado: %s \n", gameBoyConf->ipBroker);
	printf(" nombre del proceso: %s \n", gameBoyConf->nombre);

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

void mostrarChar(void *elemento) {
	//log_info(logger,"%s",(char*)elemento);
	printf("%s\n", (char*) elemento);
}

void mostrarListaChar(t_list *lista) {
	t_list *aux = list_duplicate(lista);

	while (aux->head != NULL) {
		mostrarChar(aux->head->data);
		aux->head = aux->head->next;
	}
	list_destroy(aux);
}

void *iniciarConexionBroker(void *arg) { //esta es una funcion que va a recibir el pthread_create
	socketBroker = crearConexion(gameBoyConf->ipBroker,
			gameBoyConf->puertoBroker, 30);
	arg = (void*) socket;
	return NULL;
}
void *iniciarConexionTeam(void *arg) {
	socketTeam = crearConexion(gameBoyConf->ipTeam, gameBoyConf->puertoTeam,
			30);
	return NULL;
}
void *iniciarConexionGameCard(void *arg) {
	socketGameCard = crearConexion(gameBoyConf->ipGameCard,
			gameBoyConf->puertoGameCard, 30);
	return NULL;
}


void* procesarMensaje() { // aca , la idea es saber que pokemon ponemos en el mapa por ejemplo.
	//printf("Rompo en procesarMensaje 1\n");
	t_paquete* bufferLoco = malloc(sizeof(t_paquete));

	while (1) {
		printf("ESPERA ACTIVA? procesarMensaje\n");
		printf("Rompo en procesarMensaje 2\n");
		sem_wait(&contadorBandeja);
		pthread_mutex_lock(&mutex_bandeja);
		printf("Rompo en procesarMensaje 3\n");
		bufferLoco = (t_paquete*) queue_pop(bandejaDeMensajes); //ver en que posicion busco, por ahi se necesita una variable.
//	printf("Rompo en procesarMensaje 4\n");
		pthread_mutex_unlock(&mutex_bandeja);

		printf("el bufferloco dice %d, %s , %d , %d , %d.\n",
				bufferLoco->codigoOperacion, bufferLoco->buffer->nombrePokemon,
				bufferLoco->buffer->cantidadPokemons, bufferLoco->buffer->posX,
				bufferLoco->buffer->posY);
		switch (bufferLoco->codigoOperacion) {
		case MENSAJE_APPEARED_POKEMON: { //ver que casos usa el team

			log_info(logEntrega, "Llego mensaje APPEARED_POKEMON.\n");

			break;
		}
		case MENSAJE_CAUGHT_POKEMON: {
			log_info(logEntrega, "Llego mensaje CAUGHT_POKEMON.\n");
			break;
		}

		case MENSAJE_LOCALIZED_POKEMON: {

			log_info(logEntrega, "Llego mensaje LOCALIZED_POKEMON.\n");
			break;
		}
		case MENSAJE_CATCH_POKEMON: {

			log_info(logEntrega, "LLego mensaje CATCH_POKEMON.\n");
			break;
		}
		case MENSAJE_NEW_POKEMON: {
			printf("entre en el case new.\n");
			log_info(logEntrega, "LLego mensaje NEW_POKEMON.\n");
			break;
		}
		case MENSAJE_GET_POKEMON: {

			log_info(logEntrega, "LLego mensaje GET_POKEMON.\n");
			break;
		}
		default: {
			printf("Ese menesaje no es interpretado por el gameBoy!!!\n");
			exit(1);
		}
		}
	}
	pthread_exit(NULL);
	return NULL;
}

void *suscribirseBrokerAppeared() {
	int socketSuscripcion = crearConexion(gameBoyConf->ipBroker,
			gameBoyConf->puertoBroker, 0);

	suscribirseAppeared(gameBoyConf->nombre, 0, socketSuscripcion);
	pthread_mutex_t mutexRecibir;
	pthread_mutex_init(&mutexRecibir, NULL);

	t_paquete *bufferLoco;

	int flagAppeared = 1;
	while (flagAppeared) {
		printf("ESPERA ACTIVA suscribirseBrokerAppeared? \n");
		pthread_mutex_lock(&mutexRecibir);
		bufferLoco = recibirMensaje(socketSuscripcion);

		if (bufferLoco != NULL) {
			enviarAck(gameBoyConf->nombre, bufferLoco, socketSuscripcion);
			pthread_mutex_lock(&mutex_bandeja);
			queue_push(bandejaDeMensajes, (void*) bufferLoco);
			pthread_mutex_unlock(&mutex_bandeja);
			pthread_mutex_unlock(&mutexRecibir);
			sem_post(&contadorBandeja);
		}
	}

	return NULL;
}
void *suscribirseBrokerLocalized() {
	int socketSuscripcion = crearConexion(gameBoyConf->ipBroker,
			gameBoyConf->puertoBroker, 0);

	suscribirseLocalized(gameBoyConf->nombre, 0, socketSuscripcion);
	pthread_mutex_t mutexRecibir;
	pthread_mutex_init(&mutexRecibir, NULL);

	t_paquete *bufferLoco;

	int flagLocalized = 1;
	while (flagLocalized) {
		printf("ESPERA ACTIVA suscribirseBrokerLocalized? \n");
		pthread_mutex_lock(&mutexRecibir);
		bufferLoco = recibirMensaje(socketSuscripcion);

		if (bufferLoco != NULL) {
			enviarAck(gameBoyConf->nombre, bufferLoco, socketSuscripcion);
			pthread_mutex_lock(&mutex_bandeja);
			queue_push(bandejaDeMensajes, (void*) bufferLoco);
			pthread_mutex_unlock(&mutex_bandeja);
			pthread_mutex_unlock(&mutexRecibir);
			sem_post(&contadorBandeja);
		}
	}

	return NULL;
}
void *suscribirseBrokerNew() {
	int socketSuscripcion = crearConexion(gameBoyConf->ipBroker,
			gameBoyConf->puertoBroker, 0);

	suscribirseNew(gameBoyConf->nombre, 0, socketSuscripcion);
	pthread_mutex_t mutexRecibir;
	pthread_mutex_init(&mutexRecibir, NULL);

	t_paquete *bufferLoco;

	int flagNew = 1;
	while (flagNew) {
		printf("ESPERA ACTIVA suscribirseBrokerNew? \n");
//log_debug(logger, "Sali del if");
		pthread_mutex_lock(&mutexRecibir);
		bufferLoco = recibirMensaje(socketSuscripcion);

		if (bufferLoco != NULL) {
			enviarAck(gameBoyConf->nombre, bufferLoco, socketSuscripcion);
			pthread_mutex_lock(&mutex_bandeja);
			queue_push(bandejaDeMensajes, (void*) bufferLoco);
			pthread_mutex_unlock(&mutex_bandeja);
			printf("meti algo en la bandeja.\n");
			sem_post(&contadorBandeja);
			pthread_mutex_unlock(&mutexRecibir);

			printf("incremente el semaforo contador.\n");
		}
	}
	liberarConexion(socketSuscripcion);
	pthread_exit(NULL);
	return NULL;
}
void *suscribirseBrokerGet() {
	int socketSuscripcion = crearConexion(gameBoyConf->ipBroker,
			gameBoyConf->puertoBroker, 0);

	suscribirseGet(gameBoyConf->nombre, 0, socketSuscripcion);
	pthread_mutex_t mutexRecibir;
	pthread_mutex_init(&mutexRecibir, NULL);

	t_paquete *bufferLoco;

	int flagGet = 1;
	while (flagGet) {
		printf("ESPERA ACTIVA suscribirseBrokerGet? \n");
//log_debug(logger, "Sali del if");
		pthread_mutex_lock(&mutexRecibir);
		bufferLoco = recibirMensaje(socketSuscripcion);

		if (bufferLoco != NULL) {
			enviarAck(gameBoyConf->nombre, bufferLoco, socketSuscripcion);
			pthread_mutex_lock(&mutex_bandeja);
			queue_push(bandejaDeMensajes, (void*) bufferLoco);
			pthread_mutex_unlock(&mutex_bandeja);
			pthread_mutex_unlock(&mutexRecibir);
			sem_post(&contadorBandeja);
		}
	}

	return NULL;
}
void *suscribirseBrokerCatch() {
	int socketSuscripcion = crearConexion(gameBoyConf->ipBroker,
			gameBoyConf->puertoBroker, 0);

	suscribirseCatch(gameBoyConf->nombre, 0, socketSuscripcion);
	pthread_mutex_t mutexRecibir;
	pthread_mutex_init(&mutexRecibir, NULL);

	t_paquete *bufferLoco;

	int flagCatch = 1;
	while (flagCatch) {
		printf("ESPERA ACTIVA suscribirseBrokerAppeared? \n");
//log_debug(logger, "Sali del if");
		pthread_mutex_lock(&mutexRecibir);
		bufferLoco = recibirMensaje(socketSuscripcion);

		if (bufferLoco != NULL) {
			enviarAck(gameBoyConf->nombre, bufferLoco, socketSuscripcion);
			pthread_mutex_lock(&mutex_bandeja);
			queue_push(bandejaDeMensajes, (void*) bufferLoco);
			pthread_mutex_unlock(&mutex_bandeja);
			pthread_mutex_unlock(&mutexRecibir);
			sem_post(&contadorBandeja);
		}
	}

	return NULL;
}
void *suscribirseBrokerCaught() {
	int socketSuscripcion = crearConexion(gameBoyConf->ipBroker,
			gameBoyConf->puertoBroker, 0);

	suscribirseCaught(gameBoyConf->nombre, 0, socketSuscripcion);
	pthread_mutex_t mutexRecibir;
	pthread_mutex_init(&mutexRecibir, NULL);

	t_paquete *bufferLoco;

	int flagCaught = 1;
	while (flagCaught) {
		printf("ESPERA ACTIVA suscribirseBrokerAppeared? \n");
//log_debug(logger, "Sali del if");
		pthread_mutex_lock(&mutexRecibir);
		bufferLoco = recibirMensaje(socketSuscripcion);

		if (bufferLoco != NULL) {
			enviarAck(gameBoyConf->nombre, bufferLoco, socketSuscripcion);
			pthread_mutex_lock(&mutex_bandeja);
			queue_push(bandejaDeMensajes, (void*) bufferLoco);
			pthread_mutex_unlock(&mutex_bandeja);
			pthread_mutex_unlock(&mutexRecibir);
			sem_post(&contadorBandeja);
		}
	}

	return NULL;
}
void inicializarLoggerEntregable() {

	logEntrega = log_create(gameBoyConf->LOG_FILE, "GAMEBOY", 1,
			LOG_LEVEL_TRACE);
	if (logEntrega == NULL) {
		perror("No se pudo inicializar el logger para la entrega\n");
	}
}
