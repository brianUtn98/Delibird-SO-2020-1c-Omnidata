#include "gameBoy.h"

int main(int argc, char *argv[]) {

	inicializarLoggerGameBoy();
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	cargarConfigGameBoy();
	liberarGameBoyConfig();
	return EXIT_SUCCESS;
}
