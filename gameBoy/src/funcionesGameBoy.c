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
	gameBoyConf->nombre = string_duplicate(
			config_get_string_value(GAMEBOYTConfig, "NOMBRE_PROCESO"));

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

//no me acuerdo si arranca en 0 o en 1 el valor de los argumentos, hay que probar esto

