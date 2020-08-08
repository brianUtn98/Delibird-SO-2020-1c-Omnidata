#include "utils.h"

int crearConexion(char *ip, int puerto, int tiempoReconexion) {
	//printf("Intentando conectar a PUERTO=%d en IP=%s\n", puerto, ip);
	//int maxIntentos = 3;
	//int retry = 0;
//	printf("Entre a crear conexion\n");
	struct sockaddr_in dirServer;
	dirServer.sin_family = AF_INET;
	dirServer.sin_addr.s_addr = inet_addr(ip);
	dirServer.sin_port = htons(puerto);

	int socketCliente = socket(AF_INET, SOCK_STREAM, 0);
	if (socketCliente == -1) {

		return socketCliente;
	}

	// if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1){
	while ((connect(socketCliente, (struct sockaddr*) &dirServer,
			sizeof(dirServer)) != 0)) {
		//perror("No se pudo conectar");
		sleep(tiempoReconexion);
	}

//		freeaddrinfo(server_info);

	return socketCliente;
}

int crearConexionSinReintento(char *ip, int puerto) {
	struct sockaddr_in dirServer;
	dirServer.sin_family = AF_INET;
	dirServer.sin_addr.s_addr = inet_addr(ip);
	dirServer.sin_port = htons(puerto);

	int socketCliente = socket(AF_INET, SOCK_STREAM, 0);

	if (socketCliente == -1) {
		return socketCliente;
	}
	int conecto = connect(socketCliente, (struct sockaddr*) &dirServer,
			sizeof(dirServer));
	//printf("El valor de connect es %d\n",conecto);
	if (conecto != 0) {
		return -1;
	}
	return socketCliente;
}

//Todo
void mostrarPaquete(t_paquete *paquete) {

}
//Todo
void mostrarListaPaquetes(t_list *lista) {

}

void mostrarCoordenada(void *data) {
	printf("%d,%d\n", ((t_posicion*) data)->x, ((t_posicion*) data)->y);
	return;
}

