#include "team.h"

//El team todavía está en pañales, le falta agarrar los entrenadores, instanciarlos de alguna manera
//y planificarlos. supongo que habrá que usar pthread.

int main(void) {

	inicializarLoggerTeam();
	iniciarEstados();

	cargarConfigTeam();

	log_info(logger, "El objetivo global de este equipo es: ");
	mostrarLista(objetivoGlobal);

	int socketBroker;
	socketBroker = crearConexion(teamConf->IP_BROKER, teamConf->PUERTO_BROKER,
			teamConf->TIEMPO_RECONEXION);

	printf("Estoy antes de planificar\n");
	threads_entreanadores = malloc(sizeof(pthread_t) * cantidadEntrenadores);
	ejecuta = malloc(sizeof(pthread_mutex_t) * cantidadEntrenadores);

	inicializarMutex();
	pthread_mutex_lock(&mutexPlani);

	pthread_t hiloPlani;
	pthread_create(&hiloPlani, NULL, planificarEntrenadores,
			(void*) socketBroker);

	pthread_t recvMsg;
	pthread_t procesarMsg;


	printf("Estoy creando el hilo de recibir\n");
	if (pthread_create(&recvMsg, NULL, recvMensajes, (void*)&socketBroker)
			< 0) {
		printf("No se pudo crear el hilo\n");
	} else {
		printf("Handler asignado para recibir mensajes.\n");
	}

	printf("Estoy creando el hilo de procesar\n");
	if (pthread_create(&procesarMsg, NULL, procesarMensaje, NULL) < 0) {
		printf("No se pudo crear el hilo\n");
	} else {
		printf("Handler asignado para procesar mensajes.\n");
	}



	int i;
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

	pthread_t tEscuchar;
	pthread_create(&tEscuchar,NULL,escucharGameboy,NULL);

	sleep(2);




	//pedirPokemons(socketBroker);
	pthread_t hiloGet;
	pthread_create(&hiloGet,NULL,pedirPokemons,(void*)&socketBroker);
	pthread_detach(hiloGet);

	printf("Estoy en el bucle\n");
	pthread_mutex_unlock(&mutexPlani);

	for (;;) {

	}
	pthread_join(recvMsg, NULL);
	pthread_join(hiloPlani, NULL);
	pthread_join(tEscuchar,NULL);
	terminarPrograma();
	return EXIT_SUCCESS;

}
