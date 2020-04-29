#include "team.h"

int main(void) {

iniciarlizar_loggerTeam();
cargarConfigTeam();

enviarMensaje(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,"HolaBroker");
	return EXIT_SUCCESS;
}
