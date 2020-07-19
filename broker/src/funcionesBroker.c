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
	brokerConf->algoritmoParticionLibre = string_duplicate(
			config_get_string_value(BROKERTConfig,
					"ALGORITMO_PARTICION_LIBRE"));
	log_info(logger, "ALGORITMO_PARTICION_LIBRE=%s",
			brokerConf->algoritmoParticionLibre);
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

	NEW_POKEMON->cola = list_create();
	NEW_POKEMON->lista = list_create();
	APPEARED_POKEMON->cola = list_create();
	APPEARED_POKEMON->lista = list_create();
	CATCH_POKEMON->cola = list_create();
	CATCH_POKEMON->lista = list_create();
	CAUGHT_POKEMON->cola = list_create();
	CAUGHT_POKEMON->lista = list_create();
	GET_POKEMON->cola = list_create();
	GET_POKEMON->lista = list_create();
	LOCALIZED_POKEMON->cola = list_create();
	LOCALIZED_POKEMON->lista = list_create();

	return;
}

void destruirColasBroker() {
	list_destroy(NEW_POKEMON->cola);
	list_destroy(NEW_POKEMON->lista);
	list_destroy(APPEARED_POKEMON->cola);
	list_destroy(APPEARED_POKEMON->lista);
	list_destroy(CATCH_POKEMON->cola);
	list_destroy(CATCH_POKEMON->lista);
	list_destroy(CAUGHT_POKEMON->cola);
	list_destroy(CAUGHT_POKEMON->lista);
	list_destroy(GET_POKEMON->cola);
	list_destroy(GET_POKEMON->lista);
	list_destroy(LOCALIZED_POKEMON->cola);
	list_destroy(LOCALIZED_POKEMON->lista);

	free(NEW_POKEMON);
	free(APPEARED_POKEMON);
	free(CATCH_POKEMON);
	free(CAUGHT_POKEMON);
	free(GET_POKEMON);
	free(LOCALIZED_POKEMON);

}

