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

#define BROKER_CONFIG_PATH "broker.config"
#define MAX_CONEXIONES 100

t_list* bandejaDeMensajes;
t_queue *bandeja;
int contadorDeMensajes;
pthread_mutex_t bandejaMensajes_mutex;
pthread_mutex_t recibir_mutex;
sem_t bandejaCounter;

t_dictionary* estructuraAdministrativa;

typedef struct {
	t_list* suscriptores;
	uint32_t idMensaje;
	int numeroParticion; //puntero a la base
	int sizeMensajeGuardado; //con esto calculo el offset o el final de la partición.
	int sizeParticion;
} t_administrativo;

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
	t_queue *cola;
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
int instanteCache, auxTra, sizeTra, nodos;

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

t_nodoListaCache particionActual;
t_nodoListaCache particionFirst;
t_nodoListaCache particionLast;
t_nodoListaCache particionBig;
t_nodoListaCache particionSmall;

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

typedef struct {
	t_cola *cola1;
	t_cola *cola2;
} t_parejaCola;

t_parejaCola *NEW_APPEARED_POKEMON;
t_parejaCola *CATCH_CAUGTH_POKEMON;
t_parejaCola *GET_LOCALIZED_POKEMON;
void* miMemoria; // ver que tipo de datos voy a manejar,seguramente es una estructura

uint32_t idMensajeUnico;
uint32_t idMensajeCorrelativo;
uint32_t offset;
void* iniMemoria;
uint32_t numeroParticion;
int praLibre;

int insertarPartition(void* mensaje, int size, int id, int orden);
int encontrarLibre(int size, int orden);
int mostrarCache(t_nodoListaCache nodo, int orden);

void inicializarLogger(void);
void cargarConfigBROKER(void);
void inicializarColasBroker(void);
void iniciarServidor(char *ip, int puerto);
void destruirColasBroker(void);
void agregarMensaje(t_cola*, void*);
char* sacarMensaje(t_cola*);
void* administrarMensajes();
void pedirMemoriaInicial(void);
void processRequest(void* bufferLoco, int clienteFd);
void *serveClient(void *socket);
void* handler(void* socketConectado);

void iniciarServidorMio(char *ip, int puerto);
void* recibirMensajesHandler(void* socketCliente);

void* manejarMemoria();
void inicializarEstructuras();

#endif /* BROKER_BROKER_H_ */
