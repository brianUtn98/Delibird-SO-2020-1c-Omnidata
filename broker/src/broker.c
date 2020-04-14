#include "broker.h"

int main(void) {
	inicializarLogger();
	cargarConfigBROKER();
	iniciarServidor();

	//return EXIT_SUCCESS;
}
