#include "team.h"
//TODO

void mostrarEstado(t_estado estado) {
	if (estado == READY) {
		printf("READY\n");
	}
	if (estado == BLOCKED) {
		printf("BLOCKED\n");
	}
	if (estado == EXEC) {
		printf("EXEC\n");
	}
	if (estado == EXIT) {
		printf("EXIT\n");
	}

}

void inicializarMutex() {
	int i;
	for (i = 0; i < cantidadEntrenadores; i++) {
		pthread_mutex_init(&ejecuta[i], NULL);
		pthread_mutex_lock(&ejecuta[i]);
	}

	pthread_mutex_init(&mutex_bandeja,NULL);
	pthread_mutex_init(&mutexPlani,NULL);
	sem_init(&contadorBandeja,1,0);

	return;
}

int sonIguales(t_posicion pos1,t_posicion pos2){
	return (pos1.x==pos2.x) && (pos1.y==pos2.y);
}
int sonDistintas(t_posicion pos1,t_posicion pos2){
	return !sonIguales(pos1,pos2);
}

void moverEntrenador(t_entrenador *entrenador,t_posicion coordenadas){
printf("Moviento entrenador %d a la posicion %d,%d\n",entrenador->indice,coordenadas.x,coordenadas.y);
	if(sonDistintas(entrenador->posicion,coordenadas)){

		while(entrenador->posicion.x!=coordenadas.x){
			printf("Estoy en: %d,%d\n",entrenador->posicion.x,entrenador->posicion.y);
			pthread_mutex_lock(&ejecuta[entrenador->indice]);
			printf("Moviendo en X\n");
			if(entrenador->posicion.x < coordenadas.x)
				entrenador->posicion.x++;
			else
				entrenador->posicion.x--;
		}
		while(entrenador->posicion.y!=coordenadas.y){
			printf("Estoy en: %d,%d\n",entrenador->posicion.x,entrenador->posicion.y);
			pthread_mutex_lock(&ejecuta[entrenador->indice]);
			printf("Moviendo en Y\n");
			if(entrenador->posicion.y<coordenadas.y)
				entrenador->posicion.y++;
			else
				entrenador->posicion.y--;
		}
	}
return;
}

void *manejarEntrenador(void *arg) {

	//int index=*(int*)arg;
	printf("Cree hilo para entrenador\n");
	t_entrenador *process = (t_entrenador*) arg;
	process->pid = process_get_thread_id();

	mostrarEstado(process->estado);
	printf("SOY EL HANDLER DE ENTRENADOR %d\n", process->indice);
	//printf("Estoy en %d,%d\n", process->posicion.x, process->posicion.y);
	t_posicion aMoverse;
	aMoverse.x=0;
	aMoverse.y=0;
	while (process->estado!=EXIT) {
		pthread_mutex_lock(&ejecuta[process->indice]);
		printf("--------------------INICIO------------------------\n");
		ESTADO_EXEC = process;
		printf("Me encuentro en %d,%d \n",process->posicion.x,process->posicion.y);
		printf("Ejecuto una rafaga - Proceso [%d]\n", process->pid);
		if(sonDistintas(process->posicion,aMoverse))
		moverEntrenador(process,aMoverse);
		process->estado=EXIT;
		list_add(ESTADO_EXIT, (void*) process);
		printf("---------------------FIN-----------------------\n");
	}
	printf("La posicion final del entrenador %d es %d,%d\n",process->indice,process->posicion.x,process->posicion.y);

	return NULL;
}

int estanTodosEnExit() {
	return ESTADO_EXIT->elements_count == cantidadEntrenadores;
}

