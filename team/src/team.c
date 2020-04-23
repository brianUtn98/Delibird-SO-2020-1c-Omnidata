#include "team.h"


int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
	cargarConfigTeam();
	crear_conexion(teamConf->IP_BROKER,teamConf->PUERTO_BROKER);
}
