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

void liberarConexion(int socket) {
	close(socket);
	return;
}

int enviarInt(int socketDestino, int numero) {
	printf("- Numero a enviar %d . ", numero); //SACARRRRR
	void *buffer = malloc(sizeof(int));
	memcpy(buffer, &numero, sizeof(int));
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
	printf("- Numero a recibir %d . ", *intRecibido);

	free(buffer);
	return leido;
}

int enviarCadena(int socketDestino, char *mensaje) {

	ssize_t len = 0;
	ssize_t sent = 0;
	len = strlen(mensaje);
	char *buffer = malloc((len + 1) * sizeof(char));
	memcpy(buffer, mensaje, len * sizeof(char));

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

void enviarMensajeRecursoNew(int pid, char* nombrePokemon, int posX, int posY,
		int cantidadPokemons, int socketCliente) {

	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, MENSAJE_NEW_POKEMON);

	enviarInt(socketCliente, strlen(nombrePokemon) + 1);

	enviarCadena(socketCliente, nombrePokemon);
	enviarInt(socketCliente, posX);
	enviarInt(socketCliente, posY);
	enviarInt(socketCliente, cantidadPokemons);

}

void enviarMensajeRecursoGet(int pid, char* nombrePokemon, int socketCliente) {

	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, MENSAJE_GET_POKEMON);

	enviarInt(socketCliente, strlen(nombrePokemon) + 1);

	enviarCadena(socketCliente, nombrePokemon);

}

void enviarMensajeRecursoAppeared(int pid, char* nombrePokemon, int posX,
		int posY, int idMensaje, int socketCliente) {

	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, MENSAJE_APPEARED_POKEMON);

	enviarInt(socketCliente, strlen(nombrePokemon) + 1);

	enviarCadena(socketCliente, nombrePokemon);
	enviarInt(socketCliente, posX);
	enviarInt(socketCliente, posY);
	enviarInt(socketCliente, idMensaje);

}

void enviarMensajeRecursoCatch(int pid, char* nombrePokemon, int posX, int posY,
		int socketCliente) {

	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, MENSAJE_CATCH_POKEMON);

	enviarInt(socketCliente, strlen(nombrePokemon) + 1);

	enviarCadena(socketCliente, nombrePokemon);
	enviarInt(socketCliente, posX);
	enviarInt(socketCliente, posY);

}

void enviarMensajeRecursoCaught(int pid, int idMensaje, bool booleano,
		int socketCliente) {

	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, MENSAJE_CAUGHT_POKEMON);

	enviarInt(socketCliente, idMensaje);
	enviarInt(socketCliente, booleano);

}

void enviarMensajeRecursoLocalized(int pid, char* nombrePokemon,
		t_list coordenadas, int socketCliente) {

	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, MENSAJE_LOCALIZED_POKEMON);

	enviarInt(socketCliente, strlen(nombrePokemon) + 1);

	enviarCadena(socketCliente, nombrePokemon);
	//enviarInt(socketCliente, list_size(coordenadas));

}

void suscribirseNew(int pid, int socketCliente) {
	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, SUSCRIBIRSE_NEW_POKEMON);
}
void suscribirseGet(int pid, int socketCliente) {
	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, SUSCRIBIRSE_GET_POKEMON);
}
void suscribirseCatch(int pid, int socketCliente) {
	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, SUSCRIBIRSE_CATCH_POKEMON);
}
void suscribirseCaught(int pid, int socketCliente) {
	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, SUSCRIBIRSE_CAUGHT_POKEMON);
}
void suscribirseAppered(int pid, int socketCliente) {
	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, SUSCRIBIRSE_APPEARED_POKEMON);
}
void suscribirseLocalized(int pid, int socketCliente) {

	enviarInt(socketCliente, pid);
	enviarInt(socketCliente, SUSCRIBIRSE_LOCALIZED_POKEMON);

}

