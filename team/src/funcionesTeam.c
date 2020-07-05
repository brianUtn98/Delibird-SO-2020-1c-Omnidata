#include "team.h"
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

	pthread_mutex_init(&mutex_bandeja, NULL);
	pthread_mutex_init(&mutexPlani, NULL);
	pthread_mutex_init(&mutexListaPokemons, NULL);
	pthread_mutex_init(&cpu, NULL);
	pthread_mutex_init(&mutexProximos, NULL);
	pthread_mutex_init(&mutexDeadlock,NULL);
	sem_init(&contadorBandeja, 1, 0);
	sem_init(&pokemonsEnLista, 1, 0);
	sem_init(&counterProximosEjecutar, 1, 0);
	sem_init(&counterDeadlock,1,0);
	return;
}

int sonIguales(t_posicion pos1, t_posicion pos2) {
	return (pos1.x == pos2.x) && (pos1.y == pos2.y);
}
int sonDistintas(t_posicion pos1, t_posicion pos2) {
	return !sonIguales(pos1, pos2);
}

void moverEntrenador(t_entrenador *entrenador, t_posicion coordenadas) {
	t_posicion guardar = entrenador->posicion;
	printf("Moviento entrenador %d a la posicion %d,%d\n", entrenador->indice,
			coordenadas.x, coordenadas.y);
	if (sonDistintas(entrenador->posicion, coordenadas)) {

		while (entrenador->posicion.x != coordenadas.x) {
			printf("Estoy en: %d,%d\n", entrenador->posicion.x,
					entrenador->posicion.y);

			if (strcmp(teamConf->ALGORITMO_PLANIFICACION, "RR") == 0) {
				if (administrativo[entrenador->indice].quantum < 1) {

					log_debug(logger, "FIN DE QUANTUM");
					printf("--------FIN DE QUANTUM--------\n");
					pthread_mutex_lock(&mutexProximos);

					log_info(logEntrega,
							"Se cambia entrenador %d a la cola BLOCKED por fin de quantum",
							entrenador->indice);
					list_add(ESTADO_BLOCKED, (void*) entrenador);
					entrenador->estado = BLOCKED;

					printf("Agregando entrenador a proximos\n");
					queue_push(proximosEjecutar, (void*) entrenador);
					sem_post(&counterProximosEjecutar);
					pthread_mutex_unlock(&mutexProximos);
					pthread_mutex_unlock(&cpu);

					printf("Bloqueado en el mutex entrenador\n");
					pthread_mutex_lock(&ejecuta[entrenador->indice]);
					printf("Bloqueado en el cpu\n");
					pthread_mutex_lock(&cpu);

					log_info(logEntrega,
							"Se cambia entrenador %d a EXEC porque se le dio quantum",
							entrenador->indice);
					int i = hallarIndice(entrenador, ESTADO_BLOCKED);
					list_remove(ESTADO_BLOCKED, i);
					ESTADO_EXEC = entrenador;
					entrenador->estado = EXEC;
					cambiosDeContexto++;

				}
			}
			printf("Moviendo en X\n");
			if (entrenador->posicion.x < coordenadas.x)
				entrenador->posicion.x++;
			else
				entrenador->posicion.x--;
			sleep(teamConf->RETARDO_CICLO_CPU);
			ciclosDeCpuTotales++;
			ciclosPorEntrenador[entrenador->indice]++;

			administrativo[entrenador->indice].quantum--;
		}
		while (entrenador->posicion.y != coordenadas.y) {
			printf("Estoy en: %d,%d\n", entrenador->posicion.x,
					entrenador->posicion.y);

			if (strcmp(teamConf->ALGORITMO_PLANIFICACION, "RR") == 0) {
				if (administrativo[entrenador->indice].quantum == 0) {

					log_debug(logger, "FIN DE QUANTUM");
					printf("--------FIN DE QUANTUM--------\n");
					pthread_mutex_lock(&mutexProximos);

					log_info(logEntrega,
							"Se cambia entrenador %d a la cola BLOCKED por fin de quantum",
							entrenador->indice);
					list_add(ESTADO_BLOCKED, (void*) entrenador);
					entrenador->estado = BLOCKED;

					printf("Agregando entrenador a proximos\n");
					queue_push(proximosEjecutar, (void*) entrenador);
					sem_post(&counterProximosEjecutar);
					pthread_mutex_unlock(&mutexProximos);
					pthread_mutex_unlock(&cpu);

					printf("Bloqueado en el mutex entrenador\n");
					pthread_mutex_lock(&ejecuta[entrenador->indice]);
					printf("Bloqueado en el cpu\n");
					pthread_mutex_lock(&cpu);

					log_info(logEntrega,
							"Se cambia entrenador %d a EXEC porque se le dio quantum",
							entrenador->indice);
					int i = hallarIndice(entrenador, ESTADO_BLOCKED);
					list_remove(ESTADO_BLOCKED, i);
					ESTADO_EXEC = entrenador;
					cambiosDeContexto++;

				}
			}

			printf("Moviendo en Y\n");
			if (entrenador->posicion.y < coordenadas.y)
				entrenador->posicion.y++;
			else
				entrenador->posicion.y--;
			sleep(teamConf->RETARDO_CICLO_CPU);
			ciclosDeCpuTotales++;
			ciclosPorEntrenador[entrenador->indice]++;

			administrativo[entrenador->indice].quantum--;
		}
	}
	log_info(logEntrega, "Se movio el entrenador %d desde %d,%d hasta %d,%d",
			entrenador->indice, guardar.x, guardar.y, coordenadas.x,
			coordenadas.y);
	return;
}

