#include "gameCard.h"

int main(void) {

	inicializarMutexGameCard();

	inicializar_logger();
	cargarConfigGameCard();
	pthread_t threadId[MAX_CONEXIONES];
	pthread_t conexionBroker;

	contadorConexiones = 0;

	iniciarTallGrass();
	actualizarBlocks();

	log_info(logger, "BLOCKS MAXIMOS: %d y BLOCKS USADOS %d", blocks_maximos,
			blocks_usados);
	// Conectarse al Broker
	//suscribirmeAColasBroker();

	pthread_mutex_init(&bandejaMensajes_mutex, NULL);
	pthread_mutex_init(&recibir_mutex, NULL);
	sem_init(&bandejaCounter, 1, 0);

	bandejaDeMensajesGameCard = queue_create();

	//pthread_t hiloProcesar;
	pthread_create(&procesarProceso, NULL, procesarMensajeGameCard, NULL);
	pthread_detach(procesarProceso);

	int socketDelCliente[MAX_CONEXIONES];
	struct sockaddr direccionCliente;
	unsigned int tamanioDireccion = sizeof(direccionCliente);
	int servidor = 0;
	servidor = initServer(gameCardConfig->puertoGameCard,
			gameCardConfig->puertoGameCard);

	while (1) {

		socketDelCliente[contadorConexiones] = accept(servidor,
				(void*) &direccionCliente, &tamanioDireccion);

		if (socketDelCliente >= 0) {

			log_info(logger, "Se ha aceptado una conexion: %i\n",
					socketDelCliente[contadorConexiones]);
			if ((pthread_create(&threadId[contadorConexiones], NULL,
					recvMensajesGameCard,
					(void*) &socketDelCliente[contadorConexiones])) < 0) {
				log_info(logger, "No se pudo crear el hilo");
				pthread_detach(socketDelCliente[contadorConexiones]);
				//return 1;
			} else {
				log_info(logger, "Handler asignado\n");
				tamanioDireccion = 0;
				//pthread_join(threadId[contadorConexiones], NULL)

			}

			//pthread_create(&pruebaProcesos[contadorConexiones], NULL, procesarMensajeGameCard, NULL);

		} else {
			log_info(logger, "Falló al aceptar conexión");
		}
		contadorConexiones++;

	}

	// LOGGEAR MENSAJE

	// 4. Terminar
	terminarProgramaGameCard();
	return EXIT_SUCCESS;

}

