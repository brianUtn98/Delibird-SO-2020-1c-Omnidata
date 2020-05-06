#include "team.h"

//El team todavía está en pañales, le falta agarrar los entrenadores, instanciarlos de alguna manera
//y planificarlos. supongo que habrá que usar pthread.

int main(void) {

inicializarLoggerTeam();
cargarConfigTeam();
//crearEntrenadores();
// 1. Crear conexion
int socketCliente;
log_info(logger,"Conectando a PUERTO=%d en IP=%s",teamConf->PUERTO_BROKER,teamConf->IP_BROKER);
socketCliente=crearConexion(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,teamConf->TIEMPO_RECONEXION);

// 2. Suscribirse a las colas del Broker

crearMensaje("1",8, socketCliente);

// 3. Recibir confirmación
char *mensaje = recibir_mensaje(socketCliente);
// LOGGEAR MENSAJE
// 4. Terminar
liberarConexion(socketCliente);
return EXIT_SUCCESS;
}
