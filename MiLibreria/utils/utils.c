#include "utils.h"

int crearConexion(char *ip, int puerto, int tiempoReconexion) {
	printf("Intentando conectar a PUERTO=%d en IP=%s\n", puerto, ip);
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


void* serializarPaquete(t_paquete* paquete, int *bytes) {
	int sizeSerializado = sizeof(pid_t)+sizeof(t_header)+sizeof(bool)+sizeof(int)+sizeof(int)+sizeof(int)+paquete->buffer->largoNombre+sizeof(int)+sizeof(int)+sizeof(int)+paquete->buffer->listaCoordenadas->elements_count*sizeof(int);
	int *coordenadas;
	printf("Estoy antes del malloc\n");
	void* buffer = malloc(sizeSerializado);
	printf("Estoy despues del malloc\n");
	int desplazamiento = 0;
	printf("Estoy en 1\n");
	memcpy(buffer+desplazamiento,&(paquete->pid),sizeof(pid_t));
	desplazamiento+= sizeof(pid_t);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento, &(paquete->codigoOperacion),
			sizeof(t_header));
	desplazamiento += sizeof(t_header);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento, &(paquete->buffer->boolean), sizeof(bool));
	desplazamiento += sizeof(bool);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento, &(paquete->buffer->cantidadPokemons),sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento,&(paquete->buffer->idMensaje),sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento,&(paquete->buffer->largoNombre),sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento, paquete->buffer->nombrePokemon,paquete->buffer->largoNombre);
	desplazamiento += paquete->buffer->largoNombre;
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento,&(paquete->buffer->posX),sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy en 1\n");
	memcpy(buffer + desplazamiento,&(paquete->buffer->posY),sizeof(int));
	desplazamiento += sizeof(int);


	printf("Estoy en 2\n");
	int cantidadCoordenadas=paquete->buffer->listaCoordenadas->elements_count;
	printf("Estoy en 2\n");
	//coordenadas=malloc(cantidadCoordenadas*sizeof(int));
	printf("Estoy en 2\n");
	//coordenadas=aplanarLista(paquete->buffer->listaCoordenadas);
	memcpy(buffer + desplazamiento,&cantidadCoordenadas,sizeof(int));
	desplazamiento+=sizeof(int);
	int i;
	t_list*aux=list_duplicate(paquete->buffer->listaCoordenadas);
	int buffercito;
	while(aux->head!= NULL){
			printf("Serializo %d\n",(int)aux->head->data);
			buffercito=(int)aux->head->data;
			printf("Buffercito vale %d\n",buffercito);
			memcpy(buffer+desplazamiento,&buffercito,sizeof(int));
			desplazamiento+=sizeof(int);
			aux->head=aux->head->next;
			//i++;
		}





	(*bytes) = sizeSerializado;
	printf("Termine de serializar\n");
	return buffer;

}

