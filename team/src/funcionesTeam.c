#include "team.h"

void iniciarlizar_logger() {
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
	log_info(logger, "- CONFIGURACION IMPORTADA\n");
	teamConf = malloc(sizeof(t_TEAMConfig));

	teamConf->POSICION_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"POSICION_ENTRENADOR");
	teamConf->POKEMON_ENTRENADOR = config_get_array_value(TEAMTConfig,
			"POKEMON_ENTRENADOR");
	teamConf->OBJETIVOS_ENTRENADOR = config_get_array_value(TEAMTConfig,
			"OBJETIVOS_ENTRENADOR");
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

//	t_list *string = list_create();
//		char**	pokes = string_split(string, '|');
//	printf("%s",string);
//
//	string_split(teamConf->POKEMON_ENTRENADOR, '|');
//	string_split(teamConf->OBJETIVOS_ENTRENADOR, '|');

	config_destroy(TEAMTConfig);
	//free(teamConf);
}

//void main(void) {
//  t_config *config = config_create("./tp_fix.config");
//  char **read_array = config_get_array_value(config, "POKEMON_ENTRENADORES");
//  t_list *pokemongos = list_create();
//  void _a_la_lista(char *poke) {
//    if (poke != NULL) {
//      list_add(pokemongos, poke);
//    }
//  }
//  void _imprimir(char *string) {
//    if(string != NULL) {
//      char **pokes = string_split(string, "|");
//      string_iterate_lines(pokes, _a_la_lista);
//      printf("Read: %s\n", string);
//    } else {
//      printf("Got NULL\n");
//    }
//  }
//  string_iterate_lines(read_array, _imprimir);
//
//  void mostrar(void *elemento) {
//    printf("El elemento: %s\n", (char *)elemento);
//  }
//
//  list_iterate(pokemongos, mostrar);
//
//  // Atencion: aca les falta liberar la memoria que haga falta
//  // Es cuarentena; no Navidad
//}

