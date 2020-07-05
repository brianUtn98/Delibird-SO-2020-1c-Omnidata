#ifndef BROKER_BROKER_H_
#define BROKER_BROKER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

#define BROKER_CONFIG_PATH "/home/utnso/workspace/tp-2020-1c-Omnidata/configs/broker.config"
#define MAX_CONEXIONES 100

t_list* bandejaDeMensajes;
t_queue *bandeja;
t_queue *bandejaSuscriptores;

int contadorDeMensajes;
pthread_mutex_t bandejaMensajes_mutex;
pthread_mutex_t bandejaSuscriptores_mutex;
pthread_mutex_t recibir_mutex;
pthread_mutex_t asignarIdMensaje_mutex;
sem_t bandejaCounter;
sem_t bandejaSuscriptorCounter;

typedef struct {
	t_header codigoOperacion;
	uint32_t socket;
	uint32_t largoNombreProceso;
	char* nombreProceso;
	uint32_t enviado;
	uint32_t ack;
} t_suscriptor;

typedef struct {
	uint32_t idMensaje;
	t_header colaMensaje;
	t_list* suscriptoresEnviados;
	t_list* suscriptoresRecibidos;
	uint32_t particion; //numero de particion
	uint32_t offsetInicioParticion; //puntero a la base
	uint32_t sizeParticion; //limite
	uint32_t sizeMensajeGuardado;
	uint32_t flagLRU; //esto no se si es una lista de flags, se sabrá cuando se implemente el algoritmo
} t_administrativo;

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
int * cache; // es un puntero a una direccion de memoria de largo TAMANO_MEMORIA
int instanteCache, auxTra, sizeTra, nodos, debugCache, partPD, partBS, reemFIFO, reemLRU, seleFF, seleBF;
int tamanoABuscar, nodoJusto;

struct nodoListaCache{
	uint32_t inicio;
	uint32_t fin;
	uint32_t largo;
	uint32_t estado;
	uint32_t instante;
	uint32_t id;
	struct nodoListaCache *sgte;
	struct nodoListaCache *ant;
	struct nodoListaCache *mayor;
	struct nodoListaCache *menor;
};
typedef struct nodoListaCache *t_nodoListaCache; // tipo del nodo de la lista que administra la CACHE

t_nodoListaCache partActual;
t_nodoListaCache partFirst;
t_nodoListaCache partLast;
t_nodoListaCache partBig;
t_nodoListaCache partSmall;
t_nodoListaCache praUsada;
t_nodoListaCache praLibre;

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

void* miMemoria; // ver que tipo de datos voy a manejar,seguramente es una estructura

uint32_t idMensajeUnico;
uint32_t idMensajeCorrelativo;
uint32_t offset;
void* iniMemoria;
uint32_t numeroParticion;
t_log *logEntrega;

void iniciarCache(void);
void mostrarPart(t_nodoListaCache nodo);
t_nodoListaCache encontrarPartLibre(int size, int orden);
void mostrarCache(t_nodoListaCache nodo, int orden);
void insertarEnParticion(t_nodoListaCache nodo, void* mensaje, int size, int alojamiento, int id);
void insertarJusto(t_nodoListaCache nodo, void* mensaje, int size, int alojamiento, int id);

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

void verificarSuscriptor(t_suscriptor* suscriptor, t_list* lista);
void enviarMensajeASuscriptores(t_list* lista, t_paquete* mensaje);

#endif /* BROKER_BROKER_H_ */
