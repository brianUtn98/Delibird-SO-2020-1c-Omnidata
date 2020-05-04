#include "utils.h"

int crearConexion(char *ip,int puerto,int tiempoReconexion)
{
		printf("Intentando conectar a PUERTO=%d en IP=%s",puerto,ip);
		int maxIntentos=3;
		int retry=0;
		struct sockaddr_in dirServer;
		dirServer.sin_family= AF_INET;
		dirServer.sin_addr.s_addr=inet_addr(ip);
		dirServer.sin_port=htons(puerto);

		int socketCliente=socket(AF_INET,SOCK_STREAM,0);

		// if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1){
		while((connect(socketCliente, (void*)&dirServer,sizeof(dirServer)) != 0) && retry<maxIntentos){
			retry = retry + 1;
			if(retry==maxIntentos){
				perror("Se supero la cantidad de intentos posibles");
				exit(1);
			}

			perror("No se pudo conectar");
			sleep(tiempoReconexion);
		}


//		freeaddrinfo(server_info);

		return socketCliente;
}

//void* serializarPaquete(t_paquete *paquete,int *bytes)
//{
//	int sizeSerializado = sizeof(op_code) + sizeof(int) + paquete->buffer->size;
//	void *buffer=malloc(sizeSerializado);
//
//	int desplazamiento=0;
//
//	memcpy(buffer+desplazamiento,&(paquete->codigo_operacion),sizeof(paquete->codigo_operacion));
//	desplazamiento+=sizeof(paquete->codigo_operacion);
//
//	memcpy(buffer+desplazamiento,&(paquete->buffer->size),sizeof(paquete->buffer->size));
//	desplazamiento+=sizeof(paquete->buffer->size);
//
//	memcpy(buffer+desplazamiento,paquete->buffer->stream,sizeof(paquete->buffer->size));
//
//	(*bytes)=sizeSerializado;
//	return buffer;
//}
//
//void enviarMensaje(char *mensaje,int socket)
//{
//	printf("Enviando mensaje %s con %d bytes",mensaje,strlen(mensaje) + 1);
//
//	t_paquete *paquete=malloc(sizeof(t_paquete));
//	paquete->codigo_operacion=MENSAJE;
//	paquete->buffer = malloc(sizeof(t_buffer));
//	paquete->buffer->stream=mensaje;
//	paquete->buffer->size=strlen(mensaje)+1;
//
//	int sizeSerializado;
//	void *serializado=serializarPaquete(paquete,&sizeSerializado);
//	int funciono=send(socket,serializado,sizeSerializado,0);
//	if(funciono<=0){
//		perror("No se pudo enviar el mensaje");
//		free(serializado);
//		exit(1);
//	}
//	free(serializado);
//	return;
//}
//
//void liberarConexion(int socket){
//close(socket);
//return;
//}

t_paquete* recv_message(int socket){
	t_paquete * message = malloc(sizeof(t_paquete));

	int res = recv(socket,&message->size,sizeof(size_t),MSG_WAITALL);
	if (res <= 0 ){
		close(socket);
		free(message);
		return error(res);
	}

	void* buffer = malloc(message->size);
	res = recv(socket,buffer,message->size,MSG_WAITALL);


	if(res <= 0){
		close(socket);
		free(message);
		free(buffer);
		return error(res);
	}

	message->stream = calloc(message->size - sizeof(int)+1,1);
	memcpy(&message->codigoOperacion, buffer, sizeof(int));
	memcpy(message->stream,buffer + sizeof(int),message->size - sizeof(int));
	message->size = message->size - sizeof(int);

	free(buffer);
	return message;
}

