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
#include <time.h>

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
	NEW=1,READY, BLOCKED, EXEC, EXIT
} t_estado;

typedef struct {
	unsigned int pid;
	t_posicion posicion;
	int rafaga;
//	int inicioRafaga;
//	int finRafaga;
	int estimacionRafagaActual; //estimacion para ejecutar rafaga.
	int ultimaRafaga; //rafaga real ejecutada.
	int quantumPendiente;
	t_list *pokemons;
	t_list *objetivos;
	t_estado estado;
	int indice;
	int flagDeadlock;
} t_entrenador;

typedef struct {
	int posX;
	int posY;
	char *nombrePokemon;
	//int socket;
	int quantum;
	t_entrenador *involucrado;
//int estaEnDeadlock;
//t_posicion destinoDeadlock;
} t_administrativoEntrenador;

typedef struct {
	t_entrenador *desbloquear;
	t_entrenador *involucrado;
} t_deadlock;

int deadlocksTotales;
int deadlocksResueltos;
int cantidadEntrenadores;
int ciclosDeCpuTotales;
int *ciclosPorEntrenador;
int cambiosDeContexto;
int flagTratamientoDeadlocks;
int segundosTotales;
pthread_t thread;
pthread_t *threads_entreanadores;
pthread_mutex_t *ejecuta;
pthread_mutex_t mutexPlani;
sem_t contadorBandeja;
sem_t pokemonsEnLista;
pthread_mutex_t mutex_bandeja;
pthread_mutex_t mutexListaPokemons;
pthread_mutex_t cpu;
sem_t counterProximosEjecutar;
pthread_mutex_t mutexProximos;
pthread_mutex_t mutexDeadlock;
pthread_mutex_t mutexReady;
pthread_mutex_t mutexBlocked;
pthread_mutex_t mutexExit;
pthread_mutex_t mutexNew;
pthread_mutex_t terminaTratamiento;
//pthread_mutex_t *puedeEjecutar;
sem_t counterDeadlock;
//pthread_mutex_t mutexCreadoDeEntrenadores;

uint32_t mapa[X_MAX][Y_MAX];

t_log *logger;
t_log *logEntrega;
t_log *logReporte;
//t_config *TEAMTConfig; // esto no parece ser blobal
t_TEAMConfig *teamConf;
t_config *TEAMTConfig;

t_list *pokemonEntrenadores;
t_list *posicionEntrenadores;
t_list *objetivoEntrenadores;
t_list *objetivoGlobal;
t_list *procesosEnDeadlock;
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
t_list *proximosEjecutar;

//-------------------------- Funciones --------------------------
void cargarConfigTeam();
void inicializarLoggerTeam();
void inicializarLoggerReporte();
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
void iniciarListasColas();
void *escucharGameboy();
void *handler(void *arg);
t_list *sinRepetidos(t_list *lista);
void agregarElementoSinRepetido(t_list *lista, void *elemento);
bool estaEn(t_list* lista, void *elemento);
void terminarPrograma();
void terminarSiPuedo();
void* pedirPokemons(void *arg);
void* planificarEntrenadores();
void* planificarEntrenadoresRR();
void* planificarEntrenadoresSJF();
void *planificarEntrenadoresSJFDesalojo();
void *ejecutor();
void calculoEstimacionSjf(t_entrenador *entrenador);
t_entrenador *buscarMenorRafaga(t_list *entrenadores);
void* recvMensajes(void* socketCliente);
void* procesarMensaje();
void inicializarMutex();
int sonIguales(t_posicion pos1, t_posicion pos2);
int sonDistintas(t_posicion pos1, t_posicion pos2);
void moverEntrenador(t_entrenador *entrenador, t_posicion coordenadas);
int hallarIndice(t_entrenador *entrenador, t_list *lista);
void tratamientoDeDeadlocks();
int stringVacio(void *data);
void tratamientoDeDeadlocks();
char *pokemonEnConflicto(t_entrenador *e1, t_entrenador *e2);
t_list *filterNoNecesita(t_entrenador *entrenador, t_list *pokemons);
void intercambiar(t_entrenador* entrenador1, t_entrenador *entrenador2,
		char* pokemon1, char *pokemon2);
t_entrenador *buscarInvolucrado(t_entrenador *desbloquear,
		t_list *entrenadoresBloqueados);
int tienenConflicto(t_entrenador *entrenador1, t_entrenador *entrenador2);
int puedeSeguir(t_entrenador *entrenador);
int puedeSeguirAtrapando(t_entrenador *entrenador);
int hallarIndice(t_entrenador *entrenador, t_list *lista);
t_entrenador *buscarMasCercano(t_posicion coordenadas);
int distanciaHasta(t_posicion pos1, t_posicion pos2);
int estanTodosEnExit();
int hayEntrenadoresDisponibles();
int cumplioObjetivo(t_entrenador *entrenador);
int buscarIndicePokemon(void* data, t_list *lista);
int sonDistintas(t_posicion pos1, t_posicion pos2);
int sonIguales(t_posicion pos1, t_posicion pos2);
void suscribirseColasBroker();
#endif /* TEAM_TEAM_H_ */
