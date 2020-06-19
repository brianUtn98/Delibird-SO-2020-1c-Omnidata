#include "broker.h"

void inicializarLogger() {
	//Crea el logger
	logger = log_create("BROKER.log", "BROKER", 1, LOG_LEVEL_TRACE);
}

void inicializarLoggerEntregable() {
	printf("Voy a crear un logger %s\n", brokerConf->logFile);

	logEntrega = log_create(brokerConf->logFile, "BROKER", 1, LOG_LEVEL_TRACE);
	if (logEntrega == NULL) {
		perror("No se pudo inicializar el logger para la entrega\n");
	}
}

void cargarConfigBROKER() {
	//printf("pude cargar la configuracion correctamente");
	//Carga la configuracion del txt de config al struct de config

	//brokerConf = (t_BROKERConfig)malloc(sizeof(t_BROKERConfig));
	brokerConf = (t_BROKERConfig*) malloc(sizeof(t_BROKERConfig));
	BROKERTConfig = config_create(BROKER_CONFIG_PATH);
	//inicializarLogger();
	if (BROKERTConfig == NULL) {
		perror("error archivo de configuracion");
		log_error(logger, "- NO SE PUDO IMPORTAR LA CONFIGURACION");
		exit(1);
	}

	brokerConf->tamanoMemoria = config_get_int_value(BROKERTConfig,
			"TAMANO_MEMORIA");
	log_info(logger, "TAMANO_MEMORIA=%d", brokerConf->tamanoMemoria);
	brokerConf->tamanoMinimoParticion = config_get_int_value(BROKERTConfig,
			"TAMANO_MINIMO_PARTICION");
	log_info(logger, "TAMANO_MINIMO_PARTICION=%d",
			brokerConf->tamanoMinimoParticion);
	brokerConf->algoritmoMemoria = string_duplicate(
			config_get_string_value(BROKERTConfig, "ALGORITMO_MEMORIA"));
	log_info(logger, "ALGORITMO_MEMORIA=%s", brokerConf->algoritmoMemoria);
	brokerConf->algoritmoReemplazo = string_duplicate(
			config_get_string_value(BROKERTConfig, "ALGORITMO_REEMPLAZO"));
	log_info(logger, "ALGORITMO_REEMPLAZO=%s", brokerConf->algoritmoReemplazo);
	brokerConf->ipBroker = string_duplicate(
			config_get_string_value(BROKERTConfig, "IP_BROKER"));
	log_info(logger, "IP_BROKER=%s", brokerConf->ipBroker);
	brokerConf->puertoBroker = config_get_int_value(BROKERTConfig,
			"PUERTO_BROKER");
	log_info(logger, "PUERTO_BROKER=%d", brokerConf->puertoBroker);
	brokerConf->frecuenciaCompactacion = config_get_int_value(BROKERTConfig,
			"FRECUENCIA_COMPACTACION");
	log_info(logger, "FRECUENCIA_COMPACTACION=%d",
			brokerConf->frecuenciaCompactacion);
	brokerConf->logFile = string_duplicate(
			config_get_string_value(BROKERTConfig, "LOG_FILE"));
	log_info(logger, "LOG_FILE=%s", brokerConf->logFile);

	log_info(logger, "- CONFIGURACION IMPORTADA\n");

	config_destroy(BROKERTConfig);

	//ver cuando liberar el brokerConf , si lo hacemos acá no se va a poder usar en el servidor por ej,
	//estariamos cargando una estructura y liberandola sin darle uso.
	// talvez podriamos devolver la estructura y que la función no sea void.
	//hacer...
	//return brokerConf;
	//free(brokerConf);

}
//Hago esta función para acordarnos que debemos liberar el malloc de Sofi
void liberarBrokerConf() {
	free(brokerConf);
}