//llamarla en la funcion main
void iniciarCache() {

// uso debugCache para mostrar cosas de la cache en pantalla mientras desarrollo.
// el que quiera que no le aparezcan, que la ponga en 0
//
	debugCache = 0;  // 0 = nottrace <-> !0 = trace
	if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {
		partPD = 0;
		partBS = -1;
	} else {
		partPD = -1;
		partBS = 0;
	};
	if (strcmp(brokerConf->algoritmoReemplazo, "LRU") == 0) {
		reemFIFO = 0;
		reemLRU = -1;
	} else {
		reemFIFO = -1;
		reemLRU = 0;
	};
	if (strcmp(brokerConf->algoritmoParticionLibre, "BF") == 0) {
		seleFF = 0;
		seleBF = -1;
	} else {
		seleFF = -1;
		seleBF = 0;
	};
	instanteCache = 0; // para guardar el instante en que ocurre cada movimiento de la cache.
	cache = malloc(brokerConf->tamanoMemoria);
	auxTra = (int) cache;
	sizeTra = auxTra + brokerConf->tamanoMemoria - 1;

	if (debugCache) {
		log_info(logger, "");
		log_info(logger,
				"(iC) debugCache is TRUE (set to 0 for not cacheTrace.)");
		log_info(logger,
				"(iC) Tamano Cache=[%d] - Particion Minima=[%d] - Frecuencia Compactacion=[%d]",
				brokerConf->tamanoMemoria, brokerConf->tamanoMinimoParticion,
				brokerConf->frecuenciaCompactacion);
		log_info(logger,
				"(iC) esquemaCache PD=[%d] BS=[%d] - algReemplazo FIFO=[%d] LRU=[%d] - selecPartLibre FF=[%d] BF=[%d]",
				partPD, partBS, reemFIFO, reemLRU, seleFF, seleBF);
		log_info(logger, "(iC) Iniciar CACHE");
		log_info(logger, "(iC) instante = %d", instanteCache);
		log_info(logger, "(iC) Memoria de la CACHE %x-%x Largo %d  FAKE", cache,
				cache + brokerConf->tamanoMemoria - 1,
				(brokerConf->tamanoMemoria) * sizeof(cache));
		log_info(logger, "(iC) Memoria de la CACHE %x-%x Largo %d  TRUE",
				auxTra, sizeTra, sizeTra - auxTra + 1);
		log_info(logger, "(iC) Size of: Nodo = %d bytes",
				sizeof(struct nodoListaCache));
	}

	// Iniciamos los valores de la cache vacia.
	partActual = (t_nodoListaCache) malloc(sizeof(struct nodoListaCache));

	partActual->inicio = 0;
	partActual->fin = brokerConf->tamanoMemoria - 1;
	partActual->largo = brokerConf->tamanoMemoria;
	partActual->estado = 0;
	partActual->instante = instanteCache;
	partActual->id = 0;
	partActual->sgte = NULL;
	partActual->ant = NULL;
	partActual->mayor = NULL;
	partActual->menor = NULL;

	instanteCache++; // instante de iniciado de la CACHE (es siempre 0).

	partFirst = partActual;
	partLast = partActual;
	partBig = partActual;
	partSmall = partActual;

	if (debugCache) {
		log_info(logger, "(iC) Pruebas Cache - Mostrar Cache Inicial");
//	log_info(logger,"(iC) Mostrar Cache Inicial en distinto orden usando el logger");
		log_info(logger,
				"(iC) Particion:%d: %3X-%3X  [L]%4d Size: %4db orden:%d", 0,
				partFirst->inicio, partFirst->fin, partFirst->estado,
				partFirst->largo, 0);
		log_info(logger,
				"(iC) Particion:%d: %3X-%3X  [L]%4d Size: %4db orden:%d", 0,
				partLast->inicio, partLast->fin, partLast->estado,
				partLast->largo, 1);
		log_info(logger,
				"(iC) Particion:%d: %3X-%3X  [L]%4d Size: %4db orden:%d", 0,
				partBig->inicio, partBig->fin, partBig->estado, partBig->largo,
				2);
		log_info(logger,
				"(iC) Particion:%d: %3X-%3X  [L]%4d Size: %4db orden:%d\n", 0,
				partSmall->inicio, partSmall->fin, partSmall->estado,
				partSmall->largo, 3);

//	log_info(logger,"(iC) Mostrar Cache Inicial en distinto orden usando mostrarCache");
		mostrarCache(partFirst, 0);
		mostrarCache(partLast, 1);
		mostrarCache(partBig, 2);
		mostrarCache(partSmall, 3);
		log_info(logger, " ");

// pa probar si funca todo bien
//		log_info(logger, "Cache Inicial");
//		mostrarCache(partActual, 0);

		log_info(logger, "(iC) Prueba de insercion con fragmentacion");

		praLibre = encontrarPartLibre(19, 0);
		if (praLibre != NULL)
			insertarEnParticion(praLibre, "1234567890123456789", 19,
					tamanoABuscar, 121);
		praLibre = encontrarPartLibre(20, 0);
		if (praLibre != NULL)
			insertarEnParticion(praLibre, "12345678901234567890", 20,
					tamanoABuscar, 122);
		praLibre = encontrarPartLibre(16, 0);
		if (praLibre != NULL)
			insertarEnParticion(praLibre, "1234567890123456", 16, tamanoABuscar,
					123);
		praLibre = encontrarPartLibre(16, 0);
		if (praLibre != NULL)
			insertarEnParticion(praLibre, "1234567890123456", 16, tamanoABuscar,
					124);
		praLibre = encontrarPartLibre(21, 0);
		if (praLibre != NULL)
			insertarEnParticion(praLibre, "123456789012345678901", 21,
					tamanoABuscar, 125);
		praLibre = encontrarPartLibre(22, 0);
		if (praLibre != NULL)
			insertarEnParticion(praLibre, "1234567890123456789012", 22,
					tamanoABuscar, 126);
		praLibre = encontrarPartLibre(16, 0);
		if (praLibre != NULL)
			insertarEnParticion(praLibre, "1234567890123456", 16, tamanoABuscar,
					127);
		praLibre = encontrarPartLibre(16, 0);
		if (praLibre != NULL)
			insertarEnParticion(praLibre, "1234567890123456", 16, tamanoABuscar,
					128);
		praLibre = encontrarPartLibre(9, 0);
		if (praLibre != NULL)
			insertarEnParticion(praLibre, "123456789", 9, tamanoABuscar, 129);

//	insertarEnParticion(praLibre, "123456789012345678901234567890123456789012345", 45, tamanoABuscar, 234);

//		log_info(logger, "Buscar libre para insertar");
//		praLibre = encontrarPartLibre(4, 0);
//	mostrarPart(praLibre);
//	insertarEnParticion(praLibre, "ABCD", 4, tamanoABuscar, 233);
//	nodos = mostrarCache( partFirst, 0);

//	log_info(logger,"Buscar otra libre para insertar");
//	praLibre = encontrarPartLibre(45,0);
//	mostrarlog_info(logger, "Part.1 %x-%x  %d", praLibre->inicio, praLibre->fin, praLibre->estado);
//	insertarEnParticion(praLibre, "123456789012345678901234567890123456789012345", 45, tamanoABuscar, 234);
//	nodos = insertarPartition("123456789012345678901234567890123456789012345", 45, 234, 0);
//	nodos = mostrarCache( partFirst, 0);

//	praLibre = encontrarPartLibre(60,0);
//	log_info(logger, "Part.1 %x-%x  %d", praLibre->inicio, praLibre->fin, praLibre->estado);
//	insertarEnParticion(praLibre, "12345678901234567890123456789012345678900123456789001234567890", 60, tamanoABuscar, 235);
//	nodos = insertarPartition("12345678901234567890123456789012345678900123456789001234567890", 60, 235, 0);
//	nodos = mostrarCache( partFirst, 0);
		log_info(logger, "(iC) Fin debugCache");
	} // fin debugCache

}

