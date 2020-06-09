#include "gameCard.h"

int main(void) {
	pthread_t threadIdgameCard;

	maximo_block_creado=0;

	inicializar_logger();
	cargarConfigGameCard();
	iniciarTallGrass();
	// Conectarse al Broker
	suscribirmeAColasBroker();

	t_list* t_coordenadas=list_create();
	list_add(t_coordenadas, (void*)5);
	list_add(t_coordenadas, (void*)1);

	agregarNewPokemon("Pikachu", t_coordenadas, 10);

	 /*
	  * Cuando el broker nos pida que guardemos un pokemon:
	  *
	  *
	  * catchPokemon()
	  * obtenerPokemon()
	  */
	int socketBroker;
			socketBroker = crearConexion(gameCardConfig->ipBroker, gameCardConfig->puertoBroker,
					gameCardConfig->tiempoReintentoConexion);




	enviarMensajeBrokerCaught(4,1,socketBroker);

	sleep(2);

	enviarMensajeTeamAppeared("pikachu",5,6,socketBroker);



	//Segmentationfault
	//enviarMensajeLocalized("Pikachu",t_coordenadas,socketBroker);



	// LOGGEAR MENSAJE

	// 4. Terminar
	terminarPrograma();
	return EXIT_SUCCESS;

}


