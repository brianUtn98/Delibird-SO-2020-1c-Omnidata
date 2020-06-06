#include "team.h"

//El team todavía está en pañales, le falta agarrar los entrenadores, instanciarlos de alguna manera
//y planificarlos. supongo que habrá que usar pthread.

int main(void) {

	inicializarLoggerTeam();
	cargarConfigTeam();
	//sleep(1);
	log_info(logger, "El objetivo global de este equipo es: ");
	mostrarLista(objetivoGlobal);

	crearEntrenadores();
	iniciarEstados();

//int contador;
//pthread_t *entrenador=(pthread_t*)malloc(cantidadEntrenadores);
//	log_info(logger,"Estoy creando hilos");
//	for(contador=0;contador<cantidadEntrenadores;contador++){
//	pthread_create(entrenador[contador],NULL,(void*)manejarEntrenador,(void*)contador);
//	}

	int socketBroker;
	socketBroker = crearConexion(teamConf->IP_BROKER, teamConf->PUERTO_BROKER,
			teamConf->TIEMPO_RECONEXION);

	pthread_t hiloPlani;
	pthread_create(&hiloPlani, NULL, planificarEntrenadores, 0);

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
	for (;;)
		;

	terminarPrograma();
	return EXIT_SUCCESS;

}
