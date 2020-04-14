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

    t_BROKERConfig *brokerConf = malloc(sizeof(t_BROKERConfig));

    brokerConf->tamanoMemoria = config_get_int_value(BROKERTConfig,"TAMANO_MEMORIA");
    brokerConf->tamanoMinimoParticion = config_get_int_value(BROKERTConfig,"TAMANO_MINIMO_PARTICION");
    brokerConf->algoritmoMemoria = config_get_string_value(BROKERTConfig,"ALGORITMO_MEMORIA");
    brokerConf->algoritmoReemplazo = config_get_string_value(BROKERTConfig,"ALGORITMO_REEMPLAZO");
    //ver esto en sockets
    brokerConf->ipBroker = config_get_string_value(BROKERTConfig, "IP_BROKER");
    brokerConf->puertoBroker = config_get_int_value(BROKERTConfig,"PUERTO_BROKER");
    brokerConf->frecuenciaCompactacion = config_get_int_value(BROKERTConfig,"FRECUENCIA_COMPACTACION");
    brokerConf->logFile = config_get_string_value(BROKERTConfig, "LOG_FILE");

    printf("Tamanio de memoria usado: %d \n", brokerConf->tamanoMemoria);
    printf("Puerto usado: %d \n", brokerConf->puertoBroker);
    printf("Tamnio minimo de particion usado: %d \n", brokerConf->tamanoMinimoParticion);

//	log_info(logger, "· Puerto escucha = %d", BROKERConfig->puertoBroker);
//	log_info(logger, "· IP  = %s", BROKERConfig->ipBroker);
//
//	printf("pude cargar la configuracion correctamente,mi puerto es %d",BROKERConfig->puertoBroker);
	free(brokerConf);
}

//void cargarConfigBROKER(){
//	printf("probando impresion en pantalla");
//	//Carga la configuracion del txt de config al struct de config
//	BROKERTConfig = config_create("broker.config");
//
//	if (BROKERTConfig == NULL)
//		{
//			perror("error archivo de configuracion");
//			log_error(logger, "- NO SE PUDO IMPORTAR LA CONFIGURACION");
//			exit(1);
//		}
//		log_info(logger, "- CONFIGURACION IMPORTADA");
//
//
//}
