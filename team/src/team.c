#include "team.h"

//El team todavía está en pañales, le falta agarrar los entrenadores, instanciarlos de alguna manera
//y planificarlos. supongo que habrá que usar pthread.

int main(void) {



inicializarLoggerTeam();
cargarConfigTeam();
log_info(logger,"El objetivo global de este equipo es: ");
mostrarLista(objetivoGlobal);
int contador;
//pthread_t *entrenador=(pthread_t*)malloc(cantidadEntrenadores);
//	log_info(logger,"Estoy creando hilos");
//	for(contador=0;contador<cantidadEntrenadores;contador++){
//	pthread_create(entrenador[contador],NULL,(void*)manejarEntrenador,(void*)contador);
//	}
// 1. Crear conexion
	int socketCliente;
	pid_t pid=process_getpid();
	log_info(logger, "Conectando a PUERTO=%d en IP=%s", teamConf->PUERTO_BROKER,
			teamConf->IP_BROKER);
	socketCliente = crearConexion(teamConf->IP_BROKER, teamConf->PUERTO_BROKER,
			teamConf->TIEMPO_RECONEXION);

	//crearMensajeGetPokemon(pid,"Pikachu",socketCliente);

// 2. Suscribirse a las colas del Broker

//crearMensajePokemon2(20, "pikachu", 29, 10, 82, socketCliente);
	enviarMensajeRecursoNew(45,"bulbalsaur",11,12,2011, socketCliente);
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