void liberarBuffer(t_bufferOmnidata *buffer){ //Todo testear
//	printf("Se rompe antes de liberar paquete\n");
	//printf("Liberar nombrePokemon\n");
	if(buffer->nombrePokemon !=NULL)
	free(buffer->nombrePokemon);
	//printf("Liberar nombreProceso\n");
	if(buffer->nombreProceso != NULL)
	free(buffer->nombreProceso);
	//printf("Liberar listaCoordenadas\n");
	list_destroy(buffer->listaCoordenadas);
	//printf("Liberar buffer\n");
	free(buffer);
	//printf("Se rompe despues de liberar buffer\n");
}
void liberarPaquete(t_paquete *paquete){ //Todo testear
//	printf("Se rompe despues de liberar paquete\n");
	//printf("Liberar buffer\n");
	liberarBuffer(paquete->buffer);
	//printf("Liberar paquete");
	free(paquete);
//	printf("Se rompe despues de liberar paquete\n");
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
//	int sizeSerializado = sizeof(t_header) + sizeof(int) + sizeof(int)
//			+ sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int)
//			+ sizeof(int) + paquete->buffer->largoNombre + sizeof(int)
//			+ sizeof(int) + sizeof(int)
//			+ paquete->buffer->listaCoordenadas->elements_count
//					* sizeof(t_posicion) + sizeof(int);

	int sizeSerializado = sizeof(t_header) + sizeof(uint32_t) * 12
			+ paquete->buffer->largoNombreProceso + paquete->buffer->largoNombre
			+ paquete->buffer->listaCoordenadas->elements_count
					* sizeof(t_posicion);

	void* buffer = malloc(sizeSerializado);
	int desplazamiento = 0;

	//Todo sacar los printf
	//Sacarlos despues de estar seguros de esto

//	printf("Serializando SizeSerializado=%d\n", sizeSerializado);
	memcpy(buffer + desplazamiento, &sizeSerializado, sizeof(int));
	desplazamiento += sizeof(int);
//	printf("Serializando OPcode=%d\n", paquete->codigoOperacion);
	memcpy(buffer + desplazamiento, &(paquete->codigoOperacion),
			sizeof(t_header));
	desplazamiento += sizeof(t_header);

	//printf("Serializando LargoNombreProceso=%d\n",
	//		paquete->buffer->largoNombreProceso);
	memcpy(buffer + desplazamiento, &(paquete->buffer->largoNombreProceso),
			sizeof(int));
	desplazamiento += sizeof(int);
	//printf("Serializando NombreProceso=%s\n", paquete->buffer->nombreProceso);
	memcpy(buffer + desplazamiento, paquete->buffer->nombreProceso,
			paquete->buffer->largoNombreProceso);
	desplazamiento += paquete->buffer->largoNombreProceso;

	//if (paquete->buffer->boolean == 0) {
	//printf("Serializando Boolean=FALSE\n");
	//} else {
	//printf("Serializando Boolean=TRUE\n");
	//}
	memcpy(buffer + desplazamiento, &(paquete->buffer->boolean), sizeof(int));
	desplazamiento += sizeof(int);
	//printf("Serializando CantidadPokemons=%d\n",
	//		paquete->buffer->cantidadPokemons);
	memcpy(buffer + desplazamiento, &(paquete->buffer->cantidadPokemons),
			sizeof(int));
	desplazamiento += sizeof(int);
	//printf("Serializando idMensaje=%d\n", paquete->buffer->idMensaje);
	memcpy(buffer + desplazamiento, &(paquete->buffer->idMensaje), sizeof(int));
	desplazamiento += sizeof(int);
	//printf("Serializando idMensajeCorrelativo=%d\n",
	//		paquete->buffer->idMensajeCorrelativo);
	memcpy(buffer + desplazamiento, &(paquete->buffer->idMensajeCorrelativo),
			sizeof(int));
	desplazamiento += sizeof(int);
	//printf("Serializando LargoNombre=%d\n", paquete->buffer->largoNombre);
	memcpy(buffer + desplazamiento, &(paquete->buffer->largoNombre),
			sizeof(int));
	desplazamiento += sizeof(int);
//	printf("Serializando NombrePokemon=%s\n", paquete->buffer->nombrePokemon);
	memcpy(buffer + desplazamiento, paquete->buffer->nombrePokemon,
			paquete->buffer->largoNombre);
	desplazamiento += paquete->buffer->largoNombre;
	//printf("Serializando PosX=%d\n", paquete->buffer->posX);
	memcpy(buffer + desplazamiento, &(paquete->buffer->posX), sizeof(int));
	desplazamiento += sizeof(int);
	//printf("Serializando PosY=%d\n", paquete->buffer->posY);
	memcpy(buffer + desplazamiento, &(paquete->buffer->posY), sizeof(int));
	desplazamiento += sizeof(int);
	//printf("Serializando Tiempo=%d\n", paquete->buffer->socket);
	memcpy(buffer + desplazamiento, &(paquete->buffer->socket), sizeof(int));
	desplazamiento += sizeof(int);
	//printf("Serializando Ack=%d\n", paquete->buffer->ack);
	memcpy(buffer + desplazamiento, &(paquete->buffer->ack), sizeof(int));
	desplazamiento += sizeof(int);

	int cantidadCoordenadas = paquete->buffer->listaCoordenadas->elements_count;
	//printf("Al serializar, cantidadCoordenadas=%d\n", cantidadCoordenadas);
	//printf("Serializando CantidadCoordenadas=%d\n", cantidadCoordenadas);
	memcpy(buffer + desplazamiento, &cantidadCoordenadas, sizeof(int));
	desplazamiento += sizeof(int);

	t_list*aux = list_duplicate(paquete->buffer->listaCoordenadas);
	//if(cantidadCoordenadas!=0){
	while (aux->head != NULL) {
		t_posicion *buffercito;
		buffercito = (t_posicion*) aux->head->data;
		//printf("Serializando coordenada %d,%d\n", buffercito->x, buffercito->y);
		memcpy(buffer + desplazamiento, buffercito, sizeof(t_posicion));
		desplazamiento += sizeof(t_posicion);
		aux->head = aux->head->next;
		//free(buffercito);
	}
	list_destroy(aux);
	//}
	(*bytes) = sizeSerializado;
	//printf("Termine de serializar\n");
	return buffer;

}

