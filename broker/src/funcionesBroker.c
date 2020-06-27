#include "broker.h"

void inicializarLogger() {
	//Crea el logger
	logger = log_create("BROKER.log", "BROKER", 1, LOG_LEVEL_TRACE);
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
	brokerConf->algoritmoParticionLibre = string_duplicate(
			config_get_string_value(BROKERTConfig, "ALGORITMO_PARTICION_LIBRE"));
	log_info(logger, "ALGORITMO_PARTICION_LIBRE=%s", brokerConf->algoritmoParticionLibre);
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

//	miMemoria = malloc(brokerConf->tamanoMemoria);
//	offset = 0;
//	iniMemoria = &miMemoria;
//	numeroParticion = 0;
	log_info(logger, "Iniciar CACHE");

	instanteCache = 0;		// para guardar el instante en que ocurre cada movimiento de la cache.
	log_info(logger, "instante = %d", instanteCache);

	cache = malloc(brokerConf->tamanoMemoria);
	log_info(logger, "Memoria del CACHE %x-%x Largo %d  FAKE", cache,
							cache + brokerConf->tamanoMemoria-1, (brokerConf->tamanoMemoria)*sizeof(cache));
	auxTra = (int) cache;
	sizeTra = auxTra + brokerConf->tamanoMemoria-1;
	log_info(logger, "Memoria del CACHE %x-%x Largo %d  TRUE", auxTra, sizeTra, sizeTra-auxTra+1);

// Iniciamos los valores de la cache vacia.

	log_info(logger, "Size of: Nodo=%d", sizeof(struct nodoListaCache));

	particionActual = (t_nodoListaCache) malloc(sizeof(struct nodoListaCache));

	particionActual->inicio = 0;
	particionActual->fin = brokerConf->tamanoMemoria-1;
	particionActual->largo = brokerConf->tamanoMemoria;
	particionActual->estado = 0;
	particionActual->instante = instanteCache;
	particionActual->id = 0;
	particionActual->sgte = NULL;
	particionActual->ant = NULL;
	particionActual->mayor = NULL;
	particionActual->menor = NULL;

	instanteCache++; // instante de iniciado de la CACHE es siempre 0.

	particionFirst = particionActual;
	particionLast = particionActual;
	particionBig = particionActual;
	particionSmall = particionActual;

	log_info(logger, "Part.1 %x-%x  %d", particionFirst->inicio, particionFirst->fin, particionFirst->estado);
	log_info(logger, "Part.1 %x-%x  %d", particionLast->inicio, particionLast->fin, particionLast->estado);
	log_info(logger, "Part.1 %x-%x  %d", particionBig->inicio, particionBig->fin, particionBig->estado);
	log_info(logger, "Part.1 %x-%x  %d\n", particionSmall->inicio, particionSmall->fin, particionSmall->estado);

	nodos = mostrarCache( particionFirst, 0);
	nodos = mostrarCache( particionLast, 1);
	nodos = mostrarCache( particionBig, 2);
	nodos = mostrarCache( particionSmall, 3);

// pa probar
	log_info(logger, "Part.1 %x-%x  %d", particionActual->inicio, particionActual->fin, particionActual->estado);

	praLibre = encontrarLibre(4,0);
	log_info(logger, "Part.1 %x-%x  %d", praLibre->inicio, praLibre->fin, praLibre->estado);

	praLibre = encontrarLibre(45,0);
	log_info(logger, "Part.1 %x-%x  %d", praLibre->inicio, praLibre->fin, praLibre->estado);

	praLibre = encontrarLibre(60,0);
	log_info(logger, "Part.1 %x-%x  %d", praLibre->inicio, praLibre->fin, praLibre->estado);


}

t_nodoListaCache encontrarLibre(int size, int orden){
//	int k = 0;
	int pos = 0;
//	t_nodoListaCache aux = malloc(sizeof(struct nodoListaCache)); //, prox = new nodoL;
		switch(orden) {
		    case 0 : particionActual = particionFirst; break;
			case 1 : particionActual = particionLast;  break;
			case 2 : particionActual = particionBig;   break;
			case 3 : particionActual = particionSmall; break;
			default :particionActual = particionFirst;  // sgte por defecto.
		}
	log_info(logger, "Buscando  [%d] en la CACHE",size);

	while(particionActual != NULL){
		pos++;
		if((particionActual->estado == 0) && ( particionActual->largo>= size)){
			log_info(logger,"Encontre [%d] de [%d]b posicion[%d]",
					particionActual->inicio, particionActual->largo, pos);
//			k = 1 ;
			return particionActual;
		}
		switch(orden) {
		    case 0 : particionActual = particionActual->sgte; break;
		    case 1 : particionActual = particionActual->ant;  break;
		    case 2 : particionActual = particionActual->mayor;break;
		    case 3 : particionActual = particionActual->menor;break;
		    default :particionActual = particionActual->sgte;  // sgte por defecto.
	}}
	log_info(logger, "No hay particion libre donde quepa [%d]", size);
return NULL;
}

