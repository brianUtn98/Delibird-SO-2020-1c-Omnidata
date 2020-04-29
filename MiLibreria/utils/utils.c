#include "utils.h"

void* serializar_paquete(t_paquete *paquete,int* bytes){
	//Serializa un paquete
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

int crear_conexion(char *ip,int *puerto){
		struct addrinfo hints;
		struct addrinfo *server_info;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		getaddrinfo(ip, puerto, &hints, &server_info);

		int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

		if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
			printf("error");

		freeaddrinfo(server_info);

		return socket_cliente;
}

void enviar_mensaje(char *mensaje,int socket){
printf("Enviando mensaje %s con %d bytes",mensaje,strlen(mensaje) + 1);
t_paquete *paquete=malloc(sizeof(t_paquete));
paquete->codigo_operacion=MENSAJE;
paquete->buffer->stream=mensaje;
paquete->buffer->size=strlen(mensaje)+1;

int size_serializado;
void *serializado=serializar_paquete(paquete,&size_serializado);
send(socket,serializado,size_serializado,0);
free(serializado);

return;
}

char* recibir_mensaje(int socket,int *buffer_size){
op_code codigo_operacion;
char *buffer;
recv(socket,&codigo_operacion,sizeof(codigo_operacion),0);
recv(socket,buffer_size,sizeof(buffer_size),0);
buffer=malloc((*buffer_size));
recv(socket,buffer,buffer_size,0);

	if(buffer[*(buffer_size - 1)] != '\0'){
		printf("WARN - El buffer recibido no es un string\n");
	}

return buffer;
}

void liberar_conexion(int socket){
close(socket);
return;
}