t_paquete* recibirMensaje(int socketCliente) {
	t_paquete *paquete;

	int bytes = 0;	//inicialice esto para que nunca llegue basura
	int recibi = recv(socketCliente, &bytes, sizeof(int), MSG_WAITALL);
	//printf("Recibi? %d\n", recibi);
	bytes -= sizeof(int);


	//printf("Bytes para recibir: %d\n", bytes);
	//int bytesRecibidos=recv(socketCliente, buffer, 100 * sizeof(void), 0);
	if (recibi > 0) {
		void *buffer = malloc(bytes);
		int bytesRecibidos = recv(socketCliente, buffer, bytes, MSG_WAITALL);
		printf("Recibi %d bytes\n", bytesRecibidos);

		paquete = malloc(sizeof(t_paquete));
		paquete->buffer = malloc(sizeof(t_bufferOmnidata));
		int desplazamiento = 0;
		memcpy(&paquete->codigoOperacion, buffer + desplazamiento,
				sizeof(t_header));
		desplazamiento += sizeof(t_header);

		memcpy(&paquete->buffer->largoNombreProceso, buffer + desplazamiento,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		paquete->buffer->nombreProceso = malloc(
				paquete->buffer->largoNombreProceso);
		memcpy(paquete->buffer->nombreProceso, buffer + desplazamiento,
				paquete->buffer->largoNombreProceso);
		desplazamiento += paquete->buffer->largoNombreProceso;

		memcpy(&paquete->buffer->boolean, buffer + desplazamiento,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(&paquete->buffer->cantidadPokemons, buffer + desplazamiento,
				sizeof(int));
		desplazamiento += sizeof(uint32_t);
		memcpy(&paquete->buffer->idMensaje, buffer + desplazamiento,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(&paquete->buffer->idMensajeCorrelativo, buffer + desplazamiento,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(&paquete->buffer->largoNombre, buffer + desplazamiento,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		paquete->buffer->nombrePokemon = malloc(paquete->buffer->largoNombre);
		memcpy(paquete->buffer->nombrePokemon, buffer + desplazamiento,
				paquete->buffer->largoNombre);
		desplazamiento += paquete->buffer->largoNombre;
		memcpy(&paquete->buffer->posX, buffer + desplazamiento,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(&paquete->buffer->posY, buffer + desplazamiento,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(&paquete->buffer->socket, buffer + desplazamiento,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(&paquete->buffer->ack, buffer + desplazamiento,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);

		int cantidadCoordenadas = 0;
		memcpy(&cantidadCoordenadas, buffer + desplazamiento, sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		int k = 0;

		t_posicion *buffercito;
		paquete->buffer->listaCoordenadas = list_create();
		t_list *bufferCoordenadas = list_create();

		//	printf("Printeando mensaje recibido: \n");

		//printf(" CantidadPokemons=%d\n", paquete->buffer->cantidadPokemons);
		//printf(" idMensaje=%d\n", paquete->buffer->idMensaje);
		//	printf(" idMensajeCorrelativo=%d\n",
		//		paquete->buffer->idMensajeCorrelativo);
		//printf(" LargoNombre=%d\n", paquete->buffer->largoNombre);
		//printf(" NombrePokemon=%s\n", paquete->buffer->nombrePokemon);
		/*printf(" LargoNombreProceso=%d\n", paquete->buffer->largoNombreProceso);
		 printf(" NombreProceso=%s\n", paquete->buffer->nombreProceso);
		 printf(" PosX=%d\n", paquete->buffer->posX);
		 printf(" PosY=%d\n", paquete->buffer->posY);
		 printf(" Tiempo=%d\n", paquete->buffer->tiempo);

		 printf("Cantidad de coordenadas=%d\n", cantidadCoordenadas);

		 */
		//if(cantidadCoordenadas!=0){
		//	printf("Cantidad de coordenadas que me llego: %d\n",
		//		cantidadCoordenadas);
		for (k = 0; k < cantidadCoordenadas; k++) {
			buffercito = malloc(sizeof(t_posicion));
			memcpy(buffercito, buffer + desplazamiento, sizeof(t_posicion));
			//		printf("Agarre: %d,%d\n", buffercito->x, buffercito->y);
			//	printf("Voy a agregar a la lista: %d,%d\n", buffercito->x,
			//	buffercito->y);
			list_add(bufferCoordenadas, (void*) buffercito);
			//free(buffercito); Esta linea no la borro para recordarme que perdí casi un día de estudio por esto
			desplazamiento += sizeof(t_posicion);
		}

		paquete->buffer->listaCoordenadas = list_duplicate(bufferCoordenadas);
		//mostrarListaCoordenadas(paquete->buffer->listaCoordenadas);
		//}
		list_destroy(bufferCoordenadas);

		//printf("Termine de recibir, devuelvo paquete\n");
		free(buffer);
		return paquete;
	} else {
		//printf("No recibi nada\n");
		//free(buffer);
		//printf("Libere la memoria\n");
		return NULL;

	}

}

void enviarMensajeBrokerNew(char* nombrePokemon, int posX, int posY, //ok
		int cantidadPokemons, int socketCliente) {

	//sleep(1);
	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_NEW_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();

	paquete->cantidadPokemons = cantidadPokemons;
	paquete->idMensaje = 0;
	paquete->idMensajeCorrelativo = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->ack = 0;
	paquete->socket = 0;
	paquete->boolean = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();

	//t_posicion *pos1, *pos2, *pos3, *pos4, *pos5;

//	printf("Se creara mensaje: \n");
//	printf("---Mensaje NEW_POKEMON---\n");
//	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
//	printf("LargoNombre: %d\n", paquete->largoNombre);
//	printf("PosX: %d\n", paquete->posX);
//	printf("PosY: %d\n", paquete->posY);
//	printf("Cantidad de pokemons: %d\n", paquete->cantidadPokemons);
//	printf("Socket: %d\n", paquete->socket);
//	printf("Id mensaje correlativo: %d\n", paquete->idMensajeCorrelativo);
//	printf("booleano: %d\n", paquete->boolean);
//	printf("numero de coordenadas: %d\n",
//			paquete->listaCoordenadas->elements_count);
//	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);
//	printf("Mande mensaje\n");

	free(serializado);
	//free(unPaquete->buffer->nombrePokemon);
	//list_destroy(unPaquete->buffer->listaCoordenadas);
	//free(unPaquete->buffer);
	//free(unPaquete);
	liberarPaquete(unPaquete);

}

void enviarMensajeBrokerGet(char* nombrePokemon, int socketCliente) { //ok

	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_GET_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->idMensaje = 0;
	paquete->idMensajeCorrelativo = 0;
	paquete->ack = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->boolean = 0;
	paquete->socket = 0;
	paquete->listaCoordenadas = list_create();
	unPaquete->buffer = paquete;

//	printf("Se creara mensaje: \n");
//	printf("---Mensaje GET_POKEMON---\n");
//	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
//	printf("LargoNombre: %d\n", paquete->largoNombre);
//	printf("PosX: %d\n", paquete->posX);
//	printf("PosY: %d\n", paquete->posY);
//	printf("Tiempo de suscripción: %d\n", paquete->socket);
//	printf("Cantidad de pokemons: %d\n", paquete->cantidadPokemons);
//	printf("Id mensaje correlativo: %d\n", paquete->idMensajeCorrelativo);
//	printf("booleano: %d\n", paquete->boolean);
//	printf("numero de coordenadas: %d\n",
//			paquete->listaCoordenadas->elements_count);
	printf("---Fin Mensaje GET_POKEMON---\n");
	unPaquete->buffer = paquete;

	printf("---Mensaje GET_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	//send(socketCliente,&sizeSerializado,sizeof(int),0);
	send(socketCliente, serializado, sizeSerializado, 0);
//	printf("Mande mensaje\n");

	free(serializado);
//
//	free(unPaquete->buffer->nombrePokemon);
//	list_destroy(unPaquete->buffer->listaCoordenadas);
//	free(unPaquete->buffer);
//	free(unPaquete);
	liberarPaquete(unPaquete);
}

void enviarMensajeGamecardGet(char* nombrePokemon, int idMensaje,
		int socketCliente) { //ok

	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_GET_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->idMensaje = idMensaje;
	paquete->idMensajeCorrelativo = 0;
	paquete->ack = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->boolean = 0;
	paquete->socket = 0;
	paquete->listaCoordenadas = list_create();
	unPaquete->buffer = paquete;

//	printf("Se creara mensaje: \n");
//	printf("---Mensaje GET_POKEMON---\n");
//	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
//	printf("LargoNombre: %d\n", paquete->largoNombre);
//	printf("PosX: %d\n", paquete->posX);
//	printf("PosY: %d\n", paquete->posY);
//	printf("Tiempo de suscripción: %d\n", paquete->socket);
//	printf("Cantidad de pokemons: %d\n", paquete->cantidadPokemons);
//	printf("Id mensaje correlativo: %d\n", paquete->idMensajeCorrelativo);
//	printf("booleano: %d\n", paquete->boolean);
//	printf("numero de coordenadas: %d\n",
//			paquete->listaCoordenadas->elements_count);
//	printf("---Fin Mensaje GET_POKEMON---\n");
	unPaquete->buffer = paquete;

//	printf("---Mensaje GET_POKEMON---\n");
//	printf("NombrePokemon: %s\n", paquete->nombrePokemon);

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	//send(socketCliente,&sizeSerializado,sizeof(int),0);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");

	free(serializado);
//
//	free(unPaquete->buffer->nombrePokemon);
//	list_destroy(unPaquete->buffer->listaCoordenadas);
//	free(unPaquete->buffer);
//	free(unPaquete);
	liberarPaquete(unPaquete);
}

void enviarMensajeBrokerAppeared(char* nombrePokemon, int posX, int posY, //ok
		int idMensajeCorrelativo, int socketCliente) {
	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));

	unPaquete->codigoOperacion = MENSAJE_APPEARED_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->ack = 0;
	paquete->idMensaje = 0;
	paquete->idMensajeCorrelativo = idMensajeCorrelativo;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();
	paquete->socket = 0;
	paquete->boolean = 0;

//	printf("Se creara mensaje: \n");
//	printf("---Mensaje APPEARED_POKEMON---\n");
//	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
//	printf("LargoNombre: %d\n", paquete->largoNombre);
//	printf("PosX: %d\n", paquete->posX);
//	printf("PosY: %d\n", paquete->posY);
//	printf("---Fin Mensaje APPEARED_POKEMON---\n");

	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);
	//printf("Mande mensaje\n");

	free(serializado);

//	free(unPaquete->buffer->nombrePokemon);
//	list_destroy(unPaquete->buffer->listaCoordenadas);
//	free(unPaquete->buffer);
//	free(unPaquete);
	liberarPaquete(unPaquete);

}

void enviarMensajeBrokerAppearedId(char* nombrePokemon, int posX, int posY,
		int idMensaje, int idMensajeCorrelativo, int socketCliente) {
	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));

	unPaquete->codigoOperacion = MENSAJE_APPEARED_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->ack = 0;
	paquete->idMensaje = 0;
	paquete->idMensajeCorrelativo = idMensajeCorrelativo;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();
	paquete->socket = 0;
	paquete->boolean = 0;

//	printf("Se creara mensaje: \n");
//	printf("---Mensaje APPEARED_POKEMON---\n");
//	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
//	printf("LargoNombre: %d\n", paquete->largoNombre);
//	printf("PosX: %d\n", paquete->posX);
//	printf("PosY: %d\n", paquete->posY);
//	printf("---Fin Mensaje APPEARED_POKEMON---\n");

	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0); //Todo . Valgrind tira error!
	//printf("Mande mensaje\n");



//	free(unPaquete->buffer->nombrePokemon);
//	list_destroy(unPaquete->buffer->listaCoordenadas);
//	free(unPaquete->buffer);
//	free(unPaquete);
	free(serializado);
	liberarPaquete(unPaquete);

}

void enviarMensajeBrokerCatch(char* nombrePokemon, int posX, int posY,
		int socketCliente) {
	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_CATCH_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->ack = 0;
	paquete->socket = 0;
	paquete->idMensaje = 0;
	paquete->idMensajeCorrelativo = 0;
	paquete->boolean = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();

	//printf("---Mensaje CATCH_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);
//	printf("Mande mensaje\n");
	free(serializado);
	liberarPaquete(unPaquete);
}

void enviarMensajeGameCardCatch(char* nombrePokemon, int posX, int posY,
		int idMensaje, int socketCliente) {
	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_CATCH_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->ack = 0;
	paquete->socket = 0;
	paquete->idMensaje = idMensaje;
	paquete->idMensajeCorrelativo = 0;
	paquete->boolean = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();

//	printf("---Mensaje CATCH_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);
	//printf("Mande mensaje\n");

	free(serializado);
	liberarPaquete(unPaquete);
}

void enviarMensajeBrokerCaught(int idMensajeCorrelativo, int booleano,
		int socketCliente) {
	//uint32_t stringSize = strlen(nombrePokemon) + 1;

	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_CAUGHT_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = 0;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->idMensaje = 0;
	paquete->idMensajeCorrelativo = idMensajeCorrelativo;
	paquete->socket = 0;
	paquete->ack = 0;
	paquete->nombrePokemon = string_new();
	//paquete->nombrePokemon[0]='\0';
	//paquete->nombrePokemon[0] = '\0';
	paquete->listaCoordenadas = list_create();
	paquete->boolean = booleano;

//	printf("---Mensaje CAUGHT_POKEMON---\n");

	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);
//	printf("Mande mensaje\n");

	free(serializado);
	liberarPaquete(unPaquete);

}

void enviarMensajeBrokerCaughtId(int idMensaje, int idMensajeCorrelativo,
		int booleano, int socketCliente) {
	//uint32_t stringSize = strlen(nombrePokemon) + 1;

	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_CAUGHT_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = 0;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->idMensaje = idMensaje;
	paquete->idMensajeCorrelativo = idMensajeCorrelativo;
	paquete->socket = 0;
	paquete->ack = 0;
	paquete->nombrePokemon = string_new();
	//paquete->nombrePokemon[0]='\0';
	//paquete->nombrePokemon[0] = '\0';
	paquete->listaCoordenadas = list_create();
	paquete->boolean = booleano;

//	printf("---Mensaje CAUGHT_POKEMON---\n");

	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);
//	printf("Mande mensaje\n");

	free(serializado);
	liberarPaquete(unPaquete);

}

void enviarMensajeLocalized(char* nombrePokemon, t_list* coordenadas,
		int socketCliente) {

	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_LOCALIZED_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->ack = 0;
	paquete->socket = 0;
	paquete->idMensaje = 0;
	paquete->idMensajeCorrelativo = 0;
	paquete->boolean = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();
	paquete->listaCoordenadas = list_duplicate(coordenadas);

	//printf("---Mensaje LOCALIZED_POKEMON---\n");

	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);
//	printf("Mande mensaje\n");

	free(serializado);
	liberarPaquete(unPaquete);

}

void enviarMensajeLocalizedId(char* nombrePokemon, t_list* coordenadas,
		int idMensaje, int idMensajeCorrelativo, int socketCliente) {

	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_LOCALIZED_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->ack = 0;
	paquete->socket = 0;
	paquete->idMensaje = idMensaje;
	paquete->idMensajeCorrelativo = idMensajeCorrelativo;
	paquete->boolean = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	//paquete->listaCoordenadas = list_create();
	paquete->listaCoordenadas = list_duplicate(coordenadas);

	//printf("---Mensaje LOCALIZED_POKEMON---\n");

	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);
//	printf("Mande mensaje\n");

	free(serializado);
	liberarPaquete(unPaquete);

}

