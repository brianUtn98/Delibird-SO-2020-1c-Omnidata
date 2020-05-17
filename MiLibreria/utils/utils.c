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

//void* serializarPaqueteNew(t_paquete* paquete, int *bytes) {
//
//	int sizeSerializado = sizeof(t_header) + sizeof(int)
//			+ paquete->buffer->size;
//	void* buffer = malloc(sizeSerializado);
//
//	int desplazamiento = 0;
//
//	memcpy(buffer + desplazamiento, &(paquete->codigoOperacion),
//			sizeof(t_header));
//	desplazamiento += sizeof(t_header);
//	memcpy(buffer + desplazamiento, &(paquete->buffer->size), sizeof(int));
//	desplazamiento += sizeof(int);
//	memcpy(buffer + desplazamiento, &(paquete->buffer->stream),
//			paquete->buffer->size);
//	desplazamiento += paquete->buffer->size;
//
//	(*bytes) = sizeSerializado;
//
//	return buffer;
//
//}
//
//void crearMensajeNewPokemon(uint32_t pid, char* nombrePokemon, uint32_t posX,
//		uint32_t posY, uint32_t cantidadPokemons, int socketCliente) {
//	//t_paquete* paquete = malloc(sizeof(t_paquete));
//
//	uint32_t stringSize = strlen(nombrePokemon);
//
////	char* string = malloc((stringSize + 1) * sizeof(char));
////
////	strcpy(string, nombrePokemon);
////
////	string[stringSize] = '\0';
//
//	t_paquete* paquete = malloc(sizeof(t_paquete));
//	paquete->codigoOperacion = MENSAJE_NEW_POKEMON;
//	paquete->buffer = malloc(sizeof(t_buffer));
//	paquete->buffer->size = stringSize + 1;
//	paquete->buffer->stream = nombrePokemon;
//	int sizeSerializado;
//	void* serializado = serializarPaqueteNew(paquete, &sizeSerializado);
//	send(socketCliente, serializado, sizeSerializado, 0);
//	free(serializado);
//
//}
//
//char* recibirMensaje(int socketCliente) {
//
//	t_header operacion;
//
//	recv(socketCliente, &operacion, sizeof(t_header),
//	MSG_WAITALL);
//
//	uint32_t bufferSize;
//	recv(socketCliente, &bufferSize, sizeof(bufferSize),
//	MSG_WAITALL);
//
//	char* buffer = malloc(sizeof(bufferSize));
//	recv(socketCliente, buffer, sizeof(bufferSize),
//	MSG_WAITALL);
//	switch (operacion) {
//	case MENSAJE_NEW_POKEMON: {
//
//		uint32_t desplazamiento = 0;
//		t_mensaje* mensaje = malloc(sizeof(t_mensaje));
//
//		void* buffer = malloc(sizeof(buffer->size));
//
//		buffer = paquete->buffer;
//
//		memcpy(&(mensaje->posX), buffer + desplazamiento, sizeof(uint32_t));
//		desplazamiento += sizeof(uint32_t);
//		memcpy(&(mensaje->posY), buffer + desplazamiento, sizeof(uint32_t));
//		desplazamiento += sizeof(uint32_t);
//		memcpy(&(mensaje->cantidadPokemons), buffer + desplazamiento,
//				sizeof(uint32_t));
//		desplazamiento += sizeof(uint32_t);
//		memcpy(&(mensaje->largoNombre), buffer + desplazamiento,
//				sizeof(uint32_t));
//		desplazamiento += sizeof(uint32_t);
//
////		memcpy(&(mensaje->nombrePokemon), buffer, mensaje->largoNombre);
////		desplazamiento += mensaje->largoNombre;
//
//		printf("mi posX es :%d .\n", mensaje->posX);
//		printf("mi posY es :%d .\n", mensaje->posY);
//		printf("mi cantidadPokemons es :%d .\n", mensaje->cantidadPokemons);
//
//		break;
//
//	}
//	}
//	return buffer;
//
//}
//void* serializarPaquete(t_paquete* paquete, int *bytes) {
//	printf("En serializar paquete me llega: \n");
//	printf("Pid: %d - OpCode: %d - Tamano de mensaje: %d - Mensaje: %s\n",
//			paquete->pid, paquete->codigoOperacion, paquete->buffer->size,
//			(char*) paquete->buffer->stream);
//	int sizeSerializado = sizeof(pid_t) + sizeof(t_header) + sizeof(uint32_t)
//			+ paquete->buffer->size;
//	void* buffer = malloc(sizeSerializado);
//	int desplazamiento = 0;
//	memcpy(buffer + desplazamiento, &(paquete->pid), sizeof(pid_t));
//	desplazamiento += sizeof(pid_t);
//	memcpy(buffer + desplazamiento, &(paquete->codigoOperacion),
//			sizeof(t_header));
//	desplazamiento += sizeof(t_header);
//	memcpy(buffer + desplazamiento, &(paquete->buffer->size), sizeof(uint32_t));
//	desplazamiento += sizeof(int);
//	memcpy(buffer + desplazamiento, paquete->buffer->stream,
//			paquete->buffer->size);
//	desplazamiento += paquete->buffer->size;
//
//	(*bytes) = sizeSerializado;
//
//	return buffer;
//
//}
//
//void *serializarMensajeNew(t_mensajeNew *mensaje, int *bytes) {
//	int sizeSerializado = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t)
//			+ sizeof(uint32_t);
//	void *buffer = malloc(sizeSerializado);
//	int desplazamiento = 0;
//	memcpy(buffer + desplazamiento, &mensaje->cantidadPokemons,
//			sizeof(uint32_t));
//	desplazamiento += sizeof(uint32_t);
//	memcpy(buffer + desplazamiento, &mensaje->largoNombre, sizeof(uint32_t));
//	desplazamiento += sizeof(uint32_t);
//	memcpy(buffer + desplazamiento, &mensaje->posX, sizeof(uint32_t));
//	desplazamiento += sizeof(uint32_t);
//	memcpy(buffer + desplazamiento, &mensaje->posY, sizeof(uint32_t));
//	desplazamiento += sizeof(uint32_t);
//	(*bytes) = sizeSerializado;
//
//	return buffer;
//}
//
//void crearMensajeNewPokemon(pid_t pid, char* nombrePokemon, uint32_t posX,
//		uint32_t posY, uint32_t cantidadPokemons, int socketCliente) {
//	//t_paquete* paquete = malloc(sizeof(t_paquete));
//
//	uint32_t stringSize = strlen(nombrePokemon) + 1;
//	t_paquete* paquete = malloc(sizeof(t_paquete));
//	paquete->codigoOperacion = MENSAJE_NEW_POKEMON;
//	paquete->pid = pid;
//	paquete->buffer = malloc(sizeof(t_buffer));
//	paquete->buffer->size = stringSize;
//
//	//strcpy(paquete->buffer->stream, nombrePokemon);
//
//	paquete->buffer->stream = nombrePokemon;
//	//paquete->buffer->stream =string_duplicate(nombrePokemon);
//	t_mensajeNew *mensaje = malloc(sizeof(t_mensajeNew));
//	mensaje->cantidadPokemons = cantidadPokemons;
//	mensaje->largoNombre = stringSize;
//	mensaje->posX = posX;
//	mensaje->posY = posY;
//	int sizeSerializado;
//	void* serializado = serializarPaquete(paquete, &sizeSerializado);
//	send(socketCliente, &sizeSerializado, sizeof(int), MSG_NOSIGNAL);
//	send(socketCliente, serializado, sizeSerializado, 0);
//	int sizeMensajeSerializado;
//	void *mensajeSerializado = serializarMensajeNew(mensaje,
//			&sizeMensajeSerializado);
//	send(socketCliente, &sizeMensajeSerializado, sizeof(int), MSG_NOSIGNAL);
//	send(socketCliente, mensajeSerializado, sizeMensajeSerializado, 0);
//	free(serializado);
//	free(mensajeSerializado);
//	return;
//}
//
//void crearMensajePokemon2(int pid, char* nombrePokemon, int posX, int posY,
//		int cantidadPokemons, int socketCliente) {
//
//	//uint32_t stringSize = strlen(nombrePokemon);
//
//	int stringSize = strlen(nombrePokemon) + 1;
//
//	//char* string = malloc(stringSize);
//	char* string = nombrePokemon;
//	//strcpy(string, nombrePokemon);
//
//	string[stringSize] = '\0';
//
//	t_paquete* paquete = malloc(sizeof(t_paquete));
//
//	paquete->pid = pid;
//
//	paquete->codigoOperacion = MENSAJE_NEW_POKEMON;
//
//	paquete->buffer = malloc(sizeof(t_buffer));
//
//	paquete->buffer->size = stringSize + 5 * (sizeof(int));
//
//	int desplazamiento = 0;
//	paquete->buffer->stream = malloc(paquete->buffer->size - sizeof(int));
//
//	memcpy(paquete->buffer->stream + desplazamiento, (void*) stringSize,
//			sizeof(int));
//	desplazamiento += sizeof(int);
//
//	memcpy(paquete->buffer->stream + desplazamiento, string, stringSize);
//	desplazamiento += stringSize;
//	free(string);
//
//	memcpy(paquete->buffer->stream + desplazamiento, (void*) posX, sizeof(int));
//	desplazamiento += sizeof(int);
//
//	memcpy(paquete->buffer->stream + desplazamiento, (void*) posY, sizeof(int));
//	desplazamiento += sizeof(int);
//
//	memcpy(paquete->buffer->stream + desplazamiento, (void*) cantidadPokemons,
//			sizeof(int));
//	desplazamiento += sizeof(int);
//
//	int sizeSerializado;
//
//	void* mensajeSerializado = serializarMensajeNew2(paquete, &sizeSerializado);
//	//send(socketCliente, &sizeSerializado, sizeof(int), MSG_NOSIGNAL);
//	send(socketCliente, mensajeSerializado, sizeSerializado, MSG_NOSIGNAL);
//
//	free(mensajeSerializado);
//	free(paquete->buffer->stream);
//	free(paquete->buffer);
//	free(paquete);
//
//}
//
//void* serializarMensajeNew2(t_paquete* paquete, int *bytes) {
//
//	int sizeSerializado = sizeof(int) + sizeof(t_header); //+ paquete->buffer->size;
//
//	void* buffer = malloc(sizeSerializado);
//
//	int desplazamiento = 0;
//
//	memcpy(buffer + desplazamiento, &(paquete->pid), sizeof(int));
//	desplazamiento += sizeof(int);
//
//	memcpy(buffer + desplazamiento, &(paquete->codigoOperacion),
//			sizeof(t_header));
//	desplazamiento += sizeof(t_header);
//
////	memcpy(buffer + desplazamiento, paquete->buffer, paquete->buffer->size);
////	desplazamiento += paquete->buffer->size;
//
//	(*bytes) = sizeSerializado;
//	return buffer;
//
//}
//
//void recibirMensaje2(int socketCliente) {
//
//	int pid;
//
//	recv(socketCliente, &pid, sizeof(int), MSG_WAITALL);
//
//	printf(" El pid recibido es : %d .\n", (int) pid);
//	uint32_t sizeTotal = 0;
//
//	recv(socketCliente, &sizeTotal, sizeof(uint32_t), MSG_WAITALL);
//
//	uint32_t pid = 0;
//
//	recv(socketCliente, &pid, sizeof(uint32_t), MSG_WAITALL);
//
//	printf(" El pid recibido es : %d .\n", (int) pid);
//
//	t_header operacion = 0;
//	recv(socketCliente, &operacion, sizeof(t_header), MSG_WAITALL);
//
//	printf("el código de operacion es : %d .\n", (int) operacion);
//
//	t_buffer *buffer = malloc(sizeof(t_buffer));
//
//	recv(socketCliente, buffer, sizeof(t_buffer), MSG_WAITALL);
//
//	printf("recibi el buffer .\n");
//}
/*
 char* recibirMensaje(int socketCliente) {
 t_paquete *paquete;
 int sizeMensaje;
 recv(socketCliente, &sizeMensaje, sizeof(int), MSG_WAITALL);
 printf("Se recibira un mensaje de tamano %d\n", sizeMensaje);
 paquete = malloc(sizeof(t_paquete));
 void *recibir = malloc(sizeMensaje);
 recv(socketCliente, recibir, sizeMensaje, MSG_WAITALL);
 int desplazamiento = 0;
 memcpy(&paquete->pid, recibir, sizeof(pid_t));
 desplazamiento += sizeof(pid_t);
 memcpy(&paquete->codigoOperacion, recibir + desplazamiento,
 sizeof(t_header));
 desplazamiento += sizeof(t_header);
 paquete->buffer = malloc(sizeof(t_buffer));
 memcpy(&paquete->buffer->size, recibir + desplazamiento, sizeof(uint32_t));
 desplazamiento += sizeof(uint32_t);
 paquete->buffer->stream = malloc(paquete->buffer->size);
 memcpy(paquete->buffer->stream, recibir + desplazamiento,
 paquete->buffer->size);
 desplazamiento += paquete->buffer->size;
 printf("----\n");
 printf("PID: %d - OpCode: %d - Tamano Mensaje: %d - Mensaje: %s\n",
 paquete->pid, paquete->codigoOperacion, paquete->buffer->size,
 (char*) paquete->buffer->stream);

 switch (paquete->codigoOperacion) {
 case MENSAJE_NEW_POKEMON: {

 sizeMensaje = 0;
 t_bufferOmnidata *mensajeNew = malloc(sizeof(t_bufferOmnidata));
 recv(socketCliente, &sizeMensaje, sizeof(int), MSG_WAITALL);
 void *recibirNew = malloc(sizeMensaje);
 recv(socketCliente, recibirNew, sizeMensaje, MSG_WAITALL);
 desplazamiento = 0;
 memcpy(&mensajeNew->cantidadPokemons, recibirNew + desplazamiento,
 sizeof(uint32_t));
 desplazamiento += sizeof(uint32_t);
 memcpy(&mensajeNew->largoNombre, recibirNew + desplazamiento,
 sizeof(uint32_t));
 desplazamiento += sizeof(uint32_t);
 memcpy(&mensajeNew->posX, recibirNew + desplazamiento,
 sizeof(uint32_t));
 desplazamiento += sizeof(uint32_t);
 memcpy(&mensajeNew->posY, recibirNew + desplazamiento,
 sizeof(uint32_t));
 desplazamiento += sizeof(uint32_t);

 paquete->buffer = mensajeNew;

 printf("Hay %d nuevos %s en (X,Y)=%d,%d\n",
 mensajeNew->cantidadPokemons, (char*) paquete->buffer->stream,
 mensajeNew->posX, mensajeNew->posY);
 }
 break;
 case MENSAJE_APPEARED_POKEMON: {

 default:
 printf("Error, paquete invalido.\n");
 break;

 }

 return paquete->buffer->stream;
 }

 void enviar2int(int pid, int codOp, char* nombrePokemon, int posX, int posY,
 int cantidadPokemons, int socketCliente) {

 t_paquete* paquete = malloc(sizeof(t_paquete));

 paquete->pid = pid;
 paquete->codigoOperacion = codOp;
 paquete->posX = posX;
 paquete->posY = posY;
 paquete->cantidadPokemons = cantidadPokemons;
 paquete->largoNombre = strlen(nombrePokemon);
 paquete->nombrePokemon = nombrePokemon;

 //paquete->buffer = malloc(sizeof(t_buffer));

 //paquete->buffer->size = sizeof(int);

 //paquete->buffer->stream = malloc(paquete->buffer->size);

 //	int X = 4;
 //
 //	int desplazamiento1 = 0;
 //
 //	memcpy(paquete->buffer->stream + desplazamiento1, &X, sizeof(int));
 //	desplazamiento1 += sizeof(int);

 //paquete->buffer->stream = (void*) posX;

 int sizeMensaje = sizeof(int) * 6 + strlen(nombrePokemon);

 void* buffer = malloc(sizeMensaje);
 int desplazamiento = 0;

 memcpy(buffer + desplazamiento, &paquete->pid, sizeof(int));
 desplazamiento += sizeof(int);

 memcpy(buffer + desplazamiento, &paquete->codigoOperacion, sizeof(int));
 desplazamiento += sizeof(int);

 memcpy(buffer + desplazamiento, &paquete->posX, sizeof(int));
 desplazamiento += sizeof(int);
 memcpy(buffer + desplazamiento, &paquete->posY, sizeof(int));
 desplazamiento += sizeof(int);
 memcpy(buffer + desplazamiento, &paquete->cantidadPokemons,
 sizeof(int));
 desplazamiento += sizeof(int);
 memcpy(buffer + desplazamiento, &paquete->largoNombre, sizeof(int));
 desplazamiento += sizeof(int);

 memcpy(buffer + desplazamiento, &paquete->nombrePokemon,
 paquete->largoNombre);
 desplazamiento += paquete->largoNombre;

 //	memcpy(buffer + desplazamiento, &paquete->buffer->size, sizeof(int));
 //	desplazamiento += sizeof(int);
 //	memcpy(buffer + desplazamiento, &paquete->buffer->stream,
 //			paquete->buffer->size);
 //	desplazamiento += sizeof(int);

 send(socketCliente, buffer, sizeMensaje, 0);

 free(paquete->buffer->stream);
 free(paquete->buffer);
 free(paquete);

 }
 void recibir2int(int socket) {

 int pid;

 int operacion;

 int posX;
 int posY;
 int cantidadPokemons;
 int largoNombre;

 char* nombre = malloc(4);

 void* buffer = malloc(sizeof(int) * 10);

 recv(socket, buffer, sizeof(int) * 10, 0);

 printf("hola llegue.");

 int desplazamiento = 0;
 memcpy(&pid, buffer + desplazamiento, sizeof(int));
 desplazamiento += sizeof(int);

 memcpy(&operacion, buffer + desplazamiento, sizeof(int));
 desplazamiento += sizeof(int);

 memcpy(&posX, buffer + desplazamiento, sizeof(int));
 desplazamiento += sizeof(int);

 memcpy(&posY, buffer + desplazamiento, sizeof(int));
 desplazamiento += sizeof(int);

 memcpy(&cantidadPokemons, buffer + desplazamiento, sizeof(int));
 desplazamiento += sizeof(int);
 memcpy(&largoNombre, buffer + desplazamiento, sizeof(int));
 desplazamiento += sizeof(int);

 memcpy(&nombre, buffer + desplazamiento, largoNombre);
 desplazamiento += largoNombre;

 printf("el pid es : %d .\n", pid);
 printf("el operación es : %d .\n", operacion);
 printf("la pos en x es : %d .\n", posX);
 printf("la pos en y  es : %d .\n", posY);
 printf("la can es : %d .\n", cantidadPokemons);
 printf("el largo del string es : %d .\n", largoNombre);
 printf("el mensaje es : %s .\n", nombre);

 //int codigoOperacion;

 //	recv(socket, &codigoOperacion, sizeof(int), 0);
 //	printf("el codigoOperacion : %d . \n", codigoOperacion);

 //	void* buffer = malloc(2 * sizeof(int));
 //
 //	recv(socket, buffer, sizeof(int) * 2, MSG_WAITALL);
 //
 //	t_mensajeNew* mensaje = malloc(sizeof(t_mensajeNew));
 //
 //	int desplazamiento = 0;
 //	memcpy(&mensaje->size, buffer + desplazamiento, sizeof(int));
 //	desplazamiento += sizeof(int);
 //	memcpy(&mensaje->posX, buffer + desplazamiento, sizeof(int));
 //	desplazamiento += sizeof(int);
 //
 //	printf("el size del buffer es : %d . \n", mensaje->posX);

 }

 //void* serializarAdministrativo(void* administrativo, int* tamanio) {
 //
 //	t_administrativo* unAdministrativo = (t_administrativo*) administrativo;
 //
 //	*tamanio = sizeof(uint16_t) + sizeof(uint32_t);
 //
 //	int desplazamiento = 0;
 //
 //	void* paqueteAdministrativo = malloc(*tamanio);
 //
 //	memcpy(paqueteAdministrativo + desplazamiento, &unAdministrativo->codigo,
 //			sizeof(uint16_t));
 //	desplazamiento += sizeof(uint16_t);
 //
 //	memcpy(paqueteAdministrativo + desplazamiento, &unAdministrativo->valor,
 //			sizeof(uint32_t));
 //	desplazamiento += sizeof(uint32_t);
 //
 //	///////////////////////////////////////CASO DE PRUEBA SERIALIZACION ADMINISTRATIVO////////////////////////////////////////////
 //
 //	printf("\n\nADMINISTRATIVO A SERIALIZAR: \n");
 //	printf("\nCodigo: %d", unAdministrativo->codigo);
 //	printf("\nValor: %d", unAdministrativo->valor);
 //	printf("\nTamaño del administrativo: %d", *tamanio);
 //
 //	t_administrativo* administrativoDeserializado = deserializarAdministrativo(
 //			unAdministrativo);
 //
 //	printf("\n\nADMINISTRATIVO DESERIALIZADO: \n");
 //	printf("\nCodigo: %d", administrativoDeserializado->codigo);
 //	printf("\nValor: %d\n", administrativoDeserializado->valor);
 //
 //	free(administrativoDeserializado);
 //
 //	///////////////////////////////////////FIN CASO DE PRUEBA SERIALIZACION ADMINISTRATIVO////////////////////////////////////////////
 //
 //	return paqueteAdministrativo;
 //
 //}
 //t_administrativo* deserializarAdministrativo(void* buffer) {
 //
 //	int desplazamiento = 0;
 //
 //	t_administrativo* unAdministrativo = malloc(sizeof(t_administrativo));
 //
 //	memcpy(&unAdministrativo->codigo, buffer + desplazamiento,
 //			sizeof(uint16_t));
 //	desplazamiento += sizeof(uint16_t);
 //
 //	memcpy(&unAdministrativo->valor, buffer + desplazamiento, sizeof(uint32_t));
 //
 //	return unAdministrativo;
 //
 //}
 */
