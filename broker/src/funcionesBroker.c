#include "broker.h"

void cargarConfiguracionBROKER()
{
	//Carga la configuracion del txt de config al struct de config

	BROKERTConfig = config_create(BROKER_CONFIG_PATH);

	if (BROKERTConfig == NULL)
	{
		log_error(logger, "- NO SE PUDO IMPORTAR LA CONFIGURACION");
		exit(1);
	}
	log_info(logger, "- CONFIGURACION IMPORTADA");

	BROKERConfig->LISTEN_PORT = config_get_int_value(BROKERTConfig, PUERTO_BROKER);
	BROKERConfig->tamanoMemoria = config_get_int_value(BROKERTConfig, TAMANO_MEMORIA);
	BROKERConfig->tamanoMinimoParticion = config_get_int_value(BROKERTConfig, TAMANO_MINIMO_PARTICION);
	BROKERConfig->algoritmoMemoria = config_get_array_value(BROKERTConfig, ALGORITMO_MEMORIA);
	BROKERConfig->algoritmoReemplazo = config_get_array_value(BROKERTConfig, ALGORITMO_REEMPLAZO);
	BROKERConfig->ipBroker = config_get_array_value(BROKERTConfig, IP_BROKER);//ver esto en sockets
	BROKERConfig->puertoBroker = config_get_int_value(BROKERTConfig, PUERTO_BROKER);
	BROKERConfig->frecuenciaCompactacion = config_get_int_value(BROKERTConfig, FRECUENCIA_COMPACTACION);
	BROKERConfig->logFile = config_get_array_value(BROKERTConfig, LOG_FILE);



	log_info(logger, "· Puerto escucha = %d", BROKERConfig->LISTEN_PORT);
	log_info(logger, "· IP  = %s", BROKERConfig->ipBroker);


	free(BROKERTConfig);
}

void inicializarLogger()
{
	//Crea el logger
	logger = log_create("logs/SUSE.log", "SUSE", true, LOG_LEVEL_TRACE);
}