t_nodoListaCache encontrarPartLibre(int size, int orden) {
	int pos = 0;
	tamanoABuscar = brokerConf->tamanoMinimoParticion;

	switch (orden) {
	case 0:
		partActual = partFirst;
		break;
	case 1:
		partActual = partLast;
		break;
	case 2:
		partActual = partBig;
		break;
	case 3:
		partActual = partSmall;
		break;
	default:
		partActual = partFirst;
	}  // sgte por defecto.

	if (size > tamanoABuscar)
		tamanoABuscar = size;
	if (debugCache)
		log_info(logger, "(ePL)-Busco size [%d] requerido [%d] en la CACHE",
				size, tamanoABuscar);

	while (partActual != NULL) {
		pos++;
		if ((partActual->estado == 0) && (partActual->largo >= tamanoABuscar)) {
			if (debugCache) {
				log_info(logger, "(ePL)-Encontre objetivo");
				mostrarPart(partActual);
			}
			return partActual;
		}
		switch (orden) {
		case 0:
			partActual = partActual->sgte;
			break;
		case 1:
			partActual = partActual->ant;
			break;
		case 2:
			partActual = partActual->mayor;
			break;
		case 3:
			partActual = partActual->menor;
			break;
		default:
			partActual = partActual->sgte;
		} // sgte por defecto.
		if (debugCache)
			printf(".");
	}
	if (debugCache)
		log_info(logger, "(ePL) No hay partLibre donde quepa [%d]", size);
	return NULL;
}

