#include "team.h"

//El team todavía está en pañales, le falta agarrar los entrenadores, instanciarlos de alguna manera
//y planificarlos. supongo que habrá que usar pthread.

int main(void) {

	inicializarLoggerTeam();
	cargarConfigTeam();

	log_info(logger, "El objetivo global de este equipo es: ");
	mostrarLista(objetivoGlobal);

	iniciarEstados();


	int socketBroker;
	socketBroker = crearConexion(teamConf->IP_BROKER, teamConf->PUERTO_BROKER,
			teamConf->TIEMPO_RECONEXION);

	printf("Estoy antes de planificar\n");
	threads_entreanadores=malloc(sizeof(pthread_t)*cantidadEntrenadores);
	ejecuta=malloc(sizeof(pthread_mutex_t)*cantidadEntrenadores);

	inicializarMutex();

	pthread_t hiloPlani;
	pthread_create(&hiloPlani, NULL, planificarEntrenadores, NULL);
	//pthread_join(hiloPlani,NULL);

	int i;
	pthread_t entrenadorThread;
	printf("Estoy por crear los  %d hilos de entrenador\n",cantidadEntrenadores);
	for(i=0;i<cantidadEntrenadores;i++){
	printf("i vale %d\n",i);
	if(pthread_create(&threads_entreanadores[i], NULL, manejarEntrenador,(void*) &entrenadores[i])<0){
		printf("No se pduo crear el hilo\n");
	}
	else
	{
		printf("Handler asignado para entrenador [%d]\n",i);
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
	int j;


	for (;;){
			for(j=0;j<cantidadEntrenadores;j++){
				sleep(1);
				pthread_mutex_unlock(&ejecuta[j]);
			}
		}

	pthread_join(hiloPlani,NULL);


	terminarPrograma();
	return EXIT_SUCCESS;

}
