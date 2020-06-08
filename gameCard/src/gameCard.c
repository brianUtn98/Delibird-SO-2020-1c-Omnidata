#include "gameCard.h"

int main(void) {

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

	// LOGGEAR MENSAJE

	// 4. Terminar
	terminarPrograma();
	return EXIT_SUCCESS;

}


