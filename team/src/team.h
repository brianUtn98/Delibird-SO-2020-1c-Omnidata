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
#include <../MiLibreria/utils/utils.h>
#include<../MiLibreria/utils/cliente.h>
#include <semaphore.h>

#define TEAM_CONFIG_PATH "team.config"


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
}t_TEAMConfig;



typedef struct {
t_posicion posicion;
t_list *pokemons;
t_list *objetivos;
}t_entrenador;



int cantidadEntrenadores;
pthread_t thread;
pthread_mutex_t mutexCreadoDeEntrenadores;

uint32_t mapa[8][8];

t_log *logger;
//t_config *TEAMTConfig; // esto no parece ser blobal
t_TEAMConfig *teamConf;
t_config *TEAMTConfig;

t_list *pokemonEntrenadores;
t_list *posicionEntrenadores;
t_list *objetivoEntrenadores;

t_entrenador *entrenadores;

void cargarConfigTeam();
void inicializarLoggerTeam();
void splitList(char **string,t_list *lista);
void agregarElemento(char *elemento,t_list *lista);
void mostrar(void *elemento);
void mostrarLista(t_list *lista);
void crearEntrenadores(t_list *posicionesEntrenadores,t_list* pokemonsEntrenadores,t_list *objetivosEntrenadores);
t_list *separarPokemons(void*data);
t_posicion separarPosiciones(void *data);
t_list *pokemonDeEntrenador(int i);
//void enviarMensaje(char *ip,int puerto,char *mensaje);
#endif /* TEAM_TEAM_H_ */
