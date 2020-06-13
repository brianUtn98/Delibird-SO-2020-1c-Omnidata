#include "gameCard.h"

int main(void) {

	maximo_block_creado=0;

	inicializar_logger();
	cargarConfigGameCard();
	pthread_t threadId[MAX_CONEXIONES];


	int contadorConexiones = 0;

	//iniciarTallGrass();

	// Conectarse al Broker
	//suscribirmeAColasBroker();

	inicializarMutexGameCard();

	pthread_mutex_init(&bandejaMensajes_mutex, NULL);
	pthread_mutex_init(&recibir_mutex, NULL);
	sem_init(&bandejaCounter, 1, 0);

	bandejaDeMensajesGameCard = queue_create();

	int socketDelCliente[MAX_CONEXIONES];
	struct sockaddr direccionCliente;
	unsigned int tamanioDireccion = sizeof(direccionCliente);
	int servidor;
	servidor = initServer(gameCardConfig->puertoGameCard, gameCardConfig->puertoGameCard);

	while(1){

	socketDelCliente[contadorConexiones] = accept(servidor,
					(void*) &direccionCliente, &tamanioDireccion);

			if (socketDelCliente >= 0) {

				log_info(logger, "Se ha aceptado una conexion: %i\n",
						socketDelCliente[contadorConexiones]);
				if ((pthread_create(&threadId[contadorConexiones], NULL, handlerGameCard,
						(void*) &socketDelCliente[contadorConexiones])) < 0) {
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






	printf("esperando /n");

	}

	// LOGGEAR MENSAJE

	// 4. Terminar
	terminarProgramaGameCard();
	return EXIT_SUCCESS;

}