int insertarPartition(void* mensaje, int size, int id, int orden){
//	int k = 0;
	int pos = 0;
	t_nodoListaCache particionActual = malloc(sizeof(struct nodoListaCache)); //, prox = new nodoL;
	switch(orden) {
	    case 0 : particionActual = particionFirst; break;
		case 1 : particionActual = particionLast;  break;
		case 2 : particionActual = particionBig;   break;
		case 3 : particionActual = particionSmall; break;
		default :particionActual = particionFirst;  // sgte por defecto.
	}
	log_info(logger, "Buscando  [%d] en la CACHE",size);

	while(particionActual != NULL){
		pos++;
		if((particionActual->estado == 0) && ( particionActual->largo>= size)){
			log_info(logger,"Encontre [%d] de [%d]b posicion[%d]", particionActual->inicio, particionActual->largo, pos);
//			k = 1 ;
			return 0;
		}
		switch(orden) {
		    case 0 : particionActual = particionActual->sgte; break;
		    case 1 : particionActual = particionActual->ant;  break;
		    case 2 : particionActual = particionActual->mayor;break;
		    case 3 : particionActual = particionActual->menor;break;
		    default :particionActual = particionActual->sgte;  // sgte por defecto.
	}}
	log_info(logger, "No hay particion libre donde quepa [%d]", size);
return 1;
}


//	if (brokerConf->algoritmoParticionLibre=="BF"){
//		log_info(logger, "Algoritmo de particion libre Best fit NO IMPLEMENTADO");
//	}
//	if (brokerConf->algoritmoParticionLibre!="FF"){
//		log_info(logger, "Algoritmo de particion libre Desconocido");
//	}

//
//	listarCacheFirst(particionFirst);
//	t_nodoCache *aux = particionFirst;
//	int particion = 0;
//		Do {
//			printf("Dump de menor a mayor\n");
//			printf("Particion %d"), particion;
//			printf(" %h "); cache+t_nodoCache->first
//			printf("la memoria arranca en la direccion : %d .\n",int insertarPartition(void* mensaje, int size, int id, int orden) (int) iniMemoria);
//			 printf("la memoria finaliza en la direccion : %d .\n", (int) finMemoria);
//			 //	print		//			printf("Dump de menor a mayor\n");
	//			printf("Particion %d"), particion;
	//			printf(" %h "); cache+t_nodoCache->first
	//			printf("la memoria arranca en la direccion : %d .\n", (int) iniMemoria);
	//			 printf("la memoria finaliza en la direccion : %d .\n", (int) finMemoria);
	//			 //	printf("memoria total : %d .\n", memoriaTotal);
	//			 particion++;"\n\n    buscando  [%d] en la lista \n\n",n
	//
//
// f("memoria total : %d .\n", memoriaTotal);
//			 particion++;
//	}while (aux->mayor);


// int insertarPartition(void* mensaje, int size, int id, int orden)
//	return;


//void insertarEnCache(void* mensaje, int size, int id) {

	// Implementamos Algoritmo Particion Libre FF
	// La lista de particiones siempre tendra, al menos, un Nodo
	//

//	int tamanoABuscar = brokerConf->tamanoMinimoParticion;
///	if (tamanoABuscar<size) tamanoABuscar=size;




//	}

//}

int mostrarCache(t_nodoListaCache nodo, int orden) {
	int i=0;
	while(nodo!=NULL) {
		i++;
		log_info(logger,"Part:%d [%x-%x] <=> %d",i, nodo->inicio, nodo->fin, nodo->estado);

		switch(orden) {
		    case 0 : nodo = nodo->sgte; break;
		    case 1 : nodo = nodo->ant;  break;
		    case 2 : nodo = nodo->mayor;break;
		    case 3 : nodo = nodo->menor;break;
		    default :nodo = nodo->sgte;  // sgte por defecto.
		}
	}

	return i;
}
//	memcpy(iniMemoria + offset, mensaje, size);
//	offset += size;
//	numeroParticion++;

