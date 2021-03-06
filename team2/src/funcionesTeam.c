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


/*Esta parte es para contar deadlocks, todavía no se si me sirve*/
//bool estaEnInt(t_list* lista, void *elemento) {
//
//	//printf("Entre en estaEn\n");
//
//	t_list *aux = list_duplicate(lista);
//	bool flag = false;
////	t_link_element *limpieza;
//	while (aux->head != NULL) {
//		//printf("Comparando %s y %s\n", (char*) aux->head->data,
//			//	(char*) elemento);
//
//		if (((int)aux->head->data)==((int)elemento)) {
//			flag = true;
//		}
//		//limpieza = aux->head;
//		aux->head = aux->head->next;
//		//free(limpieza);
//	}
//	list_destroy(aux);
//	return flag;
//
//}
//
//
//bool hayRepetidos(t_list *lista){
//	bool resultado = false;
//	t_list *resultada = list_create();
//	t_list *aux = list_duplicate(lista);
//		while(aux->head != NULL){
//			if(estaEnInt(resultada,aux->head->data))
//				return true;
//			list_add(resultada,aux->head->data);
//		aux->head = aux->head->next;
//		}
//	return resultado;
//}
/*Hasta aca*/



void mostrarPokemon(void *arg){
	printf("%s ",(char*)arg);
}
void mostrarPokemons(t_list *pokemons){
	if(!list_is_empty(pokemons))
	list_iterate(pokemons,mostrarPokemon);
	else
	printf("-");
}
void mostrarProceso(void *arg){
	t_entrenador *entrenador = (t_entrenador*)arg;
	printf("{");
	printf("Entrenador %d (X,Y)=(%d,%d), Pokemons:",entrenador->indice,entrenador->posicion.x,entrenador->posicion.y);
	printf("[");
	mostrarPokemons(entrenador->pokemons);

	printf("], Objetivos: ");
	printf("[");
	mostrarPokemons(entrenador->objetivos);
	printf("]");
	printf("}\n");
}
void mostrarCola(t_list *cola){
	if(list_is_empty(cola)){
		log_info(logger,"{-}");
	}
	else
	list_iterate(cola,mostrarProceso);
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
	pthread_mutex_init(&mutexDeadlock, NULL);
	pthread_mutex_init(&mutexBlocked, NULL);
	pthread_mutex_init(&mutexExit, NULL);
	pthread_mutex_init(&mutexNew, NULL);
	pthread_mutex_init(&mutexReady, NULL);
	pthread_mutex_init(&terminaTratamiento, NULL);
	pthread_mutex_init(&mutexCambiosDeContexto,NULL);
	pthread_mutex_init(&mutexSegundosTotales,NULL);
	pthread_mutex_init(&mutexCiclosTotales,NULL);
	pthread_mutex_init(&mutexDeteccion,NULL);
	pthread_mutex_init(&mutexListaExtra,NULL);
	pthread_mutex_init(&mutexIdGet,NULL);
	sem_init(&pokemonsEnListaExtra,1,0);
	sem_init(&contadorBandeja, 1, 0);
	sem_init(&pokemonsEnLista, 1, 0);
	sem_init(&counterProximosEjecutar, 1, 0);
	sem_init(&counterDeadlock, 1, 0);
	sem_init(&counterBlocked,1,0);
	sem_init(&counterNew,1,0);
	sem_init(&counterReady,1,0);
	//sem_init(&entrenadoresDisponibles,1,0);
	return;
}


bool flagDeadlockApagado(void *arg) {
	t_entrenador *entrenador = (t_entrenador*) arg;
	return entrenador->flagDeadlock == 0;
}

int sonIguales(t_posicion pos1, t_posicion pos2) {
	return (pos1.x == pos2.x) && (pos1.y == pos2.y);
}
int sonDistintas(t_posicion pos1, t_posicion pos2) {
	return !sonIguales(pos1, pos2);
}

void moverEntrenador(t_entrenador *entrenador, t_posicion coordenadas) {
	t_posicion guardar = entrenador->posicion;
	//printf("Moviento entrenador %d a la posicion %d,%d\n", entrenador->indice,
	//		coordenadas.x, coordenadas.y);
	if (sonDistintas(entrenador->posicion, coordenadas)) {

		while (entrenador->posicion.x != coordenadas.x) {
			//printf("Estoy en: %d,%d\n", entrenador->posicion.x,
			//		entrenador->posicion.y);

			if (strcmp(teamConf->ALGORITMO_PLANIFICACION, "RR") == 0) {
				if (administrativo[entrenador->indice].quantum < 1) {

					//log_debug(logger, "FIN DE QUANTUM");
					//printf("--------FIN DE QUANTUM--------\n");


					log_info(logEntrega,
							"Se cambia entrenador %d a la cola READY por fin de quantum",
							entrenador->indice);
					entrenador->estado = READY;
					pthread_mutex_lock(&mutexReady);
					list_add(ESTADO_READY, (void*) entrenador);
					pthread_mutex_unlock(&mutexReady);

					pthread_mutex_lock(&mutexProximos);
					//printf("Agregando entrenador a proximos\n");
					//queue_push(proximosEjecutar, (void*) entrenador);
					list_add(proximosEjecutar, (void*) entrenador);
					sem_post(&counterProximosEjecutar);
					pthread_mutex_unlock(&mutexProximos);
					pthread_mutex_unlock(&cpu);

				//	printf("Bloqueado en el mutex entrenador\n");
					pthread_mutex_lock(&ejecuta[entrenador->indice]);
					//printf("Bloqueado en el cpu\n");
					//pthread_mutex_lock(&cpu);

					log_info(logEntrega,
							"Se cambia entrenador %d a EXEC porque se le dio quantum",
							entrenador->indice);
					pthread_mutex_lock(&mutexReady);
					int i = hallarIndice(entrenador, ESTADO_READY);
					//if (i != -1)
						list_remove(ESTADO_READY, i);
					//else
						//log_error(logger,
							//	"El indice es -1, no lo pude encontrar!");
					pthread_mutex_unlock(&mutexReady);
					ESTADO_EXEC = entrenador;
					entrenador->estado = EXEC;
					pthread_mutex_lock(&mutexCambiosDeContexto);
					cambiosDeContexto++;
					pthread_mutex_unlock(&mutexCambiosDeContexto);

//					printf("llegue hast aca\n");

				}
			}
			if(strcmp(teamConf->ALGORITMO_PLANIFICACION,"SJFD")==0){
									//double rafagaPendiente; //Esto es un mock
									//Todo
									/*Tengo que fijarme si hay un entrenador con más prioridad --> estimacion de rafaga es menor que el que está ejecutando*/
									double menorRafaga = entrenadorMenorRafaga(proximosEjecutar);
								//	printf("Comparo menorRafaga: %f y rafagaPendiente: %f\n",menorRafaga,entrenador->rafagaPendiente);
									if(menorRafaga<entrenador->rafagaPendiente){
										log_info(logEntrega,"Se cambia al entrenador %d a READY por desalojo",entrenador->indice);
									//	entrenador->disponible=1;
										entrenador->estado = READY;
										pthread_mutex_lock(&mutexReady);
										list_add(ESTADO_READY, (void*) entrenador);
										pthread_mutex_unlock(&mutexReady);

										pthread_mutex_lock(&mutexProximos);

										list_add(proximosEjecutar, (void*) entrenador);
										sem_post(&counterProximosEjecutar);																				pthread_mutex_unlock(&mutexProximos);
										pthread_mutex_unlock(&cpu);

										pthread_mutex_lock(&ejecuta[entrenador->indice]);

									//	pthread_mutex_lock(&cpu);

										log_info(logEntrega,
										"Se cambia entrenador %d a EXEC porque se le dio quantum",
										entrenador->indice);

										pthread_mutex_lock(&mutexReady);
										int i = hallarIndice(entrenador, ESTADO_READY);

									//	if (i != -1)
											list_remove(ESTADO_READY, i);
									//	else
										//	log_error(logger,
											//		"El indice es -1, no lo pude encontrar!");
										pthread_mutex_unlock(&mutexReady);
										ESTADO_EXEC = entrenador;
										entrenador->estado = EXEC;
										pthread_mutex_lock(&mutexCambiosDeContexto);
										cambiosDeContexto++;
										pthread_mutex_unlock(&mutexCambiosDeContexto);


									}
								}
			//printf("Moviendo en X\n");
			if (entrenador->posicion.x < coordenadas.x)
				entrenador->posicion.x++;
			else
				entrenador->posicion.x--;
			sleep(teamConf->RETARDO_CICLO_CPU);
			segundosTotales += teamConf->RETARDO_CICLO_CPU;
			pthread_mutex_lock(&mutexCiclosTotales);
			ciclosDeCpuTotales++;
			pthread_mutex_unlock(&mutexCiclosTotales);
			ciclosPorEntrenador[entrenador->indice]++;
			entrenador->ultimaRafaga++;
			entrenador->rafagaPendiente-=1;

			administrativo[entrenador->indice].quantum--;

			//log_debug(logger, "Log de debug final de X");
		}
		while (entrenador->posicion.y != coordenadas.y) {
		//	printf("Estoy en: %d,%d\n", entrenador->posicion.x,
			//		entrenador->posicion.y);

			if (strcmp(teamConf->ALGORITMO_PLANIFICACION, "RR") == 0) {
				if (administrativo[entrenador->indice].quantum == 0) {

				//	log_debug(logger, "FIN DE QUANTUM");
					//printf("--------FIN DE QUANTUM--------\n");
					pthread_mutex_lock(&mutexProximos);

					log_info(logEntrega,
							"Se cambia entrenador %d a la cola READY por fin de quantum",
							entrenador->indice);
					entrenador->estado = READY;
					pthread_mutex_lock(&mutexReady);
					list_add(ESTADO_READY, (void*) entrenador);

					pthread_mutex_unlock(&mutexReady);

				//	printf("Agregando entrenador a proximos\n");
					//queue_push(proximosEjecutar, (void*) entrenador);
					list_add(proximosEjecutar, (void*) entrenador);
					sem_post(&counterProximosEjecutar);
					pthread_mutex_unlock(&mutexProximos);
					pthread_mutex_unlock(&cpu);

				//	printf("Bloqueado en el mutex entrenador\n");
					pthread_mutex_lock(&ejecuta[entrenador->indice]);

				//	printf("Bloqueado en el cpu\n");
					//pthread_mutex_lock(&cpu);

					log_info(logEntrega,
							"Se cambia entrenador %d a EXEC porque se le dio quantum",
							entrenador->indice);
					pthread_mutex_lock(&mutexReady);
					int i = hallarIndice(entrenador, ESTADO_READY);
					//if (i != -1)
						list_remove(ESTADO_READY, i);
					//else
						//log_error(logger,
							//	"El indice es -1, no lo pude encontrar");
					pthread_mutex_unlock(&mutexReady);
					ESTADO_EXEC = entrenador;
					pthread_mutex_lock(&mutexCambiosDeContexto);
					cambiosDeContexto++;
					pthread_mutex_unlock(&mutexCambiosDeContexto);


				}
			}

			if(strcmp(teamConf->ALGORITMO_PLANIFICACION,"SJFD")==0){
												//double rafagaPendiente; //Esto es un mock
												//Todo
												/*Tengo que fijarme si hay un entrenador con más prioridad --> estimacion de rafaga es menor que el que está ejecutando*/
												double menorRafaga = entrenadorMenorRafaga(proximosEjecutar);
											//	printf("Comparo menorRafaga: %f y rafagaPendiente: %f\n",menorRafaga,entrenador->rafagaPendiente);
												if(menorRafaga<entrenador->rafagaPendiente){
													log_info(logEntrega,"Se cambia al entrenador %d a READY por desalojo",entrenador->indice);
												//	entrenador->disponible=1;
													entrenador->estado = READY;
													pthread_mutex_lock(&mutexReady);
													list_add(ESTADO_READY, (void*) entrenador);
													pthread_mutex_unlock(&mutexReady);

													pthread_mutex_lock(&mutexProximos);

													list_add(proximosEjecutar, (void*) entrenador);
													sem_post(&counterProximosEjecutar);																				pthread_mutex_unlock(&mutexProximos);
													pthread_mutex_unlock(&cpu);

													pthread_mutex_lock(&ejecuta[entrenador->indice]);

												//	pthread_mutex_lock(&cpu);

													log_info(logEntrega,
													"Se cambia entrenador %d a EXEC porque se le dio quantum",
													entrenador->indice);

													pthread_mutex_lock(&mutexReady);
													int i = hallarIndice(entrenador, ESTADO_READY);

													//if (i != -1)
														list_remove(ESTADO_READY, i);
												//	else
													//	log_error(logger,
														//		"El indice es -1, no lo pude encontrar!");
													pthread_mutex_unlock(&mutexReady);
													ESTADO_EXEC = entrenador;
													entrenador->estado = EXEC;
													pthread_mutex_lock(&mutexCambiosDeContexto);
													cambiosDeContexto++;
													pthread_mutex_unlock(&mutexCambiosDeContexto);


												}
											}

		//	printf("Moviendo en Y\n");
			if (entrenador->posicion.y < coordenadas.y)
				entrenador->posicion.y++;
			else
				entrenador->posicion.y--;
			sleep(teamConf->RETARDO_CICLO_CPU);
			segundosTotales += teamConf->RETARDO_CICLO_CPU;
			pthread_mutex_lock(&mutexCiclosTotales);
			ciclosDeCpuTotales++;
			pthread_mutex_unlock(&mutexCiclosTotales);
			ciclosPorEntrenador[entrenador->indice]++;
			entrenador->ultimaRafaga++;
			administrativo[entrenador->indice].quantum--;
			entrenador->rafagaPendiente-=1;


			//log_debug(logger, "Log de debug final de Y");
		}
	}
	log_info(logEntrega, "Se movio el entrenador %d desde %d,%d hasta %d,%d",
			entrenador->indice, guardar.x, guardar.y, coordenadas.x,
			coordenadas.y);
	return;
}

