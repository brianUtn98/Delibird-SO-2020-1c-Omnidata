#include "team.h"

void iniciarlizar_logger(){
logger=log_create("team.log","TEAM",1,LOG_LEVEL_TRACE);
	if(logger == NULL){
	perror("No se pudo inicializar el logger\n");
	exit(1);
	}
}

void cargarConfigTeam(){
TEAMTConfig=config_create(TEAM_CONFIG_PATH);
	if(TEAMTConfig == NULL){
	perror("No se pudo leer la configuracion\n");
	exit(2);
	}

teamConf=malloc(sizeof(t_TEAMConfig));
teamConf->ALGORITMO_PLANIFICACION=config_get_string_value(TEAMTConfig,"ALGORITMO_PLANIFICACION");
teamConf->ESTIMACION_INICIAL=config_get_double_value(TEAMTConfig,"ESTIMACION_INICIAL");
teamConf->IP_BROKER=config_get_string_value(TEAMTConfig,"IP_BROKER");
teamConf->LOG_FILE=config_get_string_value(TEAMTConfig,"LOG_FILE");
teamConf->PUERTO_BROKER=config_get_string_value(TEAMTConfig,"PUERTO_BROKER");
teamConf->OBJETIVOS_ENTRENADOR=config_get_string_value(TEAMTConfig,"OBJETIVOS_ENTRENADOR");
teamConf->POKEMON_ENTRENADOR=config_get_string_value(TEAMTConfig,"POKEMON_ENTRENADOR");
teamConf->POSICION_ENTRENADORES=config_get_string_value(TEAMTConfig,"POSICION_ENTRENADOR");
teamConf->QUANTUM=config_get_int_value(TEAMTConfig,"QUANTUM");
teamConf->RETARDO_CICLO_CPU=config_get_int_value(TEAMTConfig,"RETARDO_CICLO_CPU");
teamConf->TIEMPO_RECONEXION=config_get_int_value(TEAMTConfig,"TIEMPO_RECONEXION");

config_destroy(TEAMTConfig);
}

void liberar_teamConf(){
free(teamConf);
}

