#ifndef BROKER_BROKER_H_
#define BROKER_BROKER_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <../MiLibreria/utils/servidor.h>
//#include "../sockets/servidor.h"
//#include "../bibliotecasPropias/serializacion/serializacion.h"
//#include "../bibliotecasPropias/sockets/sockets.h"

#define BROKER_CONFIG_PATH "../broker.config"

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

typedef struct {
	t_queue *cola;
	t_list *lista;
} t_cola;

t_log *logger;
t_config *BROKERTConfig;
t_BROKERConfig *brokerConf;

t_cola *NEW_POKEMON;
t_cola *APPEARED_POKEMON;
t_cola *CATCH_POKEMON;
t_cola *CAUGTH_POKEMON;
t_cola *GET_POKEMON;
t_cola *LOCALIZED_POKEMON;

typedef struct {
	t_cola *cola1;
	t_cola *cola2;
} t_parejaCola;
t_parejaCola *NEW_APPEARED_POKEMON;
t_parejaCola *CATCH_CAUGTH_POKEMON;
t_parejaCola *GET_LOCALIZED_POKEMON;

void inicializarLogger(void);
void cargarConfigBroker(void);
void correrServidor(char *ip,int puerto);
void inicializarColasBroker(void);
void destruirColasBroker(void);
void agregarMensaje(t_cola*, void*);
char* sacarMensaje(t_cola*);

#endif /* BROKER_BROKER_H_ */
