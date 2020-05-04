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

	int socketServidor = iniciarServidor(brokerConf->ipBroker,
			brokerConf->puertoBroker);

	while (flag) {
		//esperarCliente(server);

		struct sockaddr_in dirCliente;
		unsigned int tamDireccion = 0;
		int socketCliente = accept(socketServidor, (void*) &dirCliente,
				&tamDireccion);

		printf("Recibi una conexion en %d\n", socketCliente);
		//cambié (void*)serveClient por serveClient a secas.
		//Agregué (void*) en el cuarto parámetro.
		pthread_create(&thread, NULL,serveClient,(void*) &socketCliente);
		pthread_detach(thread);
	}

	free(brokerConf);
	return EXIT_SUCCESS;
}

//void esperarCliente(int socketServidor)
//{
//	struct sockaddr_in dirCliente;
//	unsigned int tamDireccion=0;
//	int socketCliente = accept(socketServidor, (void*)&dirCliente, &tamDireccion);
//
//	printf("Recibi una conexion en %d\n",socketCliente);
//	pthread_create(&thread,NULL,(void*)serveClient,&socketCliente);
//	pthread_detach(thread);
//	printf("Estoy despues del detach\n");
//}