void insertarEnParticion(t_nodoListaCache nodo, void* mensaje, int size,
		int alojamiento, int id) {
	nodoJusto = 0;
	if (nodo->largo == alojamiento)
		nodoJusto = -1;

	if (!nodoJusto) {
		t_nodoListaCache partNueva = malloc(sizeof(struct nodoListaCache)); //, alojar la nueva part
		partNueva->inicio = nodo->inicio;
		partNueva->fin = nodo->inicio + alojamiento - 1;
		partNueva->largo = alojamiento;
		partNueva->estado = size;
		partNueva->id = id;
		partNueva->instante = instanteCache;
		partNueva->sgte = nodo;
		partNueva->ant = nodo->ant;
		if (partNueva->ant)
			nodo->ant->sgte = partNueva;
//  	partNueva->mayor = ???? ;
//  	partNueva->menor = ???? ;

		memcpy(auxTra + partNueva->inicio, mensaje, size);
		if (debugCache)
			mostrarPart(partNueva);

		nodo->inicio = nodo->inicio + alojamiento;
		nodo->largo = nodo->largo - alojamiento;
		nodo->instante = instanteCache;
		nodo->ant = partNueva;
		if (debugCache)
			mostrarPart(nodo);
		if (partFirst == nodo)
			partFirst = partNueva;
//		partLast nunca podria cambiar por una insertarEnParticion libre
	}

	if (nodoJusto) {
		nodo->estado = size;
		nodo->id = id;
		nodo->instante = instanteCache;
		memcpy(auxTra + nodo->inicio, mensaje, size);
	}
	if (debugCache) {
		mostrarCache(partFirst, 0);
		mostrarCache(partLast, 1);
//		mostrarCache( partBig, 2);
//		mostrarCache( partSmall, 3);
	}
	instanteCache++;
}
void insertarJusto(t_nodoListaCache nodo, void* mensaje, int size,
		int alojamiento, int id) {
//	t_nodoListaCache partNueva = malloc(sizeof(struct nodoListaCache)); //, alojar la nueva part
//    partNueva->inicio   = nodo->inicio;
//    partNueva->fin      = nodo->inicio+alojamiento-1;
//    partNueva->largo    = alojamiento;
	nodo->estado = size;
	nodo->id = id;
	nodo->instante = instanteCache;
//    partNueva->sgte     = nodo;
//    partNueva->ant      = nodo->ant;
//    if (partNueva->ant != NULL) partNueva->ant->sgte=partNueva;
//  partNueva->mayor = ???? ;
//  partNueva->menor = ???? ;

	memcpy(auxTra + nodo->inicio, mensaje, size);

	if (debugCache)
		mostrarPart(nodo);

	instanteCache++;

//   nodo->inicio        = nodo->inicio+alojamiento;
//   nodo->largo         = nodo->largo-alojamiento;
//   nodo->instante      = instanteCache;
//   nodo->ant           =partNueva;
//	if(debugCache) mostrarPart(nodo);
// 	if(partFirst == nodo) partFirst = partNueva;
//	partLast nunca podria cambiar por una insertarEnParticion libre

	if (debugCache)
		mostrarCache(partFirst, 0);
}

// a esta funcion hay que agregarle que busque la Cola a la que pertenece usando el ID
void mostrarPart(t_nodoListaCache nodo) {

	if (nodo->estado != 0) {
		log_info(logger,
				"(mP) Particion:*: %3X-%3X  [X]%4d Size: %4db LRU:<%d> Cola:<> ID:<%d>",
				nodo->inicio, nodo->fin, nodo->estado, nodo->largo,
				nodo->instante, nodo->id);
	} else {
		log_info(logger, "(mP) Particion:*: %3X-%3X  [L]%4d Size: %4db",
				nodo->inicio, nodo->fin, nodo->estado, nodo->largo);
	}
	return;
}
void mostrarCache(t_nodoListaCache nodo, int orden) {
	int part = 0;
	while (nodo != NULL) {
		part++;
		if (nodo->estado != 0) {
			log_info(logger,
					"(mc) Particion:%d: %3X-%3X  [X]%4d Size: %4db LRU:<%d> Cola:<> ID:<%d> orden:%d",
					part, nodo->inicio, nodo->fin, nodo->estado, nodo->largo,
					nodo->instante, nodo->id, orden);
		} else {
			log_info(logger,
					"(mC) Particion:%d: %3X-%3X  [L]%4d Size: %4db orden:%d",
					part, nodo->inicio, nodo->fin, nodo->estado, nodo->largo,
					orden);
		}

		switch (orden) {
		case 0:
			nodo = nodo->sgte;
			break;
		case 1:
			nodo = nodo->ant;
			break;
		case 2:
			nodo = nodo->mayor;
			break;
		case 3:
			nodo = nodo->menor;
			break;
		default:
			nodo = nodo->sgte;  // sgte por defecto.
		}
	}
}
//void* buscarEspacioDisponible(int sizeMensaje) {

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
//

//void insertarEnCache(void* mensaje, int size, int id) {