int enviarInt(int socketDestino, int numero) {
	printf("- Numero a enviar %d . ", numero); //SACARRRRR
	void *buffer = malloc(sizeof(int));
	memcpy(buffer, &numero, sizeof(int));
	ssize_t sent = 0;
	sent = send(socketDestino, buffer, sizeof(int), MSG_NOSIGNAL);
	free(buffer);
	return sent;

}

int recibirInt(int socketDestino, int *intRecibido) {
	ssize_t leido = 0;
	void *buffer = malloc(sizeof(int));
	leido = recv(socketDestino, buffer, sizeof(buffer), 0);
	if (leido == 0) {
		return 0;
	}
	memcpy(intRecibido, buffer, sizeof(int));
	printf("- Numero a recibir %d . ", *intRecibido);

	free(buffer);
	return leido;
}

int enviarCadena(int socketDestino, char *mensaje) {

	ssize_t len = 0;
	ssize_t sent = 0;
	len = strlen(mensaje);
	char *buffer = malloc((len + 1) * sizeof(char));
	memcpy(buffer, mensaje, len * sizeof(char));

	buffer[len] = '\0';

	enviarInt(socketDestino, len + 1);

	sent = send(socketDestino, buffer, len + 1, MSG_NOSIGNAL);
	if (sent == -1) {
		return -1;
	}

	return len;

}

