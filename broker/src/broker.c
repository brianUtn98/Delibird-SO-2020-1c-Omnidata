#include "broker.h"

int main(void) {

	inicializarLogger();
	cargarConfigBROKER();
	log_info(logger, "Desde el main veo IP=%s y PUERTO=%d",
			brokerConf->ipBroker, brokerConf->puertoBroker);
	inicializarColasBroker();
	bandejaDeMensajes = list_create();
	contadorDeMensajes = 0;

	pthread_t hilo;

	pthread_create(&hilo, NULL, administrarMensajes, NULL);
	iniciarServidorMio(brokerConf->ipBroker, brokerConf->puertoBroker);


	destruirColasBroker();
	free(brokerConf);
	return EXIT_SUCCESS;
}

