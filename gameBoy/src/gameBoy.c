#include "gameBoy.h"

int main(int argc, char *argv[]) {

	inicializarLoggerGameBoy();
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	cargarConfigGameBoy();

//	int socketCliente = crearConexion(gameBoyConf->ipBroker,
//			gameBoyConf->puertoBroker, 30);
//	enviarMensajeRecursoNew(argv[3], (int) argv[4], (int) argv[5],
//			(int) argv[6], socketCliente);

	manejarMensajes(argc, argv);

	liberarGameBoyConfig();
	return EXIT_SUCCESS;
}