void enviarMensajeTeamAppeared(char* nombrePokemon, int posX, int posY,
		int socketCliente) {
	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));

	unPaquete->codigoOperacion = MENSAJE_APPEARED_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->socket = 0;
	paquete->ack = 0;
	paquete->idMensaje = 0;
	paquete->idMensajeCorrelativo = 0;
	paquete->boolean = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();

//	printf("---Mensaje APPEARED_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");



	free(serializado);
	//free(paquete);
	//free(unPaquete);
	liberarPaquete(unPaquete);
}

void enviarMensajeGameCardNewPokemon(char* nombrePokemon, int posX, int posY,
		int cantidadPokemons, int idMensaje, int socketCliente) {

	printf("1\n");
	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	printf("2\n");
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_NEW_POKEMON;
	printf("3\n");
	//unPaquete->buffer= malloc(sizeof(t_bufferOmnidata));
	printf("4\n");
	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = cantidadPokemons;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->ack = 0;
	paquete->idMensaje = idMensaje;
	paquete->idMensajeCorrelativo = 0;
	paquete->boolean = 0;
	paquete->socket = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();

	//printf("---Mensaje NEW_POKEMON---\n");

	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	printf("5\n");
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	printf("6\n");
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");

	free(serializado);
	liberarPaquete(unPaquete);

}

