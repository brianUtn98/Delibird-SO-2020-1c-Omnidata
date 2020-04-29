#include "broker.h"

int main(void) {
	inicializarLogger();
	inicializarColasBroker();
	cargarConfigBROKER();

	correrServidor(brokerConf->ipBroker,brokerConf->puertoBroker);


	return EXIT_SUCCESS;
}