t_paquete* recibirMensajeRecurso(int socketCliente) {

	t_paquete* paquete = malloc(sizeof(t_paquete));

	int pid;
	recibirInt(socketCliente, &pid);

	paquete->pid = pid;

	printf("recibi el pid : %d .\n", pid);

	int codOp;
	recibirInt(socketCliente, &codOp);

	paquete->codigoOperacion = codOp;

	printf("recibi el codOp : %d .\n", codOp);

	paquete->buffer = malloc(sizeof(t_bufferOmnidata));

	switch (codOp) {
	case MENSAJE_NEW_POKEMON: {

		int sizeNombre;
		recibirInt(socketCliente, &sizeNombre);
		paquete->buffer->largoNombre = sizeNombre;

		printf("recibi el size del nombre : %d .\n",
				paquete->buffer->largoNombre);

		char* nombre = malloc(sizeNombre);
		recibirCadena(socketCliente, nombre);

		paquete->buffer->nombrePokemon = nombre;

		printf("recibi el nombre del pokemon y es : %s .\n",
				paquete->buffer->nombrePokemon);

		int posX;
		recibirInt(socketCliente, &posX);
		paquete->buffer->posX = posX;

		printf("recibi la pos en x : %d .\n", paquete->buffer->posX);

		int posY;
		recibirInt(socketCliente, &posY);
		paquete->buffer->posY = posY;
		printf("recibi la pos en y : %d .\n", paquete->buffer->posY);

		int cantidadPokemons;
		recibirInt(socketCliente, &cantidadPokemons);

		paquete->buffer->cantidadPokemons = cantidadPokemons;

		printf("recibi la cantidad : %d .\n",
				paquete->buffer->cantidadPokemons);

		break;

	}

	case MENSAJE_APPEARED_POKEMON: {

		int sizeNombre;
		recibirInt(socketCliente, &sizeNombre);
		paquete->buffer->largoNombre = sizeNombre;

		printf("recibi el size del nombre : %d .\n",
				paquete->buffer->largoNombre);

		char* nombre = malloc(sizeNombre);
		recibirCadena(socketCliente, nombre);

		paquete->buffer->nombrePokemon = nombre;

		printf("recibi el nombre del pokemon y es : %s .\n",
				paquete->buffer->nombrePokemon);

		int posX;
		recibirInt(socketCliente, &posX);
		paquete->buffer->posX = posX;

		printf("recibi la pos en x : %d .\n", paquete->buffer->posX);

		int posY;
		recibirInt(socketCliente, &posY);
		paquete->buffer->posY = posY;
		printf("recibi la pos en y : %d .\n", paquete->buffer->posY);

		int idMensaje;
		recibirInt(socketCliente, &idMensaje);
		paquete->buffer->idMensaje = idMensaje;
		printf("recibi el idMensaje : %d .\n", paquete->buffer->idMensaje);

		break;

	}

	case MENSAJE_CATCH_POKEMON: {

		int sizeNombre;
		recibirInt(socketCliente, &sizeNombre);
		paquete->buffer->largoNombre = sizeNombre;

		printf("recibi el size del nombre : %d .\n",
				paquete->buffer->largoNombre);

		char* nombre = malloc(sizeNombre);
		recibirCadena(socketCliente, nombre);

		paquete->buffer->nombrePokemon = nombre;

		printf("recibi el nombre del pokemon y es : %s .\n",
				paquete->buffer->nombrePokemon);

		int posX;
		recibirInt(socketCliente, &posX);
		paquete->buffer->posX = posX;

		printf("recibi la pos en x : %d .\n", paquete->buffer->posX);

		int posY;
		recibirInt(socketCliente, &posY);
		paquete->buffer->posY = posY;
		printf("recibi la pos en y : %d .\n", paquete->buffer->posY);

		break;

	}
	case MENSAJE_CAUGHT_POKEMON: {

		int idMensaje;
		recibirInt(socketCliente, &idMensaje);
		paquete->buffer->idMensaje = idMensaje;

		int booleano;
		recibirInt(socketCliente, &booleano);
		paquete->buffer->boolean = booleano;

		break;
	}
	case MENSAJE_GET_POKEMON: {
		int sizeNombre;
		recibirInt(socketCliente, &sizeNombre);
		paquete->buffer->largoNombre = sizeNombre;

		printf("recibi el size del nombre : %d .\n",
				paquete->buffer->largoNombre);

		char* nombre = malloc(sizeNombre);
		recibirCadena(socketCliente, nombre);

		paquete->buffer->nombrePokemon = nombre;

		printf("recibi el nombre del pokemon y es : %s .\n",
				paquete->buffer->nombrePokemon);

		break;
	}
	case MENSAJE_LOCALIZED_POKEMON: {
		break;
	}
//	case SUSCRIBIRSE_NEW_POKEMON: {
//		break;
//	}
//	case SUSCRIBIRSE_APPEARED_POKEMON: {
//		break;
//	}
//	case SUSCRIBIRSE_CATCH_POKEMON: {
//		break;
//	}
//	case SUSCRIBIRSE_CAUGHT_POKEMON:{
//		break;
//	}
//	case SUSCRIBIRSE_GET_POKEMON:{
//		break;
//	}
//	case SUSCRIBIRSE_LOCALIZED_POKEMON:
//	{
//		break;
//	}
	}

//free(nombre);
	return paquete;
}