int buscarIndicePokemon(void* data, t_list *lista) {
	t_list *aux = list_duplicate(lista);
	int indice = 0;
	int hallado = 0;
	while (aux->head != NULL
			&& strcmp((char*) data, (char*) aux->head->data) != 0) {
		aux->head = aux->head->next;
		indice++;
		if (strcmp((char*) data, (char*) aux->head->data) == 0)
			hallado = 1;
	}
	if (strcmp((char*) data, (char*) aux->head->data) == 0)
		hallado = 1;
	if (hallado == 1)
		return indice;
	else
		return -1;

}

int cumplioObjetivo(t_entrenador *entrenador) {
	t_list *aux1 = list_duplicate(entrenador->objetivos);
	t_list *aux2 = list_duplicate(entrenador->pokemons);

	int i, cantidad = entrenador->objetivos->elements_count;
	if (entrenador->objetivos->elements_count
			== entrenador->pokemons->elements_count) {
		for (i = 0; i < cantidad; i++) {
			if (!estaEn(aux2, aux1->head->data)) {
				return 0;
			}

			list_remove(aux2, buscarIndicePokemon(aux1->head->data, aux2));
			aux1->head = aux1->head->next;
		}
		return 1;

	} else {
		return 0;
	}
}

int hayEntrenadoresDisponibles() {
	return ESTADO_READY->elements_count > 0;
}

void *manejarEntrenador(void *arg) {

//int index=*(int*)arg;

	printf("Cree hilo para entrenador\n");
	t_entrenador *process = (t_entrenador*) arg;
	process->pid = process_get_thread_id();

	administrativo[process->indice].quantum = teamConf->QUANTUM;

	mostrarEstado(process->estado);
	printf("SOY EL HANDLER DE ENTRENADOR %d\n", process->indice);

	printf("Mi Quantum es de %d\n", administrativo[process->indice].quantum);
	int index;
	while (process->estado != EXIT) {

		if (!hayEntrenadoresDisponibles()) {
			tratamientoDeDeadlocks();
		}
		if(process->flagDeadlock == 0){
		pthread_mutex_lock(&ejecuta[process->indice]);

		pthread_mutex_lock(&cpu);

		/*Estructuras administrativas del hilo: */
		t_administrativoEntrenador recurso = administrativo[process->indice];
		t_posicion aMoverse;
		aMoverse.x = recurso.posX;
		aMoverse.y = recurso.posY;
		printf("--------------------INICIO------------------------\n");
		index = hallarIndice(process, ESTADO_READY);
		if (index != -1) {
			list_remove(ESTADO_READY, index);
			log_info(logEntrega,"Se cambia entrenador %d a la cola EXEC para atrapar pokemon",process->indice);
			ESTADO_EXEC = process;
		} else {
			log_error(logger, "El entrenador no puede ejecutar!");
			if(ESTADO_READY->elements_count == 0)
			tratamientoDeDeadlocks();
		}
		process->estado = EXEC;
		cambiosDeContexto++;

		moverEntrenador(process, aMoverse);

//		int socket = crearConexion(teamConf->IP_BROKER, teamConf->PUERTO_BROKER,
//				teamConf->TIEMPO_RECONEXION);
//		enviarMensajeBrokerCatch(recurso.nombrePokemon, recurso.posX,
//				recurso.posY, socket);
//		t_paquete *idMensaje = malloc(sizeof(t_paquete));
//		idMensaje = recibirMensaje(socket);
//		list_add(listaIdCatch, (void*) idMensaje->buffer->idMensaje);
//
//		liberarConexion(socket);

//Tiene que quedarse bloqueado hasta recibir caught del mismo ID.
//Todo
		/*Falta aca toda la logica de atrapar el pokemon*/

		log_info(logEntrega, "Se atrapa %s en %d,%d", recurso.nombrePokemon,
				recurso.posX, recurso.posY);
		list_add(process->pokemons, (void*) recurso.nombrePokemon);

		if (cumplioObjetivo(process)) {
			log_info(logEntrega,
					"Se cambia entrenador %d a la cola EXIT porque cumplio su objetivo",
					process->indice);
			list_add(ESTADO_EXIT, (void*) process);
			process->estado = EXIT;

			terminarSiPuedo();
		} else {

			if (puedeSeguirAtrapando(process)) {
				log_info(logEntrega,
						"Se cambia entrenador %d a la cola READY por fin de rafaga",
						process->indice);
				list_add(ESTADO_READY, (void*) process);
				process->estado = READY;
			} else {
				log_info(logEntrega,
						"Se cambia entrenador %d a la cola BLOCKED porque posee tantos pokemons como objetivos tiene",
						process->indice);
				list_add(ESTADO_BLOCKED, (void*) process);
				process->estado = BLOCKED;
			}
		}

		printf("Me encuentro en %d,%d \n", process->posicion.x,
				process->posicion.y);
		printf("---------------------FIN-----------------------\n");

		printf("Los pokemons del entrenador %d son: \n", process->indice);

		mostrarListaChar(process->pokemons);
		pthread_mutex_unlock(&cpu);
	}
	}
	printf("La posicion final del entrenador %d es %d,%d\n", process->indice,
			process->posicion.x, process->posicion.y);

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

	int flag = 1;
	while (flag) {
		bufferLoco = malloc(sizeof(t_paquete));
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
		} else {
			pthread_mutex_unlock(&mutexRecibir);
			flag = 0;
		}

		//log_info(logger, "Estoy dentro del handler loco\n");

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

	while (1) {
//	printf("Rompo en procesarMensaje 2\n");
		sem_wait(&contadorBandeja);
		pthread_mutex_lock(&mutex_bandeja);
//	printf("Rompo en procesarMensaje 3\n");
		bufferLoco = (t_paquete*) queue_pop(bandejaDeMensajes); //ver en que posicion busco, por ahi se necesita una variable.
//	printf("Rompo en procesarMensaje 4\n");
		pthread_mutex_unlock(&mutex_bandeja);
		switch (bufferLoco->codigoOperacion) {
		case MENSAJE_APPEARED_POKEMON: { //ver que casos usa el team
			if (estaEn(objetivoGlobal,
					(void*) bufferLoco->buffer->nombrePokemon)) {
				printf("Hay un %s que necesito en %d,%d\n",
						bufferLoco->buffer->nombrePokemon,
						bufferLoco->buffer->posX, bufferLoco->buffer->posY);
				log_info(logEntrega,
						"Llego mensaje APPEARED_POKEMON - %s %d,%d",
						bufferLoco->buffer->nombrePokemon,
						bufferLoco->buffer->posX, bufferLoco->buffer->posY);

				pthread_mutex_lock(&mutexListaPokemons);
				queue_push(appearedPokemon, (void*) bufferLoco);
				pthread_mutex_unlock(&mutexListaPokemons);
				sem_post(&pokemonsEnLista);

			} else {
				free(bufferLoco->buffer);
				free(bufferLoco);
				printf("Libere memoria\n");
			}
			break;
		}
		case MENSAJE_CAUGHT_POKEMON: {
			log_info(logEntrega, "Llego mensaje CAUGHT_POKEMON\n");
			break;
		}

		case MENSAJE_LOCALIZED_POKEMON: {
			log_info(logEntrega, "Llego mensaje LOCALIZED_POKEMON\n");
			break;
		}
		case ENVIAR_ID_MENSAJE: {
			printf("Se asignó un id: %d\n", bufferLoco->buffer->idMensaje);
			log_debug(logger, "Se asignó un id: %d\n",
					bufferLoco->buffer->idMensaje);
			log_info(logEntrega, "Broker asigno id: %d",
					bufferLoco->buffer->idMensaje);
			break;
		}
		}
//	printf("Rompo en procesarMensaje 5\n");
	}
	return NULL;
}

