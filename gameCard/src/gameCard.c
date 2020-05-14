#include "gameCard.h"

int main(void) {
	inicializar_logger();
	cargarConfigGameCard();

	// Crear Metadata general
	char* rutaMetadata = crearRutaArchivo("/Metadata/Metadata.bin");
	char* Linea1Metadata= "BLOCK_SIZE=64\n";
	char* Linea2Metadata= "BLOCKS=5192\n";
	char* Linea3Metadata= "MAGIC_NUMBER=TALL_GRASS\n";
	crearEscribirArchivo(rutaMetadata, Linea1Metadata);
	crearEscribirArchivo(rutaMetadata, Linea2Metadata);
	crearEscribirArchivo(rutaMetadata, Linea3Metadata);

	// Crear Bitmap general
	char* rutaBitmap = crearRutaArchivo("/Metadata/Bitmap.bin");
	crearEscribirArchivo(rutaBitmap, "\n");

	// 1. Crear conexivoyon
	int socketCliente;
	log_info(logger,"Conectando a PUERTO=%d en IP=%s",gameCardConfig->puertoBroker,gameCardConfig->ipBroker);
	socketCliente=crearConexion(gameCardConfig->ipBroker,gameCardConfig->puertoBroker,gameCardConfig->tiempoReintentoConexion);


	// 2. Suscribirse a las colas del Broker
		// 2.a Suscribirse a tNEW_POKEMON
		//crearMensaje("1",8,socketCliente);

		// 2.b Suscribirse a tCATCH_POKEMON
		// 2.c Suscribirse a tGET_POKEMON


	// 3. Recibir confirmación
	//char *mensaje = recibirConfirmacion(socketCliente);
	//log_info(logger, "Confirmacion recibida: %s\n", mensaje);

	 /*
	  * Cuando el broker nos pida que guardemos un pokemon:
	  *
	  * agregarNewPokemon();
	  * catchPokemon()
	  * obtenerPokemon()
	  */

	// LOGGEAR MENSAJE

	// 4. Terminar
	liberarConexion(socketCliente);
	terminarPrograma();
	return EXIT_SUCCESS;

}


