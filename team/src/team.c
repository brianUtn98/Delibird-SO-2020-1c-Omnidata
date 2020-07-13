#include "team.h"

//El team todavía está en pañales, le falta agarrar los entrenadores, instanciarlos de alguna manera
//y planificarlos. supongo que habrá que usar pthread.

int main(void) {

	deadlocksTotales = 0;
	deadlocksResueltos = 0;
	inicializarLoggerTeam();
	iniciarListasColas();

	cargarConfigTeam();

	inicializarLoggerEntregable();

	log_info(logger, "El objetivo global de este equipo es: ");
	mostrarListaChar(objetivoGlobal);

//	int socketBroker = 0;
//	socketBroker = crearConexion(teamConf->IP_BROKER, teamConf->PUERTO_BROKER,
//			teamConf->TIEMPO_RECONEXION);

	printf("Estoy antes de planificar\n");
	threads_entreanadores = malloc(sizeof(pthread_t) * cantidadEntrenadores);
	ejecuta = malloc(sizeof(pthread_mutex_t) * cantidadEntrenadores);

	administrativo = malloc(
			sizeof(t_administrativoEntrenador) * cantidadEntrenadores);

	inicializarMutex();
	pthread_mutex_lock(&mutexPlani);

	pthread_t hiloPlani;

	if (strcmp(teamConf->ALGORITMO_PLANIFICACION, "FIFO") == 0) {
		pthread_create(&hiloPlani, NULL, planificarEntrenadores,
		NULL);
	}
	if (strcmp(teamConf->ALGORITMO_PLANIFICACION, "RR") == 0) {
		pthread_create(&hiloPlani, NULL, planificarEntrenadoresRR, NULL);
	}
	if (strcmp(teamConf->ALGORITMO_PLANIFICACION, "SJF") == 0) {
		pthread_create(&hiloPlani, NULL, planificarEntrenadoresSJF, NULL);
	}

	pthread_t recvMsg;
	pthread_t procesarMsg;

//	printf("Estoy creando el hilo de recibir\n");
//	if (pthread_create(&recvMsg, NULL, recvMensajes, (void*) &socketBroker)
//			< 0) {
//		printf("No se pudo crear el hilo\n");
//	} else {
//		printf("Handler asignado para recibir mensajes.\n");
//	}

	printf("Estoy creando el hilo de procesar\n");
	if (pthread_create(&procesarMsg, NULL, procesarMensaje, NULL) < 0) {
		printf("No se pudo crear el hilo\n");
	} else {
		printf("Handler asignado para procesar mensajes.\n");
	}

	pthread_t tEscuchar;
	pthread_create(&tEscuchar, NULL, escucharGameboy, NULL);

	sleep(2);

	//pedirPokemons(socketBroker);
	pthread_t hiloGet;
	pthread_create(&hiloGet, NULL, pedirPokemons, NULL);
	//pthread_detach(hiloGet);

	printf("Estoy en el bucle\n");
	pthread_mutex_unlock(&mutexPlani);

	for (;;) {

	}
	//pthread_join(recvMsg, NULL);
	pthread_join(hiloPlani, NULL);
	//pthread_join(tEscuchar, NULL);
	terminarPrograma();
	return EXIT_SUCCESS;

}