// Implementamos Algoritmo Particion Libre FF
// La lista de particiones siempre tendra, al menos, un Nodo
//

//	int tamanoABuscar = brokerConf->tamanoMinimoParticion;
///	if (tamanoABuscar<size) tamanoABuscar=size;

//	}

//}
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
 //		}			/
 }
 printf("la memoria arranca en la direccion : %d .\n", (int) iniMemoria);
 printf("la memoria finaliza en la direccion : %d .\n", (int) finMemoria);
 //	printf("memoria total : %d .\n", memoriaTotal);

 return NULL;
 }
 */

void verificarSuscriptor(t_suscriptor* suscriptor, t_list* lista) { //esto es para ver si se reconecto o si es nuevo.

	t_suscriptor* suscriptorExistente = malloc(sizeof(t_suscriptor));
	int i = 0;
	int flag = 0;
	for (i = 0; i < list_size(lista); i++) {
		suscriptorExistente = list_get(lista, i);
		if ((strcmp(suscriptor->nombreProceso,
				suscriptorExistente->nombreProceso)) == 0) {

			list_replace(lista, i, suscriptor); // a este le tengo que mandar los mensajes que no le envie antes.
			flag = 1;
			break;
		}

	}
	if (flag == 0) {
		list_add(lista, suscriptor); //a este suscriptor nuevo tengo que recorrer la cache y enviarle los mensajes
	}
	free(suscriptorExistente);
}

t_administrativo* enviarMensajeASuscriptores(t_list* lista, t_paquete* mensaje) {
	t_suscriptor* suscriptorExistente = malloc(sizeof(t_suscriptor));
	t_administrativo* mensajeAdmin = malloc(sizeof(t_administrativo));
	mensajeAdmin->idMensaje = mensaje->buffer->idMensaje;
	mensajeAdmin->colaMensaje = mensaje->codigoOperacion;
	mensajeAdmin->suscriptoresEnviados = list_create();
	mensajeAdmin->suscriptoresRecibidos = list_create();

	int i;
	for (i = 0; i < list_size(lista); i++) {
		suscriptorExistente = list_get(lista, i);
		switch (mensaje->codigoOperacion) {
		case MENSAJE_NEW_POKEMON: {

			enviarMensajeBrokerNew(mensaje->buffer->nombrePokemon,
					mensaje->buffer->posX, mensaje->buffer->posY,
					mensaje->buffer->cantidadPokemons,
					suscriptorExistente->socket);
			list_add(mensajeAdmin->suscriptoresEnviados,
					(void*) suscriptorExistente);
			break;
		}
		case MENSAJE_APPEARED_POKEMON: {
			enviarMensajeBrokerAppeared(mensaje->buffer->nombrePokemon,
					mensaje->buffer->posX, mensaje->buffer->posY,
					mensaje->buffer->idMensajeCorrelativo,
					suscriptorExistente->socket);
			list_add(mensajeAdmin->suscriptoresEnviados,
					(void*) suscriptorExistente);
			break;
		}
		case MENSAJE_GET_POKEMON: {
//			enviarMensajeBrokerGet(mensaje->buffer->nombrePokemon,
//					suscriptorExistente->socket);
			enviarMensajeGameCardGetPokemon(mensaje->buffer->nombrePokemon,
					mensaje->buffer->idMensaje, suscriptorExistente->socket);

			list_add(mensajeAdmin->suscriptoresEnviados,
					(void*) suscriptorExistente);
			break;
		}
		case MENSAJE_CATCH_POKEMON: {
			enviarMensajeBrokerCatch(mensaje->buffer->nombrePokemon,
					mensaje->buffer->posX, mensaje->buffer->posY,
					suscriptorExistente->socket);
			list_add(mensajeAdmin->suscriptoresEnviados,
					(void*) suscriptorExistente);
			break;
		}
		case MENSAJE_CAUGHT_POKEMON: {
			enviarMensajeBrokerCaught(mensaje->buffer->idMensajeCorrelativo,
					mensaje->buffer->boolean, suscriptorExistente->socket);
			list_add(mensajeAdmin->suscriptoresEnviados,
					(void*) suscriptorExistente);
			break;
		}
		case MENSAJE_LOCALIZED_POKEMON: {

			enviarMensajeLocalizedId(mensaje->buffer->nombrePokemon,
					mensaje->buffer->listaCoordenadas,
					mensaje->buffer->idMensajeCorrelativo,
					suscriptorExistente->socket);
			list_add(mensajeAdmin->suscriptoresEnviados,
					(void*) suscriptorExistente);
			break;
		}
		default: {
			printf("error de mensaje o de suscriptor.\n");
		}
		}
	}

//	free(suscriptorExistente);

	return mensajeAdmin;

}

