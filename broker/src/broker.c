#include "broker.h"

int main(void) {

	//int contador = 0;
	int flag = 1;
	//esto no va,estoy hardcodeando un numero para pasarle a mensaje y compile
	//int *bufferSize = malloc(sizeof(int));

	inicializarLogger();
	cargarConfigBROKER();
	log_info(logger, "Desde el main veo IP=%s y PUERTO=%d",
			brokerConf->ipBroker, brokerConf->puertoBroker);
	inicializarColasBroker();

//	int socketDelCliente;
//	struct sockaddr direccionCliente;
//	unsigned int tamanioDireccion = sizeof(direccionCliente);
//	int servidor = iniciarServidor(brokerConf->ipBroker,
//			brokerConf->puertoBroker);

//		log_info(logger,"ESCHUCHANDO CONEXIONES");
//			log_info(logger,"iiiiIIIII!!!");
//			while((socketDelCliente = accept(servidor, (void*) &direccionCliente, &tamanioDireccion))>=0) {
//				pthread_t threadId;
//				log_info(logger, "Se ha aceptado una conexion: %i\n", socketDelCliente);
//				if((pthread_create(&threadId, NULL, handler, (void*) &socketDelCliente)) < 0) {
//					log_info(logger, "No se pudo crear el hilo");
//					return 1;
//				} else {
//					log_info(logger, "Handler asignado\n");
//				}
//
//
//			}
//			if(socketDelCliente < 0) {
//				log_info(logger, "Falló al aceptar conexión");
//			}
//
//			close(servidor);

	int server = iniciarServidor(brokerConf->ipBroker,
			brokerConf->puertoBroker);



	while (flag) {
		esperarCliente(server);

		//esperarCliente esta acoplado con recibir mensaje esta sería la ide pero no se me ocurre como
		//separalos

	}
	//este free no va, fue solo una prueba por no saber como recibir mensaje

	free(brokerConf);
	return EXIT_SUCCESS;
}
