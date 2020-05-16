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
	log_info(logger, "Conectando a PUERTO=%d en IP=%s", teamConf->PUERTO_BROKER,
			teamConf->IP_BROKER);
	socketCliente = crearConexion(teamConf->IP_BROKER, teamConf->PUERTO_BROKER,
			teamConf->TIEMPO_RECONEXION);

// 2. Suscribirse a las colas del Broker
	pid_t pid=process_getpid();

	crearMensajeNewPokemon(pid, "Pikachu", 29, 10, 82, socketCliente);
	liberarConexion(socketCliente);
	sleep(5);

	socketCliente = crearConexion(teamConf->IP_BROKER, teamConf->PUERTO_BROKER,
				teamConf->TIEMPO_RECONEXION);
	crearMensajeNewPokemon(pid,"Charmander",5,3,1,socketCliente);
	liberarConexion(socketCliente);
	sleep(5);

	socketCliente = crearConexion(teamConf->IP_BROKER, teamConf->PUERTO_BROKER,
					teamConf->TIEMPO_RECONEXION);
	crearMensajeNewPokemon(pid,"Pidgey",10,10,2,socketCliente);
	liberarConexion(socketCliente);
	sleep(5);

	socketCliente = crearConexion(teamConf->IP_BROKER, teamConf->PUERTO_BROKER,
					teamConf->TIEMPO_RECONEXION);
	crearMensajeNewPokemon(pid,"Squirtle",2,2,3,socketCliente);
	liberarConexion(socketCliente);
	sleep(5);

	socketCliente = crearConexion(teamConf->IP_BROKER, teamConf->PUERTO_BROKER,
					teamConf->TIEMPO_RECONEXION);
	crearMensajeNewPokemon(pid,"Mewtwo",6,2,3,socketCliente);
	liberarConexion(socketCliente);
	sleep(5);
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
