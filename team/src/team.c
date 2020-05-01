#include "team.h"

int main(void) {

iniciarlizarLoggerTeam();
cargarConfigTeam();

// 1. Crear conexion
int socket_cliente;
socket_cliente=crear_conexion(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,teamConf->TIEMPO_RECONEXION);

// 2. Suscribirse a las colas del Broker
// No funciona la funcion enviar mensaje
//enviar_mensaje("1", socket_cliente);

// 3. Recibir confirmación
// 4. Terminar
liberar_conexion(socket_cliente);

return EXIT_SUCCESS;
}
