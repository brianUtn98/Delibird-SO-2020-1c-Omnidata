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

void liberarConexion(int socket) {
	close(socket);
	return;
}


/*
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
*/


void liberarMensaje(t_mensaje *mensaje){
	free(mensaje->nombrePokemon);
	list_destroy(mensaje->paresDeCoordenadas);
	return;
}

void liberarPaquete(t_paquete *paquete){
liberarMensaje(paquete->buffer->mensaje);
free(paquete->buffer);
free(paquete);
return;
}

t_paquete* recibirMensaje(int socketCliente) {
	t_paquete *paquete;
	int sizeMensaje;
	recv(socketCliente,&sizeMensaje,sizeof(int),MSG_WAITALL);
	printf("Se recibira un mensaje de tamano %d\n",sizeMensaje);
	paquete=malloc(sizeof(t_paquete));
	void *recibir=malloc(sizeMensaje);
	recv(socketCliente,recibir,sizeMensaje,MSG_WAITALL);
	int desplazamiento=0;
	memcpy(&paquete->pid,recibir,sizeof(pid_t));
	desplazamiento+=sizeof(pid_t);
	memcpy(&paquete->codigoOperacion,recibir+desplazamiento,sizeof(t_header));
	desplazamiento+=sizeof(t_header);
	paquete->buffer=malloc(sizeof(t_buffer));
	memcpy(&paquete->buffer->size,recibir+desplazamiento,sizeof(uint32_t));
	desplazamiento+=sizeof(uint32_t);

//	printf("Paquete ---> pid: %d - codigo de operacion: %d - Size del buffer: %d",paquete->pid,paquete->codigoOperacion,paquete->buffer->size);
//	paquete->buffer->stream=malloc(paquete->buffer->size);
//	memcpy(paquete->buffer->stream,recibir+desplazamiento,paquete->buffer->size);
//	desplazamiento+=paquete->buffer->size;
//	printf("----\n");
//	printf("PID: %d - OpCode: %d - Tamano Mensaje: %d - Mensaje: %s\n",paquete->pid,paquete->codigoOperacion,paquete->buffer->size,(char*)paquete->buffer->stream);

	t_mensaje *mensaje=malloc(sizeof(t_mensaje));
	switch (paquete->codigoOperacion) {
	case MENSAJE_NEW_POKEMON:
	{
		sizeMensaje=0;

		recv(socketCliente,&sizeMensaje,sizeof(int),MSG_WAITALL);
		void *recibirNew=malloc(sizeMensaje);
		recv(socketCliente,recibirNew,sizeMensaje,MSG_WAITALL);
		desplazamiento=0;
		memcpy(&mensaje->cantidadPokemons,recibirNew+desplazamiento,sizeof(uint32_t));
		desplazamiento+=sizeof(uint32_t);
		memcpy(&mensaje->largoNombre,recibirNew+desplazamiento,sizeof(uint32_t));
		desplazamiento+=sizeof(uint32_t);
		mensaje->nombrePokemon=malloc(mensaje->largoNombre);
		memcpy(&mensaje->posX,recibirNew+desplazamiento,sizeof(uint32_t));
		desplazamiento+=sizeof(uint32_t);
		memcpy(&mensaje->posY,recibirNew+desplazamiento,sizeof(uint32_t));
		desplazamiento+=sizeof(uint32_t);
		memcpy(mensaje->nombrePokemon,recibirNew+desplazamiento,mensaje->largoNombre);

		mensaje->booleano=true;
		mensaje->cantidadDePares=0;
		mensaje->paresDeCoordenadas=list_create();
		paquete->buffer->size=sizeof(mensaje);
		paquete->buffer->mensaje=malloc(sizeof(t_mensaje));
		paquete->buffer->mensaje->nombrePokemon=malloc(mensaje->largoNombre);
		paquete->buffer->mensaje->nombrePokemon=string_duplicate(mensaje->nombrePokemon);
		paquete->buffer->mensaje->booleano=true;
		paquete->buffer->mensaje->cantidadDePares=0;
		paquete->buffer->mensaje->cantidadPokemons=mensaje->cantidadPokemons;
		paquete->buffer->mensaje->largoNombre=mensaje->largoNombre;
		paquete->buffer->mensaje->paresDeCoordenadas=list_create();
		paquete->buffer->mensaje->posX=mensaje->posX;
		paquete->buffer->mensaje->posY=mensaje->posY;

		printf("Hay %d nuevos %s en (X,Y)=%d,%d\n",mensaje->cantidadPokemons,mensaje->nombrePokemon,mensaje->posX,mensaje->posY);

	}
	break;
	case MENSAJE_GET_POKEMON:
	{
		sizeMensaje=0;

		recv(socketCliente,&sizeMensaje,sizeof(int),MSG_WAITALL);
		void *recibirGet=malloc(sizeMensaje);
		recv(socketCliente,recibirGet,sizeMensaje,MSG_WAITALL);
		desplazamiento=0;
		memcpy(&mensaje->largoNombre,recibirGet+desplazamiento,sizeof(uint32_t));
		desplazamiento+=sizeof(uint32_t);
		mensaje->nombrePokemon=malloc(mensaje->largoNombre);
		memcpy(mensaje->nombrePokemon,recibirGet+desplazamiento,mensaje->largoNombre);
		desplazamiento+=mensaje->largoNombre;

		mensaje->booleano=true;
		mensaje->cantidadDePares=0;
		mensaje->cantidadPokemons=0;
		mensaje->paresDeCoordenadas=list_create();
		mensaje->posX=0;
		mensaje->posY=0;

		paquete->buffer->size=sizeof(mensaje);
		paquete->buffer->mensaje->booleano=mensaje->booleano;
		paquete->buffer->mensaje->cantidadDePares=mensaje->cantidadDePares;
		paquete->buffer->mensaje->cantidadPokemons=mensaje->cantidadPokemons;
		paquete->buffer->mensaje->largoNombre=mensaje->largoNombre;
		paquete->buffer->mensaje->nombrePokemon=string_duplicate(mensaje->nombrePokemon);
		paquete->buffer->mensaje->paresDeCoordenadas=list_duplicate(mensaje->paresDeCoordenadas);
		paquete->buffer->mensaje->posX=mensaje->posX;
		paquete->buffer->mensaje->posY=mensaje->posY;

		printf("El team de PID: %d quiere obtener %s\n",paquete->pid,paquete->buffer->mensaje->nombrePokemon);

	}
	break;
	default:
		printf("Error, paquete invalido.\n");
	break;

	}
	liberarMensaje(mensaje);
	return paquete;

}