void enviarMensajeGameCardCatchPokemon(char* nombrePokemon, int posX, int posY,
		int idMensaje, int socketCliente) {
	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_CATCH_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->idMensaje = idMensaje;
	paquete->idMensajeCorrelativo = 0;
	paquete->socket = 0;
	paquete->boolean = 0;
	paquete->ack = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();

//	printf("Se creara mensaje: \n");
//	printf("---Mensaje NEW_POKEMON---\n");
//	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
//	printf("LargoNombre: %d\n", paquete->largoNombre);
//	printf("PosX: %d\n", paquete->posX);
//	printf("PosY: %d\n", paquete->posY);
//	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");

//	free(unPaquete->buffer->nombrePokemon);
//	list_destroy(unPaquete->buffer->listaCoordenadas);
//	free(unPaquete->buffer);
//	free(unPaquete);
	free(serializado);
	liberarPaquete(unPaquete);
}

void enviarMensajeGameCardGetPokemon(char* nombrePokemon, int idMensaje,
		int socketCliente) {

	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_GET_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = stringSize;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->idMensaje = idMensaje;
	paquete->idMensajeCorrelativo = 0;
	paquete->socket = 0;
	paquete->boolean = 0;
	paquete->ack = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();

//	printf("Se creara mensaje: \n");
//	printf("---Mensaje NEW_POKEMON---\n");
//	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
//	printf("LargoNombre: %d\n", paquete->largoNombre);
//	printf("PosX: %d\n", paquete->posX);
//	printf("PosY: %d\n", paquete->posY);
//	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	free(serializado);
//	free(unPaquete->buffer);
//	free(unPaquete);
	liberarPaquete(unPaquete);

}

