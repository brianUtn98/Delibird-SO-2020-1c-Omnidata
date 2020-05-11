#include "team.h"

//El team todavía está en pañales, le falta agarrar los entrenadores, instanciarlos de alguna manera
//y planificarlos. supongo que habrá que usar pthread.

int main(void) {

	inicializarLoggerTeam();
	cargarConfigTeam();
//for(int i=0;i<cantidadEntrenadores;i++){
//log_info(logger,"------ Entrenador %d ------",i+1);
//log_info(logger,"------Posicion: X=%d,Y=%d ------",entrenadores[i].posicion.x,entrenadores[i].posicion.y);
//log_info(logger,"------- Los pokemons del entrenador %d son: ",i+1);
//mostrarLista(entrenadores[i].pokemons);
//log_info(logger,"------ Los objetivos del entrenador %d son: ",i+1);
//mostrarLista(entrenadores[i].objetivos);
//}
// 1. Crear conexion
	int socketCliente;
	log_info(logger, "Conectando a PUERTO=%d en IP=%s", teamConf->PUERTO_BROKER,
			teamConf->IP_BROKER);
	socketCliente = crearConexion(teamConf->IP_BROKER, teamConf->PUERTO_BROKER,
			teamConf->TIEMPO_RECONEXION);

// 2. Suscribirse a las colas del Broker

	crearMensajeANewPokemon(30, "putoElQueLee", 5, 10, 2, socketCliente);

// 3. Recibir confirmación
//char* mensaje = recibirConfirmacion(socketCliente);
//log_info(logger,"Mensaje de confirmacion recibido: %s", mensaje);
//printf("Mensaje de confirmacion recibido: %s\n", mensaje);

// LOGGEAR MENSAJE
// 4. Terminar

	liberarConexion(socketCliente);

//terminarPrograma();
	return EXIT_SUCCESS;

}
