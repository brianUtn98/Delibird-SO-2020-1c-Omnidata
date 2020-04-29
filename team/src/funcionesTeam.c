#include "team.h"

void iniciarlizar_loggerTeam() {
	logger = log_create("team.log", "TEAM", 1, LOG_LEVEL_TRACE);
	if (logger == NULL) {
		perror("No se pudo inicializar el logger\n");
		exit(1);
	}
return;
}

void agregarElemento(char* elemento){
	if(elemento!=NULL){
		list_add(pokemons_Entrenadores,elemento);
	}
return;
}

void mostrar(void *elemento){
	printf("Elemento %s\n",(char*)elemento);
return;
}

void splitear(char* string){
	if(string!=NULL){
	char **pokes=string_split(string,"|");
	string_iterate_lines(pokes,agregarElemento);
//	printf("Read: %s\n", string);
	}
	else
	{
	printf("Cadena NULL\n");
	}
return;
}

void cargarConfigTeam() {

	TEAMTConfig = config_create(TEAM_CONFIG_PATH);
	if (TEAMTConfig == NULL) {
		perror("No se pudo leer la configuracion\n");
		exit(2);
	}

	teamConf = malloc(sizeof(t_TEAMConfig)); //Reservando memoria

	teamConf->POSICIONES_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"POSICIONES_ENTRENADORES"); //Leo la config posicion_entrenadores
	log_info(logger,"Lei POSICIONES_ENTRENADORES=%s de la configuracion\n",
			teamConf->POSICIONES_ENTRENADORES); //Logeo

	teamConf->POKEMON_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"POKEMON_ENTRENADORES"); //Leo la config Pokemon_entrenadores
	pokemons_Entrenadores=list_create();
	string_iterate_lines(teamConf->POKEMON_ENTRENADORES,splitear);
	log_info(logger,"Lei POKEMON_ENTRENADORES=%s de la configuracion\n",
			teamConf->POKEMON_ENTRENADORES);

	teamConf->OBJETIVOS_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"OBJETIVOS_ENTRENADORES");
	log_info(logger,"Lei OBJETIVOS_ENTRENADORES=%s de la configuracion\n",
			teamConf->OBJETIVOS_ENTRENADORES);
	teamConf->TIEMPO_RECONEXION = config_get_int_value(TEAMTConfig,
			"TIEMPO_RECONEXION");
	log_info(logger,"Lei TIEMPO_RECONEXION=%d de la configuracion\n",
			teamConf->TIEMPO_RECONEXION);

	teamConf->RETARDO_CICLO_CPU = config_get_int_value(TEAMTConfig,
			"RETARDO_CICLO_CPU");
	log_info(logger,"Lei RETARDO_CICLO_CPU=%d de la configuracion\n",
			teamConf->RETARDO_CICLO_CPU);

	teamConf->ALGORITMO_PLANIFICACION = config_get_string_value(TEAMTConfig,
			"ALGORITMO_PLANIFICACION");
	log_info(logger,"Lei ALGORITMO_PLANIFICACION=%s de la configuracion\n",
			teamConf->ALGORITMO_PLANIFICACION);

	teamConf->QUANTUM = config_get_int_value(TEAMTConfig, "QUANTUM");
	log_info(logger,"Lei QUANTUM=%d de la configuracion\n",
			teamConf->QUANTUM);

	teamConf->ESTIMACION_INICIAL = config_get_double_value(TEAMTConfig,
			"ESTIMACION_INICIAL");
	log_info(logger,"Lei ESTIMACION_INICIAL=%f de la configuracion\n",
			teamConf->ESTIMACION_INICIAL);

	teamConf->IP_BROKER = config_get_string_value(TEAMTConfig, "IP_BROKER");
	log_info(logger,"Lei IP_BROKER=%s de la configuracion\n",
			teamConf->IP_BROKER);

	teamConf->PUERTO_BROKER = config_get_string_value(TEAMTConfig,
			"PUERTO_BROKER");
	log_info(logger,"Lei PUERTO_BROKER=%s de la configuracion\n",
			teamConf->PUERTO_BROKER);

	teamConf->LOG_FILE = config_get_string_value(TEAMTConfig, "LOG_FILE");
	log_info(logger,"Lei LOG_FILE=%s de la configuracion\n",
			teamConf->LOG_FILE);
	//Fin de importar configuracion
	log_info(logger, "- CONFIGURACION IMPORTADA\n");

//	t_list *string = list_create();
//		char**	pokes = string_split(string, '|');
//	printf("%s",string);
//	string_split(teamConf->POKEMON_ENTRENADOR, '|');
//	string_split(teamConf->OBJETIVOS_ENTRENADOR, '|');
//	string_iterate_lines(teamConf->POKEMON_ENTRENADORES,splitear);

	config_destroy(TEAMTConfig);
	list_iterate(pokemons_Entrenadores,mostrar);

return;
}

void enviarMensaje(char *ip,int *puerto,char *mensaje){
int socket_servidor=crear_conexion(ip,puerto);

	if((*socket)==-1){
		perror("No se pudo crear la conexion");
		exit(1);
	}
enviar_mensaje(mensaje,socket_servidor);
int *size;
char *recibir=recibir_mensaje(socket_servidor,&size);
printf("Recibi %d bytes: %s del socket %d",size,recibir,socket_servidor);
}