void* serializarPaquete(t_paquete* paquete, int *bytes) {
	int sizeSerializado = sizeof(pid_t)+sizeof(t_header)+sizeof(uint32_t);
	void* buffer = malloc(sizeSerializado);
	int desplazamiento = 0;
	memcpy(buffer+desplazamiento,&(paquete->pid),sizeof(pid_t));
	desplazamiento+= sizeof(pid_t);
	memcpy(buffer + desplazamiento, &(paquete->codigoOperacion),
			sizeof(t_header));
	desplazamiento += sizeof(t_header);
	memcpy(buffer + desplazamiento, &(paquete->buffer->size), sizeof(uint32_t));
	desplazamiento += sizeof(int);


	(*bytes) = sizeSerializado;

	return buffer;

}

//MENSAJE NEW_POKEMON

void *serializarMensajeNewPokemon(t_mensaje *mensaje,int *bytes){
	int sizeSerializado=sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+mensaje->largoNombre;
	void *buffer=malloc(sizeSerializado);
	int desplazamiento=0;
	memcpy(buffer+desplazamiento,&mensaje->cantidadPokemons,sizeof(uint32_t));
	desplazamiento+=sizeof(uint32_t);
	memcpy(buffer+desplazamiento,&mensaje->largoNombre,sizeof(uint32_t));
	desplazamiento+=sizeof(uint32_t);
	memcpy(buffer+desplazamiento,&mensaje->posX,sizeof(uint32_t));
	desplazamiento+=sizeof(uint32_t);
	memcpy(buffer+desplazamiento,&mensaje->posY,sizeof(uint32_t));
	desplazamiento+=sizeof(uint32_t);
	memcpy(buffer+desplazamiento,mensaje->nombrePokemon,mensaje->largoNombre);
	desplazamiento+=mensaje->largoNombre;
	(*bytes)=sizeSerializado;

	return buffer;
}

void crearMensajeNewPokemon(pid_t pid, char* nombrePokemon, uint32_t posX,
		uint32_t posY, uint32_t cantidadPokemons, int socketCliente) {
	//t_paquete* paquete = malloc(sizeof(t_paquete));

	uint32_t stringSize = strlen(nombrePokemon)+1;
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigoOperacion = MENSAJE_NEW_POKEMON;
	paquete->pid=pid;
	paquete->buffer = malloc(sizeof(t_buffer));
	t_mensaje *mensaje=malloc(sizeof(t_mensaje));
	mensaje->cantidadPokemons=cantidadPokemons;
	mensaje->largoNombre=stringSize;
	mensaje->posX=posX;
	mensaje->posY=posY;
	mensaje->nombrePokemon=string_duplicate(nombrePokemon);
	mensaje->booleano=true;
	mensaje->cantidadDePares=0;
	mensaje->paresDeCoordenadas=list_create();
	paquete->buffer->size=sizeof(mensaje);

	printf("Se creara mensaje: \n");
	printf("---Mensaje NEW_POKEMON---\n");
	printf("NombrePokemon: %s\n",mensaje->nombrePokemon);
	printf("LargoNombre: %d\n",mensaje->largoNombre);
	printf("PosX: %d\n",mensaje->posX);
	printf("PosY: %d\n",mensaje->posY);
	printf("---Fin Mensaje NEW_POKEMON---\n");


	int sizeSerializado;
	void* serializado = serializarPaquete(paquete, &sizeSerializado);
	send(socketCliente,&sizeSerializado,sizeof(int),MSG_NOSIGNAL);
	send(socketCliente, serializado, sizeSerializado, 0);
	int sizeMensajeSerializado;
	void *mensajeSerializado=serializarMensajeNewPokemon(mensaje,&sizeMensajeSerializado);
	send(socketCliente,&sizeMensajeSerializado,sizeof(int),MSG_NOSIGNAL);
	send(socketCliente,mensajeSerializado,sizeMensajeSerializado,0);
	free(serializado);
	free(mensajeSerializado);
return;
}
//FIN Mensaje NEW_POKEMON

