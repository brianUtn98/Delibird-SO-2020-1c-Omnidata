#include "team.h"

void iniciarlizar_loggerTeam() {
	logger = log_create("team.log", "TEAM", 1, LOG_LEVEL_TRACE);
	if (logger == NULL) {
		perror("No se pudo inicializar el logger\n");
		exit(1);
	}
}

void cargarConfigTeam() {

	TEAMTConfig = config_create(TEAM_CONFIG_PATH);
	if (TEAMTConfig == NULL) {
		perror("No se pudo leer la configuracion\n");
		exit(2);
	}

	teamConf = malloc(sizeof(t_TEAMConfig));
	teamConf->POSICIONES_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"POSICIONES_ENTRENADORES");
	teamConf->POKEMON_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"POKEMON_ENTRENADORES");
	teamConf->OBJETIVOS_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"OBJETIVOS_ENTRENADORES");
	teamConf->TIEMPO_RECONEXION = config_get_int_value(TEAMTConfig,
			"TIEMPO_RECONEXION");
	teamConf->RETARDO_CICLO_CPU = config_get_int_value(TEAMTConfig,
			"RETARDO_CICLO_CPU");
	teamConf->ALGORITMO_PLANIFICACION = config_get_string_value(TEAMTConfig,
			"ALGORITMO_PLANIFICACION");
	teamConf->QUANTUM = config_get_int_value(TEAMTConfig, "QUANTUM");
	teamConf->ESTIMACION_INICIAL = config_get_double_value(TEAMTConfig,
			"ESTIMACION_INICIAL");
	teamConf->IP_BROKER = config_get_string_value(TEAMTConfig, "IP_BROKER");
	teamConf->PUERTO_BROKER = config_get_string_value(TEAMTConfig,
			"PUERTO_BROKER");
	teamConf->LOG_FILE = config_get_string_value(TEAMTConfig, "LOG_FILE");
	log_info(logger, "- CONFIGURACION IMPORTADA\n");

//	t_list *string = list_create();
//		char**	pokes = string_split(string, '|');
//	printf("%s",string);
//
//	string_split(teamConf->POKEMON_ENTRENADOR, '|');
//	string_split(teamConf->OBJETIVOS_ENTRENADOR, '|');

	config_destroy(TEAMTConfig);
return;
}
