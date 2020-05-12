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

/*
 // Mensaje a cola NEW_POKEMON
 t_paquete *recibirMensajeNewPokemon(int socketCliente, int* bytes) {

 t_paquete *paquete = malloc(sizeof(t_paquete));

 paquete->buffer = malloc(sizeof(t_buffer));

 recv(socketCliente, &bytes, sizeof(int), 0);
 recv(socketCliente, &paquete->pid, sizeof(int), 0);
 recv(socketCliente, &paquete->codigoOperacion, sizeof(int), 0);
 recv(socketCliente, &paquete->colaMensaje, sizeof(int), 0);
 recv(socketCliente, &paquete->buffer->size, sizeof(int), 0);

 recv(socketCliente, &paquete->buffer->stream, paquete->buffer->size, 0);

 //(*size) = sizeMensaje;
 return paquete;

 free(paquete->buffer);

 free(paquete);
 }
 void* serializarPaqueteNewPokemon(t_paquete* paquete) {
 void * magic = malloc(paquete->bytesMensaje);
 int desplazamiento = 0;

 memcpy(magic + desplazamiento, &(paquete->bytesMensaje), sizeof(int));
 desplazamiento += sizeof(int);

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

 printf("Mensaje serializado: %s", (char*) magic);
 return magic;
 free(magic);
 }
 void crearMensajeNewPokemon(int pid, char* nombrePokemon, int posX, int posY,
 int cantidadPokemons, int socketCliente) {
 t_paquete* paquete = malloc(sizeof(t_paquete));

 int desplazamiento = 0;
 int stringSize = strlen(nombrePokemon);

 char* string = malloc((stringSize + 1) * sizeof(char));

 strcpy(string, nombrePokemon);

 string[stringSize] = '\0';

 paquete->header = MENSAJE_NEW_POKEMON;

 paquete->buffer = malloc(sizeof(t_buffer));

 paquete->buffer->size = ((stringSize + 1) * sizeof(char)) + 4 * sizeof(int);

 paquete->buffer->stream = malloc(paquete->buffer->size);

 memcpy(paquete->buffer->stream, (void*) stringSize + 1, sizeof(int));
 desplazamiento += sizeof(int);

 memcpy(paquete->buffer->stream, (void*) string, stringSize + 1);
 desplazamiento += stringSize + 1;

 memcpy(paquete->buffer->stream, (void*) posX, sizeof(int));
 desplazamiento += sizeof(int);
 memcpy(paquete->buffer->stream, (void*) posY, sizeof(int));
 desplazamiento += sizeof(int);
 memcpy(paquete->buffer->stream, (void*) cantidadPokemons, sizeof(int));
 desplazamiento += sizeof(int);

 send(socketCliente, (void*) paquete, paquete->buffer->size + 1, 0);

 //free(aEnviar);
 free(paquete->buffer->stream);
 free(paquete->buffer);
 free(paquete);
 }


 // Mensaje a cola GET_POKEMON envia el nombre

 t_paquete *recibirMensajeGetPokemon(int socket_cliente, int* size) {

 int sizeMensaje = 0;

 void*buffer;

 t_paquete *paquete = malloc(sizeof(t_paquete));
 int pid;
 t_opCode codigoOperacion;
 t_colaMensaje colaMensaje;

 recv(socket_cliente, &pid, sizeof(pid), 0);
 recv(socket_cliente, &codigoOperacion, sizeof(codigoOperacion), 0);
 recv(socket_cliente, &colaMensaje, sizeof(colaMensaje), 0);
 recv(socket_cliente, &sizeMensaje, sizeof(int), 0);
 buffer = malloc(*size);
 recv(socket_cliente, buffer, sizeMensaje, 0);

 paquete->pid = pid;
 paquete->codigoOperacion = codigoOperacion;
 paquete->colaMensaje = colaMensaje;
 paquete->buffer = malloc(sizeMensaje);
 paquete->buffer->size = sizeMensaje;

 paquete->buffer->stream = buffer;

 (*size) = sizeMensaje;
 return paquete;

 free(paquete->buffer);
 free(buffer);
 free(paquete);
 }
 void* serializarPaqueteGetPokemon(t_paquete* paquete, int bytes) {
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

 printf("Mensaje serializado: %s", (char*) magic);
 return magic;
 free(magic);
 }
 void crearMensajeGetPokemon(int pid, char* nombrePokemon, int socket_cliente) {
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

 printf("Se creara mensaje\n");
 printf("Mi pid es=%d\n", paquete->pid);
 printf("Codigo de operacion=%d\n", paquete->codigoOperacion);
 printf("Cola de mensaje=%d\n", paquete->colaMensaje);
 printf("Contenido=%s\n", (char*) paquete->buffer->stream);
 printf("Tamano %d bytes\n", paquete->buffer->size);
 int bytes = paquete->buffer->size + 4 * sizeof(int);
 printf("Tamano %d bytes en total para serializar.\n", bytes);

 void* aEnviar = malloc(sizeof(bytes));
 aEnviar = serializarPaqueteGetPokemon(paquete, bytes);

 send(socket_cliente, aEnviar, bytes, 0);

 free(aEnviar);
 free(paquete->buffer->stream);
 free(paquete->buffer);
 free(paquete);
 }

 //Mensaje a cola APPEARED_POKEMON
 t_paquete *recibirMensajeAppearedPokemon(int socket_cliente, int* size) {

 int sizeMensaje = 0;

 void*buffer;

 t_paquete *paquete = malloc(sizeof(t_paquete));

 int pid;
 t_opCode codigoOperacion;
 t_colaMensaje colaMensaje;

 int posX;
 int posY;

 recv(socket_cliente, &pid, sizeof(pid), 0);
 recv(socket_cliente, &codigoOperacion, sizeof(codigoOperacion), 0);
 recv(socket_cliente, &colaMensaje, sizeof(colaMensaje), 0);
 recv(socket_cliente, &sizeMensaje, sizeof(int), 0);
 buffer = malloc(*size);
 recv(socket_cliente, buffer, sizeMensaje, 0);

 recv(socket_cliente, &posX, sizeof(int), 0);

 recv(socket_cliente, &posY, sizeof(int), 0);

 paquete->pid = pid;
 paquete->codigoOperacion = codigoOperacion;
 paquete->colaMensaje = colaMensaje;
 paquete->buffer = malloc(sizeMensaje);
 paquete->buffer->size = sizeMensaje;

 paquete->buffer->stream = buffer;

 paquete->buffer->posX = posX;

 paquete->buffer->posY = posY;

 (*size) = sizeMensaje;
 return paquete;

 free(paquete->buffer);
 free(buffer);
 free(paquete);
 }
 void* serializarPaqueteAppearedPokemon(t_paquete* paquete, int bytes) {
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

 printf("Mensaje serializado: %s", (char*) magic);
 return magic;
 free(magic);
 }
 void crearMensajeApperedPokemon(int pid, char* nombrePokemon, int posX,
 int posY, int socket_cliente) {
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

 printf("Se creara mensaje\n");
 printf("Mi pid es=%d\n", paquete->pid);
 printf("Codigo de operacion=%d\n", paquete->codigoOperacion);
 printf("Cola de mensaje=%d\n", paquete->colaMensaje);
 printf("Contenido=%s\n", (char*) paquete->buffer->stream);
 printf("Tamano %d bytes\n", paquete->buffer->size);
 int bytes = paquete->buffer->size + 6 * sizeof(int);
 printf("Tamano %d bytes en total para serializar.\n", bytes);

 void* aEnviar = malloc(sizeof(bytes));
 aEnviar = serializarPaqueteNewPokemon(paquete, bytes);

 send(socket_cliente, aEnviar, bytes, 0);

 free(aEnviar);
 free(paquete->buffer->stream);
 free(paquete->buffer);
 free(paquete);
 }

 // Mensaje a cola CATCH_POKEMON
 t_paquete *recibirMensajeCatchPokemon(int socket_cliente, int* size) {

 int sizeMensaje = 0;

 void*buffer;

 t_paquete *paquete = malloc(sizeof(t_paquete));
 int pid;
 t_opCode codigoOperacion;
 t_colaMensaje colaMensaje;
 int posX;
 int posY;

 recv(socket_cliente, &pid, sizeof(pid), 0);
 recv(socket_cliente, &codigoOperacion, sizeof(codigoOperacion), 0);
 recv(socket_cliente, &colaMensaje, sizeof(colaMensaje), 0);
 recv(socket_cliente, &size, sizeof(colaMensaje), 0);

 recv(socket_cliente, &sizeMensaje, sizeof(int), 0);
 buffer = malloc(sizeMensaje);
 recv(socket_cliente, buffer, sizeMensaje, 0);

 recv(socket_cliente, &posX, sizeof(int), 0);

 recv(socket_cliente, &posY, sizeof(int), 0);

 paquete->pid = pid;
 paquete->codigoOperacion = codigoOperacion;
 paquete->colaMensaje = colaMensaje;
 paquete->buffer = malloc(sizeMensaje);
 paquete->buffer->size = sizeMensaje;

 paquete->buffer->stream = buffer;

 paquete->buffer->posX = posX;

 paquete->buffer->posY = posY;

 //(*size) = sizeMensaje;
 return paquete;

 free(paquete->buffer);
 free(buffer);
 free(paquete);
 }
 void* serializarPaqueteCatchPokemon(t_paquete* paquete, int bytes) {
 void * magic = malloc(bytes);
 int desplazamiento = 0;

 memcpy(magic + desplazamiento, &(paquete->pid), sizeof(int));
 desplazamiento += sizeof(int);

 memcpy(magic + desplazamiento, &(paquete->codigoOperacion), sizeof(int));
 desplazamiento += sizeof(int);

 memcpy(magic + desplazamiento, &paquete->colaMensaje, sizeof(int));
 desplazamiento += sizeof(int);

 memcpy(magic + desplazamiento, &bytes, sizeof(int));
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

 printf("Mensaje serializado: %s", (char*) magic);
 return magic;
 free(magic);
 }
 void crearMensajeCatchPokemon(int pid, char* nombrePokemon, int posX, int posY,
 int socket_cliente) {
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


 // Mensaje a cola CAUGHT_POKEMON
 t_paquete *recibirMensajeCaughtPokemon(int socket_cliente, int* size) {

 int sizeMensaje = 0;

 t_paquete *paquete = malloc(sizeof(t_paquete));
 int pid;
 t_opCode codigoOperacion;
 t_colaMensaje colaMensaje;
 int booleano;

 recv(socket_cliente, &pid, sizeof(pid), 0);
 recv(socket_cliente, &codigoOperacion, sizeof(codigoOperacion), 0);
 recv(socket_cliente, &colaMensaje, sizeof(colaMensaje), 0);
 recv(socket_cliente, &sizeMensaje, sizeof(int), 0);

 recv(socket_cliente, &booleano, sizeof(int), 0);

 paquete->pid = pid;
 paquete->codigoOperacion = codigoOperacion;
 paquete->colaMensaje = colaMensaje;
 paquete->buffer = malloc(sizeMensaje);
 paquete->buffer->size = sizeMensaje;

 paquete->buffer->booleano = booleano;

 (*size) = sizeMensaje;
 return paquete;

 free(paquete->buffer);
 free(paquete);
 }
 void* serializarPaqueteCaughtPokemon(t_paquete* paquete, int bytes) {
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

 memcpy(magic + desplazamiento, &(paquete->buffer->booleano), sizeof(int));
 desplazamiento += sizeof(int);

 printf("Mensaje serializado: %s", (char*) magic);
 return magic;
 free(magic);
 }
 void crearMensajeCaughtPokemon(int pid, int booleano, int socket_cliente) {
 t_paquete* paquete = malloc(sizeof(t_paquete));

 paquete->pid = pid;
 paquete->codigoOperacion = SUSCRIPCION;
 paquete->colaMensaje = tNEW_POKEMON;
 paquete->buffer = malloc(sizeof(t_buffer));
 paquete->buffer->size = 1;

 paquete->buffer->booleano = booleano;

 printf("Se creara mensaje\n");
 printf("Mi pid es=%d\n", paquete->pid);
 printf("Codigo de operacion=%d\n", paquete->codigoOperacion);
 printf("Cola de mensaje=%d\n", paquete->colaMensaje);
 printf("Contenido=%s\n", (char*) paquete->buffer->stream);
 printf("Tamano %d bytes\n", paquete->buffer->size);
 int bytes = paquete->buffer->size + 4 * sizeof(int);
 printf("Tamano %d bytes en total para serializar.\n", bytes);

 void* aEnviar = malloc(sizeof(bytes));
 aEnviar = serializarPaqueteCaughtPokemon(paquete, bytes);

 send(socket_cliente, aEnviar, bytes, 0);

 free(aEnviar);
 free(paquete->buffer->stream);
 free(paquete->buffer);
 free(paquete);
 }


 //TODO Mensaje a cola LOCALIZED_POKEMON
 t_paquete *recibirMensajeLocalizedPokemon(int socket_cliente, int* size) {

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
 void* serializarPaqueteLocalizedPokemon(t_paquete* paquete, int bytes) {
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
 void crearMensajeLocalizedPokemon(int pid, char* nombrePokemon, int posX,
 int posY, int cantidadPokemons, int socket_cliente) {
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
 int bytes = paquete->buffer->size + 6 * sizeof(int);
 printf("Tamano %d bytes en total para serializar.\n", bytes);

 void* aEnviar = malloc(sizeof(bytes));
 aEnviar = serializarPaqueteNewPokemon(paquete, bytes);

 send(socket_cliente, aEnviar, bytes, 0);

 free(aEnviar);
 free(paquete->buffer->stream);
 free(paquete->buffer);
 free(paquete);
 }

 //CONFIRMACION DE MENSAJES
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

 //SUSCRIPCION A COLAS

 t_paquete *recibirSuscriptor(int socketCliente, int*size) {

 int sizeMensaje = 0;

 t_paquete *suscriptor = malloc(sizeof(t_paquete));
 int pid;
 t_opCode codigoOperacion;
 t_colaMensaje colaMensaje;
 int sizeRecibido;

 recv(socketCliente, &pid, sizeof(pid), 0);
 recv(socketCliente, &codigoOperacion, sizeof(codigoOperacion), 0);
 recv(socketCliente, &colaMensaje, sizeof(colaMensaje), 0);
 recv(socketCliente, &sizeRecibido, sizeof(int), 0); // esto es para validar que haya llegado bien el paquete

 suscriptor->pid = pid;
 suscriptor->codigoOperacion = codigoOperacion;
 suscriptor->colaMensaje = colaMensaje;

 (*size) = sizeMensaje;
 return suscriptor;

 free(suscriptor);
 }

 void* serializarSuscriptor(t_paquete* suscriptor, int bytes) {
 void * magic = malloc(bytes);
 int desplazamiento = 0;

 memcpy(magic + desplazamiento, &(bytes), sizeof(int));
 desplazamiento += sizeof(int);

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

 t_paquete* suscriptor = malloc(sizeof(t_paquete));

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
 suscriptor->colaMensaje = tCAUGHT_POKEMON;
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

 t_paquete *recibirMensaje(int socketCliente, int* size) {

 t_paquete *paquete = malloc(sizeof(t_paquete));
 int pid;
 t_opCode codigoOperacion;
 t_colaMensaje colaMensaje;

 recv(socketCliente, &pid, sizeof(pid), 0);
 recv(socketCliente, &codigoOperacion, sizeof(codigoOperacion), 0);
 recv(socketCliente, &colaMensaje, sizeof(colaMensaje), 0);
 recv(socketCliente, size, sizeof(colaMensaje), 0);
 paquete->pid = pid;
 paquete->codigoOperacion = codigoOperacion;
 paquete->colaMensaje = colaMensaje;

 if (paquete->codigoOperacion == SUSCRIPCION) {
 recibirSuscriptor(socketCliente, size);
 }

 if (paquete->codigoOperacion == MENSAJE) {

 switch (paquete->colaMensaje) {

 case tNEW_POKEMON: {

 printf("meti algo en la lista : %s . \n",
 (char*) paquete->buffer->stream);

 void*buffer;
 int sizeMensaje;
 int posX;
 int posY;
 int cantidadPokemons;
 recv(socketCliente, &sizeMensaje, sizeof(int), 0);
 buffer = malloc(sizeMensaje);
 //buffer = malloc(*size);
 recv(socketCliente, buffer, sizeMensaje, 0);

 recv(socketCliente, &posX, sizeof(int), 0);

 recv(socketCliente, &posY, sizeof(int), 0);

 recv(socketCliente, &cantidadPokemons, sizeof(int), 0);

 //paquete->bytes = bytes;
 paquete->buffer = malloc(sizeMensaje);
 paquete->buffer->size = sizeMensaje;

 paquete->buffer->stream = buffer;

 paquete->buffer->posX = posX;

 paquete->buffer->posY = posY;

 paquete->buffer->cantidadPokemons = cantidadPokemons;

 //(*size) = sizeMensaje;
 return paquete;

 //			free(paquete->buffer);
 //			free(buffer);
 //			free(paquete);

 //break;
 }
 case tAPPEARED_POKEMON: {
 //list_add(APPEARED_POKEMON->lista, suscriptor);
 //devolverMensaje();
 break;
 }

 case tCATCH_POKEMON: {
 //list_add(CATCH_POKEMON->lista, suscriptor);
 //devolverMensaje();
 break;
 }

 case tCAUGHT_POKEMON: {
 //list_add(CAUGTH_POKEMON->lista, suscriptor);
 //devolverMensaje();
 break;
 }

 case tGET_POKEMON: {
 //list_add(GET_POKEMON->lista, suscriptor);
 //devolverMensaje();
 break;
 }

 case tLOCALIZED_POKEMON: {
 //list_add(LOCALIZED_POKEMON->lista, suscriptor);
 //devolverMensaje();
 break;
 }
 default: {
 printf("error de modulo, no se conoce quien envia paquetes\n");
 }
 }
 }
 return paquete;
 }

 */

