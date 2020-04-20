#include "gameCard.h"

int main(void) {
	inicializar_logger();
	cargarConfigGameCard();

	log_info(logger,"Aqui proceso GameCard");
	log_info(logger,"Lei IP=%s y PUERTO=%s del archivo config",gameCardConfig->ipBroker,gameCardConfig->puertoBroker);
	conexion=crear_conexion(gameCardConfig->ipBroker,gameCardConfig->puertoBroker);
	enviar_mensaje("HOLA_BROKER",conexion);
	char *mensaje=recibir_mensaje(conexion);
	log_info(logger,"Recibi de vuelta el mensaje %s con %d bytes",mensaje,strlen(mensaje));

	terminar_programa(conexion,logger,GAMECARDTConfig);

}


