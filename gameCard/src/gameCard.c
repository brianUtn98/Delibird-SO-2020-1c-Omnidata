#include "gameCard.h"

int main(void) {

	maximo_block_creado=0;

	inicializar_logger();
	cargarConfigGameCard();
	//iniciarTallGrass();
	// Conectarse al Broker
	suscribirmeAColasBroker();
	inicializarMutexGameCard();

	t_list* t_coordenadas=list_create();
	list_add(t_coordenadas, (void*)5);
	list_add(t_coordenadas, (void*)1);
	bandejaDeMensajesGameCard = queue_create();

	//agregarNewPokemon("Pikachu", t_coordenadas, 10);

	 /*
	  * Cuando el broker nos pida que guardemos un pokemon:
	  *
	  *
	  * catchPokemon()
	  * obtenerPokemon()
	  */
	//crear socket broker para usarla en un futuro

	//int socketBroker;
		//socketBroker = crearConexion(gameCardConfig->ipBroker, gameCardConfig->puertoBroker,
			//		gameCardConfig->tiempoReintentoConexion);
	//iniciar servidor



	int socketDelCliente;
		struct sockaddr direccionCliente;
		unsigned int tamanioDireccion = sizeof(direccionCliente);
		int servidor;
		servidor = initServer(gameCardConfig->puertoGameCard, gameCardConfig->puertoGameCard);

		socketDelCliente = accept(servidor, (void*) &direccionCliente,
					&tamanioDireccion);


		pthread_t recvMsg1;
		pthread_t procesarMsg1;


			printf("Estoy creando el hilo de recibir\n");
			if (pthread_create(&recvMsg1, NULL, recvMensajesGameCard, (void*)&socketDelCliente)
					< 0) {
				printf("No se pudo crear el hilo\n");
			} else {
				printf("Handler asignado para recibir mensajes.\n");
			}

			printf("Estoy creando el hilo de procesar\n");
			if (pthread_create(&procesarMsg1, NULL, procesarMensajeGameCard, NULL) < 0) {
				printf("No se pudo crear el hilo\n");
			} else {
				printf("Handler asignado para procesar mensajes.\n");
			}

			printf("esperando /n");











	pthread_join(recvMsg1, NULL);

	// LOGGEAR MENSAJE

	// 4. Terminar
	terminarProgramaGameCard();
	return EXIT_SUCCESS;

}


