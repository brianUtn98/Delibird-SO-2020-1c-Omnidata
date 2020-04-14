#include "broker.h"

void inicializarLogger() {
	//Crea el logger
	logger = log_create("logs/BROKER.log", "BROKER", true, LOG_LEVEL_TRACE);
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
//	int BROKERConfig;
//	BROKERConfig = config_get_int_value(BROKERTConfig,"TAMANO_MEMORIA");

	//printf("pude cargar la configuracion correctamente,mi puerto es %d",BROKERConfig);


	BROKERConfig->tamanoMemoria = config_get_int_value(BROKERTConfig,"TAMANO_MEMORIA");
	printf("pude cargar la configuration correctamente, mi puerto es %d",BROKERConfig->tamanoMemoria);

	BROKERConfig->tamanoMinimoParticion = config_get_int_value(BROKERTConfig,"TAMANO_MINIMO_PARTICION");
	BROKERConfig->algoritmoMemoria = config_get_string_value(BROKERTConfig,"ALGORITMO_MEMORIA");
	BROKERConfig->algoritmoReemplazo = config_get_string_value(BROKERTConfig,"ALGORITMO_REEMPLAZO");
	BROKERConfig->ipBroker = config_get_string_value(BROKERTConfig, "IP_BROKER");//ver esto en sockets
	BROKERConfig->puertoBroker = config_get_int_value(BROKERTConfig,"PUERTO_BROKER");

	BROKERConfig->frecuenciaCompactacion = config_get_int_value(BROKERTConfig,"FRECUENCIA_COMPACTACION");
	BROKERConfig->logFile = config_get_string_value(BROKERTConfig, "LOG_FILE");

//	log_info(logger, "· Puerto escucha = %d", BROKERConfig->puertoBroker);
//	log_info(logger, "· IP  = %s", BROKERConfig->ipBroker);
//
//	printf("pude cargar la configuracion correctamente,mi puerto es %d",BROKERConfig->puertoBroker);
	//free(BROKERTConfig);
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