int distanciaHasta(t_posicion pos1, t_posicion pos2) {
	int desp = 0;
	t_posicion aux = pos1;
	if (sonDistintas(pos1, pos2)) {
		while (pos1.x != pos2.x) {
			if (pos1.x < pos2.x) {
				pos1.x++;
				desp++;
			} else {
				pos1.x--;
				desp++;
			}
		}
		while (pos1.y != pos2.y) {
			if (pos1.y < pos2.y) {
				pos1.y++;
				desp++;
			} else {
				pos1.y--;
				desp++;
			}
		}
	}
	printf("La distancia entre %d,%d y %d,%d es %d\n", aux.x, aux.y, pos2.x,
			pos2.y, desp);
	return desp;
}

t_entrenador *buscarMasCercano(t_posicion coordenadas) {
	t_entrenador *masCercano;
	t_entrenador *aux;
	int distancia, min = 1000000; //asumo que nunca me van a dar un mapa que me haga superar este numero
	int i, entrenadoresDisponibles = ESTADO_READY->elements_count;
	if (entrenadoresDisponibles == 0) {
		printf(
				"No hay entrenadores disponibles para atender el pedido en este momento\n");
		return NULL;
	}
	for (i = 0; i < entrenadoresDisponibles; i++) {
		aux = (t_entrenador*) list_get(ESTADO_READY, i);
		distancia = distanciaHasta(aux->posicion, coordenadas);

		if (distancia < min) {
			min = distancia;
			masCercano = aux;
		}
	}

	return masCercano;
}

int hallarIndice(t_entrenador *entrenador, t_list *lista) {
	t_list *aux = list_duplicate(lista);
	int indice = 0;
	int hallado = 0;
	if (!list_is_empty(lista)) {
		while (aux->head != NULL && aux->head->data != (void*) entrenador) {
			aux->head = aux->head->next;
			indice++;
			if (aux->head->data == (void*) entrenador)
				hallado = 1;
		}
		if (aux->head->data == (void*) entrenador)
			hallado = 1;
		if (hallado == 1)
			return indice;
		else
			return -1;

	} else
		return -1;
}
int puedeSeguirAtrapando(t_entrenador *entrenador) {
	return entrenador->pokemons->elements_count
			< entrenador->objetivos->elements_count;
}

//int cumplioObjetivo(t_entrenador *entrenador){
//	return list_all_satisfy(entrenador->pokemons,estaEn(entrenador->objetivos));
//}

int puedeSeguir(t_entrenador *entrenador) {
	return !(cumplioObjetivo(entrenador));
}
//Todo
int tienenConflicto(t_entrenador *entrenador1, t_entrenador *entrenador2) {
	int booleano = 0;

	return booleano;
}

//Todo
//t_entrenador *buscarInvolucrado(t_entrenador *entrenador) {
//	t_list *aux = list_duplicate(ESTADO_BLOCKED);
//	t_entrenador *involucrado;
//
//	while(aux->head!=NULL)
//	{
//		t_entrenador *busqueda;
//		if(busqueda->indice != entrenador->indice && tienenConflicto(busqueda,entrenador)){
//			//Tratar el deadlock.
//		}
//		aux->head = aux->head->next;
//	}
//return involucrado;
//}

//t_entrenador *buscarInvolucrado(t_entrenador *desbloquear,
//		t_list *entrenadoresBloqueados) {
//	t_entrenador* involucrado;
//	t_list *aux = list_duplicate(entrenadoresBloqueados);
//	t_list *auxPok = list_duplicate(desbloquear->pokemons);
//
//	while (aux->head != NULL) {
//		involucrado = (t_entrenador*) aux->head->data;
//		while (auxPok->head != NULL) {
//			if (estaEn(involucrado->objetivos, auxPok->head->data)) {
//				return involucrado;
//			}
//			auxPok->head = auxPok->head->next;
//		}
//		aux->head = aux->head->next;
//	}
//	if(aux->head == NULL)
//	return NULL;
//	else
//	return (t_entrenador*)aux->head->data;
//}

