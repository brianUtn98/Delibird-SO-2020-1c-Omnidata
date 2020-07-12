#include "gameCard.h"

int main(void) {

	inicializarMutexGameCard();

	inicializar_logger();
	cargarConfigGameCard();
	//pthread_t threadId[MAX_CONEXIONES];
	//pthread_t conexionBroker;

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

	//int socketDelCliente[MAX_CONEXIONES];
	//struct sockaddr direccionCliente;
	//unsigned int tamanioDireccion = sizeof(direccionCliente);



		pthread_t hiloEscucha;
		pthread_create(&hiloEscucha, NULL, escucharConexionesGameCard, NULL);

		pthread_t hilo;
		pthread_create(&hilo, NULL,  consumirMensajesGameCard, NULL);

		//pthread_t suscripcion;
		//pthread_create(&suscripcion,NULL,suscribirme,NULL);

		for (;;) {

		}

		pthread_join(hiloEscucha, NULL);
		pthread_join(hilo, NULL);

	/*while (1) {

		socketDelCliente[contadorConexiones] = accept(servidor,
				(void*) &direccionCliente, &tamanioDireccion);

		if (socketDelCliente >= 0) {

			log_info(logger, "Se ha aceptado una conexion: %i\n",
					socketDelCliente[contadorConexiones]);

			if ((pthread_create(&threadId[contadorConexiones], NULL,
					recvMensajesGameCard,
					(void*) &socketDelCliente[contadorConexiones])) < 0) {
				log_info(logger, "No se pudo crear el hilo");
				//pthread_detach(socketDelCliente[contadorConexiones]);

			} else {

				tamanioDireccion = 0;

			}

			pthread_create(&procesarProceso, NULL, procesarMensajeGameCard,
					NULL);



			//pthread_create(&pruebaProcesos[contadorConexiones], NULL, procesarMensajeGameCard, NULL);
			//pthread_detach(pruebaProcesos[contadorConexiones]);

		} else {
			log_info(logger, "Falló al aceptar conexión");
		}
		contadorConexiones++;

	}*/

	// LOGGEAR MENSAJE

		free(gameCardConfig);
	terminarProgramaGameCard();
	return EXIT_SUCCESS;

}

