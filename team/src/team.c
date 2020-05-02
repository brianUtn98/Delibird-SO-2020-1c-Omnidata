#include "team.h"

//El team todavía está en pañales, le falta agarrar los entrenadores, instanciarlos de alguna manera
//y planificarlos. supongo que habrá que usar pthread.

int main(void) {

inicializarLoggerTeam();
cargarConfigTeam();

// 1. Crear conexion
int socket_cliente;
socket_cliente=crearConexion(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,teamConf->TIEMPO_RECONEXION);

// 2. Suscribirse a las colas del Broker
enviar_mensaje("1", socket_cliente);

// 3. Recibir confirmación
//char *mensaje = recibir_mensaje(socket_cliente);
// LOGGEAR MENSAJE
// 4. Terminar
liberar_conexion(socket_cliente);

return EXIT_SUCCESS;
}