t_entrenador *buscarInvolucrado(t_entrenador *desbloquear,
		t_list *entrenadoresBloqueados) {
	t_entrenador* involucrado;
	t_list *aux = list_duplicate(entrenadoresBloqueados);


	while (aux->head != NULL) {
		t_list *auxPok = list_duplicate(desbloquear->objetivos);
		involucrado = (t_entrenador*) aux->head->data;
		while (auxPok->head != NULL) {
			if (estaEn(involucrado->pokemons, auxPok->head->data)) {
				return involucrado;
			}
			auxPok->head = auxPok->head->next;
		}
		aux->head = aux->head->next;
	}


	return NULL;
}

void intercambiar(t_entrenador* entrenador1, t_entrenador *entrenador2,
		char* pokemon1, char *pokemon2) {

	printf(
			"El entrenador %d le dara un %s al entrenador %d a cambio de un %s\n",
			entrenador1->indice, pokemon1, entrenador2->indice, pokemon2);

	int indice;
	indice = buscarIndicePokemon((void*) pokemon1, entrenador1->pokemons);
	list_remove(entrenador1->pokemons, indice);
	list_add(entrenador2->pokemons, (void*) pokemon1);

	indice = buscarIndicePokemon((void*) pokemon2, entrenador2->pokemons);
	list_remove(entrenador2->pokemons, indice);
	list_add(entrenador1->pokemons, (void*) pokemon2);

	sleep(teamConf->RETARDO_CICLO_CPU * 5);

	ciclosDeCpuTotales += 5;
	ciclosPorEntrenador[entrenador1->indice] += 5;
	//ciclosPorEntrenador[entrenador2->indice] += 5;

	log_info(logEntrega,
			"Se ha solucionado el deadlock entre entrenador %d y entrenador %d",
			entrenador1->indice, entrenador2->indice);
	deadlocksResueltos++;

	if (cumplioObjetivo(entrenador1)) {
		entrenador1->estado = EXIT;
		list_add(ESTADO_EXIT, (void*) entrenador1);
		log_info(logEntrega,
				"Se cambia entrenador %d a la cola EXIT porque cumplio su objetivo",
				entrenador1->indice);
		terminarSiPuedo();
	} else {
		list_add(ESTADO_BLOCKED, (void*) entrenador1);
		log_info(logEntrega,
				"Se cambia entrenador %d a la cola BLOCKED porque tiene tantos pokemons como la cantidad que necesita",
				entrenador1->indice);
		entrenador1->flagDeadlock = 0;
	}

	if (cumplioObjetivo(entrenador2)) {
		entrenador2->estado = EXIT;
		list_add(ESTADO_EXIT, (void*) entrenador2);
		log_info(logEntrega,
				"Se cambia entrenador %d a la cola EXIT porque cumplio su objetivo",
				entrenador2->indice);
		terminarSiPuedo();
	} else {
		list_add(ESTADO_BLOCKED, (void*) entrenador2);
		log_info(logEntrega,
				"Se cambia entrenador %d a la cola BLOCKED porque tiene tantos pokemons como la cantidad que necesita",
				entrenador2->indice);
		entrenador2->flagDeadlock =0;
	}

}

t_list *filterNoNecesita(t_entrenador *entrenador, t_list *pokemons) {
	t_list *filtrada = list_create();
	t_list *aux = list_duplicate(pokemons);

	while (aux->head != NULL) {
		if (!estaEn(entrenador->objetivos, aux->head->data)) {
			list_add(filtrada, aux->head->data);
		}
		aux->head = aux->head->next;
	}
	return filtrada;
}

char *pokemonEnConflicto(t_entrenador *e1, t_entrenador *e2) {
	printf("Entre a buscarconflico\n");
	t_list *auxAtrapar = list_duplicate(e2->pokemons);

	while (auxAtrapar->head != NULL) {
		if (estaEn(e1->objetivos, auxAtrapar->head->data)) {
			return (char*) auxAtrapar->head->data;
		}
		auxAtrapar->head = auxAtrapar->head->next;
	}

	printf("Salgo de buscarconflicto\n");

	if (auxAtrapar->head != NULL) {
		return (char*) auxAtrapar->head->data;
	} else {

		//Si ninguno le sirve, le devuelve cualquiera.
		t_list *noNecesita = filterNoNecesita(e2, e2->pokemons);
		return (char*) list_get(noNecesita, 0);
	}
}

void *deteccionDeDealock(){
	t_list *aux = list_filter(ESTADO_BLOCKED,puedeSeguir);
	int indice;
	log_info(logEntrega,"Se ha iniciado el algoritmo de detección de deadlocks");
	while(!estanTodosEnExit()){
		//t_list *aux = list_filter(ESTADO_BLOCKED,puedeSeguir);

		if(ESTADO_BLOCKED->elements_count>0){
		t_entrenador *desbloquear = list_remove(ESTADO_BLOCKED,0);
		//printf("Buscando involucrado con %d\n",desbloquear->indice);
			if(desbloquear->flagDeadlock == 0){
		t_entrenador *involucrado = buscarInvolucrado(desbloquear,ESTADO_BLOCKED);


			if(involucrado != NULL && (involucrado->flagDeadlock == 0)){
				//indice = hallarIndice(desbloquear,ESTADO_BLOCKED);
				//list_remove(ESTADO_BLOCKED,indice);
				//indice = hallarIndice(involucrado,aux);
				//list_remove(aux,indice);
				indice = hallarIndice(involucrado,ESTADO_BLOCKED);
				list_remove(ESTADO_BLOCKED,indice);
				log_debug(logger,"Los entrenadores %d y %d estan en deadlock entre si",desbloquear->indice,involucrado->indice);
				log_info(logEntrega,"Se detecto deadlock entre entrenador %d y entrenador %d",desbloquear->indice,involucrado->indice);
				deadlocksTotales++;
				t_deadlock *deadlock = malloc (sizeof(t_deadlock));
				desbloquear->flagDeadlock =1;
				involucrado->flagDeadlock =1;
				deadlock->desbloquear = desbloquear;
				deadlock->involucrado = involucrado;
				pthread_mutex_lock(&mutexDeadlock);
				list_add(procesosEnDeadlock,(void*)deadlock);
				pthread_mutex_unlock(&mutexDeadlock);
				sem_post(&counterDeadlock);
				log_error(logger,"Se ha agregado un deadlock a la cola");
			}
			else{
				//printf("No encontre involucrado\n");
				list_add(ESTADO_BLOCKED, (void*) desbloquear);
				if(involucrado != NULL)
					list_add(ESTADO_BLOCKED,involucrado);
			}
			}
			else{
				//printf("Este proceso esta en proceso de deadlock\n");
				list_add(ESTADO_BLOCKED,desbloquear);

			}
	//aux = list_filter(ESTADO_BLOCKED,puedeSeguir);

	}
	else
		{
			return NULL;
			//printf("Banca un toque que no tenemos elementos en la lista flaco\n");
		}
	}

	return NULL;
}