void* recvMensajes(void* socketCliente) {
	int socket = *(int*) socketCliente;
		pthread_mutex_t mutexRecibir;
		pthread_mutex_init(&mutexRecibir, NULL);
		//printf("Mi semaforo vale %d\n", mutexRecibir.__data.__count);

		t_paquete *bufferLoco;
		bufferLoco = malloc(sizeof(t_paquete));
		int flag=1;
		while (flag) {

			printf("Esperando por un nuevo mensaje...\n");

			//pthread_mutex_lock(&recibir_mutex);
			pthread_mutex_lock(&mutexRecibir);
			bufferLoco = recibirMensaje(socket);



			if (bufferLoco != NULL) {
				printf("%s\n", bufferLoco->buffer->nombrePokemon);
				pthread_mutex_lock(&mutex_bandeja);
				queue_push(bandejaDeMensajes, (void*) bufferLoco);
				sem_post(&contadorBandeja);
				pthread_mutex_unlock(&mutex_bandeja);
				pthread_mutex_unlock(&mutexRecibir);
				printf("estoy despues del unlock de bandeja de mensajes\n");
			}
			else
			{
			pthread_mutex_unlock(&mutexRecibir);
			flag=0;
			}




			log_info(logger, "Estoy dentro del handler loco\n");

		}

		//pthread_detach(socket);	//ver si es esto lo que finaliza el hilo y libera los recursos;
	//hacer un free completo de bufferLoco

	//free(bufferLoco);

	//free_t_message(bufferLoco);

		//pthread_exit(NULL);
		return NULL;

}

void* procesarMensaje() { // aca , la idea es saber que pokemon ponemos en el mapa por ejemplo.
	printf("Rompo en procesarMensaje 1\n");
	t_paquete* bufferLoco = malloc(sizeof(t_paquete));

	while(1){
//	printf("Rompo en procesarMensaje 2\n");
	sem_wait(&contadorBandeja);
	pthread_mutex_lock(&mutex_bandeja);
//	printf("Rompo en procesarMensaje 3\n");
	bufferLoco = (t_paquete*) queue_pop(bandejaDeMensajes); //ver en que posicion busco, por ahi se necesita una variable.
//	printf("Rompo en procesarMensaje 4\n");
	pthread_mutex_unlock(&mutex_bandeja);
	switch (bufferLoco->codigoOperacion) {
	case MENSAJE_APPEARED_POKEMON: { //ver que casos usa el team
		if(estaEn(objetivoGlobal,(void*)bufferLoco->buffer->nombrePokemon)){
			printf("Hay un %s que necesito en %d,%d\n",bufferLoco->buffer->nombrePokemon,bufferLoco->buffer->posX,bufferLoco->buffer->posY);
		}
		break;
	}
	case MENSAJE_CAUGHT_POKEMON: {
		break;
	}

	case MENSAJE_LOCALIZED_POKEMON: {
		break;
	}
	}
//	printf("Rompo en procesarMensaje 5\n");
	}
	return NULL;
}

void* planificarEntrenadores(void* socketServidor) { //aca vemos que entrenador esta en ready y mas cerca del pokemon
//agarramos el pokemon o lo que sea que el entrenador tenga que hacer y enviamos un mensaje al broker avisando.

	int i,j;
	pthread_mutex_lock(&mutexPlani);
	while (!estanTodosEnExit()) {

		for (j = 0; j < cantidadEntrenadores; j++) {
			//sleep(2);
			pthread_mutex_unlock(&ejecuta[j]);
			printf("Esta en ejec el proceso %d\n",ESTADO_EXEC->pid);
		}
	}
	printf("Todos los procesos estan en EXIT\n");
	return NULL;

}

void inicializarLoggerTeam() {
	logger = log_create("team.log", "TEAM", 1, LOG_LEVEL_TRACE);
	if (logger == NULL) {
		perror("No se pudo inicializar el logger\n");
		exit(1);
	}
	return;
}

void splitList(char **string, t_list *lista) {
	if (string != NULL) {
		char **elem = string_split(string, "|"); //odio este warning, si el string de adentro se castea a char* y elem se borra o utiliza, el warning desaparece.
		while (*string != NULL) {
			agregarElemento(*string, lista);
			string++;
		}
	}
}

