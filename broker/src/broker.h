#ifndef BROKER_BROKER_H_
#define BROKER_BROKER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/error.h>
#include <commons/string.h>
#include <commons/memory.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/collections/dictionary.h>
#include "../MiLibreria/utils/servidor.h"
#include "../MiLibreria/utils/utils.h"
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>
#include <signal.h>

#define BROKER_CONFIG_PATH "/home/utnso/workspace/tp-2020-1c-Omnidata/configs/broker.config"
#define MAX_CONEXIONES 100
#define ASCEND 1
#define DESCEND 2
#define AGRANDA 3
#define ACHICA 4
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//struct nodoListaCache {
//	uint32_t inicio;
//	uint32_t fin;
//	uint32_t largo;
//	uint32_t estado;
//	uint32_t instante;
//	uint32_t id;
//	uint32_t cola;
//	struct nodoListaCache *sgte;
//	struct nodoListaCache *ant;
//	struct nodoListaCache *mayor;
//	struct nodoListaCache *menor;
//};
//typedef struct nodoListaCache *t_part;

//////////////////////////////////////////////////////////////BuddySystem/////////////////////////////////////////////////////

typedef struct {
	int posicionParticion; //es como el bytes escritos de serializacion. la primer posicion es 0. la ultima posicion es TAMANO_PARTICION - 1
	bool libre; //1 si esta libre, 0 si no.
	int tamanio;
	int tamanioMensaje;
	int idMensaje;
	int cola;
	int contadorLRU; //Se actualiza cuando lo agregas a memoria y cuando lo envias

} t_partBuddy;

t_list* particionesEnMemoriaBuddy;
void* principioMemoriaBuddy;
t_queue* colaMensajesMemoriaBuddy;
int CONTADORLRUBUDDY;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

t_list* bandejaDeMensajes;
t_queue *bandeja;
t_queue *bandejaSuscriptores;

int contadorDeMensajes;
pthread_mutex_t bandejaMensajes_mutex;
pthread_mutex_t bandejaSuscriptores_mutex;
pthread_mutex_t recibir_mutex;
pthread_mutex_t asignarIdMensaje_mutex;
pthread_mutex_t mutexCache;
sem_t bandejaCounter;
sem_t bandejaSuscriptorCounter;

pthread_mutex_t mutexQueueNew;
pthread_mutex_t mutexQueueAppeared;
pthread_mutex_t mutexQueueGet;
pthread_mutex_t mutexQueueLocalized;
pthread_mutex_t mutexQueueCatch;
pthread_mutex_t mutexQueueCaught;

typedef struct {
	t_header codigoOperacion;
	uint32_t socket;
	uint32_t largoNombreProceso;
	char* nombreProceso;
} t_suscriptor; //fixed

typedef struct {
	uint32_t idMensaje;
	t_header colaMensaje;
	t_list* suscriptoresEnviados;
	t_list* suscriptoresRecibidos;
} t_administrativo; //fixed

typedef struct {
	uint32_t particion; //numero de particion
	uint32_t offsetInicioParticion; //puntero a la base
	uint32_t sizeParticion; //limite
	uint32_t flagLRU;
} t_particionLibre;

typedef struct {
	uint32_t sizeNombre;
	char * pokemon;
	uint32_t cantidadPokemons;
	uint32_t posX;
	uint32_t posY;
} t_newPokemon;

typedef struct {
	uint32_t sizeNombre;
	char * pokemon;
	uint32_t posX;
	uint32_t posY;
} t_appearedPokemon;

typedef struct {
	uint32_t sizeNombre;
	char * pokemon;
	uint32_t posX;
	uint32_t posY;
} t_catchPokemon;

typedef struct {
	uint32_t booleano;
} t_caughtPokemon;

typedef struct {
	uint32_t sizeNombre;
	char* pokemon;
} t_getPokemon;

typedef struct {
	uint32_t sizeNombre;
	char* pokemon;
	uint32_t cantidadDePosiciones;
	t_list *posiciones;
} t_localizedPokemon;

typedef struct {
	int tamanoMemoria;
	int tamanoMinimoParticion;
	char *algoritmoMemoria;
	char *algoritmoReemplazo;
	char *algoritmoParticionLibre;
	char *ipBroker;
	int puertoBroker;
	int frecuenciaCompactacion;
	char *logFile;
} t_BROKERConfig;

typedef struct {
	t_list *cola;
	t_list *lista;
} t_cola;

// Definamos las estructuras para administrar la cache de mensajes
//
// cola doblemente enlazada para administrar la cache.
//
// typedef struct t_cacheNodo *tp_cacheNodo;
//
// datos de la lista t_cacheInfo
//
char * cache; // es un puntero a una direccion de memoria de largo TAMANO_MEMORIA

int instanteCache, sizeTra, debugCache, debugFino, debugTrace, partPD, partBS,
		reemFIFO, reemLRU, seleFF, seleBF;
int tamanoABuscar, nodoJusto, inserto, consolidaciones,
		cantidadMaximaConsolidaciones, verbose;

struct nodoListaCache {
	uint32_t inicio;
	uint32_t fin;
	uint32_t largo;
	uint32_t estado;
	uint32_t instante;
	uint32_t id;
	uint32_t cola;
	struct nodoListaCache *sgte;
	struct nodoListaCache *ant;
	struct nodoListaCache *mayor;
	struct nodoListaCache *menor;
};
typedef struct nodoListaCache *t_part; // tipo del nodo de la lista que administra la CACHE