void *tratarDeadlock(void* arg){
	t_deadlock *deadlock = (t_deadlock*)arg;
	printf("Rompision\n");
	t_entrenador *desbloquear = deadlock->desbloquear;
	t_entrenador *involucrado = deadlock->involucrado;
	printf("Rompision\n");
	int indice;
	desbloquear->estado = EXEC;
	//indice = hallarIndice(desbloquear, ESTADO_BLOCKED);
	//list_remove(ESTADO_BLOCKED, indice);
	ESTADO_EXEC = desbloquear;
	printf("Rompision\n");
	log_info(logEntrega,
			"Se cambia al entrenador %d de la COLA_BLOCKED a COLA_EXEC para intercambiar un pokemon",
			desbloquear->indice);

	administrativo[desbloquear->indice].quantum = teamConf->QUANTUM;
	moverEntrenador(desbloquear, involucrado->posicion);

	printf("Buscando pokemons del intercambio\n");
	char* pokemon1 = pokemonEnConflicto(desbloquear, involucrado);
	char *pokemon2 = pokemonEnConflicto(involucrado, desbloquear);

	printf("Los entrenadores %d y %d van a intercambiar %s - %s \n",
			desbloquear->indice, involucrado->indice, pokemon1,
			pokemon2);

	intercambiar(desbloquear, involucrado, pokemon2, pokemon1);

return NULL;
}

void *tratarDeadlocks(){
	pthread_t hiloDeadlock[100];
	int i=0;
	while(!estanTodosEnExit()){
	sem_wait(&counterDeadlock);
	printf("Estoy por sacar deadlock de la cola.\n");
	pthread_mutex_lock(&mutexDeadlock);
	t_deadlock *deadlock = (t_deadlock*)list_remove(procesosEnDeadlock,0);
	pthread_mutex_unlock(&mutexDeadlock);
	printf("Saque deadlock de la cola\n");
	log_debug(logger,"Se resolvera deadlock entre enrenadores %d y %d",deadlock->desbloquear->indice,deadlock->involucrado->indice);
	printf("Creo hilo para tratar deadlock\n");
	pthread_create(&hiloDeadlock[i],NULL,tratarDeadlock,(void*)deadlock);
	printf("Cree hilo para tratar deadlock\n");
	i++;
	}
	int j;
	for(j=0;j<i;j++){
		pthread_join(hiloDeadlock[j],NULL);
	}

return NULL;
}

void tratamientoDeDeadlocks(){
	pthread_t deteccion,tratamiento;
	pthread_create(&tratamiento,NULL,tratarDeadlocks,NULL);



	pthread_create(&deteccion,NULL,deteccionDeDealock,NULL);


	pthread_join(deteccion,NULL);



	pthread_join(tratamiento,NULL);
}

//void tratamientoDeDeadlocks() {
//	t_list *aux = list_filter(ESTADO_BLOCKED, puedeSeguir);
//	int indice;
//
//	log_error(logger, "HAY DEADLOCK");
//	while (aux->elements_count > 1) {
//		log_info(logEntrega,
//				"Se ha iniciado el algoritmo de deteccion de deadlocks");
//		printf("Hay %d entrenadores en deadlock\n", aux->elements_count);
//
//		t_entrenador *desbloquear = list_remove(aux, 0);
//		printf("Saque al entrenador %d para resolver el deadlock\n",
//				desbloquear->indice);
//
//		t_entrenador *involucrado = buscarInvolucrado(desbloquear, aux);
//		if (involucrado != NULL) {
//			log_debug(logger,
//					"Los entrenadores %d y %d estan en deadlock entre si\n",
//					desbloquear->indice, involucrado->indice);
//			log_info(logEntrega,
//					"Se detecto deadlock entre entrenador %d y entrenador %d",
//					desbloquear->indice, involucrado->indice);
//			deadlocksTotales++;
//
//			desbloquear->estado = EXEC;
//			indice = hallarIndice(desbloquear, ESTADO_BLOCKED);
//			list_remove(ESTADO_BLOCKED, indice);
//			ESTADO_EXEC = desbloquear;
//			log_info(logEntrega,
//					"Se cambia al entrenador %d de la COLA_BLOCKED a COLA_EXEC para intercambiar un pokemon",
//					desbloquear->indice);
//
//			administrativo[desbloquear->indice].quantum = 1000;
//			moverEntrenador(desbloquear, involucrado->posicion);
//
//			printf("Buscando pokemons del intercambio\n");
//			char* pokemon1 = pokemonEnConflicto(desbloquear, involucrado);
//			char *pokemon2 = pokemonEnConflicto(involucrado, desbloquear);
//
//			printf("Los entrenadores %d y %d van a intercambiar %s - %s \n",
//					desbloquear->indice, involucrado->indice, pokemon1,
//					pokemon2);
//
//			intercambiar(desbloquear, involucrado, pokemon2, pokemon1);
//		} else {
//			list_add(aux, (void*) desbloquear);
//		}
//
//	}
//	return;
//}