void suscribirseNew(char* nombreProceso, int tiempo, int socketCliente) {
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = SUSCRIBIRSE_NEW_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	uint32_t stringSizeProceso = strlen(nombreProceso) + 1;
	paquete->largoNombreProceso = stringSizeProceso;
	paquete->nombreProceso = string_duplicate(nombreProceso);

	paquete->cantidadPokemons = 0;
	paquete->idMensaje = 0;
	paquete->idMensajeCorrelativo = 0;
	paquete->largoNombre = 0;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->ack = 0;
	paquete->socket = 0;
	paquete->boolean = 0;
	paquete->nombrePokemon = string_new();
	//paquete->nombrePokemon[0]='\0';
	paquete->listaCoordenadas = list_create();

	//t_posicion *pos1, *pos2, *pos3, *pos4, *pos5;

//	printf("Se creara mensaje: \n");
//	printf("---Mensaje SUSCRIBIRSE_NEW_POKEMON---\n");
//	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
//	printf("LargoNombre: %d\n", paquete->largoNombre);
//	printf("PosX: %d\n", paquete->posX);
//	printf("PosY: %d\n", paquete->posY);
//	printf("---Fin Mensaje SUSCRIBIRSE_NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);

	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);

	free(serializado);
	//free(unPaquete->buffer);