void agregarElemento(char *elemento, t_list *lista) {
	if (elemento != NULL) {
		list_add(lista, elemento);
	}
}

void mostrar(void *elemento) {
	//log_info(logger,"%s",(char*)elemento);
	printf("%s\n", (char*) elemento);
}

void mostrarLista(t_list *lista) {
	t_list *aux = list_duplicate(lista);

	while (aux->head != NULL) {
		mostrar(aux->head->data);
		aux->head = aux->head->next;
	}
	list_destroy(aux);
}

t_posicion separarPosiciones(void *data) {
	char *coord = (char*) data;
	char *x, *y;
	x = strtok(coord, "|");
	y = strtok(NULL, "|");
	t_posicion aDevolver;
	aDevolver.x = atoi(x);
	aDevolver.y = atoi(y);
	return aDevolver;
}

t_list *separarPokemons(void*data, int flag) {

	t_list* pokemongos = list_create();
	char *string, *token;
	string = (char*) data;

	token = strtok(string, "|");
	while (token != NULL) {
		list_add(pokemongos, (void*) token);
		if (flag == 1) {
			list_add(objetivoGlobal, (void*) token);
		}
		token = strtok(NULL, "|");
	}
	//printf("La lista quedo: \n");
	//mostrarLista(pokemongos);
	return pokemongos;
}
//void crearEntrenadores(t_list *posicionesEntrenadores,t_list* pokemonsEntrenadores,t_list *objetivosEntrenadores)
void crearEntrenadores() {

	t_list *auxPos, *auxPok, *auxObj;
	objetivoGlobal = list_create();
	log_info(logger, "Instanciando entrenadores");
	int i;
	entrenadores = (t_entrenador*) malloc(cantidadEntrenadores);
	t_link_element *limpieza;
	//t_posicion *posiciones=(t_posicion*)malloc(cantidadEntrenadores);
	//t_list *pokemons;
	//t_list *objetivos;
	auxPos = list_duplicate(posicionEntrenadores);
	auxPok = list_duplicate(pokemonEntrenadores);
	auxObj = list_duplicate(objetivoEntrenadores);
	for (i = 0; i < cantidadEntrenadores; i++) {
		t_entrenador *nuevoEntrenador = malloc(sizeof(t_entrenador));
		nuevoEntrenador->posicion = separarPosiciones(auxPos->head->data);
		//entrenadores[i].posicion = separarPosiciones(auxPos->head->data);

		//posiciones[i]=separarPosiciones(auxPos->head->data);
		if ((i + 1) < cantidadEntrenadores) {
			limpieza = auxPos->head;
			auxPos->head = auxPos->head->next;
			auxPos->elements_count--;
			free(limpieza);
		}
		nuevoEntrenador->pokemons = list_duplicate(
				separarPokemons(auxPok->head->data, 0));
//		entrenadores[i].pokemons = list_duplicate(
//				separarPokemons(auxPok->head->data, 0));

		if ((i + 1) < cantidadEntrenadores) {
			limpieza = auxPok->head;
			auxPok->head = auxPok->head->next;
			auxPok->elements_count--;
			free(limpieza);
		}

		nuevoEntrenador->objetivos = list_duplicate(
				separarPokemons(auxObj->head->data, 1));
//		entrenadores[i].objetivos = list_duplicate(
//				separarPokemons(auxObj->head->data, 1));
		if ((i + 1) < cantidadEntrenadores) {
			limpieza = auxObj->head;
			auxObj->head = auxObj->head->next;
			auxObj->elements_count--;
			free(limpieza);
		}

		printf("Entrenador %d, está en X=%d e Y=%d.\n", i + 1,
				nuevoEntrenador->posicion.x, nuevoEntrenador->posicion.y);
		printf("Los pokemons del entrenador %d son:\n", i + 1);
		mostrarLista(nuevoEntrenador->pokemons);
		printf("Los objetivos del entrenador %d son:\n", i + 1);
		//log_info(logger,"Los objetivos del entrenador %d son:",i+1);
		mostrarLista(nuevoEntrenador->objetivos);

		//list_destroy(pokemons);
		//list_destroy(objetivos);

		//entrenadores[i].estado=NEW;

		nuevoEntrenador->estado = READY;
		nuevoEntrenador->estimacionRafagaActual = 0;
		nuevoEntrenador->finRafaga = 0;
		nuevoEntrenador->inicioRafaga = 0;
		nuevoEntrenador->quantumPendiente = 0;
		nuevoEntrenador->rafaga = 0;
		nuevoEntrenador->indice = i;

		list_add(ESTADO_READY, (void*) nuevoEntrenador);

	}
//	int j;
//	for(j=0;j<cantidadEntrenadores;j++){
//	pthread_create(...,NULL,planificarEntrenadores,(void*)j);
//	}

	free(posicionEntrenadores);
//list_destroy(objetivos);
//list_destroy(pokemons);
	list_destroy(auxObj);
	list_destroy(auxPok);
	list_destroy(auxPos);

}