//Mensaje GET_POKEMON
void *serializarMensajeGetPokemon(t_mensaje *mensaje,int *bytes)
{
	int sizeSerializado=sizeof(uint32_t)+mensaje->largoNombre;
	void *buffer=malloc(sizeSerializado);
	int desplazamiento=0;
	memcpy(buffer+desplazamiento,&mensaje->largoNombre,sizeof(uint32_t));
	desplazamiento+=sizeof(uint32_t);
	memcpy(buffer+desplazamiento,mensaje->nombrePokemon,mensaje->largoNombre);
	desplazamiento+=mensaje->largoNombre;
	(*bytes)=sizeSerializado;

	return buffer;
}
//Todo
void crearMensajeGetPokemon(int pid, char* nombrePokemon, int socketCliente)
{
	uint32_t stringSize=strlen(nombrePokemon)+1;
	t_paquete *paquete=malloc(sizeof(t_paquete));
	paquete->codigoOperacion=MENSAJE_GET_POKEMON;
	paquete->pid=pid;
	paquete->buffer=malloc(sizeof(t_buffer));
	t_mensaje *mensaje=malloc(sizeof(t_mensaje));
	mensaje->largoNombre=stringSize;
	mensaje->nombrePokemon=string_duplicate(nombrePokemon);
	paquete->buffer->size=sizeof(mensaje);

	printf("Se creara mensaje: \n");
	printf("---Mensaje GET_POKEMON---\n");
	printf("NombrePokemon: %s\n",mensaje->nombrePokemon);
	printf("LargoNombre: %d\n",mensaje->largoNombre);
	printf("---Fin Mensaje GET_POKEMON---\n");

	int sizeSerializado;
	void *serializado=serializarPaquete(paquete,&sizeSerializado);
	send(socketCliente,&sizeSerializado,sizeof(int),MSG_NOSIGNAL);
	send(socketCliente,serializado,sizeSerializado,0);
	int sizeMensajeSerializado;
	void *mensajeSerializado=serializarMensajeGetPokemon(mensaje,&sizeMensajeSerializado);
	send(socketCliente,&sizeMensajeSerializado,sizeof(int),MSG_NOSIGNAL);
	send(socketCliente,mensajeSerializado,sizeMensajeSerializado,0);
	free(serializado);
	free(mensajeSerializado);
return;
}

//FIN Mensaje GET_POKEMON

//Mensaje APPEARED_POKEMON
//Todo
void *serializarMensajeAppearedPokemon(t_mensaje *mensaje,int *bytes)
{

}
//Todo
void crearMensajeAppearedPokemon(int pid, char* nombrePokemon, int posX,
		int posY, int socket_cliente)
{

}

//FIN Mensaje APPEARED_POKEMON

//Mensaje CATCH_POKEMON
//Todo
void* serializarMensajeCatchPokemon(t_paquete* paquete, int *bytes)
{

}
//Todo
void crearMensajeCatchPokemon(int pid, char* nombrePokemon, int posX, int posY,
		int socket_cliente)
{

}

//FIN Mensaje CATCH_POKEMON

//Mensaje CAUGTH_POKEMON
//Todo
void* serializarPaqueteCaughtPokemon(t_paquete* paquete, int *bytes)
{

}
//Todo
void crearMensajeCaughtPokemon(int pid, int booleano, int socket_cliente)
{

}

//FIN Mensaje CAUGHT_POKEMON

//Mensaje LOCALIZED_POKEMON
//Todo
void* serializarPaqueteLocalizedPokemon(t_paquete* paquete, int *bytes)
{

}
//Todo
void crearMensajeLocalizedPokemon(int pid, char* nombrePokemon, int posX,
		int posY, int cantidadPokemons, int socket_cliente)
{

}

//FIN Mensaje LOCALIZED_POKEMON





