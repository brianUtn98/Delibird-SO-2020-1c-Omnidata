#include "utils.h"

int crearConexion(char *ip, int puerto, int tiempoReconexion) {
	printf("Intentando conectar a PUERTO=%d en IP=%s\n", puerto, ip);
	int maxIntentos = 3;
	int retry = 0;
	struct sockaddr_in dirServer;
	dirServer.sin_family = AF_INET;
	dirServer.sin_addr.s_addr = inet_addr(ip);
	dirServer.sin_port = htons(puerto);

	int socketCliente = socket(AF_INET, SOCK_STREAM, 0);

	// if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1){
	while ((connect(socketCliente, (void*) &dirServer, sizeof(dirServer)) != 0)
			&& retry < maxIntentos) {
		retry = retry + 1;
		if (retry == maxIntentos) {
			perror("Se supero la cantidad de intentos posibles");
			exit(1);
		}

		perror("No se pudo conectar");
		sleep(tiempoReconexion);
	}

//		freeaddrinfo(server_info);

	return socketCliente;
}

void mostrarCoordenada(void *data) {
	printf("%d,%d\n", ((t_posicion*) data)->x, ((t_posicion*) data)->y);
	return;
}

void mostrarListaCoordenadas(t_list *lista) {
	t_list *aux = list_duplicate(lista);

	if (list_is_empty(aux)) {
		printf("La lista esta vacia, no hay elementos para mostar\n");
	} else {
		while (aux->head != NULL) {
			mostrarCoordenada(aux->head->data);
			aux->head = aux->head->next;
		}
		list_destroy(aux);
	}
}

void liberarConexion(int socket) {
	close(socket);
	return;
}