//}
/*
 void* manejarMemoria() {

 t_paquete* paquete = malloc(sizeof(t_paquete));

 //pthread_mutex_lock(bandeja);
 //paquete = queue_peek(bandeja);

 int *iniMemoria = (int*) &miMemoria;
 int *finMemoria = iniMemoria + brokerConf->tamanoMemoria;//aritmetica de punteros para calcular el final de la memoria
 int memoriaTotal = finMemoria - iniMemoria;
 //int i = 0;
 //int estado = 0;
 int cantidadBloques = (brokerConf->tamanoMemoria
 / brokerConf->tamanoMinimoParticion) - 1;
 offSet = 0;
 int sizesTotalMensajes = 0;
 int memoriaDisponible = memoriaTotal - sizesTotalMensajes;//esto es cualquiera...
 //t_bitarray* bitMap = bitarray_create(miMemoria, cantidadBloques);

 //int tabla[][5];

 int* punteroBase = iniMemoria;
 //mem_hexdump(miMemoria, paquete->buffer->largoNombre);

 if (memoriaDisponible > paquete->buffer->largoNombre) {
 memcpy(miMemoria + offSet, paquete, paquete->buffer->largoNombre);//aca va el tamaño del mensaje y el mensaje
 offSet += paquete->buffer->largoNombre;
 punteroBase += offSet;
 //		for (i; i < paquete->buffer->largoNombre; i++) {
 //			bitarray_set_bit(bitMap, i);
 //		}
 }
 printf("la memoria arranca en la direccion : %d .\n", (int) iniMemoria);
 printf("la memoria finaliza en la direccion : %d .\n", (int) finMemoria);
 //	printf("memoria total : %d .\n", memoriaTotal);

 return NULL;
 }
 */
