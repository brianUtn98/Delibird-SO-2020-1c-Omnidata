#include "team.h"

void inicializarLoggerTeam() {
	logger = log_create("team.log", "TEAM", 1, LOG_LEVEL_TRACE);
	if (logger == NULL) {
		perror("No se pudo inicializar el logger\n");
		exit(1);
	}
	return;
}

void splitList(char **string,t_list *lista){
if(string != NULL){
	char **elem=string_split(string,"|");
	while (*string != NULL) {
			agregarElemento(*string,lista);
			string++;
		}
}
}

void agregarElemento(char *elemento,t_list *lista){
	if(elemento!=NULL){
	list_add(lista,elemento);
	}
}

void mostrar(void *elemento) {
	    printf("El elemento: %s\n", (char *)elemento);
	  }

void mostrarLista(t_list *lista){
t_list *aux=list_duplicate(lista);

	while(aux->head!=NULL){
	mostrar(aux->head->data);
	aux->head=aux->head->next;
	}
	list_destroy(aux);
}

//TODO
t_posicion separarPosiciones(void *data){
char *coord=(char*)data;
char *x,*y;
	x=strtok(coord,"|");
	y=strtok(NULL,"|");

	printf("Las coordenadas son %s y %s\n",x,y);

t_posicion aDevolver;
	aDevolver.x=atoi(x);
	aDevolver.y=atoi(y);
	printf("Las coordenadas en int son X=%d e Y=%d\n",aDevolver.x,aDevolver.y);
}

void crearEntrenadores(){
	log_info(logger,"Instanciando entrenadores");
	int i, cantidadEntrenadores;
	cantidadEntrenadores=posicionEntrenadores->elements_count;
	entrenadores=(t_entrenador*)malloc(sizeof(cantidadEntrenadores));

	t_posicion *posiciones=(t_posicion*)malloc(sizeof(cantidadEntrenadores));
	t_list *pokemons;
	t_list *objetivos;

	for(i=0;i<cantidadEntrenadores;i++){
	//entrenadores[i].posicion=posiciones[i];
	pokemons=pokemonDeEntrenador(i);
	//entrenadores[i].objetivos=objetivos;
	entrenadores[i].pokemons=pokemons;
	}
}

t_list *pokemonDeEntrenador(int i){
int j=0;

char **string;
t_list *aDevolver;
	t_list *aux=list_duplicate(pokemonEntrenadores);
		while(aux->head!=NULL && j<i){
		printf("%s",(char*)aux->head->data);
		aux->head=aux->head->next;
		}
		log_info(logger,"%s",(char*)aux->head->data);
		sleep(3);
		string=(char *)aux->head->data;
		log_info(logger,"%s",string);
		aDevolver=list_create();
		aDevolver=string_split(string,"|");
		//splitList(string,aDevolver);
		mostrarLista(aDevolver);
		list_destroy(aux);
		return aDevolver;
}

void cargarConfigTeam() {
int cantidadEntrenadores;

	TEAMTConfig= config_create(TEAM_CONFIG_PATH);
	if (TEAMTConfig == NULL) {
		perror("No se pudo leer la configuracion\n");
		exit(2);
	}

	teamConf = (t_TEAMConfig*)malloc(sizeof(t_TEAMConfig)); //Reservando memoria

	teamConf->POKEMON_ENTRENADORES=config_get_array_value(TEAMTConfig,"POKEMON_ENTRENADORES");
	pokemonEntrenadores=list_create();
	splitList(teamConf->POKEMON_ENTRENADORES,pokemonEntrenadores);
	mostrarLista(pokemonEntrenadores);

	teamConf->POSICIONES_ENTRENADORES=config_get_array_value(TEAMTConfig,"POSICIONES_ENTRENADORES");
	posicionEntrenadores=list_create();
	splitList(teamConf->POSICIONES_ENTRENADORES,posicionEntrenadores);
	mostrarLista(posicionEntrenadores);
	separarPosiciones(posicionEntrenadores->head->data);

	teamConf->OBJETIVOS_ENTRENADORES=config_get_array_value(TEAMTConfig,"OBJETIVOS_ENTRENADORES");
	objetivoEntrenadores=list_create();
	splitList(teamConf->OBJETIVOS_ENTRENADORES,objetivoEntrenadores);
	mostrarLista(objetivoEntrenadores);


	teamConf->TIEMPO_RECONEXION = config_get_int_value(TEAMTConfig,
			"TIEMPO_RECONEXION");
	log_info(logger, "Lei TIEMPO_RECONEXION=%d de la configuracion\n",
			teamConf->TIEMPO_RECONEXION);

	teamConf->RETARDO_CICLO_CPU = config_get_int_value(TEAMTConfig,
			"RETARDO_CICLO_CPU");
	log_info(logger, "Lei RETARDO_CICLO_CPU=%d de la configuracion\n",
			teamConf->RETARDO_CICLO_CPU);

	teamConf->ALGORITMO_PLANIFICACION=string_duplicate(config_get_string_value(TEAMTConfig,"ALGORITMO_PLANIFICACION"));
	log_info(logger, "Lei ALGORITMO_PLANIFICACION=%s de la configuracion\n",
			teamConf->ALGORITMO_PLANIFICACION);

	teamConf->QUANTUM = config_get_int_value(TEAMTConfig, "QUANTUM");
	log_info(logger, "Lei QUANTUM=%d de la configuracion\n", teamConf->QUANTUM);

	teamConf->ESTIMACION_INICIAL = config_get_double_value(TEAMTConfig,
			"ESTIMACION_INICIAL");
	log_info(logger, "Lei ESTIMACION_INICIAL=%f de la configuracion\n",
			teamConf->ESTIMACION_INICIAL);

	teamConf->IP_BROKER=string_duplicate(config_get_string_value(TEAMTConfig,"IP_BROKER"));

	log_info(logger, "Lei IP_BROKER=%s de la configuracion\n",
			teamConf->IP_BROKER);

	teamConf->PUERTO_BROKER = config_get_int_value(TEAMTConfig,
			"PUERTO_BROKER");
	log_info(logger, "Lei PUERTO_BROKER=%d de la configuracion\n",
			teamConf->PUERTO_BROKER);

	teamConf->LOG_FILE=string_duplicate(config_get_string_value(TEAMTConfig,"LOG_FILE"));
				log_info(logger, "Lei LOG_FILE=%s de la configuracion\n",
						teamConf->LOG_FILE);


	cantidadEntrenadores=pokemonEntrenadores->elements_count;
	log_info(logger,"Este equipo tiene %d entrenadores",cantidadEntrenadores);
	//Fin de importar configuracion
	log_info(logger, "- CONFIGURACION IMPORTADA\n");

// 	if(TEAMTConfig!=NULL){
//	config_destroy(TEAMTConfig);
// 	}

	return;
}


/*
void enviarMensaje(char *ip, int puerto, char *mensaje) {
	int socket_servidor = crear_conexion(ip, puerto);

	if (socket_servidor == -1) {
		perror("No se pudo crear la conexion");
		exit(1);
	}
	enviar_mensaje(mensaje, socket_servidor);
	int *size;
	char *recibir = recibir_mensaje(socket_servidor, &size);
	printf("Recibi %d bytes: %s del socket %d", size, recibir, socket_servidor);
}*/