void inicializarColasBroker() {
	NEW_POKEMON = malloc(sizeof(t_cola));
	APPEARED_POKEMON = malloc(sizeof(t_cola));
	CATCH_POKEMON = malloc(sizeof(t_cola));
	CAUGHT_POKEMON = malloc(sizeof(t_cola));
	GET_POKEMON = malloc(sizeof(t_cola));
	LOCALIZED_POKEMON = malloc(sizeof(t_cola));

	NEW_POKEMON->cola = queue_create();
	NEW_POKEMON->lista = list_create();
	APPEARED_POKEMON->cola = queue_create();
	APPEARED_POKEMON->lista = list_create();
	CATCH_POKEMON->cola = queue_create();
	CATCH_POKEMON->lista = list_create();
	CAUGHT_POKEMON->cola = queue_create();
	CAUGHT_POKEMON->lista = list_create();
	GET_POKEMON->cola = queue_create();
	GET_POKEMON->lista = list_create();
	LOCALIZED_POKEMON->cola = queue_create();
	LOCALIZED_POKEMON->lista = list_create();

	NEW_APPEARED_POKEMON = malloc(sizeof(t_parejaCola));
	CATCH_CAUGTH_POKEMON = malloc(sizeof(t_parejaCola));
	GET_LOCALIZED_POKEMON = malloc(sizeof(t_parejaCola));
	return;
}

void destruirColasBroker() {
	queue_destroy(NEW_POKEMON->cola);
	list_destroy(NEW_POKEMON->lista);
	queue_destroy(APPEARED_POKEMON->cola);
	list_destroy(APPEARED_POKEMON->lista);
	queue_destroy(CATCH_POKEMON->cola);
	list_destroy(CATCH_POKEMON->lista);
	queue_destroy(CAUGHT_POKEMON->cola);
	list_destroy(CAUGHT_POKEMON->lista);
	queue_destroy(GET_POKEMON->cola);
	list_destroy(GET_POKEMON->lista);
	queue_destroy(LOCALIZED_POKEMON->cola);
	list_destroy(LOCALIZED_POKEMON->lista);

	free(NEW_POKEMON);
	free(APPEARED_POKEMON);
	free(CATCH_POKEMON);
	free(CAUGHT_POKEMON);
	free(CATCH_CAUGTH_POKEMON);
	free(LOCALIZED_POKEMON);

	free(NEW_APPEARED_POKEMON);
	free(CATCH_CAUGTH_POKEMON);
	free(GET_LOCALIZED_POKEMON);

}
void inicializarEstructuras() {
	estructuraAdministrativa = dictionary_create();
}
//llamarla en la funcion main
void pedirMemoriaInicial() {

	miMemoria = malloc(brokerConf->tamanoMemoria);
	offset = 0;
	iniMemoria = &miMemoria;
	numeroParticion = 0;

}
void* buscarEspacioDisponible(int sizeMensaje) {
	void* puntero = iniMemoria + offset;

	return puntero;
}

t_particionLibre* insertarEnCache(void* mensaje, int size) {
	t_particionLibre *particion = malloc(sizeof(t_particionLibre));

	void* direccionLibre = buscarEspacioDisponible(size); //hacer la funcion buscar espacio y cambiar en el memcpy
	memcpy(direccionLibre, mensaje, size);
	offset += size;
	numeroParticion++;

	return particion;
}

