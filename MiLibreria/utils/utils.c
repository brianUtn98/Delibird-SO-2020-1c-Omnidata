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

//void mostrarListaCoordenadas(t_list *lista){
//t_list *aux=list_duplicate(lista);
//
//	while(aux->head!=NULL){
//	mostrarCoordenada(aux->head->data);
//	aux->head=aux->head->next;
//	}
//	list_destroy(aux);
//}

//void mostrarCoordenada(void *data){
//	t_posicion *buffer=malloc(sizeof(t_posicion));
//	buffer=(t_posicion*)data;
//	printf("X=%d,Y=%d\n",buffer->x,buffer->y);
//	free(buffer);
//
//return;
//}

void liberarConexion(int socket) {
	close(socket);
	return;
}

void* serializarPaquete(t_paquete* paquete, int *bytes) {
	int sizeSerializado = sizeof(t_header) + sizeof(bool) + sizeof(int)
			+ sizeof(int) + sizeof(int) + sizeof(int)
			+ paquete->buffer->largoNombre + sizeof(int) + sizeof(int)
			+ sizeof(int)
			+ paquete->buffer->listaCoordenadas->elements_count * sizeof(t_posicion);

	printf("Estoy antes del malloc\n");
	void* buffer = malloc(sizeSerializado);
	printf("Estoy despues del malloc\n");
	int desplazamiento = 0;
	printf("Estoy en 1\n");
//	memcpy(buffer + desplazamiento, &(paquete->pid), sizeof(pid_t));
//	desplazamiento += sizeof(pid_t);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento, &(paquete->codigoOperacion),
			sizeof(t_header));
	desplazamiento += sizeof(t_header);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento, &(paquete->buffer->boolean), sizeof(bool));
	desplazamiento += sizeof(bool);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento, &(paquete->buffer->cantidadPokemons),
			sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento, &(paquete->buffer->idMensaje), sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento, &(paquete->buffer->largoNombre),
			sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento, paquete->buffer->nombrePokemon,
			paquete->buffer->largoNombre);
	desplazamiento += paquete->buffer->largoNombre;
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento, &(paquete->buffer->posX), sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento, &(paquete->buffer->posY), sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento, &(paquete->buffer->tiempo), sizeof(int));
	desplazamiento += sizeof(int);

	printf("Estoy en 2\n");
	int cantidadCoordenadas = paquete->buffer->listaCoordenadas->elements_count;
	printf("Estoy en 2\n");
	//coordenadas=malloc(cantidadCoordenadas*sizeof(int));
	printf("Estoy en 2\n");
	//coordenadas=aplanarLista(paquete->buffer->listaCoordenadas);
	memcpy(buffer + desplazamiento, &cantidadCoordenadas, sizeof(int));
	desplazamiento += sizeof(int);

	t_list*aux = list_duplicate(paquete->buffer->listaCoordenadas);

	while (aux->head != NULL) {
		t_posicion *buffercito=malloc(sizeof(t_posicion));
		buffercito=(t_posicion*)aux->head->data;
		printf("Buffercito vale %d,%d\n", buffercito->x,buffercito->y);
		memcpy(buffer + desplazamiento, buffercito, sizeof(t_posicion));
		desplazamiento += sizeof(t_posicion);
		aux->head = aux->head->next;
		free(buffercito);
		//i++;
	}

	(*bytes) = sizeSerializado;
	printf("Termine de serializar\n");
	return buffer;

}

