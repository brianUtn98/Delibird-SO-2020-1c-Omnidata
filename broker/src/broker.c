#include "broker.h"

int main(void) {

	//int flag = 1;

	inicializarLogger();
	cargarConfigBROKER();
	log_info(logger, "Desde el main veo IP=%s y PUERTO=%d",
			brokerConf->ipBroker, brokerConf->puertoBroker);
	inicializarColasBroker();

//	int socketServidor = iniciarServidor(brokerConf->ipBroker,
//			brokerConf->puertoBroker);

	int socketDelCliente;
	struct sockaddr direccionCliente;
	unsigned int tamanioDireccion = sizeof(direccionCliente);
	int servidor = init_server(brokerConf->puertoBroker);

	log_info(logger, "ESCHUCHANDO CONEXIONES");
	log_info(logger, "iiiiIIIII!!!");
	while ((socketDelCliente = accept(servidor, (void*) &direccionCliente,
			&tamanioDireccion)) >= 0) {
		pthread_t threadId;
		log_info(logger, "Se ha aceptado una conexion: %i\n", socketDelCliente);
		if ((pthread_create(&threadId, NULL, handler, (void*) &socketDelCliente))
				< 0) {
			log_info(logger, "No se pudo crear el hilo");
			return 1;
		} else {
			log_info(logger, "Handler asignado\n");
		}

	}
	if (socketDelCliente < 0) {
		log_info(logger, "Falló al aceptar conexión");
	}

	close(servidor);

	free(brokerConf);
	return EXIT_SUCCESS;
}

