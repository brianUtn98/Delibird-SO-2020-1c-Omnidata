#ifndef BROKER_BROKER_H_
#define BROKER_BROKER_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include "../sockets/servidor.h"

#define BROKER_CONFIG_PATH "broker.config"

typedef struct {
	int tamanoMemoria;
	int tamanoMinimoParticion;
	char *algoritmoMemoria;
	char *algoritmoReemplazo;
	char *ipBroker;
	int puertoBroker;
	int frecuenciaCompactacion;
	char *logFile;

} t_BROKERConfig;

t_log *logger;
t_config *BROKERTConfig;

void inicializarLogger(void);
void cargarConfigBroker(void);
void iniciarServidor(void);

#endif /* BROKER_BROKER_H_ */