t_paquete* recibirMensaje(int socketCliente) {
	t_paquete *paquete;
	void *buffer = malloc(100 * sizeof(void));
	printf("Estoy en 1\n");
	recv(socketCliente, buffer, 100 * sizeof(void), MSG_WAITALL);
	printf("Estoy en 1\n");
	paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_bufferOmnidata));
	int desplazamiento = 0;
	printf("Estoy en 1\n");
	printf("Estoy asignando %d\n", (int) buffer + desplazamiento);
	printf("Estoy en 1\n");
	memcpy(&paquete->codigoOperacion, buffer + desplazamiento,
			sizeof(t_header));
	desplazamiento += sizeof(t_header);
	printf("Estoy en 1\n");
	memcpy(&paquete->buffer->boolean, buffer + desplazamiento, sizeof(bool));
	desplazamiento += sizeof(bool);
	printf("Estoy en 1\n");
	printf("Estoy asignando %d\n", (int) buffer + desplazamiento);
	memcpy(&paquete->buffer->cantidadPokemons, buffer + desplazamiento,
			sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy asignando %d\n", (int) buffer + desplazamiento);
	printf("Estoy en 1\n");
	memcpy(&paquete->buffer->idMensaje, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy asignando %d\n", (int) buffer + desplazamiento);
	printf("Estoy en 1\n");
	memcpy(&paquete->buffer->largoNombre, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);
	paquete->buffer->nombrePokemon = malloc(paquete->buffer->largoNombre);
	printf("Estoy en 1\n");
	memcpy(paquete->buffer->nombrePokemon, buffer + desplazamiento,
			paquete->buffer->largoNombre);
	desplazamiento += paquete->buffer->largoNombre;
	printf("Estoy asignando %d\n", (int) buffer + desplazamiento);
	printf("Estoy en 1\n");
	memcpy(&paquete->buffer->posX, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy asignando %d\n", (int) buffer + desplazamiento);
	printf("Estoy en 1\n");
	memcpy(&paquete->buffer->posY, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy en 1\n");
	memcpy(&paquete->buffer->tiempo, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	int cantidadCoordenadas;
	memcpy(&cantidadCoordenadas, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);
	//int *coordenadas = malloc(sizeof(int) * cantidadCoordenadas);
	int k;
	t_posicion *agregar;
	t_posicion *buffercito;
	paquete->buffer->listaCoordenadas=list_create();
	for (k = 0; k < cantidadCoordenadas; k++) {
		buffercito=malloc(sizeof(t_posicion));
		memcpy(buffercito, buffer + desplazamiento, sizeof(t_posicion));
		printf("Agarre: %d,%d\n", buffercito->x,buffercito->y);
		agregar=buffercito;
		list_add(paquete->buffer->listaCoordenadas,(void*)agregar);
		free(buffercito);
		desplazamiento += sizeof(t_posicion);
	}


	//mostrarListaCoordenadas(paquete->buffer->listaCoordenadas);

	void mostrarCoordenada(void *data){
		t_posicion *buffer=malloc(sizeof(t_posicion));
		buffer=(t_posicion*)data;
		printf("X=%d,Y=%d\n",buffer->x,buffer->y);
		free(buffer);
	}
	list_iterate(paquete->buffer->listaCoordenadas,mostrarCoordenada);

	printf("En el campo de texto recibo: %s\n", paquete->buffer->nombrePokemon);

	printf("Termine\n");
	free(buffer);
	return paquete;


}

int *aplanarLista(t_list* lista) {
	t_list*aux = list_duplicate(lista);
	int *array = malloc(sizeof(int) * lista->elements_count);
	int i = 0;
	while (aux->head != NULL) {
		printf("Aplanando %d\n", (int) aux->head->data);
		*(array + i) = (int) aux->head->data;
		printf("Aplanado: %d\n", array[i]);
		aux->head = aux->head->next;
		i++;
	}
	return array;
}

void enviarMensajeBrokerNew(char* nombrePokemon, int posX, int posY,
		int cantidadPokemons, int socketCliente) {

	uint32_t stringSize = strlen(nombrePokemon) + 1;
	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_NEW_POKEMON;

	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
	paquete->cantidadPokemons = cantidadPokemons;
	paquete->largoNombre = stringSize;
	paquete->posX = posX;
	paquete->posY = posY;
	paquete->tiempo = 0;
	paquete->nombrePokemon = string_duplicate(nombrePokemon);
	paquete->listaCoordenadas = list_create();

	t_posicion *pos1,*pos2,*pos3,*pos4,*pos5;
	pos1=malloc(sizeof(t_posicion));
	pos2=malloc(sizeof(t_posicion));
	pos3=malloc(sizeof(t_posicion));
	pos4=malloc(sizeof(t_posicion));
	pos5=malloc(sizeof(t_posicion));
	pos1->x=3; pos1->y=2;
	pos2->x=5; pos2->y=5;
	pos3->x=2; pos3->y=1;
	pos4->x=3; pos4->y=6;
	pos5->x=6; pos5->y=1;
	list_add(paquete->listaCoordenadas,(void*)pos1);
	list_add(paquete->listaCoordenadas,(void*)pos2);
	list_add(paquete->listaCoordenadas,(void*)pos3);
	list_add(paquete->listaCoordenadas,(void*)pos4);
	list_add(paquete->listaCoordenadas,(void*)pos5);

	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");

	free(serializado);

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
	paquete->listaCoordenadas = list_create();

	printf("Se creara mensaje: \n");
	printf("---Mensaje GET_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje GET_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
		free(serializado);

		free(unPaquete->buffer->nombrePokemon);
		list_destroy(unPaquete->buffer->listaCoordenadas);
		free(unPaquete->buffer);
		free(unPaquete);
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

	printf("Se creara mensaje: \n");
	printf("---Mensaje APPEARED_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje APPEARED_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado;
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

	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	free(serializado);
	free(unPaquete->buffer);
	free(paquete);
	free(unPaquete);
}

void enviarMensajeBrokerCaught(int idMensaje, bool booleano, int socketCliente) {
	//uint32_t stringSize = strlen(nombrePokemon) + 1;

	t_paquete* unPaquete = malloc(sizeof(t_paquete));
	t_bufferOmnidata *paquete = malloc(sizeof(t_bufferOmnidata));
	unPaquete->codigoOperacion = MENSAJE_CAUGHT_POKEMON;
	unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));

//	paquete->cantidadPokemons = 0;
//	paquete->largoNombre = stringSize;
//	paquete->posX = 0;
//	paquete->posY = 0;
	paquete->idMensaje = idMensaje;
//	paquete->nombrePokemon = string_duplicate(nombrePokemon);
//	paquete->listaCoordenadas = list_create();
//

	paquete->boolean = booleano;

	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	free(serializado);
	free(unPaquete->buffer);
	free(paquete);
	free(unPaquete);

}

void enviarMensajeLocalized(char* nombrePokemon, t_list coordenadas,
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
	list_add(paquete->listaCoordenadas, (void*) 1);
	list_add(paquete->listaCoordenadas, (void*) 2);
	list_add(paquete->listaCoordenadas, (void*) 3);
	list_add(paquete->listaCoordenadas, (void*) 4);
	list_add(paquete->listaCoordenadas, (void*) 5);
	list_add(paquete->listaCoordenadas, (void*) 6);
	list_add(paquete->listaCoordenadas, (void*) 7);
	list_add(paquete->listaCoordenadas, (void*) 8);
	//		list_add(paquete->listaCoordenadas,3);
	//		list_add(paquete->listaCoordenadas,1000);
	//unPaquete->buffer->size=sizeof(paquete);

	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	free(serializado);
	free(unPaquete->buffer);
	free(paquete);
	free(unPaquete);

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

	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	free(serializado);
	free(unPaquete->buffer);
	free(paquete);
	free(unPaquete);

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

	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n", paquete->nombrePokemon);
	printf("LargoNombre: %d\n", paquete->largoNombre);
	printf("PosX: %d\n", paquete->posX);
	printf("PosY: %d\n", paquete->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");
	unPaquete->buffer = paquete;

	int sizeSerializado;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");

	free(serializado);

	//	free(unPaquete->buffer);
	//	free(paquete);
	//	free(unPaquete);

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

	int sizeSerializado;
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

	int sizeSerializado;
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

	int sizeSerializado;
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

	int sizeSerializado;
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

	int sizeSerializado;
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

	int sizeSerializado;
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

	int sizeSerializado;
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

	int sizeSerializado;
	void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
	send(socketCliente, serializado, sizeSerializado, 0);
	printf("Mande mensaje\n");
	free(serializado);
	free(unPaquete->buffer);
	free(paquete);
	free(unPaquete);
}