void* administrarMensajes() {

	t_paquete* paquete;

	while (1) {
		paquete = malloc(sizeof(t_paquete));
		printf("Bloqueado en el mutex\n");
		sem_wait(&bandejaCounter);
		pthread_mutex_lock(&bandejaMensajes_mutex);
		paquete = (t_paquete*) queue_pop(bandeja);
		pthread_mutex_unlock(&bandejaMensajes_mutex);

		printf(" Mi opCode es : %d,\n ", paquete->codigoOperacion);

		switch (paquete->codigoOperacion) {

		case SUSCRIBIRSE_NEW_POKEMON: {
			list_add(NEW_POKEMON->lista, (void*) paquete->codigoOperacion);
			printf("meti algo en la lista : ");

			break;
		}
		case SUSCRIBIRSE_APPEARED_POKEMON: {
			list_add(APPEARED_POKEMON->lista, (void*) paquete->codigoOperacion);

			break;
		}

		case SUSCRIBIRSE_CATCH_POKEMON: {
			list_add(CATCH_POKEMON->lista, (void*) paquete->codigoOperacion);

			break;
		}

		case SUSCRIBIRSE_CAUGHT_POKEMON: {
			list_add(CAUGHT_POKEMON->lista, (void*) paquete->codigoOperacion);

			break;
		}

		case SUSCRIBIRSE_GET_POKEMON: {
			list_add(GET_POKEMON->lista, (void*) paquete->codigoOperacion);

			break;
		}

		case SUSCRIBIRSE_LOCALIZED_POKEMON: {
			list_add(LOCALIZED_POKEMON->lista,
					(void*) paquete->codigoOperacion);

			break;
		}

		case MENSAJE_NEW_POKEMON: {

			t_newPokemon* bufferLoco = malloc(sizeof(t_newPokemon));
			bufferLoco->sizeNombre = paquete->buffer->largoNombre;
			bufferLoco->pokemon = paquete->buffer->nombrePokemon;
			bufferLoco->cantidadPokemons = paquete->buffer->cantidadPokemons;
			bufferLoco->posX = paquete->buffer->posX;
			bufferLoco->posY = paquete->buffer->posY;

			t_administrativo* bufferAdmin = malloc(sizeof(t_administrativo));
			bufferAdmin->idMensaje = paquete->buffer->idMensaje;
			bufferAdmin->sizeMensajeGuardado = sizeof(uint32_t) * 4
					+ bufferLoco->sizeNombre;
			bufferAdmin->numeroParticion = 0;//esto sacarlo del estado actual de la memoria
			bufferAdmin->sizeParticion = 0;	//esto sacarlo del estado actual de la memoria
			bufferAdmin->suscriptores = list_duplicate(NEW_POKEMON->lista);

			dictionary_put(estructuraAdministrativa,
					string_itoa(bufferAdmin->idMensaje), bufferAdmin);

//			insertarEnCache(bufferLoco, bufferAdmin->sizeMensajeGuardado);
			//queue_push(NEW_POKEMON->cola, bufferLoco);//esto habria que copiarlo en la cache
			printf("ENCOLE EN NEW : %s . \n", bufferLoco->pokemon);

			//pthread_exit(NULL);

			break;
		}
		case MENSAJE_APPEARED_POKEMON: {

			t_appearedPokemon* bufferLoco = malloc(sizeof(t_appearedPokemon));
			bufferLoco->sizeNombre = paquete->buffer->largoNombre;
			bufferLoco->pokemon = paquete->buffer->nombrePokemon;
			bufferLoco->posX = paquete->buffer->posX;
			bufferLoco->posY = paquete->buffer->posY;

			t_administrativo* bufferAdmin = malloc(sizeof(t_administrativo));
			bufferAdmin->idMensaje = paquete->buffer->idMensaje;
			bufferAdmin->sizeMensajeGuardado = sizeof(uint32_t) * 3
					+ bufferLoco->sizeNombre;
			bufferAdmin->numeroParticion = 0;//esto sacarlo del estado actual de la memoria
			bufferAdmin->sizeParticion = 0;	//esto sacarlo del estado actual de la memoria
			bufferAdmin->suscriptores = list_duplicate(APPEARED_POKEMON->lista);

			dictionary_put(estructuraAdministrativa,
					string_itoa(bufferAdmin->idMensaje), bufferAdmin);

//			insertarEnCache(bufferLoco, bufferAdmin->sizeMensajeGuardado);

			//queue_push(APPEARED_POKEMON->cola, bufferLoco);
			printf("ENCOLE EN APPEARED : %s . \n", bufferLoco->pokemon);
			break;
		}

		case MENSAJE_CATCH_POKEMON: {

			t_catchPokemon* bufferLoco = malloc(sizeof(t_catchPokemon));
			bufferLoco->sizeNombre = paquete->buffer->largoNombre;
			bufferLoco->pokemon = paquete->buffer->nombrePokemon;
			bufferLoco->posX = paquete->buffer->posX;
			bufferLoco->posY = paquete->buffer->posY;

			t_administrativo* bufferAdmin = malloc(sizeof(t_administrativo));
			bufferAdmin->idMensaje = paquete->buffer->idMensaje;
			bufferAdmin->sizeMensajeGuardado = sizeof(uint32_t) * 3
					+ bufferLoco->sizeNombre;
			bufferAdmin->numeroParticion = 0;//esto sacarlo del estado actual de la memoria
			bufferAdmin->sizeParticion = 0;	//esto sacarlo del estado actual de la memoria
			bufferAdmin->suscriptores = list_duplicate(CATCH_POKEMON->lista);

			dictionary_put(estructuraAdministrativa,
					string_itoa(bufferAdmin->idMensaje), bufferAdmin);
//			insertarEnCache(bufferLoco, bufferAdmin->sizeMensajeGuardado);

			//queue_push(CATCH_POKEMON->cola, bufferLoco);
			printf("ENCOLE EN CATCH : %s . \n", bufferLoco->pokemon);
			break;
		}

		case MENSAJE_CAUGHT_POKEMON: {

			t_caughtPokemon* bufferLoco = malloc(sizeof(t_caughtPokemon));
			bufferLoco->booleano = paquete->buffer->boolean;

			t_administrativo* bufferAdmin = malloc(sizeof(t_administrativo));
			bufferAdmin->idMensaje = paquete->buffer->idMensaje;
			bufferAdmin->sizeMensajeGuardado = sizeof(uint32_t);
			bufferAdmin->numeroParticion = 0;//esto sacarlo del estado actual de la memoria
			bufferAdmin->sizeParticion = 0;	//esto sacarlo del estado actual de la memoria
			bufferAdmin->suscriptores = list_duplicate(CAUGHT_POKEMON->lista);

			dictionary_put(estructuraAdministrativa,
					string_itoa(bufferAdmin->idMensaje), bufferAdmin);
//			insertarEnCache(bufferLoco, bufferAdmin->sizeMensajeGuardado);

			//queue_push(CAUGHT_POKEMON->cola, bufferLoco);
			printf("ENCOLE EN CAUGHT : %d . \n", bufferLoco->booleano);
			break;
		}

		case MENSAJE_GET_POKEMON: {
			t_catchPokemon* bufferLoco = malloc(sizeof(t_catchPokemon));
			bufferLoco->sizeNombre = paquete->buffer->largoNombre;
			bufferLoco->pokemon = paquete->buffer->nombrePokemon;

			t_administrativo* bufferAdmin = malloc(sizeof(t_administrativo));
			bufferAdmin->idMensaje = paquete->buffer->idMensaje;
			bufferAdmin->sizeMensajeGuardado = sizeof(uint32_t)
					+ bufferLoco->sizeNombre;
			bufferAdmin->numeroParticion = 0;//esto sacarlo del estado actual de la memoria
			bufferAdmin->sizeParticion = 0;	//esto sacarlo del estado actual de la memoria
			bufferAdmin->suscriptores = list_duplicate(GET_POKEMON->lista);

			dictionary_put(estructuraAdministrativa,
					string_itoa(bufferAdmin->idMensaje), bufferAdmin);
//			insertarEnCache(bufferLoco, bufferAdmin->sizeMensajeGuardado);

			//queue_push(GET_POKEMON->cola, bufferLoco);
			printf("ENCOLE EN GET : %s . \n", bufferLoco->pokemon);
			break;
		}

		case MENSAJE_LOCALIZED_POKEMON: {

			t_localizedPokemon* bufferLoco = malloc(sizeof(t_localizedPokemon));

			bufferLoco->sizeNombre = paquete->buffer->largoNombre;
			bufferLoco->pokemon = paquete->buffer->nombrePokemon;
			bufferLoco->cantidadDePosiciones =
					paquete->buffer->listaCoordenadas->elements_count;
			bufferLoco->posiciones = list_create();
			bufferLoco->posiciones = list_duplicate(
					paquete->buffer->listaCoordenadas);

			t_administrativo* bufferAdmin = malloc(sizeof(t_administrativo));
			bufferAdmin->idMensaje = paquete->buffer->idMensaje;
			bufferAdmin->sizeMensajeGuardado = sizeof(uint32_t)
					* (2 + paquete->buffer->listaCoordenadas->elements_count)
					+ bufferLoco->sizeNombre;
			bufferAdmin->numeroParticion = 0;//esto sacarlo del estado actual de la memoria
			bufferAdmin->sizeParticion = 0;	//esto sacarlo del estado actual de la memoria
			bufferAdmin->suscriptores = list_duplicate(
					LOCALIZED_POKEMON->lista);

			dictionary_put(estructuraAdministrativa,
					string_itoa(bufferAdmin->idMensaje), bufferAdmin);
//			insertarEnCache(bufferLoco, bufferAdmin->sizeMensajeGuardado);

			//queue_push(LOCALIZED_POKEMON->cola, bufferLoco);
			printf("ENCOLE EN LOCALIZED : %s . \n", bufferLoco->pokemon);
			break;
		}
		default: {
			printf("error de modulo, no se conoce quien envia paquetes\n");
		}
		}

		//free(paquete->buffer);
		//free(paquete);
	}
	//free(paquete);
//"\n\n    buscando  [%d] en la lista \n\n",n
//	free( bufferLoco);

	printf("estoy en el final de administrar mensajes\n");
	return NULL;
}

