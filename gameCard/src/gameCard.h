#ifndef GAMECARD_GAMECARD_H_
#define GAMECARD_GAMECARD_H_
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
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
#include <pthread.h>
#include <../MiLibreria/utils/utils.h>
#include<../MiLibreria/utils/cliente.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <commons/process.h>
#include <commons/collections/queue.h>
#include <semaphore.h>
#include <pthread.h>


#define GAMECARD_CONFIG_PATH "gameCard.config"
#define GAMECARD_LOG_PATH "GAMECARD.log"

#define RUTA_METADATA_GENERAL "/Metadata/Metadata.bin"
#define RUTA_BITMAP_GENERAL "/Metadata/Bitmap.bin"
#define MAX_CONEXIONES 100


pthread_mutex_t lock;

typedef struct {
	int tiempoReintentoConexion;
	int tiempoReintentoOperacion;
	char *puntoDeMontaje;
	char *ipBroker;
	int puertoBroker;
	int puertoGameCard;
	char *ipGameCard;
	char *nombreProceso;

} t_GAMECARDConfig;

t_log *logger;
t_config *GAMECARDTConfig;
t_GAMECARDConfig *gameCardConfig;
//SEMAFORO
sem_t contadorBandejaGameCard;
pthread_mutex_t mutex_bandejaGameCard;
pthread_mutex_t mutex_cant_blockers;
pthread_mutex_t mutex_crear_carpeta;
t_queue *bandejaDeMensajesGameCard;

int conexion;
int blocks_maximos;
int blocks_usados;
int socketBroker;
t_list *ListIdMensajes;



void inicializar_logger(void);
void cargarConfigGameCard(void);

/**
 * @NAME: suscribirmeAColasBroker
 * @DESC: Suscribe al proceso GameCard a 3 colas del Broker
 * y espera una confirmacion de este ultimo.
 * Colas:
 *  - tNEW_POKEMON
 *  - tCATCH_POKEMON
 *  - tGET_POKEMON
 */
void suscribirmeAColasBroker();

/**
 * @NAME: iniciarTallGrass
 * @DESC: Crea dentro del filesystem los archivo generales
 * metadata y bitmap.
 */
void iniciarTallGrass();

int crearBlock(int block, int x, int y, int cant);

void terminarPrograma();
void crearEscribirArchivo(char* rutaArchivo, char* stringAEscribir);
char* crearRutaArchivo(char* nombreArchivo);
void agregarNewPokemon(char* pokemon, int x, int y, int cantidad);
int catchPokemon(char* pokemon, int x, int y);
int existePokemon(char* pokemon);
int archivoAbierto(char* rutaArchivo);

void* recvMensajesGameCard(void* socketCliente);
void* procesarMensajeGameCard();
void inicializarMutexGameCard();
void terminarProgramaGameCard();
void* handlerGameCard(void* socketDelCliente);
void actualizarBlocks();
void *iniciarConexionBroker(void *arg);
t_paquete* obtenerPokemon(char* pokemon);


int contadorDeMensajes;
pthread_mutex_t bandejaMensajes_mutex;
pthread_mutex_t recibir_mutex;
sem_t bandejaCounter;

#endif /* GAMECARD_GAMECARD_H_ */

