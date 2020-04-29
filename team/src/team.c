#include "team.h"

int main(void) {

iniciarlizar_loggerTeam();
cargarConfigTeam();

//enviarMensaje(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,"HolaBroker");
conectarseAServidor(teamConf->IP_BROKER,teamConf->PUERTO_BROKER);
return EXIT_SUCCESS;
}
