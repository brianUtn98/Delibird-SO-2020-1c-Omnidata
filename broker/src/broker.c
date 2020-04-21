#include "broker.h"

int main(void) {
	inicializarLogger();
	cargarConfigBROKER();
	inicializarColasBroker();
	iniciarServidor();


	return EXIT_SUCCESS;
}
