#include "servidor.h"
/*
 ** server.c -- Ejemplo de servidor de sockets de flujo
 */

void sigchld_handler(int s) {
	while (wait(NULL) > 0)
		;
}
//int escucharSocket(int puertoObjetivo, t_log *log) {
//
//	int socketEscucha;
//	int activador = 1;
//	struct sockaddr_in direccion;
//
//  	if ((socketEscucha = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//    	log_error(log, "- FALLO AL SOLICITAR SOCKET");
//    	return -1;
//  	}
//
//  	if (setsockopt(socketEscucha, SOL_SOCKET, SO_REUSEADDR, (char *) &activador, sizeof(activador)) < 0) {
//  		return -1;
//  	}
//
//  	direccion.sin_family = AF_INET;
//  	direccion.sin_addr.s_addr = INADDR_ANY;
//  	direccion.sin_port = htons(puertoObjetivo);
//
//  	if (bind(socketEscucha, (struct sockaddr*) &direccion, sizeof(direccion)) < 0) {
//  		log_error(log, "- FALLO AL ASIGNAR PUERTO AL SOCKET");
//  		return -1;
//  	}
//
//  	if (listen(socketEscucha, TOPE_CLIENTES_ACTIVOS) < 0) {
//  		log_error(log, "- FALLO AL ESCUCHAR");
//  		return -1;
//  	}
//
//  	log_trace(log, "- SOCKET %d ESCUCHANDO DESDE EL PUERTO: %d", socketEscucha, puertoObjetivo);
//	return socketEscucha;
//
//}
//void iniciarServidor(void) {
//	int sockfd, new_fd; // Escuchar sobre sock_fd, nuevas conexiones sobre new_fd
//
//// información sobre mi dirección
//	struct sockaddr_in their_addr; // información sobre la dirección del cliente
//	int sin_size;
//	struct sigaction sa;
//	int yes = 1;
//	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
//		perror("socket");
//		exit(1);
//	}
//	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
//		perror("setsockopt");
//		log_info(logger, "No se pudo crear el socket");
//		exit(1);
//	}
//	my_addr.sin_family = AF_INET;
//	my_addr.sin_port = htons(6009); //MY_PORT, harcodeado BROKERConfig->puertoBroker
//// Ordenación de bytes de la máquina
//// short, Ordenación de bytes de la red
//	my_addr.sin_addr.s_addr = INADDR_ANY;//BROKERConfig->ipBroker; //Rellenar con mi dirección IP
//	memset(&(my_addr.sin_zero), '\0', 8); // Poner a cero el resto de la estructura
//	if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr))
//			== -1) {
//		perror("bind");
//		log_error(logger, "Bind falló. Error");
//		exit(1);
//	}
//	if (listen(sockfd, BACKLOG) == -1) {
//		perror("listen");
//		log_error(logger, "Listen falló. Error");
//		exit(1);
//	}
//	sa.sa_handler = sigchld_handler; // Eliminar procesos muertos
//	sigemptyset(&sa.sa_mask);
//	sa.sa_flags = SA_RESTART;
//	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
//		perror("sigaction");
//		log_error(logger, "Eliminar procesos muertos falló. Error");
//
//		exit(1);
//	}
//	while (1) { // main accept() loop
//		sin_size = sizeof(struct sockaddr_in);
//		if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size))== -1) {
//			perror("accept");
//			log_error(logger, "Aceptar falló. Error");
//			continue;
//		}
//		printf("server: got connection from %s\n",
//				inet_ntoa(their_addr.sin_addr));
//		if (!fork()) { // Este es el proceso hijo
//			close(sockfd); // El hijo no necesita este descriptor
//			if (send(new_fd, "Hello, world!\n", 14, 0) == -1)
//				perror("send");
//			log_error(logger, "Send falló. Error");
//			close(new_fd);
//			exit(0);
//		}
//		close(new_fd); // El proceso padre no lo necesita
//	}
//	//return 0;
//}
void iniciarServidor(void) {
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = INADDR_ANY;
	direccionServidor.sin_port = htons(6009);//(BROKERConfig->puertoBroker);
	int servidor = socket(AF_INET, SOCK_STREAM, 0);
	int activado = 1;
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));
	if (bind(servidor, (void*) &direccionServidor, sizeof(direccionServidor))
			!= 0) {
		perror("Falló el bind");
		exit(1);
	}
	printf("estoy escuchando\n");
	listen(servidor, 100);

	//-----------------
	struct sockaddr_in direccionCliente;
	unsigned int tamanoDireccion;
	int cliente = accept(servidor, (void*) &direccionCliente, &tamanoDireccion);

	printf("Recibi una conexión en %d!!!\n", cliente);

	for (;;)
		;

}