void cargarConfigTeam() {

	TEAMTConfig = config_create(TEAM_CONFIG_PATH);
	if (TEAMTConfig == NULL) {
		perror("No se pudo leer la configuracion\n");
		exit(2);
	}

	teamConf = (t_TEAMConfig*) malloc(sizeof(t_TEAMConfig)); //Reservando memoria
	log_info(logger, "Comenzando a importar configuracion");
	teamConf->POKEMON_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"POKEMON_ENTRENADORES");
	pokemonEntrenadores = list_create();
	splitList(teamConf->POKEMON_ENTRENADORES, pokemonEntrenadores);
	mostrarLista(pokemonEntrenadores);

	teamConf->POSICIONES_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"POSICIONES_ENTRENADORES");
	posicionEntrenadores = list_create();
	splitList(teamConf->POSICIONES_ENTRENADORES, posicionEntrenadores);
	mostrarLista(posicionEntrenadores);

	teamConf->OBJETIVOS_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"OBJETIVOS_ENTRENADORES");
	objetivoEntrenadores = list_create();
	splitList(teamConf->OBJETIVOS_ENTRENADORES, objetivoEntrenadores);
	mostrarLista(objetivoEntrenadores);

	teamConf->TIEMPO_RECONEXION = config_get_int_value(TEAMTConfig,
			"TIEMPO_RECONEXION");
	log_info(logger, "Lei TIEMPO_RECONEXION=%d de la configuracion\n",
			teamConf->TIEMPO_RECONEXION);

	teamConf->RETARDO_CICLO_CPU = config_get_int_value(TEAMTConfig,
			"RETARDO_CICLO_CPU");
	log_info(logger, "Lei RETARDO_CICLO_CPU=%d de la configuracion\n",
			teamConf->RETARDO_CICLO_CPU);

	teamConf->ALGORITMO_PLANIFICACION = string_duplicate(
			config_get_string_value(TEAMTConfig, "ALGORITMO_PLANIFICACION"));
	log_info(logger, "Lei ALGORITMO_PLANIFICACION=%s de la configuracion\n",
			teamConf->ALGORITMO_PLANIFICACION);

	teamConf->QUANTUM = config_get_int_value(TEAMTConfig, "QUANTUM");
	log_info(logger, "Lei QUANTUM=%d de la configuracion\n", teamConf->QUANTUM);

	teamConf->ESTIMACION_INICIAL = config_get_double_value(TEAMTConfig,
			"ESTIMACION_INICIAL");
	log_info(logger, "Lei ESTIMACION_INICIAL=%f de la configuracion\n",
			teamConf->ESTIMACION_INICIAL);

	teamConf->IP_BROKER = string_duplicate(
			config_get_string_value(TEAMTConfig, "IP_BROKER"));

	log_info(logger, "Lei IP_BROKER=%s de la configuracion\n",
			teamConf->IP_BROKER);

	teamConf->PUERTO_BROKER = config_get_int_value(TEAMTConfig,
			"PUERTO_BROKER");
	log_info(logger, "Lei PUERTO_BROKER=%d de la configuracion\n",
			teamConf->PUERTO_BROKER);

	teamConf->LOG_FILE = string_duplicate(
			config_get_string_value(TEAMTConfig, "LOG_FILE"));
	log_info(logger, "Lei LOG_FILE=%s de la configuracion\n",
			teamConf->LOG_FILE);

	cantidadEntrenadores = pokemonEntrenadores->elements_count;
	log_info(logger, "Este equipo tiene %d entrenadores", cantidadEntrenadores);

	//Esta funcion recibe todoo esto porque me estoy atajando.