void* administrarMensajes() {

	t_paquete* paquete;

	//paquete = malloc(sizeof(t_paquete));
	paquete = malloc(sizeof(t_paquete));
	printf("Bloqueado en el mutex\n");
	//sem_wait(&bandejaCounter);
	pthread_mutex_lock(&bandejaMensajes_mutex);
	paquete = (t_paquete*) queue_pop(bandeja);
	pthread_mutex_unlock(&bandejaMensajes_mutex);

	printf(" Mi opCode es : %d,\n ", paquete->codigoOperacion);

	switch (paquete->codigoOperacion) {

	case SUSCRIBIRSE_NEW_POKEMON: { //falta chequear si el suscriptor existe en la lista, si existe lo dejo tal cual, sino lo agrego.
									//si existe me fijo que el socket coincida, aca es donde detecto la reconexion.

		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		//suscriptor->codigoOperacion = paquete->codigoOperacion;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = paquete->buffer->nombreProceso;
		suscriptor->socket = paquete->buffer->socket;

		verificarSuscriptor(suscriptor, NEW_POKEMON->lista);//lista de suscriptores
		printf("meti algo en la lista : ");
		log_info(logEntrega, "Se ha suscripto a la cola New.\n");
		break;
	}
	case SUSCRIBIRSE_APPEARED_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		//suscriptor->codigoOperacion = paquete->codigoOperacion;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = paquete->buffer->nombreProceso;
		suscriptor->socket = paquete->buffer->socket;

		verificarSuscriptor(suscriptor, APPEARED_POKEMON->lista);//lista de suscriptores
		log_info(logEntrega, "Se ha suscripto a la cola Appeared.\n");
		break;
	}

	case SUSCRIBIRSE_CATCH_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		//suscriptor->codigoOperacion = paquete->codigoOperacion;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = paquete->buffer->nombreProceso;
		suscriptor->socket = paquete->buffer->socket;

		verificarSuscriptor(suscriptor, CATCH_POKEMON->lista);//lista de suscriptores
		log_info(logEntrega, "Se ha suscripto a la cola Catch.\n");
		break;
	}

	case SUSCRIBIRSE_CAUGHT_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		//suscriptor->codigoOperacion = paquete->codigoOperacion;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = paquete->buffer->nombreProceso;
		suscriptor->socket = paquete->buffer->socket;

		verificarSuscriptor(suscriptor, CAUGHT_POKEMON->lista);	//lista de suscriptores
		log_info(logEntrega, "Se ha suscripto a la cola Caught.\n");
		break;
	}

	case SUSCRIBIRSE_GET_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		//suscriptor->codigoOperacion = paquete->codigoOperacion;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = paquete->buffer->nombreProceso;
		suscriptor->socket = paquete->buffer->socket;

		verificarSuscriptor(suscriptor, GET_POKEMON->lista);//lista de suscriptores
		log_info(logEntrega, "Se ha suscripto a la cola Get.\n");
		break;
	}

	case SUSCRIBIRSE_LOCALIZED_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		//suscriptor->codigoOperacion = paquete->codigoOperacion;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = paquete->buffer->nombreProceso;
		suscriptor->socket = paquete->buffer->socket;

		verificarSuscriptor(suscriptor, LOCALIZED_POKEMON->lista);//lista de suscriptores
		log_info(logEntrega, "Se ha suscripto a la cola Localized.\n");
		break;
	}

	case MENSAJE_NEW_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola New.\n");
		t_administrativo* mensajeAdmin = enviarMensajeASuscriptores(
				NEW_POKEMON->lista, paquete);

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

		list_add(NEW_POKEMON->cola, mensajeAdmin);
		printf(" ENCOLE EN NEW : %s . \n", bufferLoco->pokemon);
		break;
	}
	case MENSAJE_APPEARED_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Appeared.\n");

		t_administrativo* mensajeAdmin = enviarMensajeASuscriptores(
				APPEARED_POKEMON->lista, paquete);

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

		//list_add(APPEARED_POKEMON->cola, bufferAdmin);

		list_add(APPEARED_POKEMON->cola, mensajeAdmin);

		printf("ENCOLE EN APPEARED : %s . \n", bufferLoco->pokemon);
		break;
	}

	case MENSAJE_CATCH_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Catch.\n");
		t_administrativo* mensajeAdmin = enviarMensajeASuscriptores(
				CATCH_POKEMON->lista, paquete);
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

		//list_add(CATCH_POKEMON->cola, bufferAdmin);
		list_add(CATCH_POKEMON->cola, (void*) mensajeAdmin);

		printf("ENCOLE EN CATCH : %s . \n", bufferLoco->pokemon);
		break;
	}

	case MENSAJE_CAUGHT_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Caught.\n");
		t_administrativo* mensajeAdmin = enviarMensajeASuscriptores(
				CAUGHT_POKEMON->lista, paquete);
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
		//list_add(CAUGHT_POKEMON->cola, bufferAdmin);
		list_add(CAUGHT_POKEMON->cola, (void*) mensajeAdmin);
		printf("ENCOLE EN CAUGHT : %d . \n", bufferLoco->booleano);
		break;
	}

	case MENSAJE_GET_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Get.\n");
		t_administrativo* mensajeAdmin = enviarMensajeASuscriptores(
				GET_POKEMON->lista, paquete);
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
		//list_add(GET_POKEMON->cola, bufferAdmin);
		list_add(GET_POKEMON->cola, (void*) mensajeAdmin);
		printf("ENCOLE EN GET : %s . \n", bufferLoco->pokemon);

		break;
	}

	case MENSAJE_LOCALIZED_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Localized.\n");
		t_administrativo* mensajeAdmin = enviarMensajeASuscriptores(
				LOCALIZED_POKEMON->lista, paquete);
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
		//list_add(LOCALIZED_POKEMON->cola, bufferAdmin);
		list_add(LOCALIZED_POKEMON->cola, (void*) mensajeAdmin);
		printf("ENCOLE EN LOCALIZED : %s . \n", bufferLoco->pokemon);
		break;
	}
	case CONFIRMACION_ACK: {	//tengo que actualizar los ack de los mensajes
		buscarMensaje(paquete);

		break;
	}
	default: {
		printf("error de modulo, no se conoce quien envia paquetes\n");
	}
	}

	//free(paquete->buffer);
	//free(paquete);

