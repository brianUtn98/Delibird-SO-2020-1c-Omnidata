#ifndef GAMECARD_GAMECARD_H_
#define GAMECARD_GAMECARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
//#include "../sockets/servidor.h"

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
t_GAMECARDConfig *gameCardConfig;

void inicializar_logger(void);
void cargarConfigGameCard(void);
//void iniciarServidor(void);
void* serializar_paquete();
int crear_conexion(char*,char*);
void enviar_mensaje(char*,int);
char* recibir_mensaje(int);
void liberar_conexion(int);
void terminar_programa(int,t_log*,t_config*);
#endif /* GAMECARD_GAMECARD_H_ */
