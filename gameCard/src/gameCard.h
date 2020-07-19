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
#include <../MiLibreria/utils/cliente.h>
#include <../MiLibreria/utils/servidor.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <commons/process.h>
#include <commons/collections/queue.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/mman.h>
#include <commons/bitarray.h>

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
	char *magicnumber;
	int blocksCantidad;
	int blocksSize;
} t_GAMECARDConfig;

t_log *logger;
t_config *GAMECARDTConfig;
t_GAMECARDConfig *gameCardConfig;
//SEMAFORO
sem_t contadorBandejaGameCard;
pthread_mutex_t mutex_bandejaGameCard;
pthread_mutex_t mutex_cant_blockers;
pthread_mutex_t mutex_crear_carpeta;
pthread_mutex_t bandejaDeMensajesGameCardSuscripcion;
t_queue *bandejaDeMensajesGameCard;
pthread_mutex_t mutex_archivo;
t_bitarray* bitmap;

int conexion;
int g_blocks_maximos;
int g_blocks_usados;
int g_block_size;
int socketBroker;
t_list *ListIdMensajes;
t_list *bandejaMensajesGameCard;
int contadorConexiones;
pthread_t pruebaProcesos[MAX_CONEXIONES];
pthread_t procesarProceso;
pthread_t procesarProceso1;

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
void* suscribirseABroker();

/**
 * @NAME: iniciarTallGrass
 * @DESC: Crea dentro del filesystem los archivo generales
 * metadata y bitmap.
 */
void iniciarTallGrass();

int agregarLineaBlock(int block, int x, int y, int cant);

void terminarPrograma();
void crearEscribirArchivo(char* rutaArchivo, char* stringAEscribir);
char* crearRutaArchivo(char* nombreArchivo);
void agregarNewPokemon(char* pokemon, int x, int y, int cantidad);
int catchPokemon(char* pokemon, int x, int y);
int catchPokemon1(char* pokemon, int x, int y, int cantidad);
int existePokemon(char* pokemon);
int archivoAbierto(char* rutaArchivo);
void actualizarBitMapen1(int blockUsado);
int tamanioBloque(char* ruta);
int chequearCoordenadasBlock(char** array_strings, int cantidad, int x, int y);

void* recvMensajesGameCard(void* socketCliente);
void* procesarMensajeGameCard();
void inicializarMutexGameCard();
void terminarProgramaGameCard();
void* handlerGameCard(void* socketDelCliente);
void setearVarGlobalesBlocks();
void *iniciarConexionBroker(void *arg);
void crearTodosLosBloquesEnFS();

void ArchivoAbiertoParaUso(char* rutaPokemon, char* pokemon);
void ArchivoEnUso(char* rutaPokemon, char* pokemon);
t_paquete* obtenerPokemon(char* pokemon);
void* hiloAgregarPokemon(void* bufferLoco1);
void* auxiliar2(void* bufferLoco);
void* auxiliar3(void* bufferLoco);
void* escucharConexionesGameCard();
void* consumirMensajesGameCard();
void* ModificarBlock(char* rutaPokemon, char* pokemon, char* newln2);
t_bitarray* crear_bitmap();
int BuscarEspacioBitMap();
int escribirPokemonOBuscarBloqueLibre(int x, int y, int cantidad);
int agregarLineaAlFinalDelBloque(char* bloque, int x, int y, int cantidad);
void actualizarSizePokemon(int nueva_cantidad, char* pokemon);
void actualizarBloquesPokemon(char* rutaPokemon, int nro_bloque);
int usarBloqueActual(char* nro_bloque, int x, int y, int cantidad);
t_paquete* obtenerCoordenadasPokemon(char* pokemon);
int existenPosicionesyReducir(char** array_strings, char* rutaPokemon, int x, int y,char* pokemon);
void actualizarBloquesPokemonParaCatch(char* rutaPokemon,int tamanioCantidad);

int contadorDeMensajes;
pthread_mutex_t bandejaMensajes_mutex;
pthread_mutex_t recibir_mutex;
sem_t bandejaCounter;

#endif /* GAMECARD_GAMECARD_H_ */

