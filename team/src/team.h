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

#define TEAM_CONFIG_PATH "team.config"
#define alfa 0.5    // este alfa deberia llegar por archivo de configuracion.

/*typedef enum {
 Pikachu,Squirtle,Charmander,Bulbasaur,Rattata,Pidgey,etc
 }t_pokemon; // Mismo que arriba, llene el espacio para que eclipse no tire warning.*/

/*typedef struct {
 t_pokemon pokemon;
 t_list_pokemon *next;
 }t_list_pokemon;*/

/*tpydef enum {
 RR,FIFO,SJF
 }t_algoritmo_planificacion;*/

typedef struct {
	char **POSICIONES_ENTRENADORES;
	char **POKEMON_ENTRENADORES;
	char **OBJETIVOS_ENTRENADORES;
	int TIEMPO_RECONEXION;
	int RETARDO_CICLO_CPU;
	char *ALGORITMO_PLANIFICACION;
	int QUANTUM;
	double ESTIMACION_INICIAL;
	char *IP_BROKER;
	int PUERTO_BROKER;
	char *LOG_FILE;
} t_TEAMConfig;

typedef enum {
	READY = 1, BLOCKED, EXEC, EXIT
} t_estado;

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
//pthread_mutex_t mutexCreadoDeEntrenadores;

uint32_t mapa[8][8];

t_log *logger;
//t_config *TEAMTConfig; // esto no parece ser blobal
t_TEAMConfig *teamConf;
t_config *TEAMTConfig;

t_list *pokemonEntrenadores;
t_list *posicionEntrenadores;
t_list *objetivoEntrenadores;
t_list *objetivoGlobal;
t_entrenador *entrenadores;

//t_queue *COLA_NEW;
t_list *ESTADO_READY;
t_list *ESTADO_BLOCKED;
t_entrenador *ESTADO_EXEC; //Cola simbólica para pensar el funcionamiento, se borrará (ya que no hay multiprocesamiento).
t_list *ESTADO_EXIT;
t_list *bandejaDeMensajes;

void cargarConfigTeam();
void inicializarLoggerTeam();
void splitList(char **string, t_list *lista);
void agregarElemento(char *elemento, t_list *lista);
void mostrar(void *elemento);
void mostrarLista(t_list *lista);
void crearEntrenadores();
void *manejarEntrenador(void *arg);
t_list *separarPokemons(void*data, int flag);
t_posicion separarPosiciones(void *data);
void iniciarEstados();
void escucharGameboy();
void *handler(void *arg);
t_list *sinRepetidos(t_list *lista);
void agregarElementoSinRepetido(t_list *lista, void *elemento);
bool estaEn(t_list* lista, void *elemento);
void terminarPrograma();
void pedirPokemons(int socket);
void* planificarEntrenadores(void* socketServidor);
void calculoEstimacionSjf(t_entrenador *entrenador);
t_entrenador *buscarMenorRafaga(t_list *entrenadores);
void* recvMensajes(void* socketCliente);
void* procesarMensaje();
void inicializarMutex();
#endif /* TEAM_TEAM_H_ */
