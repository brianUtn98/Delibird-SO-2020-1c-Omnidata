#include "broker.h"

void inicializarLogger() {
	//Crea el logger
	logger = log_create("logs/BROKER.log", "BROKER", 1, LOG_LEVEL_TRACE);
}

void cargarConfigBROKER() {
	//printf("pude cargar la configuracion correctamente");
	//Carga la configuracion del txt de config al struct de config

	BROKERTConfig = config_create(BROKER_CONFIG_PATH);
	//inicializarLogger();
	if (BROKERTConfig == NULL) {
		perror("error archivo de configuracion");
		log_error(logger, "- NO SE PUDO IMPORTAR LA CONFIGURACION");
		exit(1);
	}

	log_info(logger, "- CONFIGURACION IMPORTADA\n");

    brokerConf = malloc(sizeof(t_BROKERConfig));

    brokerConf->tamanoMemoria = config_get_int_value(BROKERTConfig,"TAMANO_MEMORIA");
    brokerConf->tamanoMinimoParticion = config_get_int_value(BROKERTConfig,"TAMANO_MINIMO_PARTICION");
    brokerConf->algoritmoMemoria = config_get_string_value(BROKERTConfig,"ALGORITMO_MEMORIA");
    brokerConf->algoritmoReemplazo = config_get_string_value(BROKERTConfig,"ALGORITMO_REEMPLAZO");
    brokerConf->ipBroker = config_get_string_value(BROKERTConfig, "IP_BROKER");
    brokerConf->puertoBroker = config_get_int_value(BROKERTConfig,"PUERTO_BROKER");
    brokerConf->frecuenciaCompactacion = config_get_int_value(BROKERTConfig,"FRECUENCIA_COMPACTACION");
    brokerConf->logFile = config_get_string_value(BROKERTConfig, "LOG_FILE");

    printf("Tamanio de memoria usado: %d \n", brokerConf->tamanoMemoria);
    printf("Puerto usado: %d \n", brokerConf->puertoBroker);
    printf("Tamanio minimo de particion usado: %d \n", brokerConf->tamanoMinimoParticion);

	log_info(logger, "· Puerto escucha = %d", brokerConf->puertoBroker);
	log_info(logger, "· IP  = %s", brokerConf->ipBroker);


	//ver cuando liberar el brokerConf , si lo hacemos acá no se va a poder usar en el servidor por ej,
	//estariamos cargando una estructura y liberandola sin darle uso.
	// talvez podriamos devolver la estructura y que la función no sea void.
	//hacer...
	//return brokerConf;
	//free(brokerConf);
}
//Hago esta función para acordarnos que debemos liberar el malloc de Sofi
void liberarBrokerConf(){
	free(brokerConf);
}
