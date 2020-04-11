#include "sockets.h"

#define TOPE_CLIENTES_ACTIVOS 100

int escucharSocket(int puertoObjetivo, t_log *log) {

	int socketEscucha;
	int activador = 1;
	struct sockaddr_in direccion;

  	if ((socketEscucha = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    	log_error(log, "- FALLO AL SOLICITAR SOCKET");
    	return -1;
  	}

  	if (setsockopt(socketEscucha, SOL_SOCKET, SO_REUSEADDR, (char *) &activador, sizeof(activador)) < 0) {
  		return -1;
  	}

  	direccion.sin_family = AF_INET;
  	direccion.sin_addr.s_addr = INADDR_ANY;
  	direccion.sin_port = htons(puertoObjetivo);

  	if (bind(socketEscucha, (struct sockaddr*) &direccion, sizeof(direccion)) < 0) {
  		log_error(log, "- FALLO AL ASIGNAR PUERTO AL SOCKET");
  		return -1;
  	}
  
  	if (listen(socketEscucha, TOPE_CLIENTES_ACTIVOS) < 0) {
  		log_error(log, "- FALLO AL ESCUCHAR");
  		return -1;
  	}

  	log_trace(log, "- SOCKET %d ESCUCHANDO DESDE EL PUERTO: %d", socketEscucha, puertoObjetivo);
	return socketEscucha;

}

int aceptarConexion(int socketEscucha, t_log *log) {
  	
  	int nuevoSocketCliente;
  	struct sockaddr_in direccionClie;
  	socklen_t tamanioDireClie = sizeof(direccionClie); //revisar si no es de la estructura sino addrlen
  	
  	if ((nuevoSocketCliente = accept(socketEscucha,(struct sockaddr*) &direccionClie, &tamanioDireClie)) < 0) {
  		log_error(log, "- FALLO AL ACEPTAR CONEXION DE UN CLIENTE");
  		return 1;
  	}

  	log_info(log, "- CONEXION ACEPTADA");
  	return nuevoSocketCliente;

}

int conectarseA(char *ip, int puertoObjetivo, t_log *log) {
	
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr(ip);
	direccionServidor.sin_port = htons(puertoObjetivo);
	int nuevoCliente = socket(AF_INET, SOCK_STREAM, 0);
	
	if (connect(nuevoCliente, (void*) &direccionServidor, sizeof(direccionServidor)) != 0) {
		log_error(log, "- FALLO AL CONECTAR AL SERVIDOR");
		return 0;
	}

	log_info(log, "- CONEXION EXITOSA");
	return nuevoCliente;

}

int enviarInt(int socketDestino, int numero) {
	printf("- Numero a enviar %d",numero); //SACARRRRR
	void *buffer = malloc(sizeof(int));
	memcpy(buffer, &numero,sizeof(int));
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
	printf("- Numero a recibir %d",*intRecibido);

	free(buffer);
	return leido;
}

int enviarCadena(int socketDestino, char *mensaje) {
	
	ssize_t len = 0;
	ssize_t sent = 0;
	len = strlen(mensaje);
	char *buffer = malloc((len + 1) * sizeof(char));
	memcpy(buffer,mensaje,len * sizeof(char));

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

int cliente(char *ipServidor, int puertoServidor, int idCliente, t_log *log) {

	int socketCliente = conectarseA(ipServidor, puertoServidor, log);
	char *nombreCliente = devuelveNombreProceso(idCliente);

	if (socketCliente == 0) {
		//return EXIT_FAILURE;
		return 0;
	} else {
		log_trace(log, "- %s conectado a su servidor a través del socket %d", nombreCliente, socketCliente);
		return socketCliente;
	}

	free(nombreCliente);

}

void servidor_inicializar(void* unaInfoServidor) {

	int socket_desc;
	int client_sock;
	int c;

	infoServidor_t* unaInfo = (infoServidor_t*) unaInfoServidor;

	struct sockaddr_in server;
	struct sockaddr_in client;

	socket_desc = socket(AF_INET,SOCK_STREAM,0);
	setsockopt(socket_desc,SOL_SOCKET,SO_REUSEADDR,1,sizeof(int));
       
	   if(socket_desc == -1)
       {
            log_info(unaInfo->log,"No se pudo crear el socket");
       }
       
	   server.sin_family = AF_INET;

	   if(!strcmp(unaInfo->ip,"0")){
		   	server.sin_addr.s_addr = INADDR_ANY;
	   }else{
	   	server.sin_addr.s_addr = inet_addr(unaInfo->ip);
	   }

	   server.sin_port = htons(unaInfo->puerto);

       if(bind(socket_desc,(struct sockaddr *)&server,sizeof(server)) < 0)
       {
               log_error(unaInfo->log,"Bind fallo. Error");
			   close(socket_desc);
               return 0;
       }

       listen(socket_desc,3);

       c = sizeof(struct sockaddr_in);

       pthread_t thread_id;

       while((client_sock = accept(socket_desc,(struct sockaddr*)&client,(socklen_t*)&c))){

            log_info(unaInfo->log,"Cliente conectado!");
            log_info(unaInfo->log,"Creando hilo de administracion de conexion con cliente ...");

			infoAdminConexiones_t* unaInfoAdmin = malloc(sizeof(infoAdminConexiones_t));
			unaInfoAdmin->log = unaInfo->log;
			unaInfoAdmin->socketCliente = client_sock;

            if(pthread_create(&thread_id,NULL,(void*)AdministradorDeConexiones,(void*)unaInfoAdmin) < 0){

                log_error(unaInfo->log,"No se pudo crear el hilo");
                return 0;

            }
            log_info(unaInfo->log,"Hilo creado correctamente");      
       }

       if(client_sock<0){

            log_error(unaInfo->log,"accept fallo");
	   		close(socket_desc);
            return 0;
			
       }
       return 1;
}

char* devuelveNombreProceso(int idProceso) {
	
	char* nombreProceso = string_new();

	switch(idProceso) {
		
		case 1: {
			string_append(&nombreProceso,"libMUSE");
			break;
		}
		case 2: {
			string_append(&nombreProceso,"Hilolay");
			break;
		}
		case 3: {
			string_append(&nombreProceso,"SAC-cli");
			break;
		}
		case -1:{
			string_append(&nombreProceso,"Chequeo conexión");
		}
		default : {
			string_append(&nombreProceso,"Proceso Inválido");
			break;
		}
	}

	return nombreProceso;

}

int enviarPorSocket(int fdCliente, const void* mensaje, int totalAEnviar, t_log *log) {
	
	int bytes_enviados;
	int totalEnviado = 0;

	while (totalEnviado < totalAEnviar) {
		bytes_enviados = send(fdCliente, mensaje + totalEnviado, totalAEnviar, 0);
	
		if (bytes_enviados == FAIL) {
			break;
		}
	
		totalEnviado += bytes_enviados;
		totalAEnviar -= bytes_enviados;
	
	}
	//if (bytes_enviados == FAIL) manejaError ("[ERROR] Funcion send");

	log_info(log, "- SE HAN ENVIADO %d BYTES CON EXITO", bytes_enviados);
	return bytes_enviados;

}

int recibirPorSocket(int fdEmisor, void *buffer, int totalARecibir, t_log *log) {
	
	int totalRecibido = 0;
	int bytesRecibidos;

 	printf("\n--->Total a recibir dentro del recibir por socket: %d",totalARecibir);

	while (totalRecibido < totalARecibir) {
		
		bytesRecibidos = recv(fdEmisor, buffer + totalRecibido, totalARecibir, 0);
		
		if ((bytesRecibidos == FAIL) || (bytesRecibidos == 0)) {
			log_error(log, "- FALLO AL RECIBIR");
			break;
		}
		
		totalRecibido += bytesRecibidos;
		totalARecibir -= bytesRecibidos;
	}

	return bytesRecibidos; // retorna -1 si fallo, 0 si se desconecto o los bytes recibidos

}