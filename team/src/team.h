#ifndef TEAM_TEAM_H_
#define TEAM_TEAM_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <pthread.h>
#include <time.h>
#include <../MiLibreria/utils/utils.h>
#include <../MiLibreria/utils/cliente.h>
#include <semaphore.h>
#include <commons/process.h>
#include <stdbool.h>
#include <../MiLibreria/utils/servidor.h>
#include <math.h>

#define TEAM_CONFIG_PATH "team.config"
#define alpha 0.5    // este alfa deberia llegar por archivo de configuracion.
#define MAX_CONEXIONES 100
#define X_MAX 1024
#define Y_MAX 1024
typedef struct {
	char **POSICIONES_ENTRENADORES;
	char **POKEMON_ENTRENADORES;
	char **OBJETIVOS_ENTRENADORES;
	int TIEMPO_RECONEXION;
	int RETARDO_CICLO_CPU;
	char *ALGORITMO_PLANIFICACION;
	int QUANTUM;
	double ALPHA;
	char *IP_BROKER;
	double ESTIMACION_INICIAL;
	int PUERTO_BROKER;
	char *LOG_FILE;
	char* NOMBRE_PROCESO;
} t_TEAMConfig;

typedef enum {
	READY = 1, BLOCKED, EXEC, EXIT
} t_estado;

typedef struct {
	int posX;
	int posY;
	char *nombrePokemon;
	int socket;
} t_administrativoEntrenador;

typedef struct {
	unsigned int pid;
	t_posicion posicion;
	int rafaga;
	int inicioRafaga;
	int finRafaga;
	int estimacionRafagaActual; //estimacion para ejecutar rafaga.
	int ultimaRafaga; //rafaga real ejecutada.
	int quantumPendiente;
	t_list *pokemons;
	t_list *objetivos;
	t_estado estado;
	int indice;
} t_entrenador;

int cantidadEntrenadores;
pthread_t thread;
pthread_t *threads_entreanadores;
pthread_mutex_t *ejecuta;
pthread_mutex_t mutexPlani;
sem_t contadorBandeja;
sem_t pokemonsEnLista;
pthread_mutex_t mutex_bandeja;
pthread_mutex_t mutexListaPokemons;
pthread_mutex_t cpu;
//pthread_mutex_t mutexCreadoDeEntrenadores;

uint32_t mapa[X_MAX][Y_MAX];

t_log *logger;
t_log *logEntrega;
//t_config *TEAMTConfig; // esto no parece ser blobal
t_TEAMConfig *teamConf;
t_config *TEAMTConfig;

t_list *pokemonEntrenadores;
t_list *posicionEntrenadores;
t_list *objetivoEntrenadores;
t_list *objetivoGlobal;
t_entrenador *entrenadores;
t_administrativoEntrenador *administrativo;

//t_queue *COLA_NEW;
t_list *ESTADO_READY;
t_list *ESTADO_BLOCKED;
t_entrenador *ESTADO_EXEC; //Cola simbólica para pensar el funcionamiento, se borrará (ya que no hay multiprocesamiento).
t_list *ESTADO_EXIT;
//t_list *bandejaDeMensajes;
t_queue *bandejaDeMensajes;
t_list *listaIdGet;
t_list *listaIdCatch;

t_queue *appearedPokemon;

//-------------------------- Funciones --------------------------
void cargarConfigTeam();
void inicializarLoggerTeam();
void inicializarLoggerEntregable();
void splitList(char **string, t_list *lista);
void agregarElemento(char *elemento, t_list *lista);
void mostrarChar(void *elemento);
void mostrarListaChar(t_list *lista);
void mostrarInt(void *elemento);
void mostrarListaInt(t_list *lista);
void crearEntrenadores();
void *manejarEntrenador(void *arg);
t_list *separarPokemons(void*data, int flag);
t_posicion separarPosiciones(void *data);
void iniciarEstados();
void *escucharGameboy();
void *handler(void *arg);
t_list *sinRepetidos(t_list *lista);
void agregarElementoSinRepetido(t_list *lista, void *elemento);
bool estaEn(t_list* lista, void *elemento);
void terminarPrograma();
void* pedirPokemons(void *arg);
void* planificarEntrenadores(void* socketServidor);
void calculoEstimacionSjf(t_entrenador *entrenador);
t_entrenador *buscarMenorRafaga(t_list *entrenadores);
void* recvMensajes(void* socketCliente);
void* procesarMensaje();
void inicializarMutex();
int sonIguales(t_posicion pos1, t_posicion pos2);
int sonDistintas(t_posicion pos1, t_posicion pos2);
void moverEntrenador(t_entrenador *entrenador, t_posicion coordenadas);
int hallarIndice(t_entrenador *entrenador, t_list *lista);

#endif /* TEAM_TEAM_H_ */