void terminarSiPuedo() {
	if (estanTodosEnExit()) {
		log_debug(logger,
				"El team %s terminara porque cumplio todos sus objetivos\n",
				teamConf->NOMBRE_PROCESO);
		log_info(logEntrega,
				"El team %s termina porque cumplio todos sus objetivos. ",
				teamConf->NOMBRE_PROCESO);
		log_info(logEntrega,
				"Se han detectado un total de %d deadlocks, resueltos: %d",
				deadlocksTotales, deadlocksResueltos);

		log_debug(logger,
				"Se han detectado un total de %d deadlocks, resueltos: %d",
				deadlocksTotales, deadlocksResueltos);
		log_info(logEntrega, "Ciclos de CPU totales: %d", ciclosDeCpuTotales);
		int i = 0;
		for (i = 0; i < cantidadEntrenadores; i++) {
			log_info(logEntrega, "El entrenador %d realizo %d ciclos de CPU", i,
					ciclosPorEntrenador[i]);
		}
		log_info(logEntrega,"Con el algoritmo %s se realizaron un total de %d cambios de contexto.",teamConf->ALGORITMO_PLANIFICACION,cambiosDeContexto);
		exit(0);
	}
}

void* planificarEntrenadores() { //aca vemos que entrenador esta en ready y mas cerca del pokemon
//agarramos el pokemon o lo que sea que el entrenador tenga que hacer y enviamos un mensaje al broker avisando.

	int i;
//	int socketBroker = crearConexion(teamConf->IP_BROKER,
//			teamConf->PUERTO_BROKER, teamConf->TIEMPO_RECONEXION);

	printf("Estoy por crear los  %d hilos de entrenador\n",
			cantidadEntrenadores);
	for (i = 0; i < cantidadEntrenadores; i++) {
		printf("i vale %d\n", i);
		t_entrenador *entrenador = (t_entrenador*) list_get(ESTADO_READY, i);
		if (pthread_create(&threads_entreanadores[i], NULL, manejarEntrenador,
				(void*) entrenador) < 0) {
			printf("No se pduo crear el hilo\n");
		} else {
			printf("Handler asignado para entrenador [%d]\n", i);
		}

	}

	pthread_mutex_lock(&mutexPlani);
	while (!estanTodosEnExit()) {
		if (hayEntrenadoresDisponibles()) {

			t_paquete *appeared = malloc(sizeof(t_paquete));

			printf("Esperando por la aparición de un pokemon\n");
			if (estanTodosEnExit()) {
				printf("El team %s cumplio su objetivo\n",
						teamConf->NOMBRE_PROCESO);
				return NULL;
			} else {
				sem_wait(&pokemonsEnLista);
				pthread_mutex_lock(&mutexListaPokemons);
				appeared = (t_paquete*) queue_pop(appearedPokemon);
				pthread_mutex_unlock(&mutexListaPokemons);

				printf(
						"Se detecto un %s en %d,%d. Se planificara el team para atraparlo\n",
						appeared->buffer->nombrePokemon, appeared->buffer->posX,
						appeared->buffer->posY);

				t_posicion posicionPokemon;
				posicionPokemon.x = appeared->buffer->posX;
				posicionPokemon.y = appeared->buffer->posY;
				t_entrenador *buscador = buscarMasCercano(posicionPokemon);
				char *nombrePokemon = string_duplicate(
						appeared->buffer->nombrePokemon);

				if (buscador != NULL) {
					printf("El entrenador mas cercano es %d en %d,%d\n",
							buscador->indice, buscador->posicion.x,
							buscador->posicion.y);
//		i=hallarIndice(buscador,ESTADO_READY);
//		printf("Estoy por sacar de ready indice %d\n",i);
//		list_remove(ESTADO_READY,i);
//		printf("Estoy por agregar a exec\n");
//		ESTADO_EXEC=buscador;
//		buscador->estado=EXEC;
					pthread_mutex_unlock(&ejecuta[buscador->indice]);
					administrativo[buscador->indice].nombrePokemon =
							string_duplicate(nombrePokemon);
					administrativo[buscador->indice].posX = posicionPokemon.x;
					administrativo[buscador->indice].posY = posicionPokemon.y;
					//administrativo[buscador->indice].socket = socketBroker;
					//moverEntrenador(buscador,posicionPokemon);

					//printf("Por enviar mensaje catch\n");
					//enviarMensajeBrokerCatch(nombrePokemon,posicionPokemon.x,posicionPokemon.y,socketBroker);
					//printf("Envie mensaje catch\n");
					//ESTADO_EXEC=NULL;
					//list_add(ESTADO_READY,(void*)buscador);

				}

//		for (j = 0; j < cantidadEntrenadores; j++) {
//			//sleep(2);
//			pthread_mutex_unlock(&ejecuta[j]);
//			printf("Esta en ejec el proceso %d\n",ESTADO_EXEC->pid);
//		}
			}
		} else {

			log_info(logEntrega, "Se ha detectado una situacion de deadlock");
			tratamientoDeDeadlocks();
			//sleep(100);
			//Todo

		}
	}
	printf("Todos los procesos estan en EXIT\n");
	exit(0);
	return NULL;

}

void *ejecutor() {
	int quantum = teamConf->QUANTUM;
	while (!estanTodosEnExit()) {
		sem_wait(&counterProximosEjecutar);
		pthread_mutex_lock(&mutexProximos);
		t_entrenador *proximo = (t_entrenador*) queue_pop(proximosEjecutar);
		pthread_mutex_unlock(&mutexProximos);
		administrativo[proximo->indice].quantum = quantum;
		//pthread_mutex_unlock(&cpu);
		pthread_mutex_unlock(&ejecuta[proximo->indice]);
	}

	pthread_exit(NULL);

	return NULL;
}