t_part partFirst;
t_part partLast;
t_part partSmall;
t_part partBig;

// ver que se necesita para el suscriptor, como manejar la cola a la que quiere suscribirse

t_log *logger;
t_BROKERConfig *brokerConf;
t_config *BROKERTConfig;

t_cola *NEW_POKEMON;
t_cola *APPEARED_POKEMON;
t_cola *CATCH_POKEMON;
t_cola *CAUGHT_POKEMON;
t_cola *GET_POKEMON;
t_cola *LOCALIZED_POKEMON;
t_list *LOCALIZED_PRUEBA;

void* miMemoria; // ver que tipo de datos voy a manejar,seguramente es una estructura

uint32_t idMensajeUnico;
uint32_t idMensajeCorrelativo;
uint32_t offset;
void* iniMemoria;
uint32_t numeroParticion;
t_log *logEntrega;

////////////////////////////////////////////////////////////////////////////BuddySystem//////////////////////////////////////
void insertarMensajeEnCacheBuddy(void* mensaje, int largo, int idMensaje,
		int cola);
bool almacenarMensajeBuddy(void* mensaje, int largo, int idMensaje, int cola);
t_partBuddy* buscarPrimerParticionLibreBuddy(uint32_t tamanioMensaje);
t_partBuddy* buscarMejorParticionLibreBuddy(uint32_t tamanioMensaje);
void agregarBuddy(t_partBuddy* particion);
t_partBuddy* cargarDatosParticionBuddy(t_partBuddy* particion, void* mensaje,
		int largo, int idMensaje, int cola);
int buscarPotenciaDeDosMasCercana(uint32_t tamanio);
void eliminarParticionBuddy();
void consolidarMemoriaBuddy();
t_list* sacarParticionesLibresBuddy();
void iniciarCacheBuddy();
t_partBuddy* crearParticionBuddyMemoria(t_partBuddy particion);
void borrarParticionBuddyMemoria(t_partBuddy* particion);

int* crearElementoColaMensajesMemoriaBuddy(int idMensaje);
void borrarElementoColaMensajesMemoriaBuddy(int* idMensaje);

void ordenarParticionesPorPosicionBuddy();
t_partBuddy* removerPorPosicionBuddy(int posicion);
char* obtenerNombreColaBuddy(int cola);
t_partBuddy* obtenerMensajeBuddy(int idMensaje);
void eliminarIdCola(uint32_t idMensaje, int idCola);
void borrarElementoCola(uint32_t* elemento);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
t_part obtenerMensaje(int id);
void dumpCache();
void liberarParticionDinamica(t_part nodo);
void compactacionDinamica();
void removerListaCola(t_part nodo);
void insertarMensajeEnCache(void * mensaje, int largo, int id, int cola);
void insertarMensajeEnCache2(void * mensaje, int largo, int id, int cola);

void consolidacionDinamica(t_part nodo);
t_part elegirFifoVictima(void);
void iniciarCache(void);
void mostrarPart(t_part nodo, int part, int orden);
t_part encontrarPartLibre(int size, int orden);
t_part encontrarBestFitPartlibre(int size, int orden);
t_part encontrarFirstFitPartLibre(int size, int orden);
t_part encontrarPartMayor(int size, int orden);
void mostrarCache(t_part nodo, int orden);
void insertarEnParticion(t_part nodo, void* mensaje, int size, int alojamiento,
		int id, int cola);
void insertarJusto(t_part nodo, void* mensaje, int size, int alojamiento,
		int id, int cola);
void removerPartPorOrden(t_part nodo);
void insertarPartPorTamano(t_part part);
void removerPartPorTamano(t_part part);
void insertarCabezaPorTamano(t_part nodo);
void insertarPiePorTamano(t_part nodo);
void insertarMedioPorTamano(t_part nodo, t_part medio);
void removerPiePorTamano();
void removerCabezaPorTamano();
void removerMedioPorTamano(t_part medio);
void mostrarPartEnBruto(t_part nodo);

void inicializarLogger(void);
void inicializarLoggerEntregable(void);
void cargarConfigBROKER(void);
void inicializarColasBroker(void);
void inicializarSemaforos(void);
void iniciarServidor(char *ip, int puerto);
void destruirColasBroker(void);
void agregarMensaje(t_cola*, void*);
char* sacarMensaje(t_cola*);
void* administrarMensajes();
void processRequest(void* bufferLoco, int clienteFd);
void *serveClient(void *socket);
void* handler(void* socketConectado);

void* recibirMensajesHandler(void* socketCliente);

void* manejarMemoria();
void inicializarEstructuras();
void* buscarEspacioDisponible(int sizeMensaje);
void* consumirMensajes();
void* escucharConexiones();
t_particionLibre* insertarEnCache(void* mensaje, int size);

void verificarSuscriptor(t_suscriptor* suscriptor, t_cola* cola);
t_administrativo* enviarMensajeASuscriptores(t_list* lista, t_paquete* mensaje);
int buscarMensaje(t_paquete* paquete);
void liberarAdministrativo(t_administrativo* admin);

void enviarMensajeCacheadoAck(t_cola* cola, t_suscriptor* suscriptor);
t_administrativo* enviarMensajeCacheado(t_cola* cola, t_suscriptor* suscriptor); //esto no se si tiene que devolver un t_administrativo.
void generarDump(int signal);

#endif /* BROKER_BROKER_H_ */
