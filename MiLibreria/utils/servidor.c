#include "servidor.h"


/*void iniciar_servidor(char *IP,int PUERTO)// yo aca pasaria por parametro el brokerConfig o directamente lo usaria, porque

{					  // es una variable global. el puerto es un int.
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, (char*)PUERTO, &hints, &servinfo);//puerto es int y esta funcion necesita un char*
    												// hay que usar alguna funcion intToChar o algo asi.
    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_servidor);
            continue;
        }
        break;
    }

	listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);

    while(1){
    	printf("estoy escuchando");
    	esperar_cliente(socket_servidor);

    }

return;
}*/

/*void esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);
return;
}*/

/*void serve_client(int* socket)
{
	int cod_op;
	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		cod_op = -1;
	process_request(cod_op, *socket);
return;
}*/

/*void process_request(int cod_op, int cliente_fd) {
	int size;
	void* msg;
		switch (cod_op) {
		case MENSAJE:
			msg = recibir_mensaje(cliente_fd,&size);

			devolver_mensaje(msg, size, cliente_fd);
			free(msg);
			break;
		case 0:
			pthread_exit(NULL);
		case -1:
			pthread_exit(NULL);
		}
return;
}*/

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

	//----------------- Fin de escuchar
	// Aceptar cliente

	struct sockaddr_in dirCliente;
	unsigned int tamDireccion=0;
	int cliente = accept(server, (void*)&dirCliente, &tamDireccion);

	printf("Recibi una conexion en %d\n",cliente);
	//--------------


	while(1){
	/*uint32_t tam;
	recv(cliente,&tam,4,0);*/
	char* buff=malloc(1000);
	int bytesRecibidos = recv(cliente,buff,1000,0);
	if(bytesRecibidos <= 0){
		perror("No se pudo conectar.");
		exit(1);
	}

	buff[bytesRecibidos]='\0';
	printf("Me llegaron %d bytes con %s\n",bytesRecibidos,buff);
	free(buff);
	}
	close(server);
	//for(;;);
	printf("\nSaliendo");
	return;
}
