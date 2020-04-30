#include "broker.h"

int main(void) {
	inicializarLogger();
	inicializarColasBroker();
	cargarConfigBROKER();

	iniciarServidor(brokerConf->ipBroker,brokerConf->puertoBroker);



	return EXIT_SUCCESS;
}