//free(paquete);
//
//	free( bufferLoco);

	printf("estoy en el final de administrar mensajes\n");
	pthread_exit(NULL);
	return NULL;
}

int buscarMensaje(t_paquete* paquete) {

	t_administrativo* unAdmin;    // = malloc(sizeof(t_administrativo));
	t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
	suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
	suscriptor->nombreProceso = paquete->buffer->nombreProceso;
	suscriptor->socket = paquete->buffer->socket;
	int idMensaje = paquete->buffer->idMensaje;
	int flag = 0;
	int i;

	switch (paquete->buffer->ack) {
	case MENSAJE_NEW_POKEMON: {
		for (i = 0; i < list_size(NEW_POKEMON->cola); i++) {
			unAdmin = (t_administrativo*) list_get(NEW_POKEMON->cola, i);
			if (idMensaje == unAdmin->idMensaje) {
				list_add(unAdmin->suscriptoresRecibidos, suscriptor);
				list_replace(NEW_POKEMON->cola, i, unAdmin);
				flag = 1;
				break;
			}

		}
		break;
	}
	case MENSAJE_APPEARED_POKEMON: {
		for (i = 0; i < list_size(APPEARED_POKEMON->cola); i++) {
			unAdmin = (t_administrativo*) list_get(APPEARED_POKEMON->cola, i);
			if (idMensaje == unAdmin->idMensaje) {
				list_add(unAdmin->suscriptoresRecibidos, suscriptor);
				list_replace(APPEARED_POKEMON->cola, i, unAdmin);
				flag = 1;
				break;
			}

		}
		break;
	}
	case MENSAJE_GET_POKEMON: {
		for (i = 0; i < list_size(GET_POKEMON->cola); i++) {
			unAdmin = (t_administrativo*) list_get(GET_POKEMON->cola, i);
			if (idMensaje == unAdmin->idMensaje) {
				list_add(unAdmin->suscriptoresRecibidos, suscriptor);
				list_replace(GET_POKEMON->cola, i, unAdmin);
				flag = 1;
				break;
			}

		}
		break;
	}
	case MENSAJE_CATCH_POKEMON: {
		for (i = 0; i < list_size(CATCH_POKEMON->cola); i++) {
			unAdmin = (t_administrativo*) list_get(CATCH_POKEMON->cola, i);
			if (idMensaje == unAdmin->idMensaje) {
				list_add(unAdmin->suscriptoresRecibidos, suscriptor);
				list_replace(CATCH_POKEMON->cola, i, unAdmin);
				flag = 1;
				break;
			}
		}
		break;
	}

	case MENSAJE_CAUGHT_POKEMON: {
		for (i = 0; i < list_size(CAUGHT_POKEMON->cola); i++) {
			unAdmin = (t_administrativo*) list_get(CAUGHT_POKEMON->cola, i);
			if (idMensaje == unAdmin->idMensaje) {
				list_add(unAdmin->suscriptoresRecibidos, suscriptor);
				list_replace(CAUGHT_POKEMON->cola, i, unAdmin);
				flag = 1;
				break;
			}
		}
		break;
	}
	case MENSAJE_LOCALIZED_POKEMON: {
		for (i = 0; i < list_size(LOCALIZED_POKEMON->cola); i++) {
			unAdmin = (t_administrativo*) list_get(LOCALIZED_POKEMON->cola, i);
			if (idMensaje == unAdmin->idMensaje) {
				list_add(unAdmin->suscriptoresRecibidos, suscriptor);
				list_replace(LOCALIZED_POKEMON->cola, i, unAdmin);
				flag = 1;
				break;
			}
		}
		break;
	}
	default: {
		printf("No se reconoce el mensaje a confirmar.\n");

	}
	}

	return flag;
}

