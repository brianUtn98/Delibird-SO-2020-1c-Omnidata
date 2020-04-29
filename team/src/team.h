#ifndef TEAM_TEAM_H_
#define TEAM_TEAM_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>

#include <../MiLibreria/utils/utils.h>
#include<../MiLibreria/utils/cliente.h>

#define TEAM_CONFIG_PATH "../team.config"

/*typedef struct {
int x;
int y;
}t_posicion; //PONGAMOS MUCHAS COMILLLAS ACA, PUSE UN TIPO PARA QUE NO ME TIRE WARNING NADA MÁS.
*/
/*typedef enum {
Pikachu,Squirtle,Charmander,Bulbasaur,Rattata,Pidgey,etc
}t_pokemon; // Mismo que arriba, llene el espacio para que eclipse no tire warning.*/

/*typedef struct {
t_posicion posicion;
t_list_posicion *next;
}t_list_posicion;*/

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

char **pokes; //variable de prueba
char *pokemons_Entrenadores;
t_log *logger;
t_config *TEAMTConfig; // esto no parece ser blobal
t_TEAMConfig *teamConf;



void cargarConfigTeam();
void iniciarlizar_loggerTeam();
void agregarElemento(char* elemento);
void mostrar(void *elemento);
void splitear(char* string);
void enviarMensaje(char *IP,int PUERTO,char *mensaje);
#endif /* TEAM_TEAM_H_ */
