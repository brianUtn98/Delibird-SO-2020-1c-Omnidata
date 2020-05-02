#include "utils.h"

int crear_conexion(char *ip,int puerto,int tiempo_reconexion)
{
		int max_intentos=3;
		int retry=0;
/*
		struct addrinfo hints;
		struct addrinfo *server_info;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		getaddrinfo(ip, puerto, &hints, &server_info);

		int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
*/

		struct sockaddr_in dirServer;
		dirServer.sin_family= AF_INET;
		dirServer.sin_addr.s_addr=inet_addr("127.0.0.1");
		dirServer.sin_port=htons(puerto);

		int socket_cliente=socket(AF_INET,SOCK_STREAM,0);

		// if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1){
		while((connect(socket_cliente, (void*)&dirServer,sizeof(dirServer)) != 0) && retry<max_intentos){
			retry = retry + 1;
			if(retry==max_intentos){
				perror("Se supero la cantidad de intentos posibles");
				exit(1);
			}

			perror("No se pudo conectar");
			sleep(tiempo_reconexion);
		}


//		freeaddrinfo(server_info);

		return socket_cliente;
}

void* serializar_paquete(t_paquete *paquete,int *bytes)
{
	int size_serializado = sizeof(op_code) + sizeof(int) + paquete->buffer->size;
	void *buffer=malloc(size_serializado);

	int desplazamiento=0;

	memcpy(buffer+desplazamiento,&(paquete->codigo_operacion),sizeof(paquete->codigo_operacion));
	desplazamiento+=sizeof(paquete->codigo_operacion);

	memcpy(buffer+desplazamiento,&(paquete->buffer->size),sizeof(paquete->buffer->size));
	desplazamiento+=sizeof(paquete->buffer->size);

	memcpy(buffer+desplazamiento,paquete->buffer->stream,sizeof(paquete->buffer->size));

	(*bytes)=size_serializado;
	return buffer;
}

void enviar_mensaje(char *mensaje,int socket)
{
	printf("Enviando mensaje %s con %d bytes",mensaje,strlen(mensaje) + 1);

	t_paquete *paquete=malloc(sizeof(t_paquete));
	paquete->codigo_operacion=MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->stream=mensaje;
	paquete->buffer->size=strlen(mensaje)+1;

	int size_serializado;
	void *serializado=serializar_paquete(paquete,&size_serializado);
	int funciono=send(socket,serializado,size_serializado,0);
	if(funciono<=0){
		perror("No se pudo enviar el mensaje");
		free(serializado);
		exit(1);
	}
	free(serializado);
	return;
}

void liberar_conexion(int socket){
close(socket);
return;
}