//Todo
void *planificarEntrenadoresRR() {
	int quantum = teamConf->QUANTUM;
	int i, j;
	pthread_t entrenadorThread;
	printf("Estoy por crear los  %d hilos de entrenador\n",
			cantidadEntrenadores);
	for (i = 0; i < cantidadEntrenadores; i++) {
		printf("i vale %d\n", i);
		t_entrenador *entrenador = (t_entrenador*) list_get(ESTADO_READY, i);
		if (pthread_create(&threads_entreanadores[i], NULL, manejarEntrenador,
				(void*) entrenador) < 0) {
			printf("No se pduo crear el hilo\n");
		} else {
			printf("Handler asignado para entrenador [%d]\n", i);
		}

	}

	pthread_t tEjecutor;
	pthread_create(&tEjecutor, NULL, ejecutor, NULL);

	pthread_mutex_lock(&mutexPlani);
	while (!estanTodosEnExit()) {
		if (hayEntrenadoresDisponibles()) {

			t_paquete *appeared = malloc(sizeof(t_paquete));

			printf("Esperando por la aparición de un pokemon\n");
			if (estanTodosEnExit()) {
				printf("El team %s cumplio su objetivo\n",
						teamConf->NOMBRE_PROCESO);
				return NULL;
			} else {
				sem_wait(&pokemonsEnLista);
				pthread_mutex_lock(&mutexListaPokemons);
				appeared = (t_paquete*) queue_pop(appearedPokemon);
				pthread_mutex_unlock(&mutexListaPokemons);

				printf(
						"Se detecto un %s en %d,%d. Se planificara el team para atraparlo\n",
						appeared->buffer->nombrePokemon, appeared->buffer->posX,
						appeared->buffer->posY);

				t_posicion posicionPokemon;
				posicionPokemon.x = appeared->buffer->posX;
				posicionPokemon.y = appeared->buffer->posY;
				t_entrenador *buscador = buscarMasCercano(posicionPokemon);
				char *nombrePokemon = string_duplicate(
						appeared->buffer->nombrePokemon);

				if (buscador != NULL) {
					printf("El entrenador mas cercano es %d en %d,%d\n",
							buscador->indice, buscador->posicion.x,
							buscador->posicion.y);

					administrativo[buscador->indice].quantum = quantum;
					administrativo[buscador->indice].nombrePokemon =
							string_duplicate(nombrePokemon);
					administrativo[buscador->indice].posX = posicionPokemon.x;
					administrativo[buscador->indice].posY = posicionPokemon.y;

					pthread_mutex_lock(&mutexProximos);
					queue_push(proximosEjecutar, (void*) buscador);
					pthread_mutex_unlock(&mutexProximos);
					sem_post(&counterProximosEjecutar);

				}

			}
		} else {

			log_info(logEntrega, "Se ha detectado una situacion de deadlock");
			tratamientoDeDeadlocks();
			//Todo

		}
	}
	printf("Todos los procesos estan en EXIT\n");
	exit(0);
	return NULL;

}

//Todo
void *planificarEntrenadoresSJF() {
return NULL;
}

void inicializarLoggerTeam() {
	logger = log_create("team.log", "TEAM", 1, LOG_LEVEL_TRACE);
	if (logger == NULL) {
		perror("No se pudo inicializar el logger\n");
		//exit(1);
	}

	return;
}