int recibirCadena(int socketOrigen, char *mensaje) {

	int longitudMensaje;
	recibirInt(socketOrigen, &longitudMensaje);
	size_t len_leida = 0; //Como se representa este tipo de dato
	len_leida = recv(socketOrigen, mensaje, longitudMensaje * sizeof(char), 0);
	mensaje[len_leida] = '\0';
	return len_leida;

}

void enviarMensajeRecursoNew(int pid, char* nombrePokemon, int posX, int posY,
		int cantidadPokemons, int socketCliente) {

	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, MENSAJE_NEW_POKEMON);

	enviarInt(socketCliente, strlen(nombrePokemon) + 1);

	enviarCadena(socketCliente, nombrePokemon);
	enviarInt(socketCliente, posX);
	enviarInt(socketCliente, posY);
	enviarInt(socketCliente, cantidadPokemons);

}

void enviarMensajeRecursoGet(int pid, char* nombrePokemon, int socketCliente) {

	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, MENSAJE_GET_POKEMON);

	enviarInt(socketCliente, strlen(nombrePokemon) + 1);

	enviarCadena(socketCliente, nombrePokemon);

}

void enviarMensajeRecursoAppeared(int pid, char* nombrePokemon, int posX,
		int posY, int socketCliente) {

	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, MENSAJE_APPEARED_POKEMON);

	enviarInt(socketCliente, strlen(nombrePokemon) + 1);

	enviarCadena(socketCliente, nombrePokemon);
	enviarInt(socketCliente, posX);
	enviarInt(socketCliente, posY);

}