void* administrarMensajes() {

	t_paquete* paquete;

	//while (1) {
	paquete = malloc(sizeof(t_paquete));
	printf("Bloqueado en el mutex\n");
	//sem_wait(&bandejaCounter);
	pthread_mutex_lock(&bandejaMensajes_mutex);
	paquete = (t_paquete*) queue_pop(bandeja);
	pthread_mutex_unlock(&bandejaMensajes_mutex);

	printf(" Mi opCode es : %d,\n ", paquete->codigoOperacion);

	switch (paquete->codigoOperacion) {

	case SUSCRIBIRSE_NEW_POKEMON: {
		list_add(NEW_POKEMON->lista, (void*) paquete->codigoOperacion);
		printf("meti algo en la lista : ");
		log_info(logEntrega, "Se ha suscripto a la cola New.\n");
		break;
	}
	case SUSCRIBIRSE_APPEARED_POKEMON: {
		list_add(APPEARED_POKEMON->lista, (void*) paquete->codigoOperacion);
		log_info(logEntrega, "Se ha suscripto a la cola Appeared.\n");
		break;
	}

	case SUSCRIBIRSE_CATCH_POKEMON: {
		list_add(CATCH_POKEMON->lista, (void*) paquete->codigoOperacion);
		log_info(logEntrega, "Se ha suscripto a la cola Catch.\n");
		break;
	}

	case SUSCRIBIRSE_CAUGHT_POKEMON: {
		list_add(CAUGHT_POKEMON->lista, (void*) paquete->codigoOperacion);
		log_info(logEntrega, "Se ha suscripto a la cola Caught.\n");
		break;
	}

	case SUSCRIBIRSE_GET_POKEMON: {
		list_add(GET_POKEMON->lista, (void*) paquete->codigoOperacion);
		log_info(logEntrega, "Se ha suscripto a la cola Get.\n");
		break;
	}

	case SUSCRIBIRSE_LOCALIZED_POKEMON: {
		list_add(LOCALIZED_POKEMON->lista, (void*) paquete->codigoOperacion);
		log_info(logEntrega, "Se ha suscripto a la cola Localized.\n");
		break;
	}

	case MENSAJE_NEW_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola New.\n");
		t_newPokemon* bufferLoco = malloc(sizeof(t_newPokemon));
		bufferLoco->sizeNombre = paquete->buffer->largoNombre;
		bufferLoco->pokemon = paquete->buffer->nombrePokemon;
		bufferLoco->cantidadPokemons = paquete->buffer->cantidadPokemons;
		bufferLoco->posX = paquete->buffer->posX;
		bufferLoco->posY = paquete->buffer->posY;

		uint32_t sizeMensaje = sizeof(uint32_t) * 4 + bufferLoco->sizeNombre;

		int desplazamiento = 0;
		void* buffer = malloc(sizeMensaje);
		memcpy(buffer + desplazamiento, &bufferLoco->sizeNombre,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(buffer + desplazamiento, bufferLoco->pokemon,
				bufferLoco->sizeNombre);
		desplazamiento += bufferLoco->sizeNombre;

		memcpy(buffer + desplazamiento, &bufferLoco->cantidadPokemons,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(buffer + desplazamiento, &bufferLoco->posX, sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(buffer + desplazamiento, &bufferLoco->posY, sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);

//		t_particionLibre *particion = insertarEnCache(buffer, sizeMensaje);
//
//		t_administrativo* bufferAdmin = malloc(sizeof(t_administrativo));
//		bufferAdmin->idMensaje = paquete->buffer->idMensaje;
//		bufferAdmin->colaMensaje = MENSAJE_NEW_POKEMON;
//		bufferAdmin->suscriptoresEnviados = list_create();
//		bufferAdmin->suscriptoresRecibidos = list_create();
//		bufferAdmin->particion = particion->particion;
//		bufferAdmin->offsetInicioParticion = particion->offsetInicioParticion;
//		bufferAdmin->sizeParticion = particion->sizeParticion;
//		bufferAdmin->sizeMensajeGuardado = sizeMensaje;
//		bufferAdmin->flagLRU = particion->flagLRU;
//
//		dictionary_put(estructuraAdministrativa,
//				string_itoa(bufferAdmin->idMensaje), bufferAdmin);

		printf(" ENCOLE EN NEW : %s . \n", bufferLoco->pokemon);
		break;
	}
	case MENSAJE_APPEARED_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Appeared.\n");
		t_appearedPokemon* bufferLoco = malloc(sizeof(t_appearedPokemon));
		bufferLoco->sizeNombre = paquete->buffer->largoNombre;
		bufferLoco->pokemon = paquete->buffer->nombrePokemon;
		bufferLoco->posX = paquete->buffer->posX;
		bufferLoco->posY = paquete->buffer->posY;

		uint32_t sizeMensaje = sizeof(uint32_t) * 3 + bufferLoco->sizeNombre;

		int desplazamiento = 0;
		void* buffer = malloc(sizeMensaje);
		memcpy(buffer + desplazamiento, &bufferLoco->sizeNombre,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(buffer + desplazamiento, bufferLoco->pokemon,
				bufferLoco->sizeNombre);
		desplazamiento += bufferLoco->sizeNombre;

		memcpy(buffer + desplazamiento, &bufferLoco->posX, sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(buffer + desplazamiento, &bufferLoco->posY, sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		///aca va un semaforo para insertar en la cache
//		t_particionLibre *particion = insertarEnCache(buffer, sizeMensaje);
//
//		t_administrativo* bufferAdmin = malloc(sizeof(t_administrativo));
//		bufferAdmin->idMensaje = paquete->buffer->idMensaje;
//		bufferAdmin->colaMensaje = MENSAJE_APPEARED_POKEMON;
//		bufferAdmin->suscriptoresEnviados = list_create();
//		bufferAdmin->suscriptoresRecibidos = list_create();
//		bufferAdmin->particion = particion->particion;
//		bufferAdmin->offsetInicioParticion = particion->offsetInicioParticion;
//		bufferAdmin->sizeParticion = particion->sizeParticion;
//		bufferAdmin->sizeMensajeGuardado = sizeMensaje;
//		bufferAdmin->flagLRU = particion->flagLRU;
//
//		dictionary_put(estructuraAdministrativa,
//				string_itoa(bufferAdmin->idMensaje), bufferAdmin);

		printf("ENCOLE EN APPEARED : %s . \n", bufferLoco->pokemon);
		break;
	}

	case MENSAJE_CATCH_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Catch.\n");
		t_catchPokemon* bufferLoco = malloc(sizeof(t_catchPokemon));
		bufferLoco->sizeNombre = paquete->buffer->largoNombre;
		bufferLoco->pokemon = paquete->buffer->nombrePokemon;
		bufferLoco->posX = paquete->buffer->posX;
		bufferLoco->posY = paquete->buffer->posY;

		uint32_t sizeMensaje = sizeof(uint32_t) * 3 + bufferLoco->sizeNombre;

		int desplazamiento = 0;
		void* buffer = malloc(sizeMensaje);
		memcpy(buffer + desplazamiento, &bufferLoco->sizeNombre,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(buffer + desplazamiento, bufferLoco->pokemon,
				bufferLoco->sizeNombre);
		desplazamiento += bufferLoco->sizeNombre;

		memcpy(buffer + desplazamiento, &bufferLoco->posX, sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(buffer + desplazamiento, &bufferLoco->posY, sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);

		///aca va un semaforo para insertar en la cache
//		t_particionLibre *particion = insertarEnCache(buffer, sizeMensaje);
//
//		t_administrativo* bufferAdmin = malloc(sizeof(t_administrativo));
//		bufferAdmin->idMensaje = paquete->buffer->idMensaje;
//		bufferAdmin->colaMensaje = MENSAJE_CATCH_POKEMON;
//		bufferAdmin->suscriptoresEnviados = list_create();
//		bufferAdmin->suscriptoresRecibidos = list_create();
//		bufferAdmin->particion = particion->particion;
//		bufferAdmin->offsetInicioParticion = particion->offsetInicioParticion;
//		bufferAdmin->sizeParticion = particion->sizeParticion;
//		bufferAdmin->sizeMensajeGuardado = sizeMensaje;
//		bufferAdmin->flagLRU = particion->flagLRU;
//
//		dictionary_put(estructuraAdministrativa,
//				string_itoa(bufferAdmin->idMensaje), bufferAdmin);


		printf("ENCOLE EN CATCH : %s . \n", bufferLoco->pokemon);
		break;
	}

	case MENSAJE_CAUGHT_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Caught.\n");
		t_caughtPokemon* bufferLoco = malloc(sizeof(t_caughtPokemon));
		bufferLoco->booleano = paquete->buffer->boolean;

		uint32_t sizeMensaje = sizeof(uint32_t);
		int desplazamiento = 0;
		void* buffer = malloc(sizeMensaje);
		memcpy(buffer + desplazamiento, &bufferLoco->booleano, sizeof(int));
		desplazamiento += sizeof(int);

//		t_particionLibre *particion = insertarEnCache(buffer, sizeMensaje);
//
//		t_administrativo* bufferAdmin = malloc(sizeof(t_administrativo));
//		bufferAdmin->idMensaje = paquete->buffer->idMensaje;
//		bufferAdmin->colaMensaje = MENSAJE_CAUGHT_POKEMON;
//		bufferAdmin->suscriptoresEnviados = list_create();
//		bufferAdmin->suscriptoresRecibidos = list_create();
//		bufferAdmin->particion = particion->particion;
//		bufferAdmin->offsetInicioParticion = particion->offsetInicioParticion;
//		bufferAdmin->sizeParticion = particion->sizeParticion;
//		bufferAdmin->sizeMensajeGuardado = sizeMensaje;
//		bufferAdmin->flagLRU = particion->flagLRU;
//
//		dictionary_put(estructuraAdministrativa,
//				string_itoa(bufferAdmin->idMensaje), bufferAdmin);

		//queue_push(CAUGHT_POKEMON->cola, bufferLoco);
		printf("ENCOLE EN CAUGHT : %d . \n", bufferLoco->booleano);
		break;
	}

	case MENSAJE_GET_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Get.\n");
		t_getPokemon* bufferLoco = malloc(sizeof(t_catchPokemon));
		bufferLoco->sizeNombre = paquete->buffer->largoNombre;
		bufferLoco->pokemon = paquete->buffer->nombrePokemon;

		uint32_t sizeMensaje = sizeof(uint32_t) + bufferLoco->sizeNombre;

		int desplazamiento = 0;
		void* buffer = malloc(sizeMensaje);
		memcpy(buffer + desplazamiento, &bufferLoco->sizeNombre,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(buffer + desplazamiento, bufferLoco->pokemon,
				bufferLoco->sizeNombre);
		desplazamiento += bufferLoco->sizeNombre;

//		t_particionLibre *particion = insertarEnCache(buffer, sizeMensaje);
//
//		t_administrativo* bufferAdmin = malloc(sizeof(t_administrativo));
//		bufferAdmin->idMensaje = paquete->buffer->idMensaje;
//		bufferAdmin->colaMensaje = MENSAJE_GET_POKEMON;
//		bufferAdmin->suscriptoresEnviados = list_create();
//		bufferAdmin->suscriptoresRecibidos = list_create();
//		bufferAdmin->particion = particion->particion;
//		bufferAdmin->offsetInicioParticion = particion->offsetInicioParticion;
//		bufferAdmin->sizeParticion = particion->sizeParticion;
//		bufferAdmin->sizeMensajeGuardado = sizeMensaje;
//		bufferAdmin->flagLRU = particion->flagLRU;
//
//		dictionary_put(estructuraAdministrativa,
//				string_itoa(bufferAdmin->idMensaje), bufferAdmin);

		printf("ENCOLE EN GET : %s . \n", bufferLoco->pokemon);

		break;
	}

	case MENSAJE_LOCALIZED_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Localized.\n");
		t_localizedPokemon* bufferLoco = malloc(sizeof(t_localizedPokemon));

		bufferLoco->sizeNombre = paquete->buffer->largoNombre;
		bufferLoco->pokemon = paquete->buffer->nombrePokemon;
		bufferLoco->cantidadDePosiciones =
				paquete->buffer->listaCoordenadas->elements_count;
		bufferLoco->posiciones = list_create();
		bufferLoco->posiciones = list_duplicate(
				paquete->buffer->listaCoordenadas);

		uint32_t sizeMensaje = sizeof(uint32_t)
				* (2 + paquete->buffer->listaCoordenadas->elements_count)
				+ bufferLoco->sizeNombre;

		uint32_t desplazamiento = 0;
		void* buffer = malloc(sizeMensaje);
		memcpy(buffer + desplazamiento, &bufferLoco->sizeNombre,
				sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
		memcpy(buffer + desplazamiento, bufferLoco->pokemon,
				bufferLoco->sizeNombre);
		desplazamiento += bufferLoco->sizeNombre;

		uint32_t cantidadCoordenadas =
				paquete->buffer->listaCoordenadas->elements_count;
		printf("Al serializar, cantidadCoordenadas=%d\n", cantidadCoordenadas);
		printf("Serializando CantidadCoordenadas=%d\n", cantidadCoordenadas);
		memcpy(buffer + desplazamiento, &cantidadCoordenadas, sizeof(int));
		desplazamiento += sizeof(uint32_t);

		t_list*aux = list_duplicate(paquete->buffer->listaCoordenadas);
		//if(cantidadCoordenadas!=0){
		while (aux->head != NULL) {
			t_posicion *buffercito;
			buffercito = (t_posicion*) aux->head->data;
			printf("Serializando coordenada %d,%d\n", buffercito->x,
					buffercito->y);
			memcpy(buffer + desplazamiento, buffercito, sizeof(t_posicion));
			desplazamiento += sizeof(t_posicion);
			aux->head = aux->head->next;
			free(buffercito);
		}

//		t_particionLibre *particion = insertarEnCache(buffer, sizeMensaje);
//
//		t_administrativo* bufferAdmin = malloc(sizeof(t_administrativo));
//		bufferAdmin->idMensaje = paquete->buffer->idMensaje;
//		bufferAdmin->colaMensaje = MENSAJE_LOCALIZED_POKEMON;
//		bufferAdmin->suscriptoresEnviados = list_create();
//		bufferAdmin->suscriptoresRecibidos = list_create();
//		bufferAdmin->particion = particion->particion;
//		bufferAdmin->offsetInicioParticion = particion->offsetInicioParticion;
//		bufferAdmin->sizeParticion = particion->sizeParticion;
//		bufferAdmin->sizeMensajeGuardado = sizeMensaje;
//		bufferAdmin->flagLRU = particion->flagLRU;
//
//		dictionary_put(estructuraAdministrativa,
//				string_itoa(bufferAdmin->idMensaje), bufferAdmin);

		printf("ENCOLE EN LOCALIZED : %s . \n", bufferLoco->pokemon);
		break;
	}
	default: {
		printf("error de modulo, no se conoce quien envia paquetes\n");
	}
	}

	//free(paquete->buffer);
	//free(paquete);
	//}//esto es del while
//free(paquete);
//
//	free( bufferLoco);

	printf("estoy en el final de administrar mensajes\n");
	//sem_post(&bandejaCounter);
	pthread_exit(NULL);
	return NULL;
}

void* handler(void* socketConectado) {
	int socket = *(int*) socketConectado;
	pthread_mutex_t mutexRecibir;	//este semaforo no lo entiendo muy bien.
	pthread_mutex_init(&mutexRecibir, NULL);
//printf("Mi semaforo vale %d\n", mutexRecibir.__data.__count);

	t_paquete *bufferLoco;
	bufferLoco = malloc(sizeof(t_paquete));
	int flag = 1;
	printf("Esperando por un nuevo mensaje...\n");

	while (flag) {

		//pthread_mutex_lock(&recibir_mutex);

		pthread_mutex_lock(&mutexRecibir);//no entiendo bien para que esta este semaforo, bufferLoco no es global y el recv es bloqueante.
		bufferLoco = recibirMensaje(socket);

		if (bufferLoco != NULL) {

			if (bufferLoco->codigoOperacion >= 7
					&& bufferLoco->codigoOperacion <= 12) {	//esto es para capturar suscriptores.
				printf(" Soy suscriptor a la cola %d.\n",
						bufferLoco->codigoOperacion);
				pthread_mutex_lock(&bandejaSuscriptores_mutex);
				queue_push(bandejaSuscriptores, bufferLoco);
				pthread_mutex_unlock(&bandejaSuscriptores_mutex);//ACA HAY QUE HACER UN SEM COMO EL DE BRIAN
				pthread_mutex_unlock(&mutexRecibir);
			} else {
				printf(" Soy un mensaje .\n");
				printf("recibi un mensaje con el nombre : %s .\n",
						bufferLoco->buffer->nombrePokemon);
				//pthread_mutex_lock(&bandejaMensajes_mutex);
				pthread_mutex_lock(&asignarIdMensaje_mutex);
				bufferLoco->buffer->idMensaje = idMensajeUnico;
				idMensajeUnico++;
				pthread_mutex_unlock(&asignarIdMensaje_mutex);

				queue_push(bandeja, (void*) bufferLoco);
				sem_post(&bandejaCounter);
				//pthread_mutex_unlock(&bandejaMensajes_mutex);
				enviarIdMensaje(idMensajeUnico, socket);

				pthread_mutex_unlock(&mutexRecibir);
				printf("estoy despues del unlock de bandeja de mensajes\n");
			}
		} else {
			pthread_mutex_unlock(&mutexRecibir);
			flag = 0;
			printf(" me desconecte .\n");
			liberarConexion(socket);
		}

		printf("Esperando por un nuevo mensaje...\n");

	}

	free(bufferLoco);

//free_t_message(bufferLoco);
	printf(" Estoy finalizando el hilo...\n");
	pthread_exit(NULL);
	return NULL;
}
void* consumirMensajes() {

	while (1) {
		pthread_t hilito;
		sem_wait(&bandejaCounter);
		//pthread_mutex_lock(&bandejaMensajes_mutex);
		pthread_create(&hilito, NULL, administrarMensajes, NULL);
		//pthread_mutex_unlock(&bandejaMensajes_mutex);

	}

	return NULL;
}
void* escucharConexiones() {
	pthread_t threadId[MAX_CONEXIONES];

	int contadorConexiones = 0;
	int socketDelCliente[MAX_CONEXIONES];
	struct sockaddr direccionCliente;
	unsigned int tamanioDireccion = sizeof(direccionCliente);

	int servidor = initServer(brokerConf->ipBroker, brokerConf->puertoBroker);

	log_info(logger, "ESCHUCHANDO CONEXIONES");
	log_info(logger, "iiiiIIIII!!!");

	while (1) {

		socketDelCliente[contadorConexiones] = accept(servidor,
				(void*) &direccionCliente, &tamanioDireccion);

		if (socketDelCliente >= 0) {

			log_info(logEntrega, "Se ha aceptado una conexion: %i\n",
					socketDelCliente[contadorConexiones]);
			if ((pthread_create(&threadId[contadorConexiones], NULL, handler,
					(void*) &socketDelCliente[contadorConexiones])) < 0) {
				log_info(logger, "No se pudo crear el hilo");
				//return 1;
			} else {
				log_info(logger, "Handler asignado\n");
				tamanioDireccion = 0;

			}
		} else {
			log_info(logger, "Falló al aceptar conexión");
		}

		contadorConexiones++;

	}
	return NULL;
}
void inicializarSemaforos() {
	pthread_mutex_init(&bandejaSuscriptores_mutex, NULL);
	pthread_mutex_init(&bandejaMensajes_mutex, NULL);
	pthread_mutex_init(&recibir_mutex, NULL);
	pthread_mutex_init(&asignarIdMensaje_mutex, NULL);
	sem_init(&bandejaCounter, 1, 0);
}
