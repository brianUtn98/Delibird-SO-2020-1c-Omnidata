#include "servidor.h"

void serve_client(int* socket)
{
	printf("Estoy en funcion serve_cliente\n");
	int funciono=0;
	if(recv(*socket, &funciono, sizeof(int), MSG_WAITALL) == -1)
		funciono = -1;
	printf("Que es funciono? %d\n", funciono);
	process_request(funciono, *socket);
return;
}

void esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dirCliente;
	unsigned int tamDireccion=0;
	int socket_cliente = accept(socket_servidor, (void*)&dirCliente, &tamDireccion);

	printf("Recibi una conexion en %d\n",socket_cliente);
	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);
	printf("Estoy despues del detach\n");
}

void process_request(int funciono, int cliente_fd) {
	int size=0;
	printf("Estoy en process request\n");
	printf("Que es funciono %d\n",funciono);

	recibir_unmensaje(cliente_fd,&size);

	//devolver_mensaje(msg, size, cliente_fd);
	//free(msg);

return;
}

void recibir_unmensaje(int socket_cliente, int* size)
{
	void * buffer;
	printf("Hola vengo a flotar\n");

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	printf("Tamano del mensaje recibido: %d\n", *size);
	printf("Mensaje: %s\n", (char*)buffer);
	return;
}

void devolver_mensaje(void* payload, int size, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, &bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
return;
}

void iniciarServidor(char *ip,int puerto) {
	int ipServidor=atoi(ip);
	struct sockaddr_in direccionServer;
	direccionServer.sin_family= AF_INET;
	direccionServer.sin_addr.s_addr = ipServidor;
	direccionServer.sin_addr.s_addr=INADDR_ANY;
	direccionServer.sin_port=htons(puerto);

	int server = socket(AF_INET, SOCK_STREAM,0);

	int activado=1;
	setsockopt(server,SOL_SOCKET,SO_REUSEADDR,&activado,sizeof(activado));
	if(bind(server, (void*) &direccionServer ,sizeof(direccionServer)) !=0)
	{
		perror("fallo el bind");
		exit(1);
	}

	printf("Estoy escuchando \n");
	listen(server,100);
	while(1)
		esperar_cliente(server);
}
