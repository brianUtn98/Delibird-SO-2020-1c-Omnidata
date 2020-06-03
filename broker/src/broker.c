#include "broker.h"

int main(void) {

	inicializarLogger();
	cargarConfigBROKER();
	log_info(logger, "Desde el main veo IP=%s y PUERTO=%d",
			brokerConf->ipBroker, brokerConf->puertoBroker);
	inicializarColasBroker();

	pthread_mutex_init(&bandejaMensajes_mutex, NULL);
	pthread_mutex_init(&recibir_mutex,NULL);
	sem_init(&bandejaCounter,1,0);
	//pthread_mutex_lock(&bandejaMensajes_mutex);

	bandejaDeMensajes = list_create();
	contadorDeMensajes = 0;
	bandeja=queue_create();

	pthread_t hilo;
	pthread_create(&hilo, NULL, administrarMensajes, NULL);

	int socketDelCliente;
	struct sockaddr direccionCliente;
	unsigned int tamanioDireccion = sizeof(direccionCliente);
	int servidor = initServer(brokerConf->ipBroker, brokerConf->puertoBroker);

	log_info(logger, "ESCHUCHANDO CONEXIONES");
	log_info(logger, "iiiiIIIII!!!");

	pthread_t threadId;
	while (1) {

		socketDelCliente = accept(servidor, (void*) &direccionCliente,
				&tamanioDireccion);

		//if (socketDelCliente >= 0)

		log_info(logger, "Se ha aceptado una conexion: %i\n", socketDelCliente);
		if ((pthread_create(&threadId, NULL, handler, (void*) &socketDelCliente))
				< 0) {
			log_info(logger, "No se pudo crear el hilo");
			//return 1;
		} else {
			log_info(logger, "Handler asignado\n");

		}

	}
	if (socketDelCliente < 0) {
		log_info(logger, "Falló al aceptar conexión");
	}

	//iniciarServidorMio(brokerConf->ipBroker, brokerConf->puertoBroker);
	printf("estoy despues del servidor mio");

	pthread_join(hilo, NULL);
	pthread_join(threadId, NULL);

	for (;;)
		;
	destruirColasBroker();
	free(brokerConf);
	return EXIT_SUCCESS;
}

