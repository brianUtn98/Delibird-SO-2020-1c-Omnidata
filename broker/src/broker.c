#include "broker.h"

int main(void) {

	inicializarLogger();
	cargarConfigBROKER();
	log_info(logger, "Desde el main veo IP=%s y PUERTO=%d",
			brokerConf->ipBroker, brokerConf->puertoBroker);
	inicializarColasBroker();

	pthread_mutex_init(&bandejaMensajes_mutex, NULL);
	pthread_mutex_lock(&bandejaMensajes_mutex);

	bandejaDeMensajes = list_create();
	contadorDeMensajes = 0;

	pthread_t hilo;
	pthread_create(&hilo, NULL, administrarMensajes, NULL);

	iniciarServidorMio(brokerConf->ipBroker, brokerConf->puertoBroker);
	printf("estoy despues del servidor mio");


	pthread_join(hilo,NULL);
	while (1) {

	};
	destruirColasBroker();
	free(brokerConf);
	return EXIT_SUCCESS;
}

