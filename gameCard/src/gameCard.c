#include "gameCard.h"

int main(void) {
	inicializar_logger();
	cargarConfigGameCard();
	char* prueba = "Metadata.bin";
	crearArchivoMetadataGeneral(prueba);

	// 1. Crear conexion
	int socketCliente;
	log_info(logger,"Conectando a PUERTO=%d en IP=%s",gameCardConfig->puertoBroker,gameCardConfig->ipBroker);
	socketCliente=crearConexion(gameCardConfig->ipBroker,gameCardConfig->puertoBroker,gameCardConfig->tiempoReintentoConexion);


	// 2. Suscribirse a las colas del Broker

	//enviarMensaje("1", socketCliente);
	crearMensaje("1",8,socketCliente);

	// 3. Recibir confirmación
	char *mensaje = recibirConfirmacion(socketCliente);
	log_info(logger, "Confirmacion recibida: %s\n", mensaje);

	// LOGGEAR MENSAJE
	// 4. Terminar
	liberarConexion(socketCliente);
	return EXIT_SUCCESS;

}


