#include "broker.h"

int main(void) {
	inicializarLogger();
	cargarConfigBROKER();
	crearColasBroker();
	iniciarServidor();


	return EXIT_SUCCESS;
}
