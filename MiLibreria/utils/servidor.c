#include "servidor.h"


//void serveClient(int* socket)
//{
//	int cod_op;
//	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
//		cod_op = -1;
//	processRequest(cod_op, *socket);
//}

//void esperarCliente(int socketServidor)
//{
//	struct sockaddr_in dirCliente;
//	unsigned int tamDireccion=0;
//	int socketCliente = accept(socketServidor, (void*)&dirCliente, &tamDireccion);
//
//	printf("Recibi una conexion en %d\n",socketCliente);
//	pthread_create(&thread,NULL,(void*)serveClient,&socketCliente);
//	pthread_detach(thread);
//	printf("Estoy despues del detach\n");
//}

//void processRequest(int cod_op, int cliente_fd) {
//	int size;
//	void* msg;
//		switch (cod_op) {
//		case MENSAJE:
//			msg = recibirMensaje(cliente_fd, &size);
//			//devolver_mensaje(msg, size, cliente_fd);
//			free(msg);
//			break;
//		case 0:
//			pthread_exit(NULL);
//		case -1:
//			pthread_exit(NULL);
//		}
//}

void* recibirMensaje(int socketCliente, int* size)
{
	void * buffer;
	printf("Hola vengo a flotar\n");

	recv(socketCliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socketCliente, buffer, *size, MSG_WAITALL);

	printf("Tamano del mensaje recibido: %d\n", *size);
	printf("Mensaje: %s\n", (char*)buffer);
	return buffer;
	free (buffer);
}
//
void devolverMensaje(void* payload, int size, int socketCliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigoOperacion = MENSAJE;
	paquete->colaMensaje = tNEW_POKEMON;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* aEnviar = serializarPaquete(paquete, &bytes);

	send(socketCliente, aEnviar, bytes, 0);

	free(aEnviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
return;
}
//
int iniciarServidor(char *ip,int puerto) {
	//int ipServidor=atoi(ip);
	printf("Iniciando servidor en el PUERTO=%d e IP=%s",puerto,ip);
	struct sockaddr_in direccionServer;
	direccionServer.sin_family= AF_INET;
	inet_aton(ip,&direccionServer.sin_addr);
	//direccionServer.sin_addr.s_addr = ipServidor;
	//direccionServer.sin_addr.s_addr=INADDR_ANY;
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
	//while(1)
		//esperarCliente(server);
	return server;
}


//int iniciarServidor(char *ip,int puerto){
//	int  socket, val = 1;
//	struct sockaddr_in servaddr;
//
//	servaddr.sin_family = AF_INET;
//	servaddr.sin_addr.s_addr =ip;
//	servaddr.sin_port = htons(puerto);
//
//	socket = createocket();
//	if (socket < 0) {
//		//char* error = strerror(errno);
////		perror(error);
////		free(error);
//		return EXIT_FAILURE;
//	}
//
//	setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
//
//	if (bind(socket,(struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
//		return EXIT_FAILURE;
//	}
//
//	if (listen(socket, 100)< 0) {
//		return EXIT_FAILURE;
//	}
//
//	return socket;
//
//}

//int createSocket(){
//	return socket(AF_INET, SOCK_STREAM, 0);
//}
