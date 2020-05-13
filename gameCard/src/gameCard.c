#include "gameCard.h"

int main(void) {
	inicializar_logger();
	cargarConfigGameCard();
	pid_t pid=process_getpid();
	// Crear Metadata general
	crearArchivo(gameCardConfig->puntoDeMontaje, "/Metadata/Metadata.bin");
	// Crear Bitmap general
	crearArchivo(gameCardConfig->puntoDeMontaje, "/Metadata/Bitmap.bin");

	// 1. Crear conexivoyon
	int socketCliente;
	log_info(logger,"Conectando a PUERTO=%d en IP=%s",gameCardConfig->puertoBroker,gameCardConfig->ipBroker);
	socketCliente=crearConexion(gameCardConfig->ipBroker,gameCardConfig->puertoBroker,gameCardConfig->tiempoReintentoConexion);


	// 2. Suscribirse a las colas del Broker
	// 2.a Suscribirse a tNEW_POKEMON
	//crearMensaje("1",8,socketCliente);
	crearMensajeNewPokemon(pid,"Galvantula",3,2,5,socketCliente);
	// 2.b Suscribirse a tCATCH_POKEMON
	// 2.c Suscribirse a tGET_POKEMON


	// 3. Recibir confirmación
	//char *mensaje = recibirConfirmacion(socketCliente);
	//log_info(logger, "Confirmacion recibida: %s\n", mensaje);

	// New Pokemon
	// int existe = existePokemon();
	//

	// LOGGEAR MENSAJE
	// 4. Terminar
	liberarConexion(socketCliente);
	terminarPrograma();
	return EXIT_SUCCESS;

}


