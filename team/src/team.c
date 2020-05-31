#include "team.h"

//El team todavía está en pañales, le falta agarrar los entrenadores, instanciarlos de alguna manera
//y planificarlos. supongo que habrá que usar pthread.

int main(void) {
pid_t pid=process_getpid(); // Esto sera descartado en breve,  ya que no nos sirve.


inicializarLoggerTeam();
cargarConfigTeam();
sleep(1);
log_info(logger,"El objetivo global de este equipo es: ");
mostrarLista(objetivoGlobal);
//int contador;
//pthread_t *entrenador=(pthread_t*)malloc(cantidadEntrenadores);
//	log_info(logger,"Estoy creando hilos");
//	for(contador=0;contador<cantidadEntrenadores;contador++){
//	pthread_create(entrenador[contador],NULL,(void*)manejarEntrenador,(void*)contador);
//	}
	int socket;

	socket=crearConexion(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,teamConf->TIEMPO_RECONEXION);
	enviarMensajeRecursoNew("Pikachu",10,5,2,socket);

	liberarConexion(socket);
	iniciarColasEjecucion();


	 // Por ahora está hardcodeado porque no se cómo obtener la ip de este proceso (no viene por config).



	//pedirPokemons();

	escucharGameboy("127.0.0.1",5002);
	sleep(5);

	/*Realizar los GET_POKEMON*/




	//enviarMensajeRecursoNew(pid,"Bulbasaur",11,12,2011, socketCliente);
// 3. Recibir confirmación
//char* mensaje = recibirConfirmacion(socketCliente);
//log_info(logger,"Mensaje de confirmacion recibido: %s", mensaje);
//printf("Mensaje de confirmacion recibido: %s\n", mensaje);

// LOGGEAR MENSAJE
// 4. Terminar


//terminarPrograma();
	return EXIT_SUCCESS;

}