t_paquete* recibirMensaje(int socketCliente) {
	t_paquete *paquete;
	void *buffer=malloc(100*sizeof(void));
	printf("Estoy en 1\n");
	recv(socketCliente,buffer,100*sizeof(void),MSG_WAITALL);
	printf("Estoy en 1\n");
	paquete=malloc(sizeof(t_paquete));
	paquete->buffer=malloc(sizeof(t_bufferOmnidata));
	int desplazamiento=0;
	printf("Estoy en 1\n");
	memcpy(&paquete->pid,buffer+desplazamiento,sizeof(pid_t));
	desplazamiento+=sizeof(pid_t);
	printf("Estoy asignando %d\n",buffer+desplazamiento);
	printf("Estoy en 1\n");
	memcpy(&paquete->codigoOperacion,buffer+desplazamiento,sizeof(t_header));
	desplazamiento+=sizeof(t_header);
	printf("Estoy en 1\n");
	memcpy(&paquete->buffer->boolean,buffer+desplazamiento,sizeof(bool));
	desplazamiento += sizeof(bool);
	printf("Estoy en 1\n");
	printf("Estoy asignando %d\n",buffer+desplazamiento);
	memcpy(&paquete->buffer->cantidadPokemons,buffer+desplazamiento,sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy asignando %d\n",buffer+desplazamiento);
	printf("Estoy en 1\n");
	memcpy(&paquete->buffer->idMensaje,buffer+desplazamiento,sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy asignando %d\n",buffer+desplazamiento);
	printf("Estoy en 1\n");
	memcpy(&paquete->buffer->largoNombre,buffer+desplazamiento,sizeof(int));
	desplazamiento += sizeof(int);
	paquete->buffer->nombrePokemon=malloc(paquete->buffer->largoNombre);
	printf("Estoy en 1\n");
	memcpy(paquete->buffer->nombrePokemon,buffer+desplazamiento,paquete->buffer->largoNombre);
	desplazamiento += paquete->buffer->largoNombre;
	printf("Estoy asignando %d\n",buffer+desplazamiento);
	printf("Estoy en 1\n");
	memcpy(&paquete->buffer->posX,buffer+desplazamiento,sizeof(int));
	desplazamiento += sizeof(int);
	printf("Estoy asignando %d\n",buffer+desplazamiento);
	printf("Estoy en 1\n");
	memcpy(&paquete->buffer->posY,buffer+desplazamiento,sizeof(int));
	desplazamiento += sizeof(int);


	int cantidadCoordenadas;
	memcpy(&cantidadCoordenadas,buffer+desplazamiento,sizeof(int));
	desplazamiento += sizeof(int);
	int *coordenadas=malloc(sizeof(int)*cantidadCoordenadas);
	int k;
	int buffercito;
	for(k=0;k<cantidadCoordenadas;k++){
	memcpy(&buffercito,buffer+desplazamiento,sizeof(int));
	printf("Agarre: %d\n",buffercito);
	//list_add(paquete->buffer->listaCoordenadas,(void*)buffercito);
	desplazamiento+= sizeof(int);
	}
	paquete->buffer->listaCoordenadas=list_create();
	int i=0;
//		while(i<cantidadCoordenadas){
//			printf("Agregando a la lista: %d\n",coordenadas[i]);
//			list_add(paquete->buffer->listaCoordenadas,coordenadas[i]);
//			i++;
//		}
		i=0;
		while(i<cantidadCoordenadas){
			printf("%d\n",(int)list_get(paquete->buffer->listaCoordenadas,i));
			i++;
		}

	printf("En el campo de texto recibo: %s\n",paquete->buffer->nombrePokemon);


	printf("Termine\n");
return paquete;

}

int *aplanarLista(t_list* lista){
	t_list*aux=list_duplicate(lista);
	int *array=malloc(sizeof(int)*lista->elements_count);
	int i=0;
	while(aux->head!= NULL){
		printf("Aplanando %d\n",(int)aux->head->data);
		*(array+i)=(int)aux->head->data;
		printf("Aplanado: %d\n",array[i]);
		aux->head=aux->head->next;
		i++;
	}
return array;
}


void enviarMensajeRecursoNew(int pid, char* nombrePokemon, int posX, int posY,
		int cantidadPokemons, int socketCliente) {

	uint32_t stringSize = strlen(nombrePokemon)+1;
		t_paquete* unPaquete = malloc(sizeof(t_paquete));
		t_bufferOmnidata *paquete=malloc(sizeof(t_bufferOmnidata));
		unPaquete->codigoOperacion = MENSAJE_NEW_POKEMON;
		unPaquete->pid=pid;
		unPaquete->buffer = malloc(sizeof(t_bufferOmnidata));
		paquete->cantidadPokemons=cantidadPokemons;
		paquete->largoNombre=stringSize;
		paquete->posX=posX;
		paquete->posY=posY;
		paquete->nombrePokemon=string_duplicate(nombrePokemon);
		paquete->listaCoordenadas=list_create();
		list_add(paquete->listaCoordenadas,(void*)1);
		list_add(paquete->listaCoordenadas,(void*)2);
		list_add(paquete->listaCoordenadas,(void*)3);
		list_add(paquete->listaCoordenadas,(void*)4);
		list_add(paquete->listaCoordenadas,(void*)5);
		list_add(paquete->listaCoordenadas,(void*)6);
		list_add(paquete->listaCoordenadas,(void*)7);
		list_add(paquete->listaCoordenadas,(void*)8);
//		list_add(paquete->listaCoordenadas,3);
//		list_add(paquete->listaCoordenadas,1000);
		//unPaquete->buffer->size=sizeof(paquete);

		printf("Se creara mensaje: \n");
		printf("---Mensaje NEW_POKEMON---\n");
		printf("NombrePokemon: %s\n",paquete->nombrePokemon);
		printf("LargoNombre: %d\n",paquete->largoNombre);
		printf("PosX: %d\n",paquete->posX);
		printf("PosY: %d\n",paquete->posY);
		printf("---Fin Mensaje NEW_POKEMON---\n");
		unPaquete->buffer=paquete;


		int sizeSerializado;
		void* serializado = serializarPaquete(unPaquete, &sizeSerializado);
		send(socketCliente,serializado,sizeSerializado,0);
		printf("Mande mensaje\n");
		free(serializado);

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