//	crearEntrenadores(posicionEntrenadores, pokemonEntrenadores,
//			objetivoEntrenadores);
	crearEntrenadores();

	//Fin de importar configuracion
	log_info(logger, "CONFIGURACION IMPORTADA\n");

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

void terminarPrograma() {
	log_destroy(logger);
	config_destroy(TEAMTConfig);
	free(teamConf);
	free(entrenadores);
	list_destroy(pokemonEntrenadores);
	list_destroy(objetivoEntrenadores);
	list_destroy(posicionEntrenadores);
}

bool estaEn(t_list* lista, void *elemento) {

	t_list *aux = list_duplicate(lista);
	bool flag = false;
	t_link_element *limpieza;
	while (aux->head != NULL) {
		//printf("Comparando %s y %s\n", (char*) aux->head->data,
		//		(char*) elemento);

		if ((strcmp((char*) aux->head->data, (char*) elemento) == 0)) {
			flag = true;
		}
		limpieza = aux->head;
		aux->head = aux->head->next;
		free(limpieza);
	}
	return flag;

}

void agregarElementoSinRepetido(t_list *lista, void *elemento) {
	if (estaEn(lista, elemento)) {
		//printf("Elemento repetido\n");
	} else {
		list_add(lista, elemento);
	}
	return;
}

t_list *sinRepetidos(t_list *lista) {
	t_list* aDevolver = list_create();
	t_list *aux = list_duplicate(lista);
	int i, limite;
	limite = aux->elements_count;

	for (i = 0; i < limite; i++) {
		//printf("Iteracion %d\n", i);
		if (list_is_empty(aDevolver)) {
			//	printf("%s\n", (char*) aux->head->data);
			list_add(aDevolver, aux->head->data);
		} else {
			agregarElementoSinRepetido(aDevolver, list_get(aux, i));

		}
	}

	return aDevolver;
}

void pedirPokemons(int socket) {
	t_list* pokemonGet = sinRepetidos(objetivoGlobal);
//	printf("El objetivo global del TEAM es: \n");
//	mostrarLista(objetivoGlobal);
//	printf("Sin repetidos es: \n");
//	mostrarLista(pokemonGet);
	printf("Se pediran los siguientes pokemons: \n");
	mostrarLista(pokemonGet);

	void _realizarGet(void* elemento) {

		char *pokemon = (char*) elemento;
		enviarMensajeBrokerGet(pokemon, socket);
		//sleep(1);
	}

	list_iterate(pokemonGet, _realizarGet);

	//liberarConexion(socket);

	return;
}