void* handler(void* socketConectado) {
	int socket = *(int*) socketConectado;
	pthread_mutex_t mutexRecibir;
	pthread_mutex_init(&mutexRecibir, NULL);
//printf("Mi semaforo vale %d\n", mutexRecibir.__data.__count);

	t_paquete *bufferLoco;
	bufferLoco = malloc(sizeof(t_paquete));
	int flag = 1;
	printf("Esperando por un nuevo mensaje...\n");

	while (flag) {

		pthread_mutex_lock(&mutexRecibir);
		bufferLoco = recibirMensaje(socket);

		if (bufferLoco != NULL) {

			if (bufferLoco->codigoOperacion >= 7
					&& bufferLoco->codigoOperacion <= 13) { //esto es para capturar suscriptores.//inclui el ack
				printf(" Soy suscriptor a la cola %d.\n",
						bufferLoco->codigoOperacion);

				bufferLoco->buffer->socket = socket;
				pthread_mutex_lock(&bandejaMensajes_mutex);	//agregue esto
				queue_push(bandeja, (void*) bufferLoco);
				pthread_mutex_unlock(&bandejaMensajes_mutex);
				sem_post(&bandejaCounter);	//esto lo copié de Brian
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
				pthread_mutex_lock(&bandejaMensajes_mutex);	//agregue esto
				queue_push(bandeja, (void*) bufferLoco);
				pthread_mutex_unlock(&bandejaMensajes_mutex);
				sem_post(&bandejaCounter);
				enviarIdMensaje(bufferLoco->buffer->idMensaje, socket);
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

//free(bufferLoco);

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

	log_info(logger, "ESCUCHANDO CONEXIONES");
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
	sem_init(&bandejaSuscriptorCounter, 1, 0);
}
