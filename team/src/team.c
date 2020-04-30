#include "team.h"

int main(void) {

iniciarlizar_loggerTeam();
cargarConfigTeam();

//enviarMensaje(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,"HolaBroker");
conectarseAServidor(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,teamConf->TIEMPO_RECONEXION);
return EXIT_SUCCESS;
}