void inicializarLoggerEntregable() {
	printf("Voy a crear un logger %s\n", teamConf->LOG_FILE);

	logEntrega = log_create(teamConf->LOG_FILE, "TEAM1", 1, LOG_LEVEL_TRACE);
	if (logEntrega == NULL) {
		perror("No se pudo inicializar el logger para la entrega\n");
	}
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

void mostrarInt(void *elemento) {
	printf("%d\n", (int) elemento);
}

void mostrarListaInt(t_list *lista) {
	t_list *aux = list_duplicate(lista);

	if (list_is_empty(aux)) {
		printf("La lista esta vacia, no hay elementos para mostrar.\n");
	} else {
		while (aux->head != NULL) {
			mostrarInt(aux->head->data);
			aux->head = aux->head->next;
		}
	}
	list_destroy(aux);
}

void mostrarChar(void *elemento) {
//log_info(logger,"%s",(char*)elemento);
	printf("%s\n", (char*) elemento);
}

void mostrarListaChar(t_list *lista) {
	t_list *aux = list_duplicate(lista);

	while (aux->head != NULL) {
		mostrarChar(aux->head->data);
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

int stringVacio(void *data) {

	return strcmp((char*) data, "") != 0;
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

	int cantidadPokemons = pokemonEntrenadores->elements_count;
	int cantidadObjetivos = objetivoEntrenadores->elements_count;
	for (i = 0; i < cantidadEntrenadores; i++) {
		printf("Print de debug1\n");
		t_entrenador *nuevoEntrenador = malloc(sizeof(t_entrenador));
		nuevoEntrenador->posicion = separarPosiciones(auxPos->head->data);
		//entrenadores[i].posicion = separarPosiciones(auxPos->head->data);

		//posiciones[i]=separarPosiciones(auxPos->head->data);
		printf("Print de debug2\n");
		if ((i + 1) < cantidadEntrenadores) {
			limpieza = auxPos->head;
			auxPos->head = auxPos->head->next;
			auxPos->elements_count--;
			free(limpieza);
		}
		printf("Print de debug3\n");
		if (auxPok->head != NULL) {
			nuevoEntrenador->pokemons = list_duplicate(
					separarPokemons(auxPok->head->data, 0));
		} else {
			nuevoEntrenador->pokemons = list_create();
		}
//		entrenadores[i].pokemons = list_duplicate(
//				separarPokemons(auxPok->head->data, 0));
		printf("Print de debug4\n");
		if ((i + 1) < cantidadPokemons) {
			printf("Debugcito1\n");
			limpieza = auxPok->head;
			printf("Debugcito2\n");
			auxPok->head = auxPok->head->next;
			printf("Debugcito3\n");
			auxPok->elements_count--;
			printf("Debugcito4\n");
			free(limpieza);
		} else {
			printf("Entre al else\n");
			auxPok->head = NULL;
		}
		printf("Print de debug5\n");
		if (auxObj->head != NULL) {
			nuevoEntrenador->objetivos = list_duplicate(
					separarPokemons(auxObj->head->data, 1));
		} else {
			printf("Entre al else\n");
			nuevoEntrenador->objetivos = list_create();
		}
//		entrenadores[i].objetivos = list_duplicate(
//				separarPokemons(auxObj->head->data, 1));
		printf("Print de debug6\n");
		if ((i + 1) < cantidadObjetivos) {
			limpieza = auxObj->head;
			auxObj->head = auxObj->head->next;
			auxObj->elements_count--;
			free(limpieza);
		} else {
			auxObj->head = NULL;
		}

		printf("Print de debug7\n");

		printf("Entrenador %d, está en X=%d e Y=%d.\n", i + 1,
				nuevoEntrenador->posicion.x, nuevoEntrenador->posicion.y);
		printf("Los pokemons del entrenador %d son:\n", i + 1);
		mostrarListaChar(nuevoEntrenador->pokemons);
		printf("Los objetivos del entrenador %d son:\n", i + 1);
		//log_info(logger,"Los objetivos del entrenador %d son:",i+1);
		mostrarListaChar(nuevoEntrenador->objetivos);

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
		nuevoEntrenador->flagDeadlock = 0;

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
	mostrarListaChar(pokemonEntrenadores);

	teamConf->POSICIONES_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"POSICIONES_ENTRENADORES");
	posicionEntrenadores = list_create();
	splitList(teamConf->POSICIONES_ENTRENADORES, posicionEntrenadores);
	mostrarListaChar(posicionEntrenadores);

	teamConf->OBJETIVOS_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"OBJETIVOS_ENTRENADORES");
	objetivoEntrenadores = list_create();
	splitList(teamConf->OBJETIVOS_ENTRENADORES, objetivoEntrenadores);
	mostrarListaChar(objetivoEntrenadores);

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

	teamConf->ALPHA = config_get_double_value(TEAMTConfig, "ALPHA");
	log_info(logger, "Lei ALPHA=%f de la configuracion\n", teamConf->ALPHA);

	teamConf->PUERTO_BROKER = config_get_int_value(TEAMTConfig,
			"PUERTO_BROKER");
	log_info(logger, "Lei PUERTO_BROKER=%d de la configuracion\n",
			teamConf->PUERTO_BROKER);

	teamConf->LOG_FILE = string_duplicate(
			config_get_string_value(TEAMTConfig, "LOG_FILE"));
	log_info(logger, "Lei LOG_FILE=%s de la configuracion\n",
			teamConf->LOG_FILE);

	teamConf->NOMBRE_PROCESO = string_duplicate(
			config_get_string_value(TEAMTConfig, "NOMBRE_PROCESO"));

	log_info(logger, "Lei NOMBRE_PROCESO=%s de la configuracion\n",
			teamConf->NOMBRE_PROCESO);

	cantidadEntrenadores = posicionEntrenadores->elements_count;
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

	ciclosPorEntrenador = malloc(sizeof(int) * cantidadEntrenadores);
	int i;
	for (i = 0; i < cantidadEntrenadores; i++) {
		ciclosPorEntrenador[i] = 0;
	}
	cambiosDeContexto=0;

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

void* pedirPokemons(void *arg) {
//int socket = *(int*) socketBroker;
	t_list* pokemonGet = sinRepetidos(objetivoGlobal);
//	printf("El objetivo global del TEAM es: \n");
//	mostrarLista(objetivoGlobal);
//	printf("Sin repetidos es: \n");
//	mostrarLista(pokemonGet);
	printf("Se pediran los siguientes pokemons: \n");
	mostrarListaChar(pokemonGet);

	void _realizarGet(void* elemento) {
		int socketEnviar = crearConexion(teamConf->IP_BROKER,
				teamConf->PUERTO_BROKER, teamConf->TIEMPO_RECONEXION);
		char *pokemon = (char*) elemento;
		enviarMensajeBrokerGet(pokemon, socketEnviar);
		t_paquete *idMensaje = malloc(sizeof(t_paquete));
		idMensaje = recibirMensaje(socketEnviar);
		printf("Voy a agregar a la lista de id: %d\n",
				idMensaje->buffer->idMensaje);
		list_add(listaIdGet, (void*) idMensaje->buffer->idMensaje);
		liberarConexion(socketEnviar);

		//sleep(1);
	}

	list_iterate(pokemonGet, _realizarGet);

//mostrarListaInt(listaId);

//liberarConexion(socket);
	pthread_exit(NULL);
	return NULL;
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
void *administrarMensajes(void *arg) {
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
			if ((pthread_create(&threadId[contadorConexiones], NULL,
					recvMensajes, (void*) &socketDelCliente[contadorConexiones]))
					< 0) {
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

void iniciarListasColas() {
	printf("Creando listas de ejecucion\n");
	ESTADO_BLOCKED = list_create();
	ESTADO_EXEC = malloc(sizeof(t_entrenador));
	ESTADO_EXIT = list_create();
	ESTADO_READY = list_create();
	bandejaDeMensajes = queue_create();
	appearedPokemon = queue_create();
	listaIdGet = list_create();
	listaIdCatch = list_create();
	proximosEjecutar = queue_create();
	procesosEnDeadlock = list_create();
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
