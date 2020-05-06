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

//void* serializarPaquete(t_paquete *paquete, int *bytes) {
//	int sizeSerializado = sizeof(t_opCode) + sizeof(t_colaMensaje) + sizeof(int)
//			+ paquete->buffer->size;
//	void *buffer = malloc(sizeSerializado);
//
//	int desplazamiento = 0;
//
//	memcpy(buffer + desplazamiento, &(paquete->codigoOperacion),
//			sizeof(paquete->codigoOperacion));
//	desplazamiento += sizeof(paquete->codigoOperacion);
//
//	memcpy(buffer + desplazamiento, &(paquete->colaMensaje),
//			sizeof(paquete->colaMensaje));
//	desplazamiento += sizeof(paquete->colaMensaje);
//
//	memcpy(buffer + desplazamiento, &(paquete->buffer->size),
//			sizeof(paquete->buffer->size));
//	desplazamiento += sizeof(paquete->buffer->size);
//
//	memcpy(buffer + desplazamiento, paquete->buffer->stream,
//			sizeof(paquete->buffer->size));
//
//	(*bytes) = sizeSerializado;
//	return buffer;
//}
////
//void enviarMensaje(char *mensaje, int socket) {
//	printf("Enviando mensaje %s con %d bytes", mensaje, strlen(mensaje) + 1);
//
//	t_paquete *paquete = malloc(sizeof(t_paquete));
//	paquete->codigoOperacion = SUSCRIPCION;
//	paquete->colaMensaje = tNEW_POKEMON;
//	paquete->buffer = malloc(sizeof(t_buffer));
//	paquete->buffer->stream = mensaje;
//	paquete->buffer->size = strlen(mensaje) + 1;
//
//	int sizeSerializado;
//	void *serializado = serializarPaquete(paquete, &sizeSerializado);
//	int funciono = send(socket, serializado, sizeSerializado, 0);
//	if (funciono <= 0) {
//		perror("No se pudo enviar el mensaje");
//		free(serializado);
//		exit(1);
//	}
//	free(serializado);
//	return;
//}
////
void liberarConexion(int socket) {
	close(socket);
	return;
}

//t_paquete* recv_message(int socket) {
//	t_paquete * message = malloc(sizeof(t_paquete));
//
//	int res = recv(socket, &message->buffer->size, sizeof(size_t), MSG_WAITALL);
////	if (res <= 0 ){
////		close(socket);
////		free(message);
////		return error(res);
////	}
//
//	void* buffer = malloc(message->buffer->size);
//	res = recv(socket, buffer, message->buffer->size, MSG_WAITALL);
//
//	//if(res <= 0){
////		close(socket);
////		free(message);
////		free(buffer);
////		return error(res);
////	}
//
//	message->buffer->stream = calloc(
//			message->buffer->size - sizeof(t_opCode) + 1, 1);
//	memcpy(&message->codigoOperacion, buffer, sizeof(t_opCode));
//	memcpy(message->buffer->stream, buffer + sizeof(t_opCode),
//			message->buffer->size - sizeof(t_opCode));
//	message->buffer->size = message->buffer->size - sizeof(t_opCode);
//
//	free(buffer);
//	return message;
//}

//t_paquete* create_t_message(t_opCode head, t_colaMensaje colaMensaje,t_buffer size,const void* content){
//	t_paquete* message = (t_paquete*)malloc(sizeof(t_paquete));
//	message->codigoOperacion = head;
//	message->colaMensaje = colaMensaje;
//	message->buffer->stream = malloc(t_buffer);
//	message->buffer->size = size + sizeof(head)+sizeof(colaMensaje);
//
//	memset(message->buffer->stream, 0, size);
//	memcpy(message->buffer->stream,content,size);
//
//	return message;
//}


t_paquete *recibirMensaje(int socket_cliente, int* size)
{
	void *buffer;
	int sizeMensaje;
	t_paquete *paquete;
	paquete=malloc(sizeof(t_paquete));

	t_opCode codigoOperacion;
	t_colaMensaje colaMensaje;
	recv(socket_cliente,&codigoOperacion,sizeof(codigoOperacion),0);
	recv(socket_cliente, &sizeMensaje, sizeof(int), 0);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, sizeMensaje, 0);
	recv(socket_cliente,&colaMensaje,sizeof(colaMensaje),0);
	paquete->codigoOperacion=codigoOperacion;
	paquete->colaMensaje=colaMensaje;
	paquete->buffer=malloc(sizeMensaje);
	paquete->buffer->size=sizeMensaje;
	paquete->buffer->stream=buffer;
	(*size)=sizeMensaje;
	return paquete;
}
void* serializarPaquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigoOperacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;
	memcpy(magic + desplazamiento,&paquete->colaMensaje,sizeof(paquete->colaMensaje));
	printf("Mensaje serializado: %s",(char*)magic);
	return magic;
}

void crearMensaje(void* payload, int size, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigoOperacion = SUSCRIPCION;
	paquete->colaMensaje = tNEW_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);
	printf("Se creara mensaje\n");
	printf("Codigo de operacion=%d\n",paquete->codigoOperacion);
	printf("Cola de mensaje=%d\n",paquete->colaMensaje);
	printf("Contenido=%s\n",(char*)paquete->buffer->stream);
	printf("Tamano %d bytes\n",paquete->buffer->size);
	int bytes = paquete->buffer->size + 3*sizeof(int);

	void* a_enviar = serializarPaquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}
