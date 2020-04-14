#ifndef GAMECARD_GAMECARD_H_
#define GAMECARD_GAMECARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include "../sockets/servidor.h"

#define GAMECARD_CONFIG_PATH "gameCard.config"

typedef struct {
	int tiempoReintentoConexion;
	int tiempoReintentoOperacion;
	char *puntoDeMontaje;
	char *ipBroker;
	char *puertoBroker;

} t_GAMECARDConfig;

t_log *logger;
t_config *GAMECARDTConfig;

void inicializarLogger(void);
void cargarConfigGameCard(void);
void iniciarServidor(void);

#endif /* GAMECARD_GAMECARD_H_ */
