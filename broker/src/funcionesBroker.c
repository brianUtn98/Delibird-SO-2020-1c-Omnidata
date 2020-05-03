#include "broker.h"

void inicializarLogger() {
	//Crea el logger
	logger = log_create("BROKER.log", "BROKER", 1, LOG_LEVEL_TRACE);
}

void cargarConfigBROKER() {
	//printf("pude cargar la configuracion correctamente");
	//Carga la configuracion del txt de config al struct de config

	//brokerConf = (t_BROKERConfig)malloc(sizeof(t_BROKERConfig));
	brokerConf = (t_BROKERConfig*) malloc(sizeof(t_BROKERConfig));
	BROKERTConfig = config_create(BROKER_CONFIG_PATH);
	//inicializarLogger();
	if (BROKERTConfig == NULL) {
		perror("error archivo de configuracion");
		log_error(logger, "- NO SE PUDO IMPORTAR LA CONFIGURACION");
		exit(1);
	}

	brokerConf->tamanoMemoria = config_get_int_value(BROKERTConfig,
			"TAMANO_MEMORIA");
	log_info(logger, "TAMANO_MEMORIA=%d", brokerConf->tamanoMemoria);
	brokerConf->tamanoMinimoParticion = config_get_int_value(BROKERTConfig,
			"TAMANO_MINIMO_PARTICION");
	log_info(logger, "TAMANO_MINIMO_PARTICION=%d",
			brokerConf->tamanoMinimoParticion);
	brokerConf->algoritmoMemoria = string_duplicate(
			config_get_string_value(BROKERTConfig, "ALGORITMO_MEMORIA"));
	log_info(logger, "ALGORITMO_MEMORIA=%s", brokerConf->algoritmoMemoria);
	brokerConf->algoritmoReemplazo = string_duplicate(
			config_get_string_value(BROKERTConfig, "ALGORITMO_REEMPLAZO"));
	log_info(logger, "ALGORITMO_REEMPLAZO=%s", brokerConf->algoritmoReemplazo);
	brokerConf->ipBroker = string_duplicate(
			config_get_string_value(BROKERTConfig, "IP_BROKER"));
	log_info(logger, "IP_BROKER=%s", brokerConf->ipBroker);
	brokerConf->puertoBroker = config_get_int_value(BROKERTConfig,
			"PUERTO_BROKER");
	log_info(logger, "PUERTO_BROKER=%d", brokerConf->puertoBroker);
	brokerConf->frecuenciaCompactacion = config_get_int_value(BROKERTConfig,
			"FRECUENCIA_COMPACTACION");
	log_info(logger, "FRECUENCIA_COMPACTACION=%d",
			brokerConf->frecuenciaCompactacion);
	brokerConf->logFile = string_duplicate(
			config_get_string_value(BROKERTConfig, "LOG_FILE"));
	log_info(logger, "LOG_FILE=%s", brokerConf->logFile);

	log_info(logger, "- CONFIGURACION IMPORTADA\n");

	config_destroy(BROKERTConfig);

	//ver cuando liberar el brokerConf , si lo hacemos acá no se va a poder usar en el servidor por ej,
	//estariamos cargando una estructura y liberandola sin darle uso.
	// talvez podriamos devolver la estructura y que la función no sea void.
	//hacer...
	//return brokerConf;
	//free(brokerConf);

}
//Hago esta función para acordarnos que debemos liberar el malloc de Sofi
void liberarBrokerConf() {
	free(brokerConf);
}
//falta inicializar la estructura pareja de colas
void inicializarColasBroker() {
	NEW_POKEMON = malloc(sizeof(t_cola));
	APPEARED_POKEMON = malloc(sizeof(t_cola));
	CATCH_POKEMON = malloc(sizeof(t_cola));
	CAUGTH_POKEMON = malloc(sizeof(t_cola));
	GET_POKEMON = malloc(sizeof(t_cola));
	LOCALIZED_POKEMON = malloc(sizeof(t_cola));

	NEW_POKEMON->cola = queue_create();
	NEW_POKEMON->lista = list_create();
	APPEARED_POKEMON->cola = queue_create();
	APPEARED_POKEMON->lista = list_create();
	CATCH_POKEMON->cola = queue_create();
	CATCH_POKEMON->lista = list_create();
	CAUGTH_POKEMON->cola = queue_create();
	CAUGTH_POKEMON->lista = list_create();
	GET_POKEMON->cola = queue_create();
	GET_POKEMON->lista = list_create();
	LOCALIZED_POKEMON->cola = queue_create();
	LOCALIZED_POKEMON->lista = list_create();

	NEW_APPEARED_POKEMON = malloc(sizeof(t_parejaCola));
	CATCH_CAUGTH_POKEMON = malloc(sizeof(t_parejaCola));
	GET_LOCALIZED_POKEMON = malloc(sizeof(t_parejaCola));
}