//Todo hacer free
int buscarIndicePokemon(void* data, t_list *lista) {
	t_list *aux = list_duplicate(lista);
	int indice = 0;
	int hallado = 0;

	if (!list_is_empty(aux)) {
		while ((aux->head != NULL)
				&& (strcmp((char*) data, (char*) aux->head->data) != 0)) {
			aux->head = aux->head->next;
			indice++;
		}

		//list_destroy(aux);

		if (aux->head == NULL) {
			list_destroy(aux);
			return -1;
		}

		if (strcmp((char*) data, (char*) aux->head->data) == 0) {
			hallado = 1;
		}
		if (hallado == 1) {
			list_destroy(aux);
			return indice;
		} else {
			list_destroy(aux);
			return -1;
		}

	} else {
		list_destroy(aux);
		return -1;
	}
}
//Todo hacer free
int cumplioObjetivo(t_entrenador *entrenador) {
	t_list *aux1 = list_duplicate(entrenador->objetivos);
	t_list *aux2 = list_duplicate(entrenador->pokemons);

	int i, cantidad = entrenador->objetivos->elements_count;
	if (entrenador->objetivos->elements_count
			== entrenador->pokemons->elements_count) {
		for (i = 0; i < cantidad; i++) {
			if (!estaEn(aux2, aux1->head->data)) {
				list_destroy(aux1);
				list_destroy(aux2);
				return 0;
			}

			list_remove(aux2, buscarIndicePokemon(aux1->head->data, aux2));
			aux1->head = aux1->head->next;
		}
		list_destroy(aux1);
		list_destroy(aux2);
		return 1;

	} else {
		list_destroy(aux1);
		list_destroy(aux2);
		return 0;
	}
}

int hayEntrenadoresDisponibles() {
	//return ESTADO_READY->elements_count > 0 || ESTADO_BLOCKED->elements_count<=(cantidadEntrenadores-ESTADO_EXIT->elements_count);
//	log_debug(logger,"ESTADO BLOCKED:%d, Entrenadores vivos:%d, ESTADO_READY:%d",ESTADO_BLOCKED->elements_count,cantidadEntrenadores-ESTADO_EXIT->elements_count,ESTADO_READY->elements_count);
	//return (ESTADO_BLOCKED->elements_count < (cantidadEntrenadores - ESTADO_EXIT->elements_count)) || ESTADO_READY->elements_count!=0;
	//log_debug(logger,"Dormidos: %d - Ready : %d - Esperando respuesta: %d",dormidos->elements_count,ESTADO_READY->elements_count,esperandoRespuesta->elements_count);

	int resu1 = dormidos->elements_count>0;
	int resu2 = ESTADO_READY->elements_count>0;
	int resu3 = esperandoRespuesta->elements_count>0;
	int resu4 = ESTADO_EXEC != NULL;
	//printf("Resu 1: %d - Resu2: %d - Resu 3: %d\n",resu1,resu2,resu3);

	return resu1 || resu2 || resu3 || resu4;
	//return (ESTADO_BLOCKED->elements_count < (cantidadEntrenadores - ESTADO_EXIT->elements_count)) || ESTADO_READY->elements_count>0;
}

void liberarProceso(t_entrenador *entrenador) {
	list_destroy_and_destroy_elements(entrenador->objetivos, free);
	list_destroy_and_destroy_elements(entrenador->pokemons, free);
	free(entrenador);
}

void mostrarColas(){
				log_info(logPantalla,"Muestro colas");
				log_info(logPantalla,"COLA EXEC");
				if(ESTADO_EXEC != NULL){
				mostrarProceso(ESTADO_EXEC);
				}
				else
				log_info(logPantalla,"{-}\n");
				log_info(logPantalla,"COLA NEW");
				mostrarCola(ESTADO_NEW);
				log_info(logPantalla,"COLA READY");
				mostrarCola(ESTADO_READY);
				log_info(logPantalla,"COLA BLOCKED");
				mostrarCola(ESTADO_BLOCKED);
				log_info(logPantalla,"COLA EXIT");
				mostrarCola(ESTADO_EXIT);
}

