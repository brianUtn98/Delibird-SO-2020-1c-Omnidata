#ifndef BROKER_BROKER_H_
#define BROKER_BROKER_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include "../sockets/servidor.h"

#define BROKER_CONFIG_PATH "configs/broker.config"
#define TAMANO_MEMORIA "TAMAÑO_MEMORIA"
#define TAMANO_MINIMO_PARTICION "TAMAÑO_MINIMO_PARTICION"
#define ALGORITMO_MEMORIA "ALGORITMO_MEMORIA"
#define ALGORITMO_REEMPLAZO "ALGORITMO_REEMPLAZO"
#define IP_BROKER "IP_BROKER"
#define PUERTO_BROKER "LISTEN_PORT"
#define FRECUENCIA_COMPACTACION "FRECUENCIA_COMPACTACION"
#define LOG_FILE "LOG_FILE"


typedef struct
{
	int16_t LISTEN_PORT;
	int16_t tamanoMemoria;
	int16_t tamanoMinimoParticion;
	char **algoritmoMemoria;
	char **algoritmoReemplazo;
	char **ipBroker;
	int16_t puertoBroker;
	int16_t frecuenciaCompactacion;
	char ** logFile;

} t_BROKERConfig;

t_log *logger;
t_BROKERConfig *BROKERConfig;
t_config *BROKERTConfig;












#endif /* BROKER_BROKER_H_ */