//	free(paquete);
//	free(unPaquete);
	liberarPaquete(unPaquete);
}
void suscribirseGet(char* nombreProceso, int tiempo, int socketCliente) {
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = SUSCRIBIRSE_GET_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	uint32_t stringSizeProceso = strlen(nombreProceso) + 1;
	paquete->largoNombreProceso = stringSizeProceso;
	paquete->nombreProceso = string_duplicate(nombreProceso);

	paquete->cantidadPokemons = 0;
	paquete->largoNombre = 0;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->idMensaje = 0;
	paquete->idMensajeCorrelativo = 0;
	paquete->boolean = 0;
	paquete->ack = 0;
	paquete->nombrePokemon = string_new();
	//paquete->nombrePokemon[0]='\0';
	paquete->listaCoordenadas = list_create();
	paquete->socket = 0;

//	printf("Se creara mensaje: \n");
//	printf("---Mensaje SUSCRIBIRSE_GET_POKEMON---\n");
//	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
//	printf("LargoNombre: %d\n", paquete->largoNombre);
//	printf("PosX: %d\n", paquete->posX);
//	printf("PosY: %d\n", paquete->posY);
//	printf("---Fin Mensaje SUSCRIBIRSE_GET_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);

	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);

	free(serializado);
	//free(unPaquete->buffer);

//	free(paquete);
//	free(unPaquete);
	liberarPaquete(unPaquete);
}
void suscribirseCatch(char* nombreProceso, int tiempo, int socketCliente) {
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = SUSCRIBIRSE_CATCH_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	uint32_t stringSizeProceso = strlen(nombreProceso) + 1;
	paquete->largoNombreProceso = stringSizeProceso;
	paquete->nombreProceso = string_duplicate(nombreProceso);

	paquete->cantidadPokemons = 0;
	paquete->largoNombre = 0;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->idMensaje = 0;
	paquete->idMensajeCorrelativo = 0;
	paquete->boolean = 0;
	paquete->ack = 0;
	paquete->nombrePokemon = string_new();
	//paquete->nombrePokemon[0]='\0';
	paquete->listaCoordenadas = list_create();
	paquete->socket = 0;

//	printf("Se creara mensaje: \n");
//	printf("---Mensaje SUSCRIBIRSE_CATCH_POKEMON---\n");
//	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
//	printf("LargoNombre: %d\n", paquete->largoNombre);
//	printf("PosX: %d\n", paquete->posX);
//	printf("PosY: %d\n", paquete->posY);
//	printf("---Fin Mensaje SUSCRIBIRSE_CATCH_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);

	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);

	free(serializado);
	//free(unPaquete->buffer);

//	free(paquete);
//	free(unPaquete);
	liberarPaquete(unPaquete);
}
void suscribirseCaught(char* nombreProceso, int tiempo, int socketCliente) {
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = SUSCRIBIRSE_CAUGHT_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	uint32_t stringSizeProceso = strlen(nombreProceso) + 1;
	paquete->largoNombreProceso = stringSizeProceso;
	paquete->nombreProceso = string_duplicate(nombreProceso);

	paquete->cantidadPokemons = 0;
	paquete->largoNombre = 0;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->idMensaje = 0;
	paquete->idMensajeCorrelativo = 0;
	paquete->boolean = 0;
	paquete->ack = 0;
	paquete->nombrePokemon = string_new();
	//paquete->nombrePokemon[0]='\0';
	paquete->listaCoordenadas = list_create();
	paquete->socket = 0;

//	printf("Se creara mensaje: \n");
//	printf("---Mensaje SUSCRIBIRSE_CAUGHT_POKEMON---\n");
//	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
//	printf("LargoNombre: %d\n", paquete->largoNombre);
//	printf("PosX: %d\n", paquete->posX);
//	printf("PosY: %d\n", paquete->posY);
//	printf("---Fin Mensaje SUSCRIBIRSE_CAUGHT_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);

	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);

	free(serializado);
	//free(unPaquete->buffer);

//	free(paquete);
//	free(unPaquete);
	liberarPaquete(unPaquete);
}
void suscribirseAppeared(char* nombreProceso, int tiempo, int socketCliente) {
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = SUSCRIBIRSE_APPEARED_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	uint32_t stringSizeProceso = strlen(nombreProceso) + 1;
	paquete->largoNombreProceso = stringSizeProceso;
	paquete->nombreProceso = string_duplicate(nombreProceso);

	paquete->cantidadPokemons = 0;
	paquete->largoNombre = 0;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->idMensaje = 0;
	paquete->idMensajeCorrelativo = 0;
	paquete->boolean = 0;
	paquete->ack = 0;
	paquete->nombrePokemon = string_new();
	//paquete->nombrePokemon[0]='\0';
	paquete->listaCoordenadas = list_create();
	paquete->socket = tiempo;

//	printf("Se creara mensaje: \n");
//	printf("---Mensaje SUSCRIBIRSE_APPEARED_POKEMON---\n");
//	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
//	printf("LargoNombre: %d\n", paquete->largoNombre);
//	printf("PosX: %d\n", paquete->posX);
//	printf("PosY: %d\n", paquete->posY);
//	printf("---Fin Mensaje SUSCRIBIRSE_APPEARED_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);

	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);

	free(serializado);
	//free(unPaquete->buffer);