void* handler(void* socketConectado) {
	int socket = *(int*) socketConectado;
	pthread_mutex_t mutexRecibir;
	pthread_mutex_init(&mutexRecibir, NULL);
	//printf("Mi semaforo vale %d\n", mutexRecibir.__data.__count);

	t_paquete *bufferLoco;
	bufferLoco = malloc(sizeof(t_paquete));
	int flag = 1;
	while (flag) {

		printf("Esperando por un nuevo mensaje...\n");

		//pthread_mutex_lock(&recibir_mutex);
		pthread_mutex_lock(&mutexRecibir);
		bufferLoco = recibirMensaje(socket);

		if (bufferLoco != NULL) {
			printf("%s\n", bufferLoco->buffer->nombrePokemon);
			pthread_mutex_lock(&bandejaMensajes_mutex);
			bufferLoco->buffer->idMensaje = idMensajeUnico;
			queue_push(bandeja, (void*) bufferLoco);
			enviarIdMensaje(idMensajeUnico, socket);/////falta un semaforo porque esto es global
			idMensajeUnico++;
			sem_post(&bandejaCounter);
			pthread_mutex_unlock(&bandejaMensajes_mutex);
			pthread_mutex_unlock(&mutexRecibir);
			printf("estoy despues del unlock de bandeja de mensajes\n");
		} else {
			pthread_mutex_unlock(&mutexRecibir);
			flag = 0;
		}

		//contadorDeMensajes++;	// hacer un mutex

		//free(pasaManos);
		log_info(logger, "Estoy dentro del handler loco\n");
//	free(bufferLoco->buffer);
//	free(bufferLoco);

	}

	//pthread_detach(socket);	//ver si es esto lo que finaliza el hilo y libera los recursos;
//hacer un free completo de bufferLoco

//free(bufferLoco);

//free_t_message(bufferLoco);

	//pthread_exit(NULL);
	return NULL;
}

