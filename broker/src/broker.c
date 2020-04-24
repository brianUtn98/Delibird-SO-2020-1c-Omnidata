#include "broker.h"

int main(void) {
	inicializarLogger();
	inicializarColasBroker();
	cargarConfigBROKER();

	iniciarServidor();


	return EXIT_SUCCESS;
}