//Todo hacer free
void *manejarEntrenador(void *arg) {

//int index=*(int*)arg;

//	printf("Cree hilo para entrenador\n");
	t_entrenador *process = (t_entrenador*) arg;
	//process->pid = process_get_thread_id();
	//log_debug(logger,"Rompo antes de agregar a new");
//	pthread_mutex_lock(&mutexNew);
//	list_add(ESTADO_NEW,(void*)process);
//	pthread_mutex_unlock(&mutexNew);
	//log_info(logEntrega,"Se agrega el entrenador %d a la cola NEW porque se lo esta inicializando",process->indice);
//	log_error(logger,"Rompo aca?");
	administrativo[process->indice].quantum = teamConf->QUANTUM;
	//log_error(logger,"Rompo aca?");
	//mostrarEstado(process->estado);
	//printf("SOY EL HANDLER DE ENTRENADOR %d\n", process->indice);

	//printf("Mi Quantum es de %d\n", administrativo[process->indice].quantum);
	int index;


	while (process->estado != EXIT) {
		process->rafagaPendiente=process->estimacionRafagaActual;

		if (!hayEntrenadoresDisponibles()) {
			pthread_t tTratarDeadlocks;
			pthread_create(&tTratarDeadlocks, NULL,
					(void*) tratamientoDeDeadlocks, NULL);
			pthread_detach(tTratarDeadlocks);

		}
	//	log_debug(logger, "Bloqueado en el mutex de entrenador %d",
		//		process->indice);
		pthread_mutex_lock(&ejecuta[process->indice]);
	//	log_debug(logger, "Esperando por el cpu");
		//pthread_mutex_lock(&cpu);

	//	log_debug(logger, "soy el entrenador %d y mi flag deadlock vale %d",
		//		process->indice, process->flagDeadlock);
		if (process->flagDeadlock == 0) {

			/*Estructuras administrativas del hilo: */
			t_administrativoEntrenador recurso = administrativo[process->indice];
			t_posicion aMoverse;
			aMoverse.x = recurso.posX;
			aMoverse.y = recurso.posY;
			//printf("--------------------INICIO------------------------\n");
			pthread_mutex_lock(&mutexReady);
			index = hallarIndice(process, ESTADO_READY);
			if (index != -1) {

			//	sem_wait(&counterReady);
				list_remove(ESTADO_READY, index);
				pthread_mutex_unlock(&mutexReady);
				log_info(logEntrega,
						"Se cambia entrenador %d a la cola EXEC para atrapar pokemon",
						process->indice);
				ESTADO_EXEC = process;

				process->estado = EXEC;
				pthread_mutex_lock(&mutexCambiosDeContexto);
				cambiosDeContexto++;
				pthread_mutex_unlock(&mutexCambiosDeContexto);

				process->ultimaRafaga = 0;

				moverEntrenador(process, aMoverse);

				/*Funcion para list_remove_by_condition*/
				bool _esPoke(void*arg){
				char *candidato = (char*)arg;
				return strcmp(candidato,recurso.nombrePokemon)==0;
				}

				int socket = crearConexionSinReintento(teamConf->IP_BROKER,
						teamConf->PUERTO_BROKER);
		if(socket >= 0){
				if (strcmp(teamConf->ALGORITMO_PLANIFICACION, "RR") == 0) {
							if (administrativo[process->indice].quantum < 1) {
								//log_debug(logger, "FIN DE QUANTUM");
													//pthread_mutex_lock(&mutexProximos);

													log_info(logEntrega,
															"Se cambia entrenador %d a la cola READY por fin de quantum",
															process->indice);
													process->estado = READY;
													pthread_mutex_lock(&mutexReady);
													list_add(ESTADO_READY, (void*) process);
													pthread_mutex_unlock(&mutexReady);

													ESTADO_EXEC = NULL;

													//printf("Agregando entrenador a proximos\n");
													pthread_mutex_lock(&mutexProximos);
													list_add(proximosEjecutar, (void*) process);
													sem_post(&counterProximosEjecutar);
													pthread_mutex_unlock(&mutexProximos);
													pthread_mutex_unlock(&cpu);


													pthread_mutex_lock(&ejecuta[process->indice]);

												//	pthread_mutex_lock(&cpu);

													log_info(logEntrega,
															"Se cambia entrenador %d a EXEC porque se le dio quantum",
															process->indice);

													pthread_mutex_lock(&mutexReady);
													int i = hallarIndice(process, ESTADO_READY);

												//	if (i != -1)
														list_remove(ESTADO_READY, i);
													//else
														//log_error(logger,
															//	"El indice es -1, no lo pude encontrar!");
													pthread_mutex_unlock(&mutexReady);
													ESTADO_EXEC = process;
													process->estado = EXEC;
													pthread_mutex_lock(&mutexCambiosDeContexto);
													cambiosDeContexto++;
													pthread_mutex_unlock(&mutexCambiosDeContexto);
							}
					}
					if(strcmp(teamConf->ALGORITMO_PLANIFICACION,"SJFD")==0){
						//double rafagaPendiente; //Esto es un mock
						//Todo
						/*Tengo que fijarme si hay un entrenador con más prioridad --> estimacion de rafaga es menor que el que está ejecutando*/
						double menorRafaga = entrenadorMenorRafaga(proximosEjecutar);
				//		printf("Comparo menorRafaga: %f y rafagaPendiente: %f\n",menorRafaga,process->rafagaPendiente);
						if(menorRafaga<process->rafagaPendiente){
							log_info(logEntrega,"Se cambia al entrenador %d a READY por desalojo",process->indice);
					//		process->disponible=1;
							process->estado = READY;
							pthread_mutex_lock(&mutexReady);
							list_add(ESTADO_READY, (void*) process);
							pthread_mutex_unlock(&mutexReady);

							ESTADO_EXEC=NULL;
							pthread_mutex_lock(&mutexProximos);
							list_add(proximosEjecutar, (void*) process);
							sem_post(&counterProximosEjecutar);
							pthread_mutex_unlock(&mutexProximos);
							pthread_mutex_unlock(&cpu);

							pthread_mutex_lock(&ejecuta[process->indice]);

							//pthread_mutex_lock(&cpu);

							log_info(logEntrega,
							"Se cambia entrenador %d a EXEC porque se le dio quantum",
							process->indice);

							pthread_mutex_lock(&mutexReady);
							int i = hallarIndice(process, ESTADO_READY);

						//	if (i != -1)
								list_remove(ESTADO_READY, i);
							//else
								//log_error(logger,
									//	"El indice es -1, no lo pude encontrar!");
							pthread_mutex_unlock(&mutexReady);
							ESTADO_EXEC = process;
							process->estado = EXEC;
							pthread_mutex_lock(&mutexCambiosDeContexto);
							cambiosDeContexto++;
							pthread_mutex_unlock(&mutexCambiosDeContexto);


						}
					}

					administrativo[process->indice].quantum--;
					sleep(teamConf->RETARDO_CICLO_CPU);
					pthread_mutex_lock(&mutexCiclosTotales);
					ciclosDeCpuTotales++;
					pthread_mutex_unlock(&mutexCiclosTotales);
					ciclosPorEntrenador[process->indice]++;
					process->rafagaPendiente-=1;

					pthread_mutex_lock(&mutexSegundosTotales);
					segundosTotales++;
					pthread_mutex_unlock(&mutexSegundosTotales);


					enviarMensajeBrokerCatch(recurso.nombrePokemon, recurso.posX,
							recurso.posY, socket);
					t_paquete *idMensaje = malloc(sizeof(t_paquete));
					idMensaje = recibirMensaje(socket);
					t_catch *catch = malloc(sizeof(t_catch));
					catch->emisor = process;
					catch->id =idMensaje->buffer->idMensaje;
					list_add(listaIdCatch, (void*) catch);

					log_info(logEntrega,"Se cambia al entrenador %d a BLOCKED porque debe esperar la respuesta de CATCH %s",process->indice,recurso.nombrePokemon);

					process->estado=BLOCKED;
					pthread_mutex_lock(&mutexRespuesta);
					list_add(esperandoRespuesta,(void*)process);
					pthread_mutex_unlock(&mutexRespuesta);

					pthread_mutex_lock(&mutexBlocked);
					list_add(ESTADO_BLOCKED,(void*)process);
					pthread_mutex_unlock(&mutexBlocked);


					pthread_mutex_unlock(&cpu);

					pthread_mutex_lock(&ejecuta[process->indice]);

				//	pthread_mutex_lock(&cpu);

					pthread_mutex_lock(&mutexBlocked);
					int indice = hallarIndice(process,ESTADO_BLOCKED);
					list_remove(ESTADO_BLOCKED,indice);
					pthread_mutex_unlock(&mutexBlocked);

					pthread_mutex_lock(&mutexRespuesta);
					indice = hallarIndice(process,esperandoRespuesta);
					list_remove(esperandoRespuesta,indice);
					pthread_mutex_unlock(&mutexRespuesta);

					if(catch->resultado == 1){


						log_info(logEntrega,"[Entrenador %d]: Se atrapa %s en %d,%d",process->indice,recurso.nombrePokemon,recurso.posX,recurso.posY);
						list_add(process->pokemons, (void*) recurso.nombrePokemon);
						list_add(atrapados,(void*)recurso.nombrePokemon);
						list_remove_by_condition(pendientes,_esPoke);

						//log_debug(logger,"Pendientes quedo: ");
						//mostrarListaChar(pendientes);
						//log_debug(logger,"Pokemons atrapados: ");
						//mostrarListaChar(atrapados);
					}
					else{
						log_error(logEntrega,"No se pudo atrapar %s en %d,%d",recurso.nombrePokemon,recurso.posX,recurso.posY);
						buscarPokemon(recurso.nombrePokemon);
					}

		}
		else
		{
			log_error(logEntrega,"El broker esta desconectado, se toma comportamiento default para CATCH_POKEMON %s",recurso.nombrePokemon);
			log_info(logEntrega, "[Entrenador %d]: Se atrapa %s en %d,%d",process->indice, recurso.nombrePokemon,
							recurso.posX, recurso.posY);
					list_add(process->pokemons, (void*) recurso.nombrePokemon);
					list_add(atrapados,(void*)recurso.nombrePokemon);
					list_remove_by_condition(pendientes,_esPoke);

				//	log_debug(logger,"Pendientes quedo: ");
				//	mostrarListaChar(pendientes);
				//	log_debug(logger,"Pokemons atrapados: ");
				//	mostrarListaChar(atrapados);

		}
		liberarConexion(socket);

//				log_info(logEntrega, "[Entrenador %d]: Se atrapa %s en %d,%d",process->indice,
//						recurso.nombrePokemon, recurso.posX, recurso.posY);
//				list_add(process->pokemons, (void*) recurso.nombrePokemon);

				if (cumplioObjetivo(process)) {
					log_info(logEntrega,
							"Se cambia entrenador %d a la cola EXIT porque cumplio su objetivo",
							process->indice);
					pthread_mutex_lock(&mutexExit);
					list_add(ESTADO_EXIT, (void*) process);
					pthread_mutex_unlock(&mutexExit);
					process->estado = EXIT;
					ESTADO_EXEC = NULL;
				//	int indice = hallarIndice(process,ESTADO_READY);
					//if(indice != -1){
					//	list_remove(ESTADO_READY,indice);
					//}
					pthread_mutex_unlock(&cpu);
					terminarSiPuedo();

					//pthread_exit(NULL);
				} else {

					if (puedeSeguirAtrapando(process)) {
						log_info(logEntrega,
								"Se cambia entrenador %d a la cola BLOCKED por fin de rafaga",
								process->indice);
						process->disponible = 1;
						process->estado = BLOCKED;
						pthread_mutex_lock(&mutexBlocked);
						list_add(ESTADO_BLOCKED, (void*) process);
						pthread_mutex_unlock(&mutexBlocked);

						pthread_mutex_lock(&mutexDormidos);
						list_add(dormidos,(void*)process);
						pthread_mutex_unlock(&mutexDormidos);
						sem_post(&counterDormidos);
						//sem_post(&counterReady);
						ESTADO_EXEC = NULL;
					} else {
						log_info(logEntrega,
								"Se cambia entrenador %d a la cola BLOCKED porque posee tantos pokemons como objetivos tiene",
								process->indice);
						process->estado = BLOCKED;
						pthread_mutex_lock(&mutexBlocked);
						list_add(ESTADO_BLOCKED, (void*) process);
						pthread_mutex_unlock(&mutexBlocked);

						ESTADO_EXEC = NULL;

					}
				}

				//printf("Me encuentro en %d,%d \n", process->posicion.x,
			//			process->posicion.y);
	//			printf("---------------------FIN-----------------------\n");

				//printf("Los pokemons del entrenador %d son: \n",
					//	process->indice);

				//mostrarListaChar(process->pokemons);
			//	log_info(logEntrega,"Muestro colas");
				//mostrarColas();

				//printf("Estoy antes del if\n");
				//int resu = hayEntrenadoresDisponibles();
				//printf("El resultado es %d\n",resu);
										if(!hayEntrenadoresDisponibles()){
										//	printf("Entre al if\n");
											pthread_t tTratarDeadlocks;
											pthread_create(&tTratarDeadlocks, NULL,(void*) tratamientoDeDeadlocks, NULL);
											pthread_detach(tTratarDeadlocks);
											//pthread_join(tTratarDeadlocks,NULL);
										}
										//printf("Estoy despues del if\n");
			} else {
				//printf("El entrenador no puede ejecutar!\n");
				pthread_mutex_unlock(&mutexReady);
				pthread_t tTratarDeadlocks;
				pthread_create(&tTratarDeadlocks, NULL,
						(void*) tratamientoDeDeadlocks, NULL);
				pthread_detach(tTratarDeadlocks);
				//pthread_join(tTratarDeadlocks,NULL);
			}
			//log_debug(logger,"Devuelvo CPU");
			pthread_mutex_unlock(&cpu);

		} else {
			//log_debug(logger, "Entro en el else del entrenador");
			t_entrenador *involucrado =
					administrativo[process->indice].involucrado;
			process->ultimaRafaga = 0;
			moverEntrenador(process, involucrado->posicion);
			pthread_mutex_lock(&mutexCambiosDeContexto);
			cambiosDeContexto++;
			pthread_mutex_unlock(&mutexCambiosDeContexto);

		//	printf("Buscando pokemons del intercambio\n");
			char* pokemon1 = pokemonEnConflicto(process, involucrado);
			char *pokemon2 = pokemonEnConflicto(involucrado, process);

		//	printf("Los entrenadores %d y %d van a intercambiar %s - %s \n",
			//		process->indice, involucrado->indice, pokemon1, pokemon2);
			log_info(logEntrega,"Entrenadores %d y %d intercambiaran %s por %s",process->indice,involucrado->indice,pokemon1,pokemon2);
			intercambiar(process, involucrado, pokemon2, pokemon1);
			//log_debug(logger, "Termine de intercambiar, devuelvo CPU");
			pthread_mutex_unlock(&cpu);
		}

		calculoEstimacionSjf(process);
		process->rafagaPendiente=process->estimacionRafagaActual;
	}
//	printf("La posicion final del entrenador %d es %d,%d\n", process->indice,
	//		process->posicion.x, process->posicion.y);

//	liberarProceso(process);
	pthread_exit(NULL);
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
		//printf("ESPERA ACTIVA? recvMensajes\n");
		//bufferLoco = malloc(sizeof(t_paquete));
		//printf("Esperando por un nuevo mensaje...\n");

		//pthread_mutex_lock(&recibir_mutex);
		pthread_mutex_lock(&mutexRecibir);
		bufferLoco = recibirMensaje(socket);

		if (bufferLoco != NULL) {
			//printf("%s\n", bufferLoco->buffer->nombrePokemon);
			pthread_mutex_lock(&mutex_bandeja);
			queue_push(bandejaDeMensajes, (void*) bufferLoco);
			sem_post(&contadorBandeja);
			pthread_mutex_unlock(&mutex_bandeja);
			pthread_mutex_unlock(&mutexRecibir);
		//	printf("estoy despues del unlock de bandeja de mensajes\n");
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

	pthread_exit(NULL);
	return NULL;

}

bool llegoMensaje(char *especie){
	//log_debug(logger,"Entre en llego mensaje");
return estaEn(especiesEnMapa,(void*)especie);
}

void *consumirMensajes(){
	pthread_t procesar;
	while(1){
		sem_wait(&contadorBandeja);
		pthread_create(&procesar,NULL,procesarMensaje,NULL);
	}
}

void* procesarMensaje() { // aca , la idea es saber que pokemon ponemos en el mapa por ejemplo.
	//printf("Rompo en procesarMensaje 1\n");
	t_paquete* bufferLoco = malloc(sizeof(t_paquete));

	while(1){
		//printf("ESPERA ACTIVA? procesarMensaje\n");
//	printf("Rompo en procesarMensaje 2\n");
		sem_wait(&contadorBandeja);
		pthread_mutex_lock(&mutex_bandeja);
//	printf("Rompo en procesarMensaje 3\n");
		bufferLoco = (t_paquete*) queue_pop(bandejaDeMensajes); //ver en que posicion busco, por ahi se necesita una variable.
//	printf("Rompo en procesarMensaje 4\n");
		pthread_mutex_unlock(&mutex_bandeja);
		switch (bufferLoco->codigoOperacion) {
		case MENSAJE_APPEARED_POKEMON: { //ver que casos usa el team
			log_info(logEntrega,
									"Llego mensaje APPEARED_POKEMON - %s %d,%d",
									bufferLoco->buffer->nombrePokemon,
									bufferLoco->buffer->posX, bufferLoco->buffer->posY);
			if (estaEn(pendientes,
					(void*) bufferLoco->buffer->nombrePokemon)) {
				//printf("Hay un %s que necesito en %d,%d\n",
				//		bufferLoco->buffer->nombrePokemon,
				//		bufferLoco->buffer->posX, bufferLoco->buffer->posY);

				list_add(especiesEnMapa,(void*)bufferLoco->buffer->nombrePokemon);


				pthread_mutex_lock(&mutexListaPokemons);
				queue_push(appearedPokemon, (void*) bufferLoco);
				pthread_mutex_unlock(&mutexListaPokemons);
				sem_post(&pokemonsEnLista);

			} else {
				log_error(logger,"No necesito al Pokemon %s, descarto el mensaje",bufferLoco->buffer->nombrePokemon);
				free(bufferLoco->buffer);
				free(bufferLoco);
			//	printf("Mensaje ignorado\n");
			}
			break;
		}
		case MENSAJE_CAUGHT_POKEMON: {
			char *resultado = string_new();
			if(bufferLoco->buffer->boolean)
				string_append(&resultado,"OK");
			else
				string_append(&resultado,"FAIL");

			log_info(logEntrega, "Llego mensaje CAUGHT_POKEMON %d %s\n",bufferLoco->buffer->idMensajeCorrelativo,resultado);

			bool esId(void* arg){
			t_catch *catch = (t_catch*)arg;

			return catch->id == bufferLoco->buffer->idMensajeCorrelativo;
			}

		//	printf("El id del caught que me llego es ID:%d, IdCorrelativo:%d\n",bufferLoco->buffer->idMensaje,bufferLoco->buffer->idMensajeCorrelativo);
			//printf("Mis IDS son: \n");
			void mostrarIdCatch(void*arg){
			t_catch *catch = (t_catch*)arg;
			printf("%d\n",catch->id);
			}
			//list_iterate(listaIdCatch,mostrarIdCatch);



			if(list_any_satisfy(listaIdCatch,esId)){
				//printf("Encontre el id en la lista!\n");
				t_catch* catch = list_find(listaIdCatch,esId);
				if(bufferLoco->buffer->boolean)
					catch->resultado = 1;
				else
					catch->resultado=0;

			//	printf("Se desbloqueara entrenador %d porque llego un mensaje CAUGHT que esperaba!\n",catch->emisor->indice);

				pthread_mutex_lock(&mutexProximos);
				list_add(proximosEjecutar,(void*)catch->emisor);
				pthread_mutex_unlock(&mutexProximos);
				sem_post(&counterProximosEjecutar);
			}
			else
			{
				free(bufferLoco->buffer);
				free(bufferLoco);
			//	printf("Mensaje ignorado\n");
			}
			break;
		}

		case MENSAJE_LOCALIZED_POKEMON: {
			int id = bufferLoco->buffer->idMensajeCorrelativo;
			log_debug(logger,"Mensaje Localized IdMensaje:%d, IdMensajeCorrelativo:%d",bufferLoco->buffer->idMensaje,bufferLoco->buffer->idMensajeCorrelativo);
			log_info(logEntrega,"Llego mensaje LOCALIZED_POKEMON - %s -%d",bufferLoco->buffer->nombrePokemon,list_size(bufferLoco->buffer->listaCoordenadas));
			int i;
			for(i=0;i<list_size(bufferLoco->buffer->listaCoordenadas);i++){
				t_posicion *pos = (t_posicion*)list_get(bufferLoco->buffer->listaCoordenadas,i);
				log_info(logEntrega,"Coord[%d]: %d,%d",i+1,pos->x,pos->y);
			}
			//printf("Los id's que tengo son\n");
			//mostrarListaInt(listaIdGet);
			//log_debug(logEntrega,"Llego mensaje LOCALIZED %s",bufferLoco->buffer->nombrePokemon);
			//printf("El mensaque localized que llego tiene idCorrelativo %d\n",bufferLoco->buffer->idMensajeCorrelativo);
			//printf("El mensaje localized que llego tiene id %d\n",bufferLoco->buffer->idMensaje);
			pthread_mutex_lock(&mutexIdGet);
			if (contieneId(listaIdGet,id)) {
				//printf("Llego mensaje LOCALIZED %s",bufferLoco->buffer->nombrePokemon);

				if(!llegoMensaje(bufferLoco->buffer->nombrePokemon)){
					list_add(especiesEnMapa,(void*)bufferLoco->buffer->nombrePokemon);
					int i;
					t_list *aux = list_duplicate(bufferLoco->buffer->listaCoordenadas);
					//log_debug(logger,"La lista de coordenadas tiene %d posiciones",aux->elements_count);
					for(i=0;i<bufferLoco->buffer->listaCoordenadas->elements_count;i++){
						t_paquete *nuevoPokemon = malloc(sizeof(t_paquete));
						nuevoPokemon->buffer=malloc(sizeof(t_bufferOmnidata));
						nuevoPokemon->buffer->nombrePokemon=string_duplicate(bufferLoco->buffer->nombrePokemon);
						nuevoPokemon->buffer->posX = ((t_posicion*)aux->head->data)->x;
						nuevoPokemon->buffer->posY = ((t_posicion*)aux->head->data)->y;
						aux->head = aux->head->next;
						list_add(especiesEnMapa,(void*)bufferLoco->buffer->nombrePokemon);
						if(i == 0){
						pthread_mutex_lock(&mutexListaPokemons);
						queue_push(appearedPokemon,(void*)nuevoPokemon);
						pthread_mutex_unlock(&mutexListaPokemons);
						sem_post(&pokemonsEnLista);
						}
						else
						{
						pthread_mutex_lock(&mutexListaExtra);
						list_add(appearedExtra,(void*)nuevoPokemon);
						pthread_mutex_unlock(&mutexListaExtra);
						sem_post(&pokemonsEnListaExtra);
						}
					}
				}
				else
				{
				free(bufferLoco->buffer);
				free(bufferLoco);
				//printf("Ignoro mensaje\n");
				}

				//Aca tengo que guardarme la información del localized.
			}
			else
			{
			free(bufferLoco->buffer);
			free(bufferLoco);
			//printf("Ignoro mensaje\n");
			}
			pthread_mutex_unlock(&mutexIdGet);

			//log_info(logEntrega, "Llego mensaje LOCALIZED_POKEMON\n");
			break;
		}
		case ENVIAR_ID_MENSAJE: {
			//printf("Se asignó un id: %d\n", bufferLoco->buffer->idMensaje);
			//log_debug(logger, "Se asignó un id: %d\n",
				//	bufferLoco->buffer->idMensaje);
			//log_info(logEntrega, "Llego mensaje ID_MENSAJE: %d",
				//	bufferLoco->buffer->idMensaje);
			break;
		}
		default: {
			printf("Ese menesaje no es interpretado por el team!!\n");
			exit(1);
			//Un mensaje distinto a estos jamas debería llegar aca, si fuese así mato al proceso porque es un error inesperado.
		}
		}
//	printf("Rompo en procesarMensaje 5\n");
	}
	pthread_exit(NULL);
	return NULL;
}

int distanciaHasta(t_posicion pos1, t_posicion pos2) {
	int desp = 0;
	//t_posicion aux = pos1;
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
//	printf("La distancia entre %d,%d y %d,%d es %d\n", aux.x, aux.y, pos2.x,
//			pos2.y, desp);
	return desp;
}

bool estaDisponible(void *arg){
	t_entrenador *entrenador = (t_entrenador*)arg;
	return entrenador->disponible == 1;
}

t_entrenador *buscarMasCercano(t_posicion coordenadas) {
	t_entrenador *masCercano;
	t_entrenador *aux;
	//t_list *readyDisponbiles = list_filter(ESTADO_READY,estaDisponible);
	int distancia, min = 1000000; //asumo que nunca me van a dar un mapa que me haga superar este numero
	int i, entrenadoresDisponibles = dormidos->elements_count;
	if (entrenadoresDisponibles == 0) {
		//printf(
			//	"No hay entrenadores disponibles para atender el pedido en este momento\n");
		return NULL;
	}
	if(entrenadoresDisponibles ==1){
		return list_get(dormidos,0);
	}

	for (i = 0; i < entrenadoresDisponibles; i++) {
		aux = (t_entrenador*) list_get(dormidos, i);
		//printf("Saque de ready entrenador %d\n",aux->indice);
		distancia = distanciaHasta(aux->posicion, coordenadas);

		if (distancia < min ) {
			min = distancia;
			masCercano = aux;
		}
	}

	return masCercano;
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

bool terminaronDeadlock(){
	return list_all_satisfy(ESTADO_BLOCKED,flagDeadlockApagado);
}

t_entrenador *buscarInvolucrado(t_entrenador *desbloquear,
		t_list *entrenadoresBloqueados) {
	t_entrenador* involucrado;
	t_list *aux = list_duplicate(entrenadoresBloqueados);

	while (aux->head != NULL) {
		t_list *auxPok = list_duplicate(desbloquear->objetivos);
		involucrado = (t_entrenador*) aux->head->data;
		while (auxPok->head != NULL) {
			if (estaEn(involucrado->pokemons, auxPok->head->data)) {
				list_destroy(aux);
				list_destroy(auxPok);
				return involucrado;
			}
			auxPok->head = auxPok->head->next;
		}
		aux->head = aux->head->next;
		list_destroy(auxPok);
	}

	list_destroy(aux);
	return NULL;
}

void intercambiar(t_entrenador* entrenador1, t_entrenador *entrenador2,
		char* pokemon1, char *pokemon2) {

	//printf(
		//	"El entrenador %d le dara un %s al entrenador %d a cambio de un %s\n",
			//entrenador1->indice, pokemon1, entrenador2->indice, pokemon2);

	int i = 0;
	administrativo[entrenador1->indice].quantum = teamConf->QUANTUM;
	while (i < 5) {
		if (strcmp(teamConf->ALGORITMO_PLANIFICACION, "RR") == 0) {
			if (administrativo[entrenador1->indice].quantum < 1) {

				//log_debug(logger, "FIN DE QUANTUM");
				//printf("--------FIN DE QUANTUM--------\n");


				log_info(logEntrega,
						"Se cambia entrenador %d a la cola READY por fin de quantum",
						entrenador1->indice);
				pthread_mutex_lock(&mutexReady);
				list_add(ESTADO_READY, (void*) entrenador1);
				pthread_mutex_unlock(&mutexReady);
				entrenador1->estado = READY;
				ESTADO_EXEC = NULL;
				//printf("Agregando entrenador a proximos\n");
				//queue_push(proximosEjecutar, (void*) entrenador1);
				pthread_mutex_lock(&mutexProximos);
				list_add(proximosEjecutar, (void*) entrenador1);
				sem_post(&counterProximosEjecutar);
				pthread_mutex_unlock(&mutexProximos);
				pthread_mutex_unlock(&cpu);

			//	printf("Bloqueado en el mutex entrenador\n");
				pthread_mutex_lock(&ejecuta[entrenador1->indice]);

				//printf("Bloqueado en el cpu\n");
			//	pthread_mutex_lock(&cpu);

				log_info(logEntrega,
						"Se cambia entrenador %d a EXEC porque se le dio quantum",
						entrenador1->indice);
				//printf("Antes de la tragedia intercambio\n");
				pthread_mutex_lock(&mutexReady);
				int i = hallarIndice(entrenador1, ESTADO_READY);
			//	printf("Despues de la tragedia intercambio\n");
				//if (i != -1)
					list_remove(ESTADO_READY, i);
				//else
					//log_error(logger, "El indice es -1, no lo pude encontrar!");
				pthread_mutex_unlock(&mutexReady);
				ESTADO_EXEC = entrenador1;
				entrenador1->estado = EXEC;
				pthread_mutex_lock(&mutexCambiosDeContexto);
				cambiosDeContexto++;
				pthread_mutex_unlock(&mutexCambiosDeContexto);

				//printf("llegue hast aca\n");

			}

			administrativo[entrenador1->indice].quantum--;

		}
		if(strcmp(teamConf->ALGORITMO_PLANIFICACION,"SJFD")==0){
														//double rafagaPendiente; //Esto es un mock
														//Todo
														/*Tengo que fijarme si hay un entrenador con más prioridad --> estimacion de rafaga es menor que el que está ejecutando*/
														double menorRafaga = entrenadorMenorRafaga(proximosEjecutar);
														//printf("Comparo menorRafaga: %f y rafagaPendiente: %f\n",menorRafaga,entrenador1->rafagaPendiente);
														if(menorRafaga<entrenador1->rafagaPendiente){
															log_info(logEntrega,"Se cambia al entrenador %d a READY por desalojo",entrenador1->indice);
														//	entrenador1->disponible=1;
															entrenador1->estado = READY;
															pthread_mutex_lock(&mutexReady);
															list_add(ESTADO_READY, (void*) entrenador1);
															pthread_mutex_unlock(&mutexReady);

															ESTADO_EXEC = NULL;
															pthread_mutex_lock(&mutexProximos);

															list_add(proximosEjecutar, (void*) entrenador1);
															sem_post(&counterProximosEjecutar);
															pthread_mutex_unlock(&mutexProximos);
															pthread_mutex_unlock(&cpu);

															pthread_mutex_lock(&ejecuta[entrenador1->indice]);

														//	pthread_mutex_lock(&cpu);

															log_info(logEntrega,
															"Se cambia entrenador %d a EXEC porque se le dio quantum",
															entrenador1->indice);

															pthread_mutex_lock(&mutexReady);
															int i = hallarIndice(entrenador1, ESTADO_READY);

															//if (i != -1)
																list_remove(ESTADO_READY, i);
															//else
																//log_error(logger,
																	//	"El indice es -1, no lo pude encontrar!");
															pthread_mutex_unlock(&mutexReady);
															ESTADO_EXEC = entrenador1;
															entrenador1->estado = EXEC;
															pthread_mutex_lock(&mutexCambiosDeContexto);
															cambiosDeContexto++;
															pthread_mutex_unlock(&mutexCambiosDeContexto);


														}
													}
		//	else {
////			sleep(teamConf->RETARDO_CICLO_CPU);
////			ciclosDeCpuTotales++;
////			ciclosPorEntrenador[entrenador1->indice]++;
////			segundosTotales++;
//
//		}

		sleep(teamConf->RETARDO_CICLO_CPU);
		segundosTotales += teamConf->RETARDO_CICLO_CPU;
		pthread_mutex_lock(&mutexCiclosTotales);
		ciclosDeCpuTotales++;
		pthread_mutex_unlock(&mutexCiclosTotales);
		ciclosPorEntrenador[entrenador1->indice]++;
		entrenador1->ultimaRafaga++;
		entrenador1->rafagaPendiente-=1;
		i++;
	}
	int indice;
	indice = buscarIndicePokemon((void*) pokemon1, entrenador1->pokemons);
	list_remove(entrenador1->pokemons, indice);
	list_add(entrenador2->pokemons, (void*) pokemon1);

	indice = buscarIndicePokemon((void*) pokemon2, entrenador2->pokemons);
	list_remove(entrenador2->pokemons, indice);
	list_add(entrenador1->pokemons, (void*) pokemon2);
//
//	sleep(teamConf->RETARDO_CICLO_CPU * 5);
//
//	ciclosDeCpuTotales += 5;
//	ciclosPorEntrenador[entrenador1->indice] += 5;
	//ciclosPorEntrenador[entrenador2->indice] += 5;

	log_info(logEntrega,
			"Se ha solucionado el deadlock entre entrenador %d y entrenador %d",
			entrenador1->indice, entrenador2->indice);
	deadlocksResueltos++;

	if (cumplioObjetivo(entrenador1)) {
		entrenador1->estado = EXIT;
		pthread_mutex_lock(&mutexExit);
		list_add(ESTADO_EXIT, (void*) entrenador1);
		pthread_mutex_unlock(&mutexExit);
		log_info(logEntrega,
				"Se cambia entrenador %d a la cola EXIT porque cumplio su objetivo",
				entrenador1->indice);
		//pthread_mutex_lock(&mutexBlocked);
		//indice = hallarIndice(entrenador1,ESTADO_BLOCKED);
			//	if(indice !=-1){
			//		list_remove(ESTADO_BLOCKED,indice);
				//}
		//pthread_mutex_unlock(&mutexBlocked);
				ESTADO_EXEC = NULL;
		terminarSiPuedo();
	} else {
		entrenador1->estado = BLOCKED;
		pthread_mutex_lock(&mutexBlocked);
		list_add(ESTADO_BLOCKED, (void*) entrenador1);
		pthread_mutex_unlock(&mutexBlocked);


		log_info(logEntrega,
				"Se cambia entrenador %d a la cola BLOCKED porque tiene tantos pokemons como la cantidad que necesita",
				entrenador1->indice);
		entrenador1->flagDeadlock = 0;
		ESTADO_EXEC = NULL;
	}

	if (cumplioObjetivo(entrenador2)) {
		pthread_mutex_lock(&mutexBlocked);
		indice = hallarIndice(entrenador2,ESTADO_BLOCKED);
		list_remove(ESTADO_BLOCKED,indice);
		pthread_mutex_unlock(&mutexBlocked);
		entrenador2->estado = EXIT;
		pthread_mutex_lock(&mutexExit);
		list_add(ESTADO_EXIT, (void*) entrenador2);
		pthread_mutex_unlock(&mutexExit);
		log_info(logEntrega,
				"Se cambia entrenador %d a la cola EXIT porque cumplio su objetivo",
				entrenador2->indice);

	//	pthread_mutex_lock(&mutexBlocked);
		//indice = hallarIndice(entrenador2,ESTADO_BLOCKED);
		//if(indice !=-1){
		//	list_remove(ESTADO_BLOCKED,indice);
		//}
	//	pthread_mutex_unlock(&mutexBlocked);
		terminarSiPuedo();
	} else {
		//pthread_mutex_lock(&mutexBlocked);
		//list_add(ESTADO_BLOCKED, (void*) entrenador2);
		//pthread_mutex_unlock(&mutexBlocked);
		entrenador2->estado = BLOCKED;
//		log_info(logEntrega,
//				"Se cambia entrenador %d a la cola BLOCKED porque tiene tantos pokemons como la cantidad que necesita",
//				entrenador2->indice);
		entrenador2->flagDeadlock = 0;
	}
	if(terminaronDeadlock()){
		pthread_mutex_unlock(&mutexDeteccion);
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
	list_destroy(aux);
	return filtrada;
}

char *pokemonEnConflicto(t_entrenador *e1, t_entrenador *e2) {
//	printf("Entre a buscarconflico\n");
	t_list *auxAtrapar = list_duplicate(e2->pokemons);

	while (auxAtrapar->head != NULL) {
		if (estaEn(e1->objetivos, auxAtrapar->head->data)) {
			char *atrapar = (char*) auxAtrapar->head->data;
			list_destroy(auxAtrapar);
			return atrapar;
		}
		auxAtrapar->head = auxAtrapar->head->next;
	}

	//printf("Salgo de buscarconflicto\n");

	if (auxAtrapar->head != NULL) {
		char *atrapar = (char*) auxAtrapar->head->data;
		list_destroy(auxAtrapar);
		return atrapar;
	} else {

		//Si ninguno le sirve, le devuelve cualquiera.
		list_destroy(auxAtrapar);
		t_list *noNecesita = filterNoNecesita(e2, e2->pokemons);
		char *atrapar = (char*) list_get(noNecesita, 0);
		list_destroy(noNecesita);
		return atrapar;
	}
}



void *tratarDeadlock(void* arg) {
	t_deadlock *deadlock = (t_deadlock*) arg;
	//printf("Rompision\n");
	t_entrenador *desbloquear = deadlock->desbloquear;
	t_entrenador *involucrado = deadlock->involucrado;
	//printf("Rompision\n");
	int indice;
	desbloquear->estado = EXEC;
	pthread_mutex_lock(&mutexBlocked);
	indice = hallarIndice(desbloquear, ESTADO_BLOCKED);
	//if (indice != -1)
		list_remove(ESTADO_BLOCKED, indice);
	pthread_mutex_unlock(&mutexBlocked);
	ESTADO_EXEC = desbloquear;
	//desbloquear->estado =EXEC;
	//printf("Rompision\n");
	log_info(logEntrega,
			"Se cambia al entrenador %d de la COLA_BLOCKED a COLA_EXEC para intercambiar un pokemon",
			desbloquear->indice);

	administrativo[desbloquear->indice].quantum = teamConf->QUANTUM;
	administrativo[desbloquear->indice].involucrado = involucrado;
	pthread_mutex_lock(&mutexProximos);
//	queue_push(proximosEjecutar, (void*) desbloquear);
	list_add(proximosEjecutar, (void*) desbloquear);
	pthread_mutex_unlock(&mutexProximos);
	sem_post(&counterProximosEjecutar);

	return NULL;
}

void *tratarDeadlocks() {
	pthread_t hiloDeadlock[100];
	int i = 0;
	while (procesosEnDeadlock->elements_count > 0) {
		//printf("ESPERA ACTIVA? TratarDeadlocks\n");
		sem_wait(&counterDeadlock);
		//printf("Estoy por sacar deadlock de la cola.\n");
		pthread_mutex_lock(&mutexDeadlock);
		t_deadlock *deadlock = (t_deadlock*) list_remove(procesosEnDeadlock, 0);
		pthread_mutex_unlock(&mutexDeadlock);
		//printf("Saque deadlock de la cola\n");
		//log_debug(logger, "Se resolvera deadlock entre enrenadores %d y %d",
			//	deadlock->desbloquear->indice, deadlock->involucrado->indice);
		//printf("Creo hilo para tratar deadlock\n");
		pthread_create(&hiloDeadlock[i], NULL, tratarDeadlock,
				(void*) deadlock);
		pthread_detach(hiloDeadlock[i]);
		//printf("Cree hilo para tratar deadlock\n");
		i++;
	}
	int j;
	for (j = 0; j < i; j++) {
		pthread_join(hiloDeadlock[j], NULL);
	}

	return NULL;
}



void *deteccionDeDealock() {
	log_info(logEntrega,
			"Se ha iniciado el algoritmo de deteccion de deadlocks");
	ejecucionDeteccion++;
	//while (!estanTodosEnExit()) {
		//printf("ESPERA ACTIVA? deteccionDeadlock. Blocked: %d\n",ESTADO_BLOCKED->elements_count);
		//log_debug(logger,"Antes de filtrar, blocked %d",ESTADO_BLOCKED->elements_count);
//		pthread_mutex_lock(&mutexBlocked);
//
//		pthread_mutex_unlock(&mutexBlocked);
		//log_debug(logger,"Despues de filtrar, blocked %d",ESTADO_BLOCKED->elements_count);
		//log_debug(logger,"La lista filtrada tiene %d y blocked tiene %",aux->elements_count,ESTADO_BLOCKED->elements_count);
		int flag = 1;
		int counter = 0;
		pthread_mutex_lock(&mutexBlocked);
		t_list *aux = list_filter(ESTADO_BLOCKED, flagDeadlockApagado);
		pthread_mutex_unlock(&mutexBlocked);
		while (aux->elements_count > 1 && flag) {
			pthread_mutex_lock(&mutexBlocked);
			aux = list_filter(ESTADO_BLOCKED, flagDeadlockApagado);
			pthread_mutex_unlock(&mutexBlocked);
		//	printf("ESPERA ACTIVA? deteccionDeadlock dentro: La lista AUX tiene %d\n",aux->elements_count);
			t_entrenador *desbloquear = list_remove(aux, 0);

			t_entrenador *involucrado = buscarInvolucrado(desbloquear, aux);

			if (involucrado != NULL
					&& involucrado->indice != desbloquear->indice) {
				deadlocksTotales++;
				t_deadlock *deadlock = malloc(sizeof(t_deadlock));
				desbloquear->flagDeadlock = 1;
				involucrado->flagDeadlock = 1;
				deadlock->desbloquear = desbloquear;
				deadlock->involucrado = involucrado;

				list_add(listaDeadlock,(void*)desbloquear->indice);

				pthread_mutex_lock(&mutexDeadlock);
				list_add(procesosEnDeadlock, (void*) deadlock);
				pthread_mutex_unlock(&mutexDeadlock);
				sem_post(&counterDeadlock);
				log_error(logger, "Se ha agregado un deadlock a la cola");
				log_info(logEntrega,"Se han detectado %d deadlocks con el algoritmo de deteccion",procesosEnDeadlock->elements_count);
			} else {
				//log_error(logger,"El involucrado dio null, no se como manejar eso aun");
				flag = 0;
			}

			counter++;
			list_destroy(aux);
		}
		pthread_t hiloDeadlock[100];
		int i = 0;

		while (procesosEnDeadlock->elements_count > 0) {
		//	printf("ESPERA ACTIVA? deteccionDeadlock 3\n");
			sem_wait(&counterDeadlock);
			//printf("Estoy por sacar deadlock de la cola.\n");
			pthread_mutex_lock(&mutexDeadlock);
			t_deadlock *deadlock = (t_deadlock*) list_remove(procesosEnDeadlock,
					0);
			pthread_mutex_unlock(&mutexDeadlock);
			//printf("Saque deadlock de la cola\n");
			//log_debug(logger, "Se resolvera deadlock entre enrenadores %d y %d",
				//	deadlock->desbloquear->indice,
					//deadlock->involucrado->indice);
		//	printf("Creo hilo para tratar deadlock\n");
			pthread_create(&hiloDeadlock[i], NULL, tratarDeadlock,
					(void*) deadlock);
			pthread_detach(hiloDeadlock[i]);
		//	printf("Cree hilo para tratar deadlock\n");
			i++;
		}
		int j;
		for (j = 0; j < i; j++) {
			pthread_join(hiloDeadlock[j], NULL);
		}

//	}

	return NULL;
}

void tratamientoDeDeadlocks() {
	pthread_t deteccion;
	//pthread_create(&tratamiento,NULL,tratarDeadlocks,NULL);
	while(!estanTodosEnExit()){
		pthread_mutex_lock(&mutexDeteccion); //evito espera activa!
	pthread_create(&deteccion, NULL, deteccionDeDealock, NULL);
	pthread_detach(deteccion);
	//pthread_create(&ejecuta,NULL,ejecutor,NULL);

	pthread_join(deteccion, NULL);
	}


	//pthread_join(tratamiento,NULL);
	//pthread_join(ejecuta,NULL);
}
void formatoHora(int input, t_log *archivoLog) {
	int hora = (int) input / 3600;
	int minutos = (int) ((input - hora * 3600) / 60);
	int segundos = input - (hora * 3600 + minutos * 60);

	log_debug(archivoLog, "El proceso tardo en planificar: %d:%d:%d", hora,
			minutos, segundos);
}

void reporteFinal(t_log *archivoLog) {

	log_trace(archivoLog,
			"El team %s termina porque cumplio todos sus objetivos. ",
			teamConf->NOMBRE_PROCESO);
	log_trace(archivoLog,
			"Se han detectado un total de %d deadlocks, resueltos: %d",
			deadlocksTotales, deadlocksResueltos);
//	if(!list_is_empty(listaDeadlock))
	//mostrarListaInt(listaDeadlock);
//	else
//	log_error(logger,"No hay deadlocks en la lista");
	log_trace(archivoLog, "Ciclos de CPU totales: %d", ciclosDeCpuTotales);
	int i = 0;
	for (i = 0; i < cantidadEntrenadores; i++) {
		log_trace(archivoLog, "El entrenador %d realizo %d ciclos de CPU", i,
				ciclosPorEntrenador[i]);
	}
	log_trace(archivoLog,
			"Con el algoritmo %s se realizaron un total de %d cambios de contexto.",
			teamConf->ALGORITMO_PLANIFICACION, cambiosDeContexto);


}

void liberarEstructuras(){
	free(ciclosPorEntrenador);
	free(threads_entreanadores);
	free(ejecuta);
	free(logger);
	log_destroy(logEntrega);
	log_destroy(logReporte);
	log_destroy(logPantalla);
	config_destroy(TEAMTConfig);
	free(teamConf);
//	list_destroy(pokemonEntrenadores);
//	list_destroy(posicionEntrenadores);
//	list_destroy(objetivoEntrenadores);
//	list_destroy(objetivoGlobal);
//	list_destroy(procesosEnDeadlock);
//	list_destroy(dormidos);
	//list_destroy(esperandoRespuesta);
	free(entrenadores);
	free(administrativo);
//	list_destroy(pendientes);
//	list_destroy(atrapados);
//	list_destroy(listaDeadlock);
	printf("Termine!\n\n\n");
}

void terminarSiPuedo() {
	if (estanTodosEnExit()) {
	//	log_debug(logger, "TERMINE");
		//log_info(logEntrega,"Muestro colas");
	//	mostrarColas();
		time_t tiempoActual = time(NULL);
		char buffer[26];
		struct tm* tm_info;
		tm_info = localtime(&tiempoActual);
		strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

	//	log_info(logReporte, "---Reporte de %s---", buffer);
	//	reporteFinal(logReporte);
	//	log_info(logReporte, "----------Fin de reporte----------");
		//printf("\033[1;35m");
		reporteFinal(logEntrega);
		reporteFinal(logPantalla);

		//log_trace(logger,"El team tardo en ejecutar: ");
	//	formatoHora(segundosTotales,logger);
		//printf("\033[0m]");
		liberarEstructuras();
		exit(0);
	}
}

void* planificarEntrenadores() { //aca vemos que entrenador esta en ready y mas cerca del pokemon
//agarramos el pokemon o lo que sea que el entrenador tenga que hacer y enviamos un mensaje al broker avisando.

	int i;
//	int socketBroker = crearConexion(teamConf->IP_BROKER,
//			teamConf->PUERTO_BROKER, teamConf->TIEMPO_RECONEXION);

	//printf("Estoy por crear los  %d hilos de entrenador\n",
		//	cantidadEntrenadores);
	for (i = 0; i < cantidadEntrenadores; i++) {
		//printf("i vale %d\n", i);
		t_entrenador *entrenador = (t_entrenador*) list_get(ESTADO_NEW, i);
		if (pthread_create(&threads_entreanadores[i], NULL, manejarEntrenador,
				(void*) entrenador) < 0) {
			printf("No se pduo crear el hilo\n");
		} else {
			//pthread_detach(threads_entreanadores[i]);
		//	printf("Handler asignado para entrenador [%d]\n", i);
		}

		pthread_t tEjecutor;
		pthread_create(&tEjecutor, NULL, ejecutor, NULL);
		pthread_detach(tEjecutor);

	}
//	mostrarColas();
	pthread_mutex_lock(&mutexPlani);
	while (!estanTodosEnExit()) {
		if (hayEntrenadoresDisponibles()) {
		//	printf("ESPERA ACTIVA? Planificar entrenadores\n");
			t_paquete *appeared;

			//printf("Esperando por la aparición de un pokemon\n");
			if (estanTodosEnExit()) {
				printf("El team %s cumplio su objetivo\n",
						teamConf->NOMBRE_PROCESO);
				return NULL;
			} else {
			//	sem_wait(&counterReady);

			//	sem_post(&counterReady);

				 //log_debug(logger,"Esperando por pokemons en appeared");
				sem_wait(&pokemonsEnLista);
				//log_debug(logger,"Ya termine de esperar pokemons");

				//log_debug(logger,"Esperando entrenadores disponibles");
						sem_wait(&counterDormidos);
					//			log_debug(logger,"Ya termine de esperar entrenadores");
				pthread_mutex_lock(&mutexListaPokemons);
				appeared = (t_paquete*) queue_pop(appearedPokemon);
				pthread_mutex_unlock(&mutexListaPokemons);

			//	printf(
				//		"Se detecto un %s en %d,%d. Se planificara el team para atraparlo\n",
					//	appeared->buffer->nombrePokemon, appeared->buffer->posX,
						//appeared->buffer->posY);

				t_posicion posicionPokemon;
				posicionPokemon.x = appeared->buffer->posX;
				posicionPokemon.y = appeared->buffer->posY;

				pthread_mutex_lock(&mutexDormidos);
				t_entrenador *buscador = buscarMasCercano(posicionPokemon);
				pthread_mutex_unlock(&mutexDormidos);
				char *nombrePokemon = string_duplicate(
						appeared->buffer->nombrePokemon);

				if (buscador != NULL) {
					//printf("El entrenador mas cercano es %d en %d,%d\n",buscador->indice,buscador->posicion.x,buscador->posicion.x);

					if(buscador->estado==BLOCKED){
						pthread_mutex_lock(&mutexBlocked);
					int index = hallarIndice(buscador,ESTADO_BLOCKED);
					list_remove(ESTADO_BLOCKED,index);
						pthread_mutex_unlock(&mutexBlocked);
					}
					if(buscador->estado==NEW){
						pthread_mutex_lock(&mutexNew);
					int index = hallarIndice(buscador,ESTADO_NEW);
					list_remove(ESTADO_NEW,index);
						pthread_mutex_unlock(&mutexNew);
					}
					pthread_mutex_lock(&mutexDormidos);
					int indice = hallarIndice(buscador,dormidos);
					list_remove(dormidos,indice);
					pthread_mutex_unlock(&mutexDormidos);

					log_info(logEntrega,"Se pasa entrenador %d a READY porque se lo planificara para atrapar un pokemon",buscador->indice);
					pthread_mutex_lock(&mutexReady);
					list_add(ESTADO_READY,(void*)buscador);
					pthread_mutex_unlock(&mutexReady);
					buscador->estado=READY;
					administrativo[buscador->indice].quantum =
							teamConf->QUANTUM;
					administrativo[buscador->indice].nombrePokemon =
							string_duplicate(nombrePokemon);
					administrativo[buscador->indice].posX = posicionPokemon.x;
					administrativo[buscador->indice].posY = posicionPokemon.y;
					buscador->disponible = 0;
					//pthread_mutex_lock(&mutexReady);
					//int i = hallarIndice(buscador,ESTADO_READY);
					//list_remove(ESTADO_READY,i);
					//pthread_mutex_unlock(&mutexReady);
					pthread_mutex_lock(&mutexProximos);
					//queue_push(proximosEjecutar, (void*) buscador);
					list_add(proximosEjecutar, (void*) buscador);
					pthread_mutex_unlock(&mutexProximos);
					sem_post(&counterProximosEjecutar);

				}
				else
				{
					pthread_mutex_lock(&mutexListaPokemons);
					queue_push(appearedPokemon,(void*) appeared);
					pthread_mutex_unlock(&mutexListaPokemons);
					sem_post(&pokemonsEnLista);
					sem_post(&counterDormidos);
					//sleep(teamConf->RETARDO_CICLO_CPU);
				}

//		for (j = 0; j < cantidadEntrenadores; j++) {
//			//sleep(2);
//			pthread_mutex_unlock(&ejecuta[j]);
//			printf("Esta en ejec el proceso %d\n",ESTADO_EXEC->pid);
//		}
			}
		} else {

			if(ESTADO_BLOCKED->elements_count == (cantidadEntrenadores-ESTADO_EXIT->elements_count)){
			log_info(logEntrega, "Se ha detectado una situacion de deadlock");
			pthread_t tTratarDeadlocks;
			//tratamientoDeDeadlocks();
			pthread_create(&tTratarDeadlocks, NULL,
					(void*) tratamientoDeDeadlocks, NULL);
			pthread_detach(tTratarDeadlocks);
			pthread_join(tTratarDeadlocks, NULL);

		}
			else
			{
			log_error(logger,"NO SE QUE HACER");
			}
		}
	}
	printf("Todos los procesos estan en EXIT\n");
	exit(0);
	return NULL;

}

void *ejecutor() {
	int quantum = teamConf->QUANTUM;
	while (!estanTodosEnExit()) {
	//	printf("ESPERA ACTIVA? Ejecutor\n");

		if (strcmp(teamConf->ALGORITMO_PLANIFICACION, "SJF") == 0 || strcmp(teamConf->ALGORITMO_PLANIFICACION,"SJFD")==0) {
			pthread_mutex_lock(&cpu);

			sem_wait(&counterProximosEjecutar);
			pthread_mutex_lock(&mutexProximos);
			t_entrenador *proximo = buscarMenorRafaga(proximosEjecutar);
			log_debug(logger,
					"El entrenador que tiene la menor rafaga es %d con %f ciclos de cpu",
					proximo->indice, proximo->estimacionRafagaActual);
			pthread_mutex_unlock(&mutexProximos);
			pthread_mutex_unlock(&ejecuta[proximo->indice]);
			//log_debug(logger, "Desbloquee %d", proximo->indice);
		} else {
			pthread_mutex_lock(&cpu);

			sem_wait(&counterProximosEjecutar);
			//log_debug(logger, "Ejecutor aqui");
			pthread_mutex_lock(&mutexProximos);
			t_entrenador *proximo = (t_entrenador*) list_remove(
					proximosEjecutar, 0);
			pthread_mutex_unlock(&mutexProximos);
			administrativo[proximo->indice].quantum = quantum;
			//pthread_mutex_unlock(&cpu);
			pthread_mutex_unlock(&ejecuta[proximo->indice]);
			//log_debug(logger, "Desbloquee %d", proximo->indice);
		}
	}

	pthread_exit(NULL);

	return NULL;
}


void inicializarLoggerTeam() {
	logger = log_create("team.log", "TEAM", 0, LOG_LEVEL_TRACE);
	if (logger == NULL) {
		perror("No se pudo inicializar el logger\n");
		//exit(1);
	}

	return;
}

void inicializarLoggerEntregable() {
	//printf("Voy a crear un logger %s\n", teamConf->LOG_FILE);

	logEntrega = log_create(teamConf->LOG_FILE, teamConf->NOMBRE_PROCESO, 0, LOG_LEVEL_TRACE);
	if (logEntrega == NULL) {
		perror("No se pudo inicializar el logger para la entrega\n");
	}
}

void inicializarLoggerPantalla(){
	logPantalla = log_create("reporteFinal.txt",teamConf->NOMBRE_PROCESO,1,LOG_LEVEL_TRACE);
	if(logPantalla == NULL){
		perror("No se pudo inicializar el logger de pantalla\n");
	}
}

void inicializarLoggerReporte() {
	logReporte = log_create("../reportes.log", "Reportes TEAM", 1,
			LOG_LEVEL_TRACE);
	if (logReporte == NULL) {
		perror("No se pudo inicializar el logger de reportes\n");
	}
	return;
}

void splitList(char **string, t_list *lista) {
	if (string != NULL) {
		//char **elem = string_split(string, "|"); //odio este warning, si el string de adentro se castea a char* y elem se borra o utiliza, el warning desaparece.
		string_split((char*)string,"|");
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
			list_add(pendientes,(void*)token);
		}
		else
			list_add(atrapados,(void*)token);
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
//Todo, la dejo para el final (revisar la memoria reservada acá).
void crearEntrenadores() {

	t_list *auxPos, *auxPok, *auxObj;
	objetivoGlobal = list_create();
	//log_info(logger, "Instanciando entrenadores");
	int i;
	entrenadores = (t_entrenador*) malloc(cantidadEntrenadores);
	//t_link_element *limpieza;
//t_posicion *posiciones=(t_posicion*)malloc(cantidadEntrenadores);
//t_list *pokemons;
//t_list *objetivos;
	auxPos = list_duplicate(posicionEntrenadores);
	auxPok = list_duplicate(pokemonEntrenadores);
	auxObj = list_duplicate(objetivoEntrenadores);

	int cantidadPokemons = pokemonEntrenadores->elements_count;
	int cantidadObjetivos = objetivoEntrenadores->elements_count;
	for (i = 0; i < cantidadEntrenadores; i++) {
		//printf("Print de debug1\n");
		t_entrenador *nuevoEntrenador = malloc(sizeof(t_entrenador));

		nuevoEntrenador->estado = NEW;
		//log_info(logEntrega,"Se agrega entrenador %d a la cola NEW porque se esta inicializando",i);
		nuevoEntrenador->posicion = separarPosiciones(auxPos->head->data);
		//entrenadores[i].posicion = separarPosiciones(auxPos->head->data);

		//posiciones[i]=separarPosiciones(auxPos->head->data);
		//printf("Print de debug2\n");
		if ((i + 1) < cantidadEntrenadores) {
			//limpieza = auxPos->head;
			auxPos->head = auxPos->head->next;
			auxPos->elements_count--;
			//free(limpieza);
		}
	//	printf("Print de debug3\n");
		if (auxPok->head != NULL) {
			nuevoEntrenador->pokemons = list_duplicate(
					separarPokemons(auxPok->head->data, 0));
		} else {
			nuevoEntrenador->pokemons = list_create();
		}
//		entrenadores[i].pokemons = list_duplicate(
//				separarPokemons(auxPok->head->data, 0));
	//	printf("Print de debug4\n");
		if ((i + 1) < cantidadPokemons) {
		//	printf("Debugcito1\n");
			//limpieza = auxPok->head;
		//	printf("Debugcito2\n");
			auxPok->head = auxPok->head->next;
			//printf("Debugcito3\n");
			auxPok->elements_count--;
			//printf("Debugcito4\n");
			//free(limpieza);
		} else {
		//	printf("Entre al else\n");
			auxPok->head = NULL;
		}
	//	printf("Print de debug5\n");
		if (auxObj->head != NULL) {
			nuevoEntrenador->objetivos = list_duplicate(
					separarPokemons(auxObj->head->data, 1));
		} else {
			//printf("Entre al else\n");
			nuevoEntrenador->objetivos = list_create();
		}
//		entrenadores[i].objetivos = list_duplicate(
//				separarPokemons(auxObj->head->data, 1));
		//printf("Print de debug6\n");
		if ((i + 1) < cantidadObjetivos) {
			//limpieza = auxObj->head;
			auxObj->head = auxObj->head->next;
			auxObj->elements_count--;
			//free(limpieza);
		} else {
			auxObj->head = NULL;
		}

		//printf("Print de debug7\n");

		//printf("Entrenador %d, está en X=%d e Y=%d.\n", i + 1,
		//		nuevoEntrenador->posicion.x, nuevoEntrenador->posicion.y);
		//printf("Los pokemons del entrenador %d son:\n", i + 1);
		//mostrarListaChar(nuevoEntrenador->pokemons);
		//printf("Los objetivos del entrenador %d son:\n", i + 1);
		//log_info(logger,"Los objetivos del entrenador %d son:",i+1);
	//	mostrarListaChar(nuevoEntrenador->objetivos);

		//list_destroy(pokemons);
		//list_destroy(objetivos);
		list_add(ESTADO_NEW,(void*)nuevoEntrenador);
				log_info(logEntrega,"Se agrega entrenador %d a la cola NEW porque se esta inicializando",i);
		//entrenadores[i].estado=NEW;
		//log_info(logEntrega,"Se cambia al entrenador %d de NEW a READY porque termino de inicializarse",i);
		//nuevoEntrenador->estado = ;
		nuevoEntrenador->estimacionRafagaActual = teamConf->ESTIMACION_INICIAL;
		//nuevoEntrenador->finRafaga = 0;
		//nuevoEntrenador->inicioRafaga = 0;
	//	nuevoEntrenador->quantumPendiente = 0;
		nuevoEntrenador->rafaga = 0;
		nuevoEntrenador->indice = i;
		nuevoEntrenador->flagDeadlock = 0;
		nuevoEntrenador->disponible =1;

		pthread_mutex_lock(&mutexDormidos);
		list_add(dormidos,(void*)nuevoEntrenador);
		pthread_mutex_unlock(&mutexDormidos);
		sem_post(&counterDormidos);
		//log_info(logEntrega,"Se cambia entrenador %d de NEW a READY porque termino de inicializar",i);
		//indice = hallarIndice(ESTADO_NEW,nuevoEntrenador);
		//list_remove(ESTADO_NEW,0);
	//	list_add(ESTADO_READY, (void*) nuevoEntrenador);
		//list_add(ESTADO_READY,(void*)nuevoEntrenador);
		//sem_post(&counterReady);
		//log_debug(logger,"Hago sem_post");
		//sem_post(&entrenadoresDisponibles);
		pthread_mutex_unlock(&mutexPlani);

	}

	void _borrarAtrapado(void *arg){
		char *aBorrar = (char*)arg;
		bool _estaAtrapado(void*arg){
			char *poke = (char*)arg;
			return strcmp(aBorrar,poke)==0;
		}

		list_remove_by_condition(pendientes,_estaAtrapado);
	}
	if(list_size(atrapados)>0)
	list_iterate(atrapados,_borrarAtrapado);
//	int j;
//	for(j=0;j<cantidadEntrenadores;j++){
//	pthread_create(...,NULL,planificarEntrenadores,(void*)j);
//	}

//	log_debug(logger,"Inicio del team... Lista de pendientes y atrapados: ");
//	log_debug(logger,"----Pendientes----");
//	mostrarListaChar(pendientes);
//	log_debug(logger,"----Atrapados----");
//	mostrarListaChar(atrapados);

	free(posicionEntrenadores);
//list_destroy(objetivos);
//list_destroy(pokemons);
	//list_destroy(auxObj);
	//list_destroy(auxPok);
	//list_destroy(auxPos);

}
//Todo revisar memoria acá tambien.
void cargarConfigTeam() {

	TEAMTConfig = config_create(TEAM_CONFIG_PATH);
	if (TEAMTConfig == NULL) {
		perror("No se pudo leer la configuracion\n");
		exit(2);
	}

	teamConf = (t_TEAMConfig*) malloc(sizeof(t_TEAMConfig)); //Reservando memoria
//	log_info(logger, "Comenzando a importar configuracion");
	teamConf->POKEMON_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"POKEMON_ENTRENADORES");
	pokemonEntrenadores = list_create();
	splitList(teamConf->POKEMON_ENTRENADORES, pokemonEntrenadores);
//	mostrarListaChar(pokemonEntrenadores);

	teamConf->POSICIONES_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"POSICIONES_ENTRENADORES");
	posicionEntrenadores = list_create();
	splitList(teamConf->POSICIONES_ENTRENADORES, posicionEntrenadores);
	//mostrarListaChar(posicionEntrenadores);

	teamConf->OBJETIVOS_ENTRENADORES = config_get_array_value(TEAMTConfig,
			"OBJETIVOS_ENTRENADORES");
	objetivoEntrenadores = list_create();
	splitList(teamConf->OBJETIVOS_ENTRENADORES, objetivoEntrenadores);
	//mostrarListaChar(objetivoEntrenadores);

	teamConf->TIEMPO_RECONEXION = config_get_int_value(TEAMTConfig,
			"TIEMPO_RECONEXION");
//	log_info(logger, "Lei TIEMPO_RECONEXION=%d de la configuracion\n",
//			teamConf->TIEMPO_RECONEXION);

	teamConf->RETARDO_CICLO_CPU = config_get_int_value(TEAMTConfig,
			"RETARDO_CICLO_CPU");
//	log_info(logger, "Lei RETARDO_CICLO_CPU=%d de la configuracion\n",
//			teamConf->RETARDO_CICLO_CPU);

	teamConf->ALGORITMO_PLANIFICACION = string_duplicate(
			config_get_string_value(TEAMTConfig, "ALGORITMO_PLANIFICACION"));
//	log_info(logger, "Lei ALGORITMO_PLANIFICACION=%s de la configuracion\n",
//			teamConf->ALGORITMO_PLANIFICACION);

	teamConf->QUANTUM = config_get_int_value(TEAMTConfig, "QUANTUM");
	//log_info(logger, "Lei QUANTUM=%d de la configuracion\n", teamConf->QUANTUM);

	teamConf->ESTIMACION_INICIAL = config_get_double_value(TEAMTConfig,
			"ESTIMACION_INICIAL");
//	log_info(logger, "Lei ESTIMACION_INICIAL=%f de la configuracion\n",
//			teamConf->ESTIMACION_INICIAL);

	teamConf->IP_BROKER = string_duplicate(
			config_get_string_value(TEAMTConfig, "IP_BROKER"));

//	log_info(logger, "Lei IP_BROKER=%s de la configuracion\n",
//			teamConf->IP_BROKER);

	teamConf->ALPHA = config_get_double_value(TEAMTConfig, "ALPHA");
	//log_info(logger, "Lei ALPHA=%f de la configuracion\n", teamConf->ALPHA);

	teamConf->PUERTO_BROKER = config_get_int_value(TEAMTConfig,
			"PUERTO_BROKER");
//	log_info(logger, "Lei PUERTO_BROKER=%d de la configuracion\n",
//			teamConf->PUERTO_BROKER);

	teamConf->LOG_FILE = string_duplicate(
			config_get_string_value(TEAMTConfig, "LOG_FILE"));
//	log_info(logger, "Lei LOG_FILE=%s de la configuracion\n",
//			teamConf->LOG_FILE);

	teamConf->NOMBRE_PROCESO = string_duplicate(
			config_get_string_value(TEAMTConfig, "NOMBRE_PROCESO"));

	teamConf->PUERTO_TEAM = config_get_int_value(TEAMTConfig,"PUERTO_TEAM");

	teamConf->IP_TEAM = string_duplicate(config_get_string_value(TEAMTConfig,"IP_TEAM"));

//	log_info(logger, "Lei NOMBRE_PROCESO=%s de la configuracion\n",
	//		teamConf->NOMBRE_PROCESO);

	cantidadEntrenadores = posicionEntrenadores->elements_count;
	//log_info(logger, "Este equipo tiene %d entrenadores", cantidadEntrenadores);

//Esta funcion recibe todoo esto porque me estoy atajando.
//	crearEntrenadores(posicionEntrenadores, pokemonEntrenadores,
//			objetivoEntrenadores);
	inicializarLoggerEntregable();
	crearEntrenadores();

//Fin de importar configuracion
	//log_info(logger, "CONFIGURACION IMPORTADA\n");

// 	if(TEAMTConfig!=NULL){
//	config_destroy(TEAMTConfig);
// 	}
	ejecuta = malloc(sizeof(pthread_mutex_t) * cantidadEntrenadores);
	ciclosPorEntrenador = malloc(sizeof(int) * cantidadEntrenadores);
	int i;
	for (i = 0; i < cantidadEntrenadores; i++) {
		ciclosPorEntrenador[i] = 0;
	}
	cambiosDeContexto = 0;

	return;
}

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

	//printf("Entre en estaEn\n");

	t_list *aux = list_duplicate(lista);
	bool flag = false;
//	t_link_element *limpieza;
	while (aux->head != NULL) {
		//printf("Comparando %s y %s\n", (char*) aux->head->data,
			//	(char*) elemento);

		if ((strcmp((char*) aux->head->data, (char*) elemento) == 0)) {
			flag = true;
		}
		//limpieza = aux->head;
		aux->head = aux->head->next;
		//free(limpieza);
	}
	list_destroy(aux);
	return flag;

}

bool contieneId(t_list *lista,int id){
	t_list *aux = list_duplicate(lista);
	bool flag = false;
//	t_link_element *limpieza;
	while(aux->head != NULL){
		if((int)aux->head->data==id){
			flag =true;
		}
		//limpieza= aux->head;
		aux->head = aux->head->next;
	//	free(limpieza);
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
	list_destroy(aux);

	return aDevolver;
}

void* pedirPokemons(void *arg) {
//int socket = *(int*) socketBroker;
	t_list* pokemonGet = sinRepetidos(objetivoGlobal);
//	printf("El objetivo global del TEAM es: \n");
//	mostrarLista(objetivoGlobal);
//	printf("Sin repetidos es: \n");
//	mostrarLista(pokemonGet);
	//printf("Se pediran los siguientes pokemons: \n");
	//mostrarListaChar(pokemonGet);

	void _realizarGet(void* elemento) {
		int socketEnviar = -1;
		socketEnviar = crearConexionSinReintento(teamConf->IP_BROKER,
				teamConf->PUERTO_BROKER);
		char *pokemon = (char*) elemento;
		if (socketEnviar >= 0) {
			//log_debug(logger, "El socket vale %d", socketEnviar);
			enviarMensajeBrokerGet(pokemon, socketEnviar);
			t_paquete *idMensaje = malloc(sizeof(t_paquete));
			idMensaje = recibirMensaje(socketEnviar);
		//	printf("Voy a agregar a la lista de id: %d\n",
		//			idMensaje->buffer->idMensaje);
			//Todo mutex aca
			pthread_mutex_lock(&mutexIdGet);
			list_add(listaIdGet, (void*) idMensaje->buffer->idMensaje);
			pthread_mutex_unlock(&mutexIdGet);
			liberarConexion(socketEnviar);
		} else {
			//log_error(logger, "Broker desconectado");
			log_error(logEntrega,"No se pudo conectar con el broker, se toma comportamiento default para GET_POKEMON %s",pokemon);
		}

		//sleep(1);
	}

	list_iterate(pokemonGet, _realizarGet);
	list_destroy(pokemonGet);
//mostrarListaInt(listaId);

//liberarConexion(socket);
	pthread_exit(NULL);
	return NULL;
}

void *escucharGameboy() {
	pthread_t threadId[MAX_CONEXIONES];

	int contadorConexiones = 0;
//	pthread_t hilo;
	//pthread_create(&hilo, NULL, administrarMensajes, NULL);

	int socketDelCliente[MAX_CONEXIONES];
	struct sockaddr direccionCliente;
	unsigned int tamanioDireccion = sizeof(direccionCliente);

	int servidor = initServer(teamConf->IP_TEAM, teamConf->PUERTO_TEAM);

	//log_info(logger, "ESCHUCHANDO CONEXIONES");
	//log_info(logger, "iiiiIIIII!!!");
	//printf("ESCUCHANDO CONEXIONES III!!!!!!\n");

	while (1) {
		//printf("ESPERA ACTIVA? EscucharGameBoy\n");
		socketDelCliente[contadorConexiones] = accept(servidor,
				(void*) &direccionCliente, &tamanioDireccion);
		//printf("Es bloqueante?\n");
		if (socketDelCliente >= 0) {

//			log_info(logger, "Se ha aceptado una conexion: %i\n",
//					socketDelCliente[contadorConexiones]);
		//	printf("Se ha aceptado una conexion: %i\n",
		//			socketDelCliente[contadorConexiones]);
			if ((pthread_create(&threadId[contadorConexiones], NULL,
					recvMensajes, (void*) &socketDelCliente[contadorConexiones]))
					< 0) {
				pthread_detach(threadId[contadorConexiones]);
				//log_info(logger, "No se pudo crear el hilo");
			//	printf("No se pudo crear hilo\n");
				//return 1;
			} else {
				//log_info(logger, "Handler asignado\n");
				//printf("Handler asignado\n");
				tamanioDireccion = 0;
				//pthread_join(threadId[contadorConexiones], NULL)

			}
		} else {
			//log_info(logger, "Falló al aceptar conexión");
		//	printf("Fallo al aceptar conexion\n");
		}
		contadorConexiones++;

	}
	pthread_exit(NULL);
	//pthread_join(hilo, NULL);

}

void iniciarListasColas() {
	//printf("Creando listas de ejecucion\n");
	ESTADO_NEW = list_create();
	ESTADO_BLOCKED = list_create();
	ESTADO_EXEC = NULL;
	ESTADO_EXIT = list_create();
	ESTADO_READY = list_create();
	bandejaDeMensajes = queue_create();
	appearedPokemon = queue_create();

	listaIdGet = list_create();
	listaIdCatch = list_create();
	proximosEjecutar = list_create();
	procesosEnDeadlock = list_create();
	especiesEnMapa = list_create();
	dormidos = list_create();
	esperandoRespuesta = list_create();
	pendientes = list_create();
	atrapados = list_create();
	listaDeadlock = list_create();
	appearedExtra = list_create();

	return;
}
void calculoEstimacionSjf(t_entrenador *entrenador) {
	double alpha = teamConf->ALPHA;
//	log_debug(logger,"ALPHA: %f - Estimacion Rafaga anterior : %f - Rafaga real: %f",teamConf->ALPHA,entrenador->estimacionRafagaActual,entrenador->ultimaRafaga);
//Modifica la estimacionRafagaActual del entrenador pasado por parametro, ver el /1000 si es necesario.
	double t,estimacion;
	t= 1-alpha;
	//printf("%f * %f + (1-%f)*%f\n",alpha,entrenador->ultimaRafaga,alpha,entrenador->rafaga);
	//double aPorTi = alpha*entrenador->estimacionRafagaActual;
	//double tPorR = t*entrenador->ultimaRafaga;
	estimacion =(alpha*entrenador->estimacionRafagaActual + t*entrenador->ultimaRafaga);
//	printf("%f\n",estimacion);
	entrenador->estimacionRafagaActual = estimacion;
	entrenador->rafagaPendiente=estimacion;
	//log_debug(logger,"Estimacion refaga: %f",entrenador->estimacionRafagaActual);
}

double entrenadorMenorRafaga(t_list *entrenadores){
	t_entrenador *menorRafaga;
		switch (list_size(entrenadores)) {
		case 0: {
			log_error(logger, "No hay entrenadores para ejecutar!");
			menorRafaga = NULL;
			break;
		}
		case 1: {
			log_info(logger,"Hay un solo entrenador");
			menorRafaga = (t_entrenador*) list_get(entrenadores, 0);
			log_debug(logger,"Estimacion Rafaga entrenador %d : %f",menorRafaga->indice,menorRafaga->estimacionRafagaActual);
			break;
		}
		default: {
			t_entrenador *aux1;
			t_entrenador *aux2;
			int posicion = 0;
			aux1 = list_get(entrenadores, 0);

			for (int i = 1; i < list_size(entrenadores); i++) {
				aux2 = list_get(entrenadores, i);
				log_debug(logger,"Estimacion Rafaga entrenador %d : %f",aux2->indice,aux2->estimacionRafagaActual);
				if (aux1->estimacionRafagaActual >= aux2->estimacionRafagaActual) {
					aux1 = aux2;
					posicion = i;
				}
			}
			menorRafaga = (t_entrenador*) list_get(entrenadores, posicion);
		}

		}

		if(menorRafaga != NULL)
		return menorRafaga->estimacionRafagaActual;
		else
		return 10000;
}

t_entrenador *buscarMenorRafaga(t_list *entrenadores) {
	t_entrenador *menorRafaga;
	switch (list_size(entrenadores)) {
	case 0: {
		log_error(logger, "No hay entrenadores para ejecutar!");
		menorRafaga = NULL;
		break;
	}
	case 1: {
		log_info(logger,"Hay un solo entrenador");
		menorRafaga = (t_entrenador*) list_remove(entrenadores, 0);
		log_debug(logger,"Estimacion Rafaga entrenador %d : %f",menorRafaga->indice,menorRafaga->estimacionRafagaActual);
		break;
	}
	default: {
		t_entrenador *aux1;
		t_entrenador *aux2;
		int posicion = 0;
		aux1 = list_get(entrenadores, 0);

		for (int i = 1; i < list_size(entrenadores); i++) {
			aux2 = list_get(entrenadores, i);
			log_debug(logger,"Estimacion Rafaga entrenador %d : %f",aux2->indice,aux2->estimacionRafagaActual);
			if (aux1->estimacionRafagaActual >= aux2->estimacionRafagaActual) {
				aux1 = aux2;
				posicion = i;
			}
		}
		menorRafaga = (t_entrenador*) list_remove(entrenadores, posicion);
	}

	}

	return menorRafaga;
}

int hallarIndice(t_entrenador *entrenador, t_list *lista) {
	t_list *aux = list_duplicate(lista);
	int indice = 0;
	int hallado = 0;
	if (!list_is_empty(lista)) {
		while (aux->head != NULL
				&& ((t_entrenador*) aux->head->data)->indice
						!= entrenador->indice) {
			aux->head = aux->head->next;
			indice++;
//			if (((t_entrenador*)aux->head->data)->indice == entrenador->indice)
//				hallado = 1;
		}

		if (aux->head != NULL) {
			t_entrenador *buscado = (t_entrenador*) aux->head->data;

			if (buscado->indice == entrenador->indice)
				hallado = 1;

			if (hallado == 1) {
				list_destroy(aux);
				return indice;
			} else {
				list_destroy(aux);
				return -1;
			}
		} else {
			list_destroy(aux);
			return -1;
		}

	} else {
		list_destroy(aux);
		return -1;
	}
}

void *suscribirseBrokerAppearedLegacy() {
	int socketSuscripcion = crearConexion(teamConf->IP_BROKER,
			teamConf->PUERTO_BROKER, teamConf->TIEMPO_RECONEXION);

	suscribirseAppeared(teamConf->NOMBRE_PROCESO, 0, socketSuscripcion);
	pthread_mutex_t mutexRecibir;
	pthread_mutex_init(&mutexRecibir, NULL);

	t_paquete *bufferLoco;

	int flag = 1;
	while (flag) {
		//printf("ESPERA ACTIVA suscribirseBrokerAppeared? \n");
		//log_debug(logger, "Sali del if");
		pthread_mutex_lock(&mutexRecibir);
		bufferLoco = recibirMensaje(socketSuscripcion);

		if (bufferLoco != NULL) {
			enviarAck(teamConf->NOMBRE_PROCESO,bufferLoco,socketSuscripcion);
			pthread_mutex_lock(&mutex_bandeja);
			queue_push(bandejaDeMensajes, (void*) bufferLoco);
			pthread_mutex_unlock(&mutex_bandeja);
			pthread_mutex_unlock(&mutexRecibir);
			sem_post(&contadorBandeja);
		} else {
			liberarConexion(socketSuscripcion);
			socketSuscripcion = 0;
			while (socketSuscripcion <= 0) {
				pthread_mutex_unlock(&mutexRecibir);
				//log_debug(logger, "Reintentando en %d segundos",
					//	teamConf->TIEMPO_RECONEXION);
				sleep(teamConf->TIEMPO_RECONEXION);

				socketSuscripcion = crearConexionSinReintento(
						teamConf->IP_BROKER, teamConf->PUERTO_BROKER);
			//	log_debug(logger, "Socket %d", socketSuscripcion);
			}

			suscribirseAppeared(teamConf->NOMBRE_PROCESO, 0, socketSuscripcion);
		}

	}
	pthread_exit(NULL);
	return NULL;
}

void *suscribirseBrokerAppeared() {
	int socketSuscripcion = 0;

	int flag = 1;

	pthread_mutex_t mutexRecibir;
	pthread_mutex_init(&mutexRecibir,NULL);

	t_paquete *bufferLoco;

	while(flag){
		log_info(logEntrega,"Se intenta conectar al broker para suscribirse a la cola APPEARED_POKEMON");
		socketSuscripcion = crearConexionSinReintento(teamConf->IP_BROKER, teamConf->PUERTO_BROKER);
		if(socketSuscripcion>0){
			log_info(logEntrega,"Se conecto al broker satisfactoriamente");
			suscribirseAppeared(teamConf->NOMBRE_PROCESO, 0, socketSuscripcion);
			printf("me suscribir a appeared\n");
			int flag2 = 1;
			while(flag2){
			//	printf("en el mutex recibir\n");
				pthread_mutex_lock(&mutexRecibir);
			//	printf("despues de mutex recibir\n");
				bufferLoco = recibirMensaje(socketSuscripcion);

				if (bufferLoco != NULL) {
				enviarAck(teamConf->NOMBRE_PROCESO,bufferLoco,socketSuscripcion);
				pthread_mutex_lock(&mutex_bandeja);
				queue_push(bandejaDeMensajes, (void*) bufferLoco);
				pthread_mutex_unlock(&mutex_bandeja);
				pthread_mutex_unlock(&mutexRecibir);
				sem_post(&contadorBandeja);
			}
			else {
				pthread_mutex_unlock(&mutexRecibir);
			log_info(logEntrega,"Se desconecto del broker");
			liberarConexion(socketSuscripcion);
			socketSuscripcion = 0;
			flag2=0;
			log_info(logEntrega,"Se reintentara en %d segundos",teamConf->TIEMPO_RECONEXION);
			sleep(teamConf->TIEMPO_RECONEXION);
			}
		}
	}
	else
	{
	log_info(logEntrega,"Fallo al conectarse al broker");
	log_info(logEntrega,"Se reintentara en %d segundos",teamConf->TIEMPO_RECONEXION);
				sleep(teamConf->TIEMPO_RECONEXION);
	}
}
	pthread_exit(NULL);
	return NULL;
}

void *suscribirseBrokerLocalizedLegacy() {
	int socketSuscripcion = crearConexion(teamConf->IP_BROKER,
			teamConf->PUERTO_BROKER, teamConf->TIEMPO_RECONEXION);

	suscribirseLocalized(teamConf->NOMBRE_PROCESO, 0, socketSuscripcion);
	pthread_mutex_t mutexRecibir;
	pthread_mutex_init(&mutexRecibir, NULL);

	t_paquete *bufferLoco;

	int flag = 1;
	while (flag) {
		//printf("ESPERA ACTIVA suscribirseBrokerLocalized? \n");
		pthread_mutex_lock(&mutexRecibir);
		//log_debug(logger,"Esperando mensaje localized");
		bufferLoco = recibirMensaje(socketSuscripcion);

		if (bufferLoco != NULL) {
			enviarAck(teamConf->NOMBRE_PROCESO,bufferLoco,socketSuscripcion);
			pthread_mutex_lock(&mutex_bandeja);
			queue_push(bandejaDeMensajes, (void*) bufferLoco);
			pthread_mutex_unlock(&mutex_bandeja);
			pthread_mutex_unlock(&mutexRecibir);
			sem_post(&contadorBandeja);
		} else {
			liberarConexion(socketSuscripcion);
			socketSuscripcion = 0;
			while (socketSuscripcion <= 0) {
				pthread_mutex_unlock(&mutexRecibir);
				//log_debug(logger, "Reintentando en %d segundos",
				//		teamConf->TIEMPO_RECONEXION);
				sleep(teamConf->TIEMPO_RECONEXION);

				socketSuscripcion = crearConexionSinReintento(
						teamConf->IP_BROKER, teamConf->PUERTO_BROKER);
			//	log_debug(logger, "Socket %d", socketSuscripcion);
			}

			suscribirseLocalized(teamConf->NOMBRE_PROCESO, 0,
					socketSuscripcion);
		}

	}
	pthread_exit(NULL);
	return NULL;
}

void *suscribirseBrokerLocalized() {
	int socketSuscripcion = 0;

	int flag = 1;

	pthread_mutex_t mutexRecibir;
	pthread_mutex_init(&mutexRecibir,NULL);

	t_paquete *bufferLoco;

	while(flag){
		log_info(logEntrega,"Se intenta conectar al broker para suscribirse a la cola LOCALIZED_POKEMON");
		socketSuscripcion = crearConexionSinReintento(teamConf->IP_BROKER, teamConf->PUERTO_BROKER);
		if(socketSuscripcion>0){
			log_info(logEntrega,"Se conecto al broker satisfactoriamente");
			suscribirseLocalized(teamConf->NOMBRE_PROCESO, 0, socketSuscripcion);
			int flag2 = 1;
			while(flag2){

				pthread_mutex_lock(&mutexRecibir);
				bufferLoco = recibirMensaje(socketSuscripcion);

				if (bufferLoco != NULL) {
				enviarAck(teamConf->NOMBRE_PROCESO,bufferLoco,socketSuscripcion);
				pthread_mutex_lock(&mutex_bandeja);
				queue_push(bandejaDeMensajes, (void*) bufferLoco);
				pthread_mutex_unlock(&mutex_bandeja);
				pthread_mutex_unlock(&mutexRecibir);
				sem_post(&contadorBandeja);
			}
			else {
				pthread_mutex_unlock(&mutexRecibir);
			log_info(logEntrega,"Se desconecto del broker");
			liberarConexion(socketSuscripcion);
			socketSuscripcion = 0;
			flag2=0;
			log_info(logEntrega,"Se reintentara en %d segundos",teamConf->TIEMPO_RECONEXION);
			sleep(teamConf->TIEMPO_RECONEXION);
			}
		}
	}
	else
	{
	log_info(logEntrega,"Fallo al conectarse al broker");
	log_info(logEntrega,"Se reintentara en %d segundos",teamConf->TIEMPO_RECONEXION);
				sleep(teamConf->TIEMPO_RECONEXION);
	}
}
	pthread_exit(NULL);
	return NULL;
}

void *suscribirseBrokerCaughtLegacy() {
	int socketSuscripcion = crearConexion(teamConf->IP_BROKER,
			teamConf->PUERTO_BROKER, teamConf->TIEMPO_RECONEXION);

	suscribirseCaught(teamConf->NOMBRE_PROCESO, 0, socketSuscripcion);
	pthread_mutex_t mutexRecibir;
	pthread_mutex_init(&mutexRecibir, NULL);

	t_paquete *bufferLoco;

	int flag = 1;
	while (flag) {
	//	printf("ESPERA ACTIVA suscribirseBrokerCaught? \n");
		pthread_mutex_lock(&mutexRecibir);
		bufferLoco = recibirMensaje(socketSuscripcion);

		if (bufferLoco != NULL) {
			enviarAck(teamConf->NOMBRE_PROCESO,bufferLoco,socketSuscripcion);
			pthread_mutex_lock(&mutex_bandeja);
			queue_push(bandejaDeMensajes, (void*) bufferLoco);
			pthread_mutex_unlock(&mutex_bandeja);
			pthread_mutex_unlock(&mutexRecibir);
			sem_post(&contadorBandeja);
		} else {
			liberarConexion(socketSuscripcion);
			socketSuscripcion = 0;
			while (socketSuscripcion <= 0) {
				pthread_mutex_unlock(&mutexRecibir);
			//	log_debug(logger, "Reintentando en %d segundos",
				//		teamConf->TIEMPO_RECONEXION);
				sleep(teamConf->TIEMPO_RECONEXION);

				socketSuscripcion = crearConexionSinReintento(
						teamConf->IP_BROKER, teamConf->PUERTO_BROKER);
				//log_debug(logger, "Socket %d", socketSuscripcion);
			}

			suscribirseCaught(teamConf->NOMBRE_PROCESO, 0, socketSuscripcion);
		}

	}
	pthread_exit(NULL);
	return NULL;
}


void *suscribirseBrokerCaught() {
	int socketSuscripcion = 0;

	int flag = 1;

	pthread_mutex_t mutexRecibir;
	pthread_mutex_init(&mutexRecibir,NULL);

	t_paquete *bufferLoco;

	while(flag){
		log_info(logEntrega,"Se intenta conectar al broker para suscribirse a la cola CAUGHT_POKEMON");
		socketSuscripcion = crearConexionSinReintento(teamConf->IP_BROKER, teamConf->PUERTO_BROKER);
		if(socketSuscripcion>0){
			log_info(logEntrega,"Se conecto al broker satisfactoriamente");
			suscribirseCaught(teamConf->NOMBRE_PROCESO, 0, socketSuscripcion);
			int flag2 = 1;
			while(flag2){
				pthread_mutex_lock(&mutexRecibir);
				bufferLoco = recibirMensaje(socketSuscripcion);

				if (bufferLoco != NULL) {
				enviarAck(teamConf->NOMBRE_PROCESO,bufferLoco,socketSuscripcion);
				pthread_mutex_lock(&mutex_bandeja);
				queue_push(bandejaDeMensajes, (void*) bufferLoco);
				pthread_mutex_unlock(&mutex_bandeja);
				pthread_mutex_unlock(&mutexRecibir);
				sem_post(&contadorBandeja);
			}
			else {
			pthread_mutex_unlock(&mutexRecibir);
			log_info(logEntrega,"Se desconecto del broker");
			liberarConexion(socketSuscripcion);
			socketSuscripcion = 0;
			flag2=0;
			log_info(logEntrega,"Se reintentara en %d segundos",teamConf->TIEMPO_RECONEXION);
			sleep(teamConf->TIEMPO_RECONEXION);
			}
		}
	}
	else
	{
	log_info(logEntrega,"Fallo al conectarse al broker");
	log_info(logEntrega,"Se reintentara en %d segundos",teamConf->TIEMPO_RECONEXION);
				sleep(teamConf->TIEMPO_RECONEXION);
	}
}
	pthread_exit(NULL);
	return NULL;
}

void suscribirseColasBroker() {
	pthread_t brokerAppeared, brokerLocalized, brokerCaught;
	pthread_create(&brokerAppeared, NULL, suscribirseBrokerAppeared, NULL);
	pthread_detach(brokerAppeared);
	pthread_create(&brokerLocalized, NULL, suscribirseBrokerLocalized, NULL);
	pthread_detach(brokerLocalized);
	pthread_create(&brokerCaught, NULL, suscribirseBrokerCaught, NULL);
	pthread_detach(brokerCaught);
}

bool buscarPokemon(char *pokemon){
		bool _esIgualAPokemon(void *arg){
			t_paquete *compare = (t_paquete*)arg;
			return strcmp(compare->buffer->nombrePokemon,pokemon)==0;
		}

		t_paquete *buscado = list_find(appearedExtra,_esIgualAPokemon);

		if(buscado != NULL){
			pthread_mutex_lock(&mutexListaPokemons);
			queue_push(appearedPokemon, (void*) buscado);
			pthread_mutex_unlock(&mutexListaPokemons);
			sem_post(&pokemonsEnLista);
			return true;
		}
		else
			return false;



}
