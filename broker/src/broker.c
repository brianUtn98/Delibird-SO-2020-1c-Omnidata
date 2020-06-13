#include "broker.h"

int main(void) {

	inicializarLogger();
	cargarConfigBROKER();
	log_info(logger, "Desde el main veo IP=%s y PUERTO=%d",
			brokerConf->ipBroker, brokerConf->puertoBroker);
	inicializarColasBroker();
	inicializarEstructuras();
	idMensajeUnico = 0;
	idMensajeCorrelativo = 0;// ver como se inicializa esto y como se usa

	pedirMemoriaInicial();
	//manejarMemoria();

	pthread_mutex_init(&bandejaMensajes_mutex, NULL);
	pthread_mutex_init(&recibir_mutex, NULL);
	sem_init(&bandejaCounter, 1, 0);
	//pthread_mutex_lock(&bandejaMensajes_mutex);

	bandejaDeMensajes = list_create();
	contadorDeMensajes = 0;
	bandeja = queue_create();

	pthread_t threadId[MAX_CONEXIONES];

	int contadorConexiones = 0;
	pthread_t hilo;
	pthread_create(&hilo, NULL, administrarMensajes, NULL);

	int socketDelCliente[MAX_CONEXIONES];
	struct sockaddr direccionCliente;
	unsigned int tamanioDireccion = sizeof(direccionCliente);

	int servidor = initServer(brokerConf->ipBroker, brokerConf->puertoBroker);

	log_info(logger, "ESCHUCHANDO CONEXIONES");
	log_info(logger, "iiiiIIIII!!!");

	while (1) {

		socketDelCliente[contadorConexiones] = accept(servidor,
				(void*) &direccionCliente, &tamanioDireccion);

		if (socketDelCliente >= 0) {

			log_info(logger, "Se ha aceptado una conexion: %i\n",
					socketDelCliente[contadorConexiones]);
			if ((pthread_create(&threadId[contadorConexiones], NULL, handler,
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

	}

	pthread_join(hilo, NULL);

	destruirColasBroker();
	free(brokerConf);
	return EXIT_SUCCESS;
}

