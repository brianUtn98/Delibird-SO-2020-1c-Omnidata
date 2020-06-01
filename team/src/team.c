#include "team.h"

//El team todavía está en pañales, le falta agarrar los entrenadores, instanciarlos de alguna manera
//y planificarlos. supongo que habrá que usar pthread.

int main(void) {


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
	enviarMensajeBrokerNew("Pikachu",10,5,2,socket);
	liberarConexion(socket);
	sleep(5);

	/*
	 * ESTO ESTA MAL POR LO QUE HABLAMOS CON NICO EL DOMINGO
	 * PERO ES LA UNICA FORMA DE MANDAR DOS MENSAJES DIFERENTES
	 * Y QUE EL BROKER LOS RECIBA.
	 *
	 * HAY QUE ARREGLAR
	 */

	socket=crearConexion(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,teamConf->TIEMPO_RECONEXION);
	enviarMensajeBrokerGet("Pikachu",socket);
	liberarConexion(socket);

	sleep(5);
	enviarMensajeBrokerAppeared("Pikachu",10,5,2,socket);
	sleep(5);
	enviarMensajeBrokerCatch("Pikachu",10,5,socket);
	sleep(5);
	t_list* t_coordenadas = list_create();
	list_add(t_coordenadas, (void*) 1);
	list_add(t_coordenadas, (void*) 2);
	enviarMensajeLocalized("Pikachu", t_coordenadas, socket);
	sleep(5);
	enviarMensajeTeamAppeared("Pikachu",10,5,socket);
	sleep(5);
	enviarMensajeBrokerCaught(1,true,socket);

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