void enviarMensajeRecursoCatch(int pid, char* nombrePokemon, int posX, int posY,
		int socketCliente) {

	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, MENSAJE_CATCH_POKEMON);

	enviarInt(socketCliente, strlen(nombrePokemon) + 1);

	enviarCadena(socketCliente, nombrePokemon);
	enviarInt(socketCliente, posX);
	enviarInt(socketCliente, posY);

}

void enviarMensajeRecursoCaught(int pid, bool booleano, int socketCliente) {

	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, MENSAJE_CAUGHT_POKEMON);

	enviarInt(socketCliente, booleano);

}

void enviarMensajeRecursoLocalized(int pid, char* nombrePokemon,
		t_list coordenadas, int socketCliente) {

	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, MENSAJE_LOCALIZED_POKEMON);

	enviarInt(socketCliente, strlen(nombrePokemon) + 1);

	enviarCadena(socketCliente, nombrePokemon);
	//enviarInt(socketCliente, list_size(coordenadas));

}

void suscribirseNew(int pid, int socketCliente) {
	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, SUSCRIBIRSE_NEW_POKEMON);
}
void suscribirseGet(int pid, int socketCliente) {
	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, SUSCRIBIRSE_GET_POKEMON);
}
void suscribirseCatch(int pid, int socketCliente) {
	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, SUSCRIBIRSE_CATCH_POKEMON);
}
void suscribirseCaught(int pid, int socketCliente) {
	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, SUSCRIBIRSE_CAUGHT_POKEMON);
}
void suscribirseAppered(int pid, int socketCliente) {
	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, SUSCRIBIRSE_APPEARED_POKEMON);
}
void suscribirseLocalized(int pid, int socketCliente) {

	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, SUSCRIBIRSE_LOCALIZED_POKEMON);

}

