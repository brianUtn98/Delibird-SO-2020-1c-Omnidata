#include "utils.h"

int crearConexion(char *ip, int puerto, int tiempoReconexion) {
	printf("Intentando conectar a PUERTO=%d en IP=%s", puerto, ip);
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

void liberarConexion(int socket) {
	close(socket);
	return;
}

t_paquete *recibirMensajeNewPokemon(int socket_cliente, int* size) {

	int sizeMensaje = 0;

	void*buffer;

	t_paquete *paquete = malloc(sizeof(t_paquete));
	int pid;
	t_opCode codigoOperacion;
	t_colaMensaje colaMensaje;
	int posX;
	int posY;
	int cantidadPokemons;

	recv(socket_cliente, &pid, sizeof(pid), 0);
	recv(socket_cliente, &codigoOperacion, sizeof(codigoOperacion), 0);
	recv(socket_cliente, &colaMensaje, sizeof(colaMensaje), 0);
	recv(socket_cliente, &sizeMensaje, sizeof(int), 0);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, sizeMensaje, 0);

	recv(socket_cliente, &posX, sizeof(int), 0);

	recv(socket_cliente, &posY, sizeof(int), 0);

	recv(socket_cliente, &cantidadPokemons, sizeof(int), 0);

	paquete->pid = pid;
	paquete->codigoOperacion = codigoOperacion;
	paquete->colaMensaje = colaMensaje;
	paquete->buffer = malloc(sizeMensaje);
	paquete->buffer->size = sizeMensaje;

	paquete->buffer->stream = buffer;

	paquete->buffer->posX = posX;

	paquete->buffer->posY = posY;

	paquete->buffer->cantidadPokemons = cantidadPokemons;

	(*size) = sizeMensaje;
	return paquete;

	free(paquete->buffer);
	free(buffer);
	free(paquete);
}
void* serializarPaqueteNewPokemon(t_paquete* paquete, int bytes) {
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->pid), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(magic + desplazamiento, &(paquete->codigoOperacion), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(magic + desplazamiento, &paquete->colaMensaje, sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(magic + desplazamiento, paquete->buffer->stream,
			paquete->buffer->size);
	desplazamiento += paquete->buffer->size;

	memcpy(magic + desplazamiento, &(paquete->buffer->posX), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(magic + desplazamiento, &(paquete->buffer->posY), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(magic + desplazamiento, &(paquete->buffer->cantidadPokemons),
			sizeof(int));
	desplazamiento += sizeof(int);

	printf("Mensaje serializado: %s", (char*) magic);
	return magic;
	free(magic);
}

void crearMensajeANewPokemon(int pid, char* nombrePokemon, int posX, int posY,
		int cantidadPokemons, int socket_cliente) {
	t_paquete* paquete = malloc(sizeof(t_paquete));

	int stringSize = strlen(nombrePokemon);

	char* string = malloc((stringSize + 1) * sizeof(char));

	strcpy(string, nombrePokemon);

	string[stringSize] = '0';

	paquete->pid = pid;
	paquete->codigoOperacion = SUSCRIPCION;
	paquete->colaMensaje = tNEW_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = stringSize + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, string, paquete->buffer->size);
	free(string);

	paquete->buffer->posX = posX;
	paquete->buffer->posY = posY;
	paquete->buffer->cantidadPokemons = cantidadPokemons;

	printf("Se creara mensaje\n");
	printf("Mi pid es=%d\n", paquete->pid);
	printf("Codigo de operacion=%d\n", paquete->codigoOperacion);
	printf("Cola de mensaje=%d\n", paquete->colaMensaje);
	printf("Contenido=%s\n", (char*) paquete->buffer->stream);
	printf("Tamano %d bytes\n", paquete->buffer->size);
	int bytes = paquete->buffer->size + 7 * sizeof(int);
	printf("Tamano %d bytes en total para serializar.\n", bytes);

	void* aEnviar = malloc(sizeof(bytes));
	aEnviar = serializarPaqueteNewPokemon(paquete, bytes);

	send(socket_cliente, aEnviar, bytes, 0);

	free(aEnviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void devolverMensajeConfirmacion(void* payload, int socket_cliente) {
	int size = sizeof(payload);
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigoOperacion = SUSCRIPCION;
	paquete->colaMensaje = tNEW_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);
	printf("Se creara mensaje\n");
	printf("Codigo de operacion=%d\n", paquete->codigoOperacion);
	printf("Cola de mensaje=%d\n", paquete->colaMensaje);
	printf("Contenido=%s\n", (char*) paquete->buffer->stream);
	printf("Tamano %d bytes\n", paquete->buffer->size);
	int bytes = paquete->buffer->size + 3 * sizeof(int);

	void* a_enviar = serializarPaqueteNewPokemon(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

char* recibirConfirmacion(int socket_cliente) {
	t_opCode operacion;
	int e = recv(socket_cliente, &operacion, sizeof(operacion), 0);
	if (e == -1) {
		perror("Error al recibir el mensaje");
		exit(1);
	}
	int buffer_size;
	int e2 = recv(socket_cliente, &buffer_size, sizeof(buffer_size), 0);
	if (e2 == -1) {
		perror("Error al recibir el mensaje");
		exit(1);
	}
	char *buffer = malloc(buffer_size);
	recv(socket_cliente, buffer, buffer_size, 0);
	if (buffer[buffer_size - 1] != '\0') {
		printf("El buffer recibido no es un string\n");
	}
	return buffer;
}

t_suscriptor *recibirSuscriptor(int socket_cliente) {

	//int sizeMensaje = 0;

	t_suscriptor *suscriptor = malloc(sizeof(t_suscriptor));
	int pid;
	t_opCode codigoOperacion;
	t_colaMensaje colaMensaje;
	int sizeRecibido;

	recv(socket_cliente, &pid, sizeof(pid), 0);
	recv(socket_cliente, &codigoOperacion, sizeof(codigoOperacion), 0);
	recv(socket_cliente, &colaMensaje, sizeof(colaMensaje), 0);
	recv(socket_cliente, &sizeRecibido, sizeof(int), 0); // esto es para validar que haya llegado bien el paquete

	suscriptor->pid = pid;
	suscriptor->codigoOperacion = codigoOperacion;
	suscriptor->colaMensaje = colaMensaje;

	//(*size) = sizeMensaje;
	return suscriptor;

	free(suscriptor);
}

void* serializarSuscriptor(t_suscriptor* suscriptor, int bytes) {
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(suscriptor->pid), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(magic + desplazamiento, &(suscriptor->codigoOperacion), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(magic + desplazamiento, &(suscriptor->colaMensaje), sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(magic + desplazamiento, &(bytes), sizeof(int));
	desplazamiento += sizeof(int);

	printf("Mensaje serializado: %s", (char*) magic);
	return magic;
	free(magic);
}

void suscribirseAcola(int pid, char* nombreCola, int socket) {

	t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));

	suscriptor->pid = pid;
	suscriptor->codigoOperacion = SUSCRIPCION;
	if (strcmp("NEW_POKEMON", nombreCola) == 0) {

		suscriptor->colaMensaje = tNEW_POKEMON;
	}
	if (strcmp("APPEARED_POKEMON", nombreCola) == 0) {
		suscriptor->colaMensaje = tAPPEARED_POKEMON;
	}
	if (strcmp("CATCH_POKEMON", nombreCola) == 0) {
		suscriptor->colaMensaje = tCATCH_POKEMON;
	}
	if (strcmp("CAUGTH_POKEMON", nombreCola) == 0) {
		suscriptor->colaMensaje = tCAUGTH_POKEMON;
	}
	if (strcmp("GET_POKEMON", nombreCola) == 0) {
		suscriptor->colaMensaje = tGET_POKEMON;
	}
	if (strcmp("LOCALIZED_POKEMON", nombreCola) == 0) {
		suscriptor->colaMensaje = tLOCALIZED_POKEMON;
	} else {
		printf("no se reconoce la cola a la que se quiere suscribir");
	}
	printf("Se creara mensaje\n");
	printf("Mi pid es=%d\n", suscriptor->pid);
	printf("Codigo de operacion=%d\n", suscriptor->codigoOperacion);
	printf("Cola de mensaje=%d\n", suscriptor->colaMensaje);

	int bytes = 4 * sizeof(int);
	printf("Tamano %d bytes en total para serializar.\n", bytes);
	void* aEnviar = malloc(sizeof(bytes));
	aEnviar = serializarSuscriptor(suscriptor, bytes);

	send(socket, aEnviar, bytes, 0);
}