//void *handler(void* arg) {
//	int socket = *(int*) arg;
//
//	t_paquete *paquete;
//
//	while(1){
//	paquete = recibirMensaje(socket);
//
//	pthread_mutex_lock(&mutex_bandeja);
//	queue_push(bandejaDeMensajes, (void*) paquete);
//	pthread_mutex_unlock(&mutex_bandeja);
//	sem_post(&contadorBandeja);
//
//
//	switch (paquete->codigoOperacion) {
//	case MENSAJE_APPEARED_POKEMON: {
//		printf("Llego un mensaje Appeared_Pokemon\n");
//	}
//		break;
//	case MENSAJE_LOCALIZED_POKEMON: {
//		printf("Llego un mensaje Localized_Pokemon\n");
//	}
//		break;
//	case MENSAJE_CAUGHT_POKEMON: {
//		printf("Llego un mensaje Caught_Pokemon\n");
//	}
//		break;
//	default:
//		printf("Este mensaje no se puede manejar por el team \n");
//		break;
//	}
//	}
//	pthread_exit(NULL);
//	return NULL;
//}
//Todo
void *administrarMensajes(void *arg){
return NULL;
}



void *escucharGameboy() {
	pthread_t threadId[MAX_CONEXIONES];

		int contadorConexiones = 0;
		pthread_t hilo;
		pthread_create(&hilo, NULL, administrarMensajes, NULL);

		int socketDelCliente[MAX_CONEXIONES];
		struct sockaddr direccionCliente;
		unsigned int tamanioDireccion = sizeof(direccionCliente);

		int servidor = initServer("127.0.0.1", 5002);

		log_info(logger, "ESCHUCHANDO CONEXIONES");
		log_info(logger, "iiiiIIIII!!!");

		while (1) {

			socketDelCliente[contadorConexiones] = accept(servidor,
					(void*) &direccionCliente, &tamanioDireccion);

			if (socketDelCliente >= 0) {

				log_info(logger, "Se ha aceptado una conexion: %i\n",
						socketDelCliente[contadorConexiones]);
				if ((pthread_create(&threadId[contadorConexiones], NULL, recvMensajes,
						(void*) &socketDelCliente[contadorConexiones])) < 0) {
					log_info(logger, "No se pudo crear el hilo");
					//return 1;
				} else {
					log_info(logger, "Handler asignado\n");
					tamanioDireccion = 0;
					//pthread_join(threadId[contadorConexiones], NULL)

				}
			} else {
				log_info(logger, "Falló al aceptar conexión");
			}
			contadorConexiones++;

		}

		pthread_join(hilo, NULL);

}

void iniciarEstados() {
	printf("Creando listas de ejecucion\n");
	ESTADO_BLOCKED = list_create();
	ESTADO_EXEC = malloc(sizeof(t_entrenador));
	ESTADO_EXIT = list_create();
	ESTADO_READY = list_create();
	bandejaDeMensajes = queue_create();
	return;
}
void calculoEstimacionSjf(t_entrenador *entrenador) {
	//Modifica la estimacionRafagaActual del entrenador pasado por parametro, ver el /1000 si es necesario.
	entrenador->estimacionRafagaActual = (alpha * entrenador->ultimaRafaga)
			+ ((1 - (alpha)) * (entrenador->estimacionRafagaActual));
}
t_entrenador *buscarMenorRafaga(t_list *entrenadores) { //ver si busca al de menor rafaga porque no pude probarlo todavia.

	t_entrenador* unEntrenador = malloc(sizeof(t_entrenador));
	switch (list_size(entrenadores)) {

	case 0: {
		log_error(logger, "- LA LISTA ESTA VACIA");
		break;
	}

	case 1: {
		unEntrenador = list_remove(entrenadores, 0);

		break;
	}

	default: {
		t_entrenador *entrenador1;
		t_entrenador *entrenador2;

		int pos = 0;

		entrenador1 = list_get(entrenadores, 0);

		for (int i = 1; i < list_size(entrenadores); i++) {

			entrenador2 = list_get(entrenadores, i);

			if (entrenador1->estimacionRafagaActual
					>= entrenador2->estimacionRafagaActual) {
				entrenador1 = entrenador2;
				pos = i;
			}
		}
		unEntrenador = list_remove(entrenadores, pos);
	}

	}

	return unEntrenador;
}
