#include "gameBoy.h"

int main(int argc, char *argv[]) {

	inicializarLoggerGameBoy();
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	cargarConfigGameBoy();

	manejarMensajes(argc, argv);

	liberarGameBoyConfig();
	return EXIT_SUCCESS;
}
