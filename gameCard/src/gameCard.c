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

	bandejaMensajesGameCard = list_create();
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
		//pthread_create(&suscripcion,NULL,suscribirseABroker,NULL);

		for (;;) {

		}

		pthread_join(hiloEscucha, NULL);
		pthread_join(hilo, NULL);



	// LOGGEAR MENSAJE

		free(gameCardConfig);
	terminarProgramaGameCard();
	return EXIT_SUCCESS;

}