void* serializarPaquete(t_paquete* paquete, int *bytes) {
	int sizeSerializado = sizeof(t_header) + sizeof(int) + sizeof(int)
			+ sizeof(int) + sizeof(int) + sizeof(int)
			+ paquete->buffer->largoNombre + sizeof(int) + sizeof(int)
			+ sizeof(int)
			+ paquete->buffer->listaCoordenadas->elements_count
					* sizeof(t_posicion);

	void* buffer = malloc(sizeSerializado);
	int desplazamiento = 0;

	memcpy(buffer + desplazamiento, &(paquete->codigoOperacion),
			sizeof(t_header));
	desplazamiento += sizeof(t_header);
	memcpy(buffer + desplazamiento, &(paquete->buffer->boolean), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(buffer + desplazamiento, &(paquete->buffer->cantidadPokemons),
			sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(buffer + desplazamiento, &(paquete->buffer->idMensaje), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(buffer + desplazamiento, &(paquete->buffer->largoNombre),
			sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(buffer + desplazamiento, paquete->buffer->nombrePokemon,
			paquete->buffer->largoNombre);
	desplazamiento += paquete->buffer->largoNombre;
	memcpy(buffer + desplazamiento, &(paquete->buffer->posX), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(buffer + desplazamiento, &(paquete->buffer->posY), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(buffer + desplazamiento, &(paquete->buffer->tiempo), sizeof(int));
	desplazamiento += sizeof(int);

	int cantidadCoordenadas = paquete->buffer->listaCoordenadas->elements_count;
	printf("Al serializar, cantidadCoordenadas=%d\n",cantidadCoordenadas);
	memcpy(buffer + desplazamiento, &cantidadCoordenadas, sizeof(int));
	desplazamiento += sizeof(int);

	t_list*aux = list_duplicate(paquete->buffer->listaCoordenadas);
	if(cantidadCoordenadas!=0){
	while (aux->head != NULL) {
		t_posicion *buffercito;
		buffercito = (t_posicion*) aux->head->data;
		printf("Buffercito vale %d,%d\n", buffercito->x,buffercito->y);
		memcpy(buffer + desplazamiento, buffercito, sizeof(t_posicion));
		desplazamiento += sizeof(t_posicion);
		aux->head = aux->head->next;
		free(buffercito);
	}
	}
	(*bytes) = sizeSerializado;
	printf("Termine de serializar\n");
	return buffer;

}

t_paquete* recibirMensaje(int socketCliente) {
	t_paquete *paquete;
	void *buffer = malloc(100 * sizeof(void));
	recv(socketCliente, buffer, 100 * sizeof(void), 0);

	paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_bufferOmnidata));
	int desplazamiento = 0;
	memcpy(&paquete->codigoOperacion, buffer + desplazamiento,
			sizeof(t_header));
	desplazamiento += sizeof(t_header);
	memcpy(&paquete->buffer->boolean, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(&paquete->buffer->cantidadPokemons, buffer + desplazamiento,
			sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(&paquete->buffer->idMensaje, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(&paquete->buffer->largoNombre, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);
	paquete->buffer->nombrePokemon = malloc(paquete->buffer->largoNombre);
	memcpy(paquete->buffer->nombrePokemon, buffer + desplazamiento,
			paquete->buffer->largoNombre);
	desplazamiento += paquete->buffer->largoNombre;
	memcpy(&paquete->buffer->posX, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(&paquete->buffer->posY, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(&paquete->buffer->tiempo, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	int cantidadCoordenadas=0;
	memcpy(&cantidadCoordenadas, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);
	int k;

	t_posicion *buffercito;
	paquete->buffer->listaCoordenadas = list_create();
	t_list *bufferCoordenadas = list_create();
	printf("Cantidad de coordenadas=%d\n",cantidadCoordenadas);

	//if(cantidadCoordenadas!=0){
	for (k = 0; k < cantidadCoordenadas; k++) {
		buffercito = malloc(sizeof(t_posicion));
		memcpy(buffercito, buffer + desplazamiento, sizeof(t_posicion));
		printf("Agarre: %d,%d\n", buffercito->x,buffercito->y);
		printf("Voy a agregar a la lista: %d,%d\n",buffercito->x,buffercito->y);
		list_add(bufferCoordenadas, (void*) buffercito);
		//free(buffercito); Esta linea no la borro para recordarme que perdí casi un día de estudio por esto
		desplazamiento += sizeof(t_posicion);
	}
	paquete->buffer->listaCoordenadas = list_duplicate(bufferCoordenadas);
	mostrarListaCoordenadas(paquete->buffer->listaCoordenadas);
	//}
	list_destroy(bufferCoordenadas);
	printf("En el campo de texto recibo: %s\n", paquete->buffer->nombrePokemon);

	printf("Termine de recibir, devuelvo paquete\n");
	free(buffer);
	return paquete;

}

void enviarMensajeBrokerNew(char* nombrePokemon, int posX, int posY,
		int cantidadPokemons, int socketCliente) {

	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_NEW_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	paquete->cantidadPokemons = cantidadPokemons;
	paquete->idMensaje = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->tiempo = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();

	//t_posicion *pos1, *pos2, *pos3, *pos4, *pos5;

	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("Cantidad de pokemons: %d\n", paquete->cantidadPokemons);
	printf("Tiempo de suscripción: %d\n", paquete->tiempo);
	printf("Id mensaje correlativo: %d\n", paquete->idMensaje);
	printf("booleano: %d\n", paquete->boolean);
	printf("numero de coordenadas: %d\n",
			paquete->listaCoordenadas->elements_count);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");

	/*
	 * ERROR: Double free or corruption (fasttop)
	 * free(serializado);
	 * free(unPaquete->buffer);
	 * free(paquete);
	 * free(unPaquete);
	 */

	free(unPaquete->buffer->nombrePokemon);
	list_destroy(unPaquete->buffer->listaCoordenadas);
	free(unPaquete->buffer);
	free(unPaquete);

}

void enviarMensajeBrokerGet(char* nombrePokemon, int socketCliente) {

	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_GET_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->boolean = 0;
	paquete->listaCoordenadas = list_create();
	unPaquete->buffer = paquete;

	printf("Se creara mensaje: \n");
	printf("---Mensaje GET_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("Tiempo de suscripción: %d\n", paquete->tiempo);
	printf("Cantidad de pokemons: %d\n", paquete->cantidadPokemons);
	printf("Id mensaje correlativo: %d\n", paquete->idMensaje);
	printf("booleano: %d\n", paquete->boolean);
	printf("numero de coordenadas: %d\n",
			paquete->listaCoordenadas->elements_count);
	printf("---Fin Mensaje GET_POKEMON---\n");
	unPaquete->buffer = paquete;

	printf("---Mensaje GET_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");

//	free(serializado);
//
//	free(unPaquete->buffer->nombrePokemon);
//	list_destroy(unPaquete->buffer->listaCoordenadas);
//	free(unPaquete->buffer);
//	free(unPaquete);
}

void enviarMensajeBrokerAppeared(char* nombrePokemon, int posX, int posY,
		int idMensaje, int socketCliente) {
	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));

	unPaquete->codigoOperacion = MENSAJE_APPEARED_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->idMensaje = idMensaje;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();
	paquete->tiempo=0;

	printf("Se creara mensaje: \n");
	printf("---Mensaje APPEARED_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje APPEARED_POKEMON---\n");

	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");

	free(serializado);

	free(unPaquete->buffer->nombrePokemon);
	list_destroy(unPaquete->buffer->listaCoordenadas);
	free(unPaquete->buffer);
	free(unPaquete);

}

void enviarMensajeBrokerCatch(char* nombrePokemon, int posX, int posY,
		int socketCliente) {
	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_CATCH_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();

	printf("---Mensaje CATCH_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	/*
	 * ERROR: Double free or corruption (fasttop)
	 * free(serializado);
	 * free(unPaquete->buffer);
	 * free(paquete);
	 * free(unPaquete);
	 */
}

void enviarMensajeBrokerCaught(int idMensaje, int booleano, int socketCliente) {
	//uint32_t stringSize = strlen(nombrePokemon) + 1;

	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_CAUGHT_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	paquete->cantidadPokemons = 0;
	paquete->largoNombre = 0;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->idMensaje = idMensaje;

	paquete->nombrePokemon = malloc(1);
	paquete->nombrePokemon[0] = '\0';
	paquete->listaCoordenadas = list_create();
//

	paquete->boolean = booleano;

	printf("---Mensaje CAUGHT_POKEMON---\n");

	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");

	/*
	 * ERROR: Double free or corruption (fasttop)
	 * free(serializado);
	 * free(unPaquete->buffer);
	 * free(paquete);
	 * free(unPaquete);
	 */

}

void enviarMensajeLocalized(char* nombrePokemon, t_list* coordenadas,
		int socketCliente) {

	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_LOCALIZED_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();
	paquete->listaCoordenadas = list_duplicate(coordenadas);

	printf("---Mensaje LOCALIZED_POKEMON---\n");

	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");

	/*
	 * ERROR: Double free or corruption (fasttop)
	 * free(serializado);
	 * free(unPaquete->buffer);
	 * free(paquete);
	 * free(unPaquete);
	 */

}

void enviarMensajeTeamAppeared(char* nombrePokemon, int posX, int posY,
		int socketCliente) {
	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));

	unPaquete->codigoOperacion = MENSAJE_APPEARED_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->idMensaje = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();

	printf("---Mensaje APPEARED_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");

	/*
	 * ERROR: Double free or corruption (fasttop)
	 * free(serializado);
	 * free(unPaquete->buffer);
	 * free(paquete);
	 * free(unPaquete);
	 */

}

void enviarMensajeGameCardNewPokemon(char* nombrePokemon, int posX, int posY,
		int cantidadPokemons, int idMensajeCorrelativo, int socketCliente) {
	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_NEW_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	paquete->cantidadPokemons = cantidadPokemons;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->idMensaje = idMensajeCorrelativo;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();

	list_add(paquete->listaCoordenadas, (void*) 1);
	list_add(paquete->listaCoordenadas, (void*) 2);
	list_add(paquete->listaCoordenadas, (void*) 3);
	list_add(paquete->listaCoordenadas, (void*) 4);
	list_add(paquete->listaCoordenadas, (void*) 5);
	list_add(paquete->listaCoordenadas, (void*) 6);
	list_add(paquete->listaCoordenadas, (void*) 7);
	list_add(paquete->listaCoordenadas, (void*) 8);
	printf("---Mensaje NEW_POKEMON---\n");

	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");

	/*
	 * ERROR: Double free or corruption (fasttop)
	 * free(serializado);
	 * free(unPaquete->buffer);
	 * free(paquete);
	 * free(unPaquete);
	 */

}

void enviarMensajeGameCardCatchPokemon(char* nombrePokemon, int posX, int posY,
		int idCorrelativo, int socketCliente) {
	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_CATCH_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->idMensaje = idCorrelativo;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();

	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	free(serializado);
	free(unPaquete->buffer);
	free(paquete);
	free(unPaquete);
}

void enviarMensajeGameCardGetPokemon(char* nombrePokemon, int idCorrelativo,
		int socketCliente) {

	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_GET_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->idMensaje = idCorrelativo;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();

	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	free(serializado);
	free(unPaquete->buffer);
	free(paquete);
	free(unPaquete);

}

void suscribirseNew(int tiempo, int socketCliente) {

	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = SUSCRIBIRSE_NEW_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	unPaquete->buffer->tiempo = tiempo;
	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	free(serializado);
	free(unPaquete->buffer);
	free(paquete);
	free(unPaquete);
}
void suscribirseGet(int tiempo, int socketCliente) {
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = SUSCRIBIRSE_GET_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	unPaquete->buffer->tiempo = tiempo;
	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	free(serializado);
	free(unPaquete->buffer);
	free(paquete);
	free(unPaquete);
}
void suscribirseCatch(int tiempo, int socketCliente) {
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = SUSCRIBIRSE_CATCH_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	unPaquete->buffer->tiempo = tiempo;
	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	free(serializado);
	free(unPaquete->buffer);
	free(paquete);
	free(unPaquete);
}
void suscribirseCaught(int tiempo, int socketCliente) {
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = SUSCRIBIRSE_CAUGHT_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	unPaquete->buffer->tiempo = tiempo;
	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	free(serializado);
	free(unPaquete->buffer);
	free(paquete);
	free(unPaquete);
}
void suscribirseAppeared(int tiempo, int socketCliente) {
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = SUSCRIBIRSE_APPEARED_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	unPaquete->buffer->tiempo = tiempo;
	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	free(serializado);
	free(unPaquete->buffer);
	free(paquete);
	free(unPaquete);
}
void suscribirseLocalized(int tiempo, int socketCliente) {
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = SUSCRIBIRSE_LOCALIZED_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	unPaquete->buffer->tiempo = tiempo;
	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	free(serializado);
	free(unPaquete->buffer);
	free(paquete);
	free(unPaquete);
}
