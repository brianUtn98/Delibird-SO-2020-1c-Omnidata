#include "servidor.h"

int initServer(char* ip,int port) {
	int socket, val = 1;
	struct sockaddr_in servaddr;

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	socket = createSocket();
	if (socket < 0) {
		char* error = strerror(errno);
		perror(error);
		free(error);
		return EXIT_FAILURE;
	}

	setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

	if (bind(socket, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
		return EXIT_FAILURE;
	}

	if (listen(socket, 100) < 0) {
		return EXIT_FAILURE;
	}

	return socket;

}

int createSocket() {
	return socket(AF_INET, SOCK_STREAM, 0);
}