//	free(paquete);
//	free(unPaquete);
	liberarPaquete(unPaquete);
}
void suscribirseLocalized(char* nombreProceso, int tiempo, int socketCliente) {
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = SUSCRIBIRSE_LOCALIZED_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	uint32_t stringSizeProceso = strlen(nombreProceso) + 1;
	paquete->largoNombreProceso = stringSizeProceso;
	paquete->nombreProceso = string_duplicate(nombreProceso);

	paquete->cantidadPokemons = 0;
	paquete->largoNombre = 0;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->idMensaje = 0;
	paquete->idMensajeCorrelativo = 0;
	paquete->boolean = 0;
	paquete->ack = 0;
	paquete->nombrePokemon = string_new();
	//paquete->nombrePokemon[0]='\0';
	paquete->listaCoordenadas = list_create();
	paquete->socket = 0;

//	printf("Se creara mensaje: \n");
//	printf("---Mensaje SUSCRIBIRSE_LOCALIZED_POKEMON---\n");
//	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
//	printf("LargoNombre: %d\n", paquete->largoNombre);
//	printf("PosX: %d\n", paquete->posX);
//	printf("PosY: %d\n", paquete->posY);
//	printf("---Fin Mensaje SUSCRIBIRSE_LOCALIZED_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);

	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);

	free(serializado);
	//free(unPaquete->buffer);

//	free(paquete);
//	free(unPaquete);
	liberarPaquete(unPaquete);
}
void enviarAck(char* nombreProceso, t_paquete* mensaje, int socketCliente) {
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = CONFIRMACION_ACK;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	uint32_t stringSizeProceso = strlen(nombreProceso) + 1;
	paquete->largoNombreProceso = stringSizeProceso;
	paquete->nombreProceso = string_duplicate(nombreProceso);
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = 0;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->idMensaje = mensaje->buffer->idMensaje;
	paquete->idMensajeCorrelativo = 0;
	paquete->boolean = 0;
	paquete->ack = mensaje->codigoOperacion;
	paquete->nombrePokemon = string_new();
	//paquete->nombrePokemon[0]='\0';
	paquete->listaCoordenadas = list_create();
	paquete->socket = 0;

	printf("Se envia mensaje ACK: \n");
	printf("---CONFIRMACION_ACK---\n");
	printf("NombreProceso: %s\n", paquete->nombreProceso);
	printf("LargoNombreProceso: %d\n", paquete->largoNombreProceso);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("IdMensaje: %d\n", paquete->idMensaje);
	printf("---Fin CONFIRMACION_ACK---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	free(serializado);
	//free(unPaquete->buffer);
//	free(paquete);
//	free(unPaquete);
	liberarPaquete(unPaquete);
}
void enviarIdMensaje(int idMensaje, int socketCliente) {
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = ENVIAR_ID_MENSAJE;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

	paquete->largoNombreProceso = 0;
	paquete->nombreProceso = string_new();
	paquete->cantidadPokemons = 0;
	paquete->largoNombre = 0;
	paquete->posX = 0;
	paquete->posY = 0;
	paquete->idMensaje = idMensaje;
	paquete->idMensajeCorrelativo = 0;
	paquete->boolean = 0;
	paquete->ack = 0;
	paquete->nombrePokemon = string_new();
	//paquete->nombrePokemon[0]='\0';
	paquete->listaCoordenadas = list_create();
	paquete->socket = 0;

//	printf("Se enviará idMensaje: \n");
//	printf("---Mensaje ENVIAR_ID_MENSAJE---\n");
//	printf("IdMensaje: %d\n", paquete->idMensaje);
//	printf("---Fin ENVIAR_ID_MENSAJE---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado = 0;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);

	//send(socketCliente, &sizeSerializado, sizeof(int), 0);
	send(socketCliente, serializado, sizeSerializado, 0);

	free(serializado);
	//free(unPaquete->buffer);

//	free(paquete);
//	free(unPaquete);
	liberarPaquete(unPaquete);
}

void check_host_name(int hostname) { //This function returns host name for local computer
	if (hostname == -1) {
		perror("gethostname");
		exit(1);
	}
}
void check_host_entry(struct hostent * hostentry) { //find host info from host name
	if (hostentry == NULL) {
		perror("gethostbyname");
		exit(1);
	}
}
void IP_formatter(char *IPbuffer) { //convert IP string to dotted decimal format
	if (NULL == IPbuffer) {
		perror("inet_ntoa");
		exit(1);
	}
}
char* getIp() {

	char host[256];
	char *IP;
	struct hostent *host_entry;
	int hostname = 0;
	hostname = gethostname(host, sizeof(host)); //find the host name
	check_host_name(hostname);
	host_entry = gethostbyname(host); //find host information
	check_host_entry(host_entry);
	IP = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])); //Convert into IP string
	printf("Current Host Name: %s\n", host);
	printf("Host IP: %s\n", IP);
	return IP;
}
char* getNombreProceso(char* nombre) {

	char* buffer = string_duplicate(nombre);
	return buffer;

}