void crearMensajeNewPokemon(int pid, char* nombrePokemon, int posX, int posY,
		int cantidadPokemons, int socketCliente) {
	//t_paquete* paquete = malloc(sizeof(t_paquete));

	uint32_t desplazamiento = 0;
	uint32_t stringSize = strlen(nombrePokemon);

	char* string = malloc((stringSize + 1) * sizeof(char));

	strcpy(string, nombrePokemon);

	string[stringSize] = '\0';

	void* magic = malloc(sizeof(t_header) + 5 * sizeof(uint32_t));

	//falta agregar lo del string en el tamaño del malloc.

	t_header header = MENSAJE_NEW_POKEMON;

	uint32_t totalString = (stringSize + 1) * sizeof(char);

	printf("largo del string es : %d .\n", totalString);

	memcpy(magic + desplazamiento, (void*) (&pid), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(magic + desplazamiento, (void*) (&header), sizeof(t_header));
	desplazamiento += sizeof(uint32_t);

	memcpy(magic + desplazamiento, (void*) (&posX), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(magic + desplazamiento, (void*) (&posY), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(magic + desplazamiento, (void*) (&cantidadPokemons),
			sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(magic + desplazamiento, (void*) (&totalString), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

//	memcpy(magic + desplazamiento, (void*) string, totalString);
//	desplazamiento += totalString;

	printf("el string es : %s .\n", string);

	send(socketCliente, magic, +sizeof(t_header) + 5 * sizeof(uint32_t), 0);

	free(string);
	free(magic);
}

t_paquete *recibirMensaje(int socketCliente) {

	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->buffer = malloc(sizeof(t_buffer));

	recv(socketCliente, &paquete->pid, sizeof(uint32_t),
	MSG_WAITALL);

	recv(socketCliente, &paquete->codigoOperacion, sizeof(t_header),
	MSG_WAITALL);

	recv(socketCliente, paquete->buffer, sizeof(t_buffer),
	MSG_WAITALL);

	switch (paquete->codigoOperacion) {
	case MENSAJE_NEW_POKEMON: {

		//uint32_t desplazamiento = 0;
		t_mensaje* mensaje = malloc(sizeof(t_mensaje));

		void* buffer = malloc(sizeof(paquete->buffer->size));

		buffer = paquete->buffer;

		memcpy(&(mensaje->posX), buffer, sizeof(uint32_t));
		buffer += sizeof(uint32_t);
		memcpy(&(mensaje->posY), buffer, sizeof(uint32_t));
		buffer += sizeof(uint32_t);
		memcpy(&(mensaje->cantidadPokemons), buffer, sizeof(uint32_t));
		buffer += sizeof(uint32_t);
		memcpy(&(mensaje->largoNombre), buffer, sizeof(uint32_t));
		buffer += sizeof(uint32_t);

//		memcpy(&(mensaje->nombrePokemon), buffer, mensaje->largoNombre);
//		desplazamiento += mensaje->largoNombre;

		printf("mi posX es :%d .\n", mensaje->posX);
		printf("mi posY es :%d .\n", mensaje->posY);
		printf("mi cantidadPokemons es :%d .\n", mensaje->cantidadPokemons);

		break;

	}
	}

	return paquete;

	free(paquete->buffer);
	free(paquete);

}
