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

	crearEntrenadores();

	pthread_t hiloPlani;
	pthread_create(&hiloPlani, NULL, planificarEntrenadores, //este es el encargado de enviar mensajes seguramente.
			(void*) socketBroker); //como argumento se podria pasar el algoritmo.
	//pthread_join(hiloPlani,NULL);
	pthread_t recvMsg;
	pthread_t procesarMsg;

	if (pthread_create(&recvMsg, NULL, recvMensajes, (void*) socketBroker)
			< 0) {
		printf("No se pudo crear el hilo\n");
	} else {
		printf("Handler asignado para recibir Mensajes.\n");
	}

	if (pthread_create(&procesarMsg, NULL, procesarMensaje, NULL) < 0) {
		printf("No se pudo crear el hilo\n");
	} else {
		printf("Handler asignado para recibir Mensajes.\n");
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
		//pthread_create(&entrenadorThread, NULL, manejarEntrenador, (void*)&i);
		//pthread_join(threads_entreanadores[i],NULL);
	}

	sleep(2);
	/*
	 * ESTO ESTA MAL POR LO QUE HABLAMOS CON NICO EL DOMINGO
	 * PERO ES LA UNICA FORMA DE MANDAR DOS MENSAJES DIFERENTES
	 * Y QUE EL BROKER LOS RECIBA.
	 *
	 * HAY QUE ARREGLAR
	 */

//	iniciarColasEjecucion();
// Por ahora está hardcodeado porque no se cómo obtener la ip de este proceso (no viene por config).
	pedirPokemons(socketBroker);
//	escucharGameboy("127.0.0.1", 5002);

// 3. Recibir confirmación
//char* mensaje = recibirConfirmacion(socketCliente);
//log_info(logger,"Mensaje de confirmacion recibido: %s", mensaje);
//printf("Mensaje de confirmacion recibido: %s\n", mensaje);
// LOGGEAR MENSAJE
// 4. Terminar

//liberarConexion(socketBroker);
	printf("Estoy en el bucle\n");

	for (;;) {

	}
	pthread_join(recvMsg, NULL);
	pthread_join(hiloPlani, NULL);

	terminarPrograma();
	return EXIT_SUCCESS;

}
