#include "broker.h"

int main(void) {

	inicializarLogger();
	cargarConfigBROKER();
	log_info(logger, "Desde el main veo IP=%s y PUERTO=%d",
			brokerConf->ipBroker, brokerConf->puertoBroker);
	inicializarColasBroker();

	iniciarServidor(brokerConf->ipBroker, brokerConf->puertoBroker);

	destruirColasBroker();
	free(brokerConf);
	return EXIT_SUCCESS;
}

