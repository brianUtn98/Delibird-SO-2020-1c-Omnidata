#include "team.h"

//El team todavía está en pañales, le falta agarrar los entrenadores, instanciarlos de alguna manera
//y planificarlos. supongo que habrá que usar pthread.

int main(void) {

	inicializarLoggerTeam();
	cargarConfigTeam();
	//sleep(1);
	log_info(logger, "El objetivo global de este equipo es: ");
	mostrarLista(objetivoGlobal);
//int contador;
//pthread_t *entrenador=(pthread_t*)malloc(cantidadEntrenadores);
//	log_info(logger,"Estoy creando hilos");
//	for(contador=0;contador<cantidadEntrenadores;contador++){
//	pthread_create(entrenador[contador],NULL,(void*)manejarEntrenador,(void*)contador);
//	}

	int socket;
	socket = crearConexion(teamConf->IP_BROKER, teamConf->PUERTO_BROKER,
			teamConf->TIEMPO_RECONEXION);
	enviarMensajeBrokerNew("Pikachu", 10, 5, 2, socket);
	//liberarConexion(socket);
	//sleep(1);

	enviarMensajeBrokerNew("Pikachu", 10, 5, 2, socket);
	//liberarConexion(socket);
	//sleep(1);
	enviarMensajeBrokerNew("Pikachu", 10, 5, 2, socket);

	//liberarConexion(socket);
	//liberarConexion(socket);
	//sleep(10);
//	enviarMensajeBrokerNew("Pikachu", 10, 5, 2, socket);
//	//liberarConexion(socket);
//	sleep(5);
//	enviarMensajeBrokerNew("Pikachu", 10, 5, 2, socket);
//	//liberarConexion(socket);
//	sleep(5);
//	enviarMensajeBrokerNew("Pikachu", 10, 5, 2, socket);
//	//liberarConexion(socket);
//	sleep(5);

	/*
	 * ESTO ESTA MAL POR LO QUE HABLAMOS CON NICO EL DOMINGO
	 * PERO ES LA UNICA FORMA DE MANDAR DOS MENSAJES DIFERENTES
	 * Y QUE EL BROKER LOS RECIBA.
	 *
	 * HAY QUE ARREGLAR
	 */

	//socket=crearConexion(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,teamConf->TIEMPO_RECONEXION);
//	enviarMensajeBrokerGet("Pikachu", socket);
//	//liberarConexion(socket);
//
//	sleep(5);
//	//socket=crearConexion(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,teamConf->TIEMPO_RECONEXION);
//	enviarMensajeBrokerAppeared("Squirtle", 10, 5, 2, socket);
//	//liberarConexion(socket);
//	sleep(5);
//	//socket=crearConexion(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,teamConf->TIEMPO_RECONEXION);
//	enviarMensajeBrokerCatch("Charmander", 10, 5, socket);
//	//liberarConexion(socket);
//	sleep(5);
//	t_list* t_coordenadas = list_create();
//	t_posicion *prueba1, *prueba2, *prueba3;
//	prueba1 = malloc(sizeof(t_posicion));
//	prueba2 = malloc(sizeof(t_posicion));
//	prueba3 = malloc(sizeof(t_posicion));
//	prueba1->x = 3;
//	prueba1->y = 2;
//	prueba2->x = 2;
//	prueba2->y = 5;
//	prueba3->x = 5;
//	prueba3->y = 2;
//	list_add(t_coordenadas, (void*) prueba1);
//	list_add(t_coordenadas, (void*) prueba2);
//	list_add(t_coordenadas, (void*) prueba3);
//	//socket=crearConexion(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,teamConf->TIEMPO_RECONEXION);
//	enviarMensajeLocalized("Bulbasaur", t_coordenadas, socket);
//	//liberarConexion(socket);
//	sleep(5);
//	//socket=crearConexion(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,teamConf->TIEMPO_RECONEXION);
//	enviarMensajeTeamAppeared("Cherrim", 10, 5, socket);
//	//liberarConexion(socket);
//	sleep(5);
//	//socket=crearConexion(teamConf->IP_BROKER,teamConf->PUERTO_BROKER,teamConf->TIEMPO_RECONEXION);
//	enviarMensajeBrokerCaught(1, true, socket);
//
//	//liberarConexion(socket);
//	iniciarColasEjecucion();
// Por ahora está hardcodeado porque no se cómo obtener la ip de este proceso (no viene por config).
	//pedirPokemons();
//	escucharGameboy("127.0.0.1", 5002);
//	sleep(5);
	/*Realizar los GET_POKEMON*/

//enviarMensajeRecursoNew(pid,"Bulbasaur",11,12,2011, socketCliente);
// 3. Recibir confirmación
//char* mensaje = recibirConfirmacion(socketCliente);
//log_info(logger,"Mensaje de confirmacion recibido: %s", mensaje);
//printf("Mensaje de confirmacion recibido: %s\n", mensaje);
// LOGGEAR MENSAJE
// 4. Terminar
//terminarPrograma();
	//liberarConexion(socket);
	printf("Estoy en el bucle\n");
	for(;;);

	return EXIT_SUCCESS;

}
