#ifndef GAMECARD_GAMECARD_H_
#define GAMECARD_GAMECARD_H_
//#include "../../libreriasCompartidas/utils.h"
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
#include <../MiLibreria/utils/utils.h>
#include<../MiLibreria/utils/cliente.h>
#include <commons/string.h>
#include <commons/txt.h>


#define GAMECARD_CONFIG_PATH "gameCard.config"
#define GAMECARD_LOG_PATH "GAMECARD.log"

typedef struct {
	int tiempoReintentoConexion;
	int tiempoReintentoOperacion;
	char *puntoDeMontaje;
	char *ipBroker;
	int puertoBroker;

} t_GAMECARDConfig;

//typedef enum
//{
//	MENSAJE = 1,
//}op_code;

//typedef struct {
//int size;
//void* stream;
//}t_buffer;

//typedef struct {
//t_buffer* buffer;
//op_code codigo_operacion;
//}t_paquete;

t_log *logger;
t_config *GAMECARDTConfig;
t_GAMECARDConfig *gameCardConfig;
int conexion;



void inicializar_logger(void);
void cargarConfigGameCard(void);
//void iniciarServidor(void);
//void* serializar_paquete(t_paquete* paquete,int*);
//int crear_conexion(char*,char*);
//void enviar_mensaje(char*,int);
//char* recibir_mensaje(int);
void terminarPrograma();
void crearArchivo(char* puntoMontaje, char* nombreArchivo);
#endif /* GAMECARD_GAMECARD_H_ */