t_paquete* recibirMensajeRecurso(int socketCliente) {

	t_paquete* paquete = malloc(sizeof(t_paquete));

	int pid;
	recibirInt(socketCliente, &pid);

	paquete->pid = pid;

	printf("recibi el pid : %d .\n", pid);

	int codOp;
	recibirInt(socketCliente, &codOp);

	paquete->codigoOperacion = codOp;

	printf("recibi el codOp : %d .\n", codOp);

	paquete->buffer = malloc(sizeof(t_bufferOmnidata));

	switch (codOp) {
	case MENSAJE_NEW_POKEMON: {

		int sizeNombre;
		recibirInt(socketCliente, &sizeNombre);
		paquete->buffer->largoNombre = sizeNombre;

		printf("recibi el size del nombre : %d .\n",
				paquete->buffer->largoNombre);

		char* nombre = malloc(sizeNombre);
		recibirCadena(socketCliente, nombre);

		paquete->buffer->nombrePokemon = nombre;

		printf("recibi el nombre del pokemon y es : %s .\n",
				paquete->buffer->nombrePokemon);

		int posX;
		recibirInt(socketCliente, &posX);
		paquete->buffer->posX = posX;

		printf("recibi la pos en x : %d .\n", paquete->buffer->posX);

		int posY;
		recibirInt(socketCliente, &posY);
		paquete->buffer->posY = posY;
		printf("recibi la pos en y : %d .\n", paquete->buffer->posY);

		int cantidadPokemons;
		recibirInt(socketCliente, &cantidadPokemons);

		paquete->buffer->cantidadPokemons = cantidadPokemons;

		printf("recibi la cantidad : %d .\n",
				paquete->buffer->cantidadPokemons);

		break;

	}

	case MENSAJE_APPEARED_POKEMON: {

		int sizeNombre;
		recibirInt(socketCliente, &sizeNombre);
		paquete->buffer->largoNombre = sizeNombre;

		printf("recibi el size del nombre : %d .\n",
				paquete->buffer->largoNombre);

		char* nombre = malloc(sizeNombre);
		recibirCadena(socketCliente, nombre);

		paquete->buffer->nombrePokemon = nombre;

		printf("recibi el nombre del pokemon y es : %s .\n",
				paquete->buffer->nombrePokemon);

		int posX;
		recibirInt(socketCliente, &posX);
		paquete->buffer->posX = posX;

		printf("recibi la pos en x : %d .\n", paquete->buffer->posX);

		int posY;
		recibirInt(socketCliente, &posY);
		paquete->buffer->posY = posY;
		printf("recibi la pos en y : %d .\n", paquete->buffer->posY);

		printf("recibi la cantidad : %d .\n",
				paquete->buffer->cantidadPokemons);

		break;

	}

	case MENSAJE_CATCH_POKEMON: {

		int sizeNombre;
		recibirInt(socketCliente, &sizeNombre);
		paquete->buffer->largoNombre = sizeNombre;

		printf("recibi el size del nombre : %d .\n",
				paquete->buffer->largoNombre);

		char* nombre = malloc(sizeNombre);
		recibirCadena(socketCliente, nombre);

		paquete->buffer->nombrePokemon = nombre;

		printf("recibi el nombre del pokemon y es : %s .\n",
				paquete->buffer->nombrePokemon);

		int posX;
		recibirInt(socketCliente, &posX);
		paquete->buffer->posX = posX;

		printf("recibi la pos en x : %d .\n", paquete->buffer->posX);

		int posY;
		recibirInt(socketCliente, &posY);
		paquete->buffer->posY = posY;
		printf("recibi la pos en y : %d .\n", paquete->buffer->posY);

		break;

	}
	case MENSAJE_CAUGHT_POKEMON: {
		int booleano;
		recibirInt(socketCliente, &booleano);
		paquete->buffer->boolean = booleano;
		break;
	}
	case MENSAJE_GET_POKEMON: {
		int sizeNombre;
		recibirInt(socketCliente, &sizeNombre);
		paquete->buffer->largoNombre = sizeNombre;

		printf("recibi el size del nombre : %d .\n",
				paquete->buffer->largoNombre);

		char* nombre = malloc(sizeNombre);
		recibirCadena(socketCliente, nombre);

		paquete->buffer->nombrePokemon = nombre;

		printf("recibi el nombre del pokemon y es : %s .\n",
				paquete->buffer->nombrePokemon);

		break;
	}
	case MENSAJE_LOCALIZED_POKEMON: {
		break;
	}
//	case SUSCRIBIRSE_NEW_POKEMON: {
//		break;
//	}
//	case SUSCRIBIRSE_APPEARED_POKEMON: {
//		break;
//	}
//	case SUSCRIBIRSE_CATCH_POKEMON: {
//		break;
//	}
//	case SUSCRIBIRSE_CAUGHT_POKEMON:{
//		break;
//	}
//	case SUSCRIBIRSE_GET_POKEMON:{
//		break;
//	}
//	case SUSCRIBIRSE_LOCALIZED_POKEMON:
//	{
//		break;
//	}
	}

//free(nombre);
	return paquete;
}