void destruirColasBroker() {
	queue_destroy(NEW_POKEMON->cola);
	list_destroy(NEW_POKEMON->lista);
	queue_destroy(APPEARED_POKEMON->cola);
	list_destroy(APPEARED_POKEMON->lista);
	queue_destroy(CATCH_POKEMON->cola);
	list_destroy(CATCH_POKEMON->lista);
	queue_destroy(CAUGTH_POKEMON->cola);
	list_destroy(CAUGTH_POKEMON->lista);
	queue_destroy(GET_POKEMON->cola);
	list_destroy(GET_POKEMON->lista);
	queue_destroy(LOCALIZED_POKEMON->cola);
	list_destroy(LOCALIZED_POKEMON->lista);

	free(NEW_POKEMON);
	free(APPEARED_POKEMON);
	free(CATCH_POKEMON);
	free(CAUGTH_POKEMON);
	free(CATCH_CAUGTH_POKEMON);
	free(LOCALIZED_POKEMON);

	free(NEW_APPEARED_POKEMON);
	free(CATCH_CAUGTH_POKEMON);
	free(GET_LOCALIZED_POKEMON);

}
//ver que se agrega a la lista y a la cola
void agregarMensaje(t_cola *cola, void* mensaje) {

	queue_push(cola->cola, mensaje);
	list_add(cola->lista, mensaje);

}

//no se bien como usar la funcion find de las commons
//int buscarEnLista(t_suscriptor *suscriptor) {
//
//	return 1;
//}
//t_suscriptor sacarSuscriptor(t_cola *cola, t_suscriptor *suscriptor) {
//
//	int *index;
//	index = list_find(cola->lista,);
//
//	return (t_suscriptor)list_get(cola->lista, index);
//
//}

//ver bien el argumento que le pasamos a la funcion & * o nada, ver si es tipo mensaje o char*
char* sacarMensaje(t_cola *cola) {
	char* mensaje;
	return mensaje = (char*) queue_pop(cola->cola);
}

void administrarColas(void* stream) {


	//esto seria deserializar paquete
	int opCode;
	t_suscriptor *suscriptor = malloc(sizeof(t_suscriptor));

	int cola;
	t_mensaje mensaje = malloc(sizeof(t_mensaje));

	switch (opCode) {
	case SUSCRIPCION: {
		switch (cola) {

		case tNEW_POKEMON: {
			list_add(NEW_POKEMON->lista, suscriptor);

			break;
		}
		case tAPPEARED_POKEMON: {
			list_add(APPEARED_POKEMON->lista, suscriptor);
			break;
		}

		case tCATCH_POKEMON: {
			list_add(CATCH_POKEMON->lista, suscriptor);
			break;
		}

		case tCAUGTH_POKEMON: {
			list_add(CAUGTH_POKEMON->lista, suscriptor);
			break;
		}

		case tGET_POKEMON: {
			list_add(GET_POKEMON->lista, suscriptor);
			break;
		}

		case tLOCALIZED_POKEMON: {
			list_add(LOCALIZED_POKEMON->lista, suscriptor);
			break;
		}
		default: {
			printf("error de modulo, no se conoce quien envia paquetes\n");
		}

		}
		case MENSAJE:
		{
			switch (mensaje) {

			case tNEW_POKEMON: {
				//list_add(NEW_POKEMON->lista, suscriptor);

				break;
			}
			case tAPPEARED_POKEMON: {
				//list_add(APPEARED_POKEMON->lista, suscriptor);
				break;
			}

			case tCATCH_POKEMON: {
				//list_add(CATCH_POKEMON->lista, suscriptor);
				break;
			}

			case tCAUGTH_POKEMON: {
				//list_add(CAUGTH_POKEMON->lista, suscriptor);
				break;
			}

			case tGET_POKEMON: {
				//list_add(GET_POKEMON->lista, suscriptor);
				break;
			}

			case tLOCALIZED_POKEMON: {
				//list_add(LOCALIZED_POKEMON->lista, suscriptor);
				break;
			}
			default: {
				printf("error de modulo, no se conoce quien envia paquetes\n");
			}
			}
		}
	}

		free(suscriptor);
		free(mensaje);
	}
}
