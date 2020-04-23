#include "gameBoy.h"

int main(int argc, char *argv[]) {

	if (argc != 4 | 7) {
		printf("Numero de argumentos invalidos, ingrese 4 o 7.Gracias.");

	}else{
		if(argc==4){

		}else{                       //entra por el 7

		}
	}

	inicializarLogger();
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	cargarConfigGameBoy();
	return EXIT_SUCCESS;
}
