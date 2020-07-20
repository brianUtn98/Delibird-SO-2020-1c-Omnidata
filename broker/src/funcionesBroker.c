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
	debugCache = 0;  //cero es igual a nottrace <-> not cereo es igual a trace
	debugFino = 0; // not cero and debugCache not cero show all fields
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
	if (brokerConf->frecuenciaCompactacion < 2)
		cantidadMaximaConsolidaciones = 1;
	else
		cantidadMaximaConsolidaciones = brokerConf->frecuenciaCompactacion;

	instanteCache = 0; // para guardar el instante en que ocurre cada movimiento de la cache.
	cache = (char *) malloc(brokerConf->tamanoMemoria);
	consolidaciones = 0;

	if (debugCache) {
		log_info(logger, "");
		log_info(logger,
				" (Ci) debugCache is TRUE (set to NULL for not cacheTrace.)");
		log_info(logger,
				" (Ci) Tamano Cache=[%d] - Particion Minima=[%d] - Frecuencia Compactacion=[%d]",
				brokerConf->tamanoMemoria, brokerConf->tamanoMinimoParticion,
				brokerConf->frecuenciaCompactacion);
		log_info(logger,
				" (Ci) esquemaCache PD=[%d] BS=[%d] - algReemplazo FIFO=[%d] LRU=[%d] - selecPartLibre FF=[%d] BF=[%d]",
				partPD, partBS, reemFIFO, reemLRU, seleFF, seleBF);
		log_info(logger, " (Ci) Iniciar CACHE");
		log_info(logger, " (Ci) instante = %d", instanteCache);
		log_info(logger, " (Ci) Memoria de la CACHE %X-%X Largo %d", cache,
				cache + brokerConf->tamanoMemoria - 1,
				(brokerConf->tamanoMemoria) * sizeof(*cache));
		log_info(logger, " (Ci) Size of: Nodo = %d bytes",
				sizeof(struct nodoListaCache));
	}

	// Iniciamos los valores de la cache vacia.

	t_part inicioCache;
	inicioCache = (t_part) malloc(sizeof(struct nodoListaCache));

	inicioCache->inicio = 0;
	inicioCache->fin = brokerConf->tamanoMemoria - 1;
	inicioCache->largo = brokerConf->tamanoMemoria;
	inicioCache->estado = 0;
	inicioCache->instante = instanteCache;
	inicioCache->id = 0;
	inicioCache->sgte = NULL;
	inicioCache->ant = NULL;
	inicioCache->mayor = NULL;
	inicioCache->menor = NULL;

	instanteCache++; // instante de iniciado de la CACHE (es siempre 0).

	partFirst = inicioCache;
	partLast = inicioCache;
	partBig = inicioCache;
	partSmall = inicioCache;

	char numeral[16] = "+123456789ABCDEF";
	char * pnumeral = numeral;
	for (int i = 0; i < brokerConf->tamanoMemoria; i += 1)
		memcpy(cache + i, pnumeral, 1); // le pone valores iniciales a la cache (ojo multiplo 16).

	if (debugCache) {
		log_info(logger, " (dC) Pruebas Cache - Mostrar Cache Inicial");
//	log_info(logger," (dC) Mostrar Cache Inicial en distinto orden usando el logger");
		log_info(logger,
				" (dC) Particion:%d:%.3X-%.3X [L] %.4d Size:%.4db orden:%d", 0,
				partFirst->inicio, partFirst->fin, partFirst->estado,
				partFirst->largo, ASCEND);
		log_info(logger,
				" (dC) Particion:%d:%.3X-%.3X [L] %.4d Size:%.4db orden:%d", 0,
				partLast->inicio, partLast->fin, partLast->estado,
				partLast->largo, DESCEND);
		log_info(logger,
				" (dC) Particion:%d:%.3X-%.3X [L] %.4d Size:%.4db orden:%d", 0,
				partSmall->inicio, partSmall->fin, partSmall->estado,
				partSmall->largo, AGRANDA);
		log_info(logger,
				" (dC) Particion:%d:%.3X-%.3X [L] %.4d Size:%.4db orden:%d\n",
				0, partBig->inicio, partBig->fin, partBig->estado,
				partBig->largo, 4);

//	printf("\n[");
//	for(int i=0;i<brokerConf->tamanoMemoria;i++) printf("{%d,%c}",i,cache[i]);
//	printf("\n]");

//	dumpCache();

//	log_info(logger,"(mCL) Mostrar Cache Inicial en distinto orden usando mostrarCache");

		mostrarCache(partFirst, ASCEND);
//		mostrarCache(partLast, DESCEND);
//		mostrarCache(partSmall, AGRANDA);
//		mostrarCache(partBig, ACHICA);
		log_info(logger, " \n");

		char messageVoid0[30] = "000000000000000000000000000000";
		char messageVoid1[30] = "111111111111111111111111111111";
		char messageVoid2[30] = "222222222222222222222222222222";
		char messageVoid3[30] = "333333333333333333333333333333";
		char messageVoid4[30] = "444444444444444444444444444444";
		char messageVoid5[30] = "555555555555555555555555555555";
		char messageVoid6[30] = "666666666666666666666666666666";
		char messageVoid7[30] = "777777777777777777777777777777";
		char messageVoid8[30] = "888888888888888888888888888888";
		char messageVoid9[30] = "999999999999999999999999999999";
		char messageVoidA[30] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
		char messageVoidB[30] = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
		char messageVoidC[30] = "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCC";
		char messageVoidD[30] = "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
		char messageVoidE[30] = "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEE";
		char messageVoidF[30] = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
//	char messageVoidG[30] = "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG";
//	char messageVoidH[30] = "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
//	char messageVoidI[30] = "IIIIIIIIIIIIIIIIIIIIIIIIIIIIII";

		char * p_messageVoid0 = messageVoid0;
		char * p_messageVoid1 = messageVoid1;
		char * p_messageVoid2 = messageVoid2;
		char * p_messageVoid3 = messageVoid3;
		char * p_messageVoid4 = messageVoid4;
		char * p_messageVoid5 = messageVoid5;
		char * p_messageVoid6 = messageVoid6;
		char * p_messageVoid7 = messageVoid7;
		char * p_messageVoid8 = messageVoid8;
		char * p_messageVoid9 = messageVoid9;
		char * p_messageVoidA = messageVoidA;
		char * p_messageVoidB = messageVoidB;
		char * p_messageVoidC = messageVoidC;
		char * p_messageVoidD = messageVoidD;
		char * p_messageVoidE = messageVoidE;
		char * p_messageVoidF = messageVoidF;
//	char * p_messageVoidG = messageVoidG;
//	char * p_messageVoidH = messageVoidH;
//	char * p_messageVoidI = messageVoidI;

//	if(debugCache){printf("\n["); for(int i=0;i<30;i++) printf("{%d,%c}",i,messageVoid[i]); printf("\n]");}

		log_info(logger, "(dC) Prueba de insercion con fragmentacion");

//	debugCache = 0;

//
//	insertarMensajeEnCache(void* mensaje, int largo, int id)
//
		insertarMensajeEnCache(p_messageVoid0, 19, 121);
		insertarMensajeEnCache(p_messageVoid1, 20, 122);
		insertarMensajeEnCache(p_messageVoid2, 16, 123);
		insertarMensajeEnCache(p_messageVoid3, 16, 124);
		insertarMensajeEnCache(p_messageVoid4, 21, 125);
		insertarMensajeEnCache(p_messageVoid5, 22, 126);
		insertarMensajeEnCache(p_messageVoid6, 16, 127);
		insertarMensajeEnCache(p_messageVoid7, 16, 128);
		insertarMensajeEnCache(p_messageVoid8, 9, 129);
		insertarMensajeEnCache(p_messageVoid9, 22, 130);
		insertarMensajeEnCache(p_messageVoidA, 17, 131);
		insertarMensajeEnCache(p_messageVoidB, 15, 132);
		insertarMensajeEnCache(p_messageVoidC, 5, 133);
		insertarMensajeEnCache(p_messageVoidD, 15, 134);
		insertarMensajeEnCache(p_messageVoidE, 13, 135);
		insertarMensajeEnCache(p_messageVoidF, 18, 136);

		t_part partAux;
		partAux = obtenerMensaje(136);
		if (partAux)
			mostrarPart(partAux, 99, 1);
		partAux = obtenerMensaje(135);
		if (partAux)
			mostrarPart(partAux, 98, 1);
		partAux = obtenerMensaje(134);
		if (partAux)
			mostrarPart(partAux, 97, 1);
		partAux = obtenerMensaje(133);
		if (partAux)
			mostrarPart(partAux, 96, 1);
		partAux = obtenerMensaje(132);
		if (partAux)
			mostrarPart(partAux, 95, 1);
		partAux = obtenerMensaje(131);
		if (partAux)
			mostrarPart(partAux, 94, 1);
		partAux = obtenerMensaje(130);
		if (partAux)
			mostrarPart(partAux, 93, 1);
		partAux = obtenerMensaje(121);
		if (partAux)
			mostrarPart(partAux, 80, 1);
		partAux = obtenerMensaje(120);
		if (partAux)
			mostrarPart(partAux, 79, 1);

		log_info(logger, "(dC) Fin debugCache\n");
	} 													// (dC) fin debugCache
	verbose = -1;
	if (verbose)
		mostrarCache(partFirst, ASCEND);
}												// (Ci) fin Cache inicializacion

t_part obtenerMensaje(int id) {
	t_part partAux = partFirst;
	while (partAux) {
		if (partAux->id == id) {
			if (debugCache)
				log_info(logger, "Encontre:%d", id);
			return partAux;
		}
		partAux = partAux->sgte;
	}
	if (debugCache)
		log_info(logger, "No encontre:%d", id);
	return NULL;
}

void insertarMensajeEnCache(void* mensaje, int largo, int id) {
	if (debugCache)
		log_info(logger, "(iMC) inserta Mensaje en Cache");
	t_part partAux;
	partAux = encontrarPartLibre(largo, ASCEND);
	while (!partAux) {
		partAux = elegirFifoVictima();
		liberarParticionDinamica(partAux);
		consolidacionDinamica(partAux);
		if (consolidaciones >= cantidadMaximaConsolidaciones) {
			compactacionDinamica();
			consolidaciones = 0;
			partAux = encontrarPartLibre(largo, ASCEND);
		}
	}
	insertarEnParticion(partAux, mensaje, largo, tamanoABuscar, id);
	if (debugCache)
		mostrarCache(partFirst, ASCEND);
	else if (verbose)
		mostrarCache(partFirst, ASCEND);

}

t_part encontrarPartLibre(int size, int orden) {
	t_part partAux;
	if (seleFF)
		partAux = encontrarFirstFitPartLibre(size, orden);
	else {
		log_info(logger, "no implementada aun, usamos FIFO");
		partAux = encontrarFirstFitPartLibre(size, orden);
	} //seleBF=-1;
	return partAux;
}
t_part encontrarBestFitPartlibre(int size, int orden) {
	t_part partAux = NULL;
	return partAux;
}

t_part encontrarFirstFitPartLibre(int size, int orden) {
	int posicion = 0;
	tamanoABuscar = brokerConf->tamanoMinimoParticion;
	t_part partAux;

	switch (orden) {
	case ASCEND:
		partAux = partFirst;
		break;
	case DESCEND:
		partAux = partLast;
		break;
	case AGRANDA:
		partAux = partSmall;
		break;
	case ACHICA:
		partAux = partBig;
		break;
	default:
		partAux = partFirst;
	}  // sgte por defecto.

	if (size > tamanoABuscar)
		tamanoABuscar = size;
	if (debugCache)
		log_info(logger,
				"(ePL)-Busco partLibre [size=%d] [requerido =%d] en la CACHE",
				size, tamanoABuscar);

	while (partAux != NULL) {
		posicion++;
		if ((partAux->estado == 0) && (partAux->largo >= tamanoABuscar)) {
			if (debugCache) {
				log_info(logger, "(ePL)-Encontre partLibre");
				mostrarPart(partAux, posicion, orden);
			}
			return partAux;
		}
		switch (orden) {
		case ASCEND:
			partAux = partAux->sgte;
			break;
		case DESCEND:
			partAux = partAux->ant;
			break;
		case AGRANDA:
			partAux = partAux->mayor;
			break;
		case ACHICA:
			partAux = partAux->menor;
			break;
		default:
			partAux = partAux->sgte;
		} // sgte por defecto.

		if (debugCache)
			printf("[%d]", posicion);
	}
	if (debugCache) {
		log_info(logger, "(ePL) No hay partLibre donde quepa [%d]", size);
		dumpCache();
	}

	return NULL;
}

t_part elegirFifoVictima(void) {
// debugCache = -1;

	if (debugCache)
		log_info(logger, "Vamos a elegirFifoVictima");
	t_part partAux, partVict;
	int first, posAux, posicion = 0;
	partVict = partFirst;
	partAux = partFirst;
	first = partAux->instante;
	posAux = posicion + 1;

	while (partAux) {
		posicion++;
		if (partAux->estado) {
			if (first > partAux->instante) {
				first = partAux->instante;
				posAux = posicion;
				partVict = partAux;
			}
		}
		if (posicion % 8 == 0)
			printf("\n");
		if (debugCache)
			printf("-{p%d}[i%d](f%d)-", posicion, partAux->instante, first);
		partAux = partAux->sgte;
	}
	if (debugCache) {
		printf("\n");
		log_info(logger, "(eFV)-EncontreVictimaFifo");
		mostrarPart(partVict, posAux, first);
	}
	if (posicion == 0)
		return NULL;
//debugCache = 0;

	return partVict;
}

void liberarParticionDinamica(t_part nodo) {
//debugCache = -1;
	if (debugCache)
		log_info(logger, "\n(lPD) Liberar Particion Dinamica de la cache");

	char menos = '-';
	char * pmenos = &menos;

//	if(debugCache) mostrarCache(partFirst,ASCEND);
	if (nodo->estado != 0) {
		removerListaCola(nodo);  // ya lo saque de las colas de distribucion.
		nodo->estado = 0;	// particion libre.

		for (int i = nodo->inicio; i < nodo->inicio + nodo->largo; i += 1)
			memcpy(cache + i, pmenos, 1);

	}
	if (debugCache)
		mostrarCache(partFirst, ASCEND);
//debugCache = 0;

}

void consolidacionDinamica(t_part nodo) {
//debugCache = -1;
	if (debugCache)
		log_info(logger, "\n(cDC) Consolidacion Dinamica de la cache");

	t_part partAux;
	// si corresponde consolidar hacia arriba.
	if (nodo->ant && nodo->ant->estado == 0) {
		if (debugCache)
			log_info(logger, "Consolidamos la de arriba");
		partAux = nodo->ant;
		removerPartPorTamano(partAux);
		removerPartPorTamano(nodo);
		nodo->inicio -= partAux->largo;
		nodo->fin += partAux->largo;
		nodo->largo += partAux->largo;
		nodo->instante = instanteCache;
		nodo->id = 0;
		nodo->ant = partAux->ant;
		if (partFirst == partAux)
			partFirst = nodo;
		insertarPartPorTamano(nodo);
		free(partAux); // libera el nodo
		if (debugCache)
			mostrarCache(partFirst, ASCEND);
	}
	// si corresponde consolidar hacia abajo.
	if (nodo->sgte && nodo->sgte->estado == 0) {
		if (debugCache)
			log_info(logger, "Consolidamos la de abajo");
		partAux = nodo->sgte;
		removerPartPorTamano(partAux);
		removerPartPorTamano(nodo);
//		nodo->inicio += partAux->largo;
		nodo->fin += partAux->largo;
		nodo->largo += partAux->largo;
		nodo->instante = instanteCache;
		nodo->id = 0;
		nodo->sgte = partAux->sgte;
		if (partLast == partAux)
			partLast = nodo;
		insertarPartPorTamano(nodo);
		free(partAux); // libera el nodo
		if (debugCache)
			mostrarCache(partFirst, ASCEND);
	}
	if (debugCache)
		mostrarCache(partSmall, AGRANDA);
	consolidaciones++;
//debugCache = 0;

}

void compactacionDinamica() {

//debugCache = -1;
	if (debugCache)
		log_info(logger, "\n(CDC) Compactacion Dinamica de la cache");
	int particion = 0, libres = 0, usadas = 0, corridas = 0, removidas = 0,
			insertadas = 0, finales = 0;
	int tamano = 0;
	t_part partAux = partFirst;
	t_part partSgte = partAux;
	int correr = 0;
	int liberarParticion = 0;
	char igual = '=';
	char * pigual = &igual;

	while (partAux) {
		particion++;
		partSgte = partAux->sgte;
		if (debugCache)
			dumpCache();
		if (partAux->estado) {
			usadas++;
			finales++;
			tamano += partAux->largo;

			if (correr) {
				corridas++;
				memcpy(cache + partAux->inicio - correr,
						cache + partAux->inicio, partAux->largo);
				partAux->inicio -= correr;
				partAux->fin -= correr;
			}

		} else {
			libres++;
			correr += partAux->largo;
			liberarParticion = -1;
			removerPartPorTamano(partAux);
			removerPartPorOrden(partAux);
			free(partAux);
			liberarParticion = 0;
			removidas++;
		}
		partAux = partSgte;
	}
	if (tamano < brokerConf->tamanoMemoria) {
		insertadas++;
		finales++;
		partAux = malloc(sizeof(struct nodoListaCache));
		partAux->inicio = tamano;
		partAux->fin = brokerConf->tamanoMemoria - 1;
		partAux->largo = brokerConf->tamanoMemoria - tamano;
		partAux->estado = 0;
		partAux->id = 0;
		partAux->instante = 0;
		partAux->sgte = NULL;
		partAux->ant = partLast;
		partLast->sgte = partAux;
		partLast = partAux;
		insertarPartPorTamano(partAux);
		for (int i = partAux->inicio; i < partAux->inicio + partAux->largo; i +=
				1)
			memcpy(cache + i, pigual, 1);

	}
	if (debugCache) {
		mostrarCache(partFirst, ASCEND);
//		log_info(logger,"Particiones<Eran:[%d]><Usadas:[%d]<>Libres:[%d]><Movidas:[%d]><Borradas:[%d]><Agregadas:[%d]><Quedan:[%d]>",
//				particion, usadas, libres, corridas, removidas, insertadas, finales);
	}
//debugCache = 0;

}

void dumpCache() {
	log_info(logger, "\n(dDC) Dump Dinamico de la cache");
	int col = 64;
	for (int i = 0; i < brokerConf->tamanoMemoria; i += col) {
		printf("\n[%5d]-[%5d]<", i, i + col - 1);
		for (int j = 0; j < col; j++) {
			printf("%c", cache[j + i]);
			if (i + j == brokerConf->tamanoMemoria) {
				printf(">\n");
				return;
			}
			if (!((j + 1) % 16))
				printf(" ");

		}
		printf(">");
	}
	printf("\nConsolidaciones:[%d] Frecuencia consolidaciones:[%d]\n     ",
			consolidaciones, cantidadMaximaConsolidaciones);
}

void removerListaCola(t_part nodo) {
	// Aqui hay que quitar este nodo de la lista, cola o ambos que tiene Marcos

}

t_part encontrarPartMayor(int size, int orden) {
	int posicion = 0;
	t_part partAux;

	switch (orden) {
	case ASCEND:
		partAux = partFirst;
		break;
	case DESCEND:
		partAux = partLast;
		break;
	case AGRANDA:
		partAux = partSmall;
		break;
	case ACHICA:
		partAux = partBig;
		break;
	default:
		partAux = partSmall;
	}  // mayor por defecto.

	if (debugCache)
		log_info(logger, "(ePM)-Busco partMayor [%d] en la CACHE", size);

	while (partAux != NULL) {
		posicion++;
		if (partAux->largo >= size) {
			if (debugCache) {
				log_info(logger, "(ePM)-Encontre partMayor");
				mostrarPart(partAux, 0, 0);
			}
			return partAux;
		}
		switch (orden) {
		case ASCEND:
			partAux = partAux->sgte;
			break;
		case DESCEND:
			partAux = partAux->ant;
			break;
		case AGRANDA:
			partAux = partAux->mayor;
			break;
		case ACHICA:
			partAux = partAux->menor;
			break;
		default:
			partAux = partAux->mayor;
		} // mayor por defecto.

		if (debugCache)
			printf("[%d]", posicion);
	}
	if (debugCache)
		log_info(logger, "(ePM) No hay partMayor que [%d]", size);
	return NULL;
}
//
void insertarEnParticion(t_part nodo, void * mensaje, int size, int alojamiento,
		int id) {

	nodoJusto = 0;       // nodoJusto set to FALSE
	if (nodo->largo == alojamiento)
		nodoJusto = -1; // Si el hueco es igual a la particion, nodoJusto is TRUE

	if (!nodoJusto) {

		t_part partNueva = malloc(sizeof(struct nodoListaCache)); //, alojar la nueva part

		partNueva->inicio = nodo->inicio;
		partNueva->fin = nodo->inicio + alojamiento - 1;
		partNueva->largo = alojamiento;
		partNueva->estado = size;
		partNueva->id = id;
		partNueva->instante = instanteCache; //despues usar LRU
		partNueva->sgte = nodo;
		partNueva->ant = nodo->ant;
		if (partNueva->ant)
			nodo->ant->sgte = partNueva;
		if (partFirst == nodo)
			partFirst = partNueva;
		//	partLast nunca podria cambiar por una insertarEnParticion libre

		debugFino = 0;

		if (debugCache) {
			log_info(logger, "Mostrar Cache con partNueva en lista 1-2");
			mostrarCache(partFirst, ASCEND);
			mostrarCache(partLast, DESCEND);
//    		mostrarCache(partSmall,AGRANDA);   // partNueva no tiene los punteros resueltos en 2-3 aun.
//    		mostrarCache(partBig,ACHICA);
		}

		memcpy(cache + partNueva->inicio, mensaje, size);

		nodo->inicio += alojamiento;
		nodo->largo -= alojamiento;
		nodo->instante = instanteCache;
		nodo->ant = partNueva;

		if (debugCache) {
			log_info(logger, "Mostrar Cache con Nodo Corregido en lista 1-2");
			mostrarCache(partFirst, ASCEND);
		}
//debugFino=NULL;

		insertarPartPorTamano(partNueva);

		if (debugCache) {
			log_info(logger, "Mostrar Cache con partNueva en lista 3-4");
			mostrarCache(partFirst, ASCEND);
		}
//debugFino=NULL;
//    	if(debugCache) {log_info(logger,"partNueva antes de mayor-menor"); mostrarPart(partNueva,0,9);}

		removerPartPorTamano(nodo);

		if (debugCache) {
			log_info(logger, "Mostrar Cache con Nodo removido en lista 3-4");
			mostrarCache(partFirst, ASCEND);
			mostrarCache(partLast, DESCEND);
			mostrarCache(partSmall, AGRANDA);
			mostrarCache(partBig, ACHICA);
		}
//debugFino=NULL;

		insertarPartPorTamano(nodo);

		if (debugCache) {
			log_info(logger, "Mostrar Cache con Nodo agregado en lista 3=4");
			mostrarCache(partFirst, ASCEND);
		}
		debugFino = 0;

	} // nodo justo no hay partNueva
	else {                             //if(nodoJusto)
		nodo->estado = size;
		nodo->id = id;
		nodo->instante = instanteCache;
		memcpy(cache + nodo->inicio, mensaje, size);
	}

	// terminamos la insercion y mostramos por los tres ordenens.

	if (debugCache) {
		mostrarCache(partFirst, ASCEND);
		mostrarCache(partLast, DESCEND);
		mostrarCache(partSmall, AGRANDA);
		mostrarCache(partBig, ACHICA);
	}
	instanteCache++;

}

void mostrarPartEnBruto(t_part nodo) {   //solo llamarla si debugFino es TRUE

	log_info(logger,
			"(mEB) \n[Ini=%d][Fin=%d][Lar=%d][Est=%d][Ins=%d][Id=%d][Nod=%.2d]"
					"\n[Sig=%9d][Ant=%9d]  punteros [Fst=%9d][Lst=%9d]"
					"\n[May=%9d][Men=%9d]           [Sml=%9d][Big=%9d]\n",
			nodo->inicio, nodo->fin, nodo->largo, nodo->estado, nodo->instante,
			nodo->id, nodo, nodo->sgte, nodo->ant, partFirst, partLast,
			nodo->mayor, nodo->menor, partSmall, partBig);
}

// a esta funcion hay que agregarle que muestre la Cola a la que pertenece usando el ID
void mostrarPart(t_part nodo, int part, int orden) {

	if (nodo->estado != 0) {   // part ocupada
		log_info(logger,
				"(mPX) Particion:%d:%.3X-%.3X [X] %.4d Size:%.4db LRU:<%d> Cola:<> ID:<%d> orden:%d",
				part, nodo->inicio, nodo->fin, nodo->estado, nodo->largo,
				nodo->instante, nodo->id, orden);

	} else {    // part libre
		log_info(logger,
				"(mPL) Particion:%d:%.3X-%.3X [L]% .4d Size:%.4db                          orden:%d",
				part, nodo->inicio, nodo->fin, nodo->estado, nodo->largo,
				orden);
	}

	if (debugFino)
		mostrarPartEnBruto(nodo);
	return;
}
void mostrarCache(t_part nodo, int orden) {
	int part = 0, partFree = 0, partUsed = 0, memTotal = 0, memFree = 0,
			memUsed = 0;
	;
	while (nodo != NULL /* && part<10*/) {
		part++;
		memTotal += nodo->largo;

		if (nodo->estado) {
			memUsed += nodo->largo;
			partUsed++;
		} else {
			memFree += nodo->largo;
			partFree++;
		}

		mostrarPart(nodo, part, orden);

		switch (orden) {
		case ASCEND:
			nodo = nodo->sgte;
			break;
		case DESCEND:
			nodo = nodo->ant;
			break;
		case AGRANDA:
			nodo = nodo->mayor;
			break;
		case ACHICA:
			nodo = nodo->menor;
			break;
		default:
			nodo = nodo->sgte;  // sgte por defecto.
		}
	}
	log_info(logger,
			"Memoria-{Total:[%X][%d]}-{Ocupada:[%X][%d]}-{Libre:[%X][%d]}-{Configurada:[%X][%d]}",
			memTotal, memTotal, memUsed, memUsed, memFree, memFree,
			brokerConf->tamanoMemoria, brokerConf->tamanoMemoria);
	log_info(logger, "Particiones<Libres:[%d]><Ocupadas:[%d]><Totales:[%d]>",
			partFree, partUsed, part);
	dumpCache();
}

void removerPartPorOrden(t_part nodo) {
	if (debugCache)
		log_info(logger, "(rPPO removerPartPorOrden");
	if (!nodo->ant && !nodo->sgte) {
		partFirst = NULL;
		partLast = NULL;
		partSmall = NULL;
		partBig = NULL;
		return;
	} else if (nodo->ant && nodo->sgte) {
		nodo->ant->sgte = nodo->sgte;
		nodo->sgte->ant = nodo->ant;
	} else if (!nodo->ant) {
		nodo->sgte->ant = NULL;
		partFirst = nodo->sgte;
	} else {
		nodo->ant->sgte = NULL;
		partLast = nodo->ant;
	}
}

void removerPartPorTamano(t_part nodo) {
	if (nodo->menor == NULL && nodo->mayor == NULL) { // es la cabeza => null punteros externos
		partSmall = NULL;
		partBig = NULL;
	} else {
		if (nodo->menor == NULL) { // es el pie
			removerPiePorTamano(nodo);
		} else {
			if (nodo->mayor == NULL) { // es la cabeza
				removerCabezaPorTamano(nodo);
			} else {
				removerMedioPorTamano(nodo);
			}
		}
	}
}
void insertarPartPorTamano(t_part nodo) {
	t_part partAux;

	if (partSmall->menor == NULL && partSmall->mayor == NULL) { // es solo una particion => pie o cabeza
		if (debugCache) {
			log_info(logger,
					"Es solo una particion y voy a insertarPie o Cabeza Portamano");
			mostrarPart(nodo, 9, AGRANDA);
		}
		if (partSmall->largo < nodo->largo) {
			if (debugCache) {
				log_info(logger, "Essolounaparticion y voy a insertarCabeza");
			}
			insertarCabezaPorTamano(nodo);
			if (debugCache) {
				log_info(logger,
						"Essolounaparticion y ya inserteCabezaPorTamano");
			}
		} else {
			if (debugCache) {
				log_info(logger, "Essolounaparticion y voy a insertarPie");
			}
			insertarPiePorTamano(nodo);
			if (debugCache) {
				log_info(logger, "Essolounaparticion y ya insertePiePorTamano");
			}
		}
		mostrarCache(partSmall, AGRANDA);
	} else {
		partAux = encontrarPartMayor(nodo->largo, AGRANDA); //buscamos donde meter el nodo.
		if (!partAux) {
			if (debugCache) {
				log_info(logger,
						"Noessololacache y no hay mayor => insertarCabezaPorTamano");
				mostrarPart(nodo, 7, AGRANDA);
				insertarCabezaPorTamano(nodo);
			}
			if (debugCache) {
				log_info(logger,
						"Noessololacache y no hay mayor => ya inserteCabezaPorTamano)");
				mostrarCache(partSmall, AGRANDA);
			}
		} else {
			if (!partAux->menor) {
				log_info(logger,
						"Noessololacache y hayMayor y es nula => insertarPiePorTamano");
				if (debugCache) {
					mostrarPart(nodo, 6, AGRANDA);
					insertarPiePorTamano(nodo);
					log_info(logger,
							"Noessololacache y hayMayor esNula => ya insertePiePorTamano)");
				}
				if (debugCache) {
					mostrarCache(partSmall, AGRANDA);
				}
			} else {

				if (debugCache) {
					log_info(logger,
							"Noessololacache, hayMayor y Noesnula => insertarMedioPorTamano");
					mostrarPart(nodo, 6, AGRANDA);
				}
				insertarMedioPorTamano(nodo, partAux);
			}
			if (debugCache) {
				log_info(logger,
						"Noessololacache, hayMayor esNula => ya inserteMedioPorTamano)");
				mostrarCache(partSmall, AGRANDA);
			}

		}
	}
}

void insertarCabezaPorTamano(t_part nodo) {
	nodo->mayor = NULL;
	nodo->menor = partBig;
	partBig->mayor = nodo;
	partBig = nodo;
}

void insertarPiePorTamano(t_part nodo) {
	nodo->menor = NULL;
	nodo->mayor = partSmall;
	partSmall->menor = nodo;
	partSmall = nodo;
}

void insertarMedioPorTamano(t_part nodo, t_part medio) {
	medio->menor->mayor = nodo;
	nodo->menor = medio->menor;
	nodo->mayor = medio;
	medio->menor = nodo;
}

void removerPiePorTamano() {
	partSmall->mayor->menor = NULL;
	partSmall = partSmall->mayor;
}

void removerCabezaPorTamano() {
	partBig->menor->mayor = NULL;
	partBig = partBig->menor;
}

void removerMedioPorTamano(t_part medio) {
	medio->menor->mayor = medio->mayor;
	medio->mayor->menor = medio->menor;

}

void verificarSuscriptor(t_suscriptor* suscriptor, t_cola* cola) { //esto es para ver si se reconecto o si es nuevo.
	t_suscriptor* suscriptorExistente = malloc(sizeof(t_suscriptor));
	int i = 0;
	int flag = 0;

	if (list_size(cola->lista) > 0) {
		for (i = 0; i < list_size(cola->lista); i++) {
			suscriptorExistente = list_get(cola->lista, i);
			if ((strcmp(suscriptor->nombreProceso,
					suscriptorExistente->nombreProceso)) == 0) {
				list_replace(cola->lista, i, suscriptor); // a este le tengo que mandar los mensajes que no le envie antes.
				flag = 1;
				break;
			}
		}
	}
	if (flag == 0) {
		list_add(cola->lista, suscriptor);
		enviarMensajeCacheado(cola, suscriptor);
	}
	free(suscriptorExistente);
}
t_administrativo* enviarMensajeCacheado(t_cola* cola, t_suscriptor* suscriptor) {
	t_part particion;
	t_administrativo* mensaje;
	t_bufferOmnidata* bufferLoco = malloc(sizeof(t_bufferOmnidata));
	int desplazamiento = 0;
	int i;

	if (list_size(cola->cola) > 0) {

		switch (suscriptor->codigoOperacion) {
		case MENSAJE_NEW_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {
				mensaje = list_get(cola->cola, i);
				list_add(mensaje->suscriptoresEnviados, suscriptor);
				list_replace(cola->cola, i, mensaje);
				particion = obtenerMensaje(mensaje->idMensaje);
//		printf("Particion Inicio:%d Particion Fin:%d Particion Size:%d Particion Estado:%d Particion Id:%d \n",
//				particion->inicio, particion->fin, particion->largo,
//				particion->id);
				if (particion != 0) {

					void* miBuffer = malloc(particion->largo);
					memcpy(miBuffer, cache + particion->inicio,
							particion->largo);

					memcpy(&bufferLoco->largoNombre, miBuffer + desplazamiento,
							sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);
					memcpy(bufferLoco->nombrePokemon, miBuffer + desplazamiento,
							bufferLoco->largoNombre);
					desplazamiento += bufferLoco->largoNombre;
					memcpy(&bufferLoco->cantidadPokemons,
							miBuffer + desplazamiento, sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);
					memcpy(&bufferLoco->posX, miBuffer + desplazamiento,
							sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);
					memcpy(&bufferLoco->posY, miBuffer + desplazamiento,
							sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);

					printf("largo del mensaje :%d\n", bufferLoco->largoNombre);
					printf("posX %d\n", bufferLoco->posX);
					printf("posY %d\n", bufferLoco->posY);
					printf("cantidad de pokemons %d \n",
							bufferLoco->cantidadPokemons);
					printf("el mensaje recuperado de la cache es : %s\n",
							bufferLoco->nombrePokemon);
					printf("largo del mensaje %d", desplazamiento);
					enviarMensajeBrokerNew(bufferLoco->nombrePokemon,
							bufferLoco->posX, bufferLoco->posY,
							bufferLoco->cantidadPokemons, suscriptor->socket);
				} else { //si la particion no existe, es que el mensaje se borro.
					//habria que borrar el t_administrativo de la cola.

				}
			}
			break;
		}
		case MENSAJE_APPEARED_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {
				mensaje = list_get(cola->cola, i);
				list_add(mensaje->suscriptoresEnviados, suscriptor);
				list_replace(cola->cola, i, mensaje);
				particion = obtenerMensaje(mensaje->idMensaje);
				//		printf("Particion Inicio:%d Particion Fin:%d Particion Size:%d Particion Estado:%d Particion Id:%d \n",
				//				particion->inicio, particion->fin, particion->largo,
				//				particion->id);
				if (particion != 0) {

					void* miBuffer = malloc(particion->largo);
					memcpy(miBuffer, cache + particion->inicio,
							particion->largo);

					memcpy(&bufferLoco->largoNombre, miBuffer + desplazamiento,
							sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);
					memcpy(bufferLoco->nombrePokemon, miBuffer + desplazamiento,
							bufferLoco->largoNombre);
					desplazamiento += bufferLoco->largoNombre;
					memcpy(&bufferLoco->posX, miBuffer + desplazamiento,
							sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);
					memcpy(&bufferLoco->posY, miBuffer + desplazamiento,
							sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);

					printf("largo del mensaje :%d\n", bufferLoco->largoNombre);
					printf("posX %d\n", bufferLoco->posX);
					printf("posY %d\n", bufferLoco->posY);
					printf("cantidad de pokemons %d \n",
							bufferLoco->cantidadPokemons);
					printf("el mensaje recuperado de la cache es : %s\n",
							bufferLoco->nombrePokemon);
					printf("largo del mensaje %d", desplazamiento);
					enviarMensajeBrokerAppeared(bufferLoco->nombrePokemon,
							bufferLoco->posX, bufferLoco->posY,
							idMensajeCorrelativo, suscriptor->socket);
				} else { //si la particion no existe, es que el mensaje se borro.
					//habria que borrar el t_administrativo de la cola.

				}
			}

			break;
		}
		case MENSAJE_CATCH_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {
				mensaje = list_get(cola->cola, i);
				list_add(mensaje->suscriptoresEnviados, suscriptor);
				list_replace(cola->cola, i, mensaje);
				particion = obtenerMensaje(mensaje->idMensaje);
				//		printf("Particion Inicio:%d Particion Fin:%d Particion Size:%d Particion Estado:%d Particion Id:%d \n",
				//				particion->inicio, particion->fin, particion->largo,
				//				particion->id);
				if (particion != 0) {

					void* miBuffer = malloc(particion->largo);
					memcpy(miBuffer, cache + particion->inicio,
							particion->largo);

					memcpy(&bufferLoco->largoNombre, miBuffer + desplazamiento,
							sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);
					memcpy(bufferLoco->nombrePokemon, miBuffer + desplazamiento,
							bufferLoco->largoNombre);
					desplazamiento += bufferLoco->largoNombre;
					memcpy(&bufferLoco->posX, miBuffer + desplazamiento,
							sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);
					memcpy(&bufferLoco->posY, miBuffer + desplazamiento,
							sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);
					printf("largo del mensaje :%d\n", bufferLoco->largoNombre);
					printf("posX %d\n", bufferLoco->posX);
					printf("posY %d\n", bufferLoco->posY);
					printf("cantidad de pokemons %d \n",
							bufferLoco->cantidadPokemons);
					printf("el mensaje recuperado de la cache es : %s\n",
							bufferLoco->nombrePokemon);
					printf("largo del mensaje %d", desplazamiento);

					enviarMensajeBrokerCatch(bufferLoco->nombrePokemon,
							bufferLoco->posX, bufferLoco->posY,
							suscriptor->socket);

				} else { //si la particion no existe, es que el mensaje se borro.
					//habria que borrar el t_administrativo de la cola.

				}
			}

			break;
		}
		case MENSAJE_CAUGHT_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {
				mensaje = list_get(cola->cola, i);
				list_add(mensaje->suscriptoresEnviados, suscriptor);
				list_replace(cola->cola, i, mensaje);
				particion = obtenerMensaje(mensaje->idMensaje);
				//		printf("Particion Inicio:%d Particion Fin:%d Particion Size:%d Particion Estado:%d Particion Id:%d \n",
				//				particion->inicio, particion->fin, particion->largo,
				//				particion->id);
				if (particion != 0) {

					void* miBuffer = malloc(particion->largo);
					memcpy(miBuffer, cache + particion->inicio,
							particion->largo);

					memcpy(&bufferLoco->boolean, miBuffer + desplazamiento,
							sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);
//					printf("largo del mensaje :%d\n", bufferLoco->largoNombre);
//					printf("posX %d\n", bufferLoco->posX);
//					printf("posY %d\n", bufferLoco->posY);
//					printf("cantidad de pokemons %d \n",
//							bufferLoco->cantidadPokemons);
//					printf("el mensaje recuperado de la cache es : %s\n",
//							bufferLoco->nombrePokemon);
//					printf("largo del mensaje %d", desplazamiento);

					enviarMensajeBrokerCaught(idMensajeCorrelativo,
							bufferLoco->boolean, suscriptor->socket);//falta el idCorrelativo, no se que le tengo que mandar.

				} else { //si la particion no existe, es que el mensaje se borro.
					//habria que borrar el t_administrativo de la cola.

				}
			}

			break;
		}
		case MENSAJE_GET_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {
				mensaje = list_get(cola->cola, i);
				list_add(mensaje->suscriptoresEnviados, suscriptor);
				list_replace(cola->cola, i, mensaje);
				particion = obtenerMensaje(mensaje->idMensaje);
				//		printf("Particion Inicio:%d Particion Fin:%d Particion Size:%d Particion Estado:%d Particion Id:%d \n",
				//				particion->inicio, particion->fin, particion->largo,
				//				particion->id);
				if (particion != 0) {

					void* miBuffer = malloc(particion->largo);
					memcpy(miBuffer, cache + particion->inicio,
							particion->largo);

					memcpy(&bufferLoco->largoNombre, miBuffer + desplazamiento,
							sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);
					memcpy(bufferLoco->nombrePokemon, miBuffer + desplazamiento,
							bufferLoco->largoNombre);
					desplazamiento += bufferLoco->largoNombre;

					printf("largo del mensaje :%d\n", bufferLoco->largoNombre);
					printf("posX %d\n", bufferLoco->posX);
					printf("posY %d\n", bufferLoco->posY);
					printf("cantidad de pokemons %d \n",
							bufferLoco->cantidadPokemons);
					printf("el mensaje recuperado de la cache es : %s\n",
							bufferLoco->nombrePokemon);
					printf("largo del mensaje %d", desplazamiento);

					enviarMensajeBrokerGet(bufferLoco->nombrePokemon,
							suscriptor->socket);

				} else { //si la particion no existe, es que el mensaje se borro.
					//habria que borrar el t_administrativo de la cola.

				}
			}

			break;
		}
		case MENSAJE_LOCALIZED_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {
				mensaje = list_get(cola->cola, i);
				list_add(mensaje->suscriptoresEnviados, suscriptor);
				list_replace(cola->cola, i, mensaje);
				particion = obtenerMensaje(mensaje->idMensaje);
				//		printf("Particion Inicio:%d Particion Fin:%d Particion Size:%d Particion Estado:%d Particion Id:%d \n",
				//				particion->inicio, particion->fin, particion->largo,
				//				particion->id);
				if (particion != 0) {

					void* miBuffer = malloc(particion->largo);
					memcpy(miBuffer, cache + particion->inicio,
							particion->largo);

					memcpy(&bufferLoco->largoNombre, miBuffer + desplazamiento,
							sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);
					memcpy(bufferLoco->nombrePokemon, miBuffer + desplazamiento,
							bufferLoco->largoNombre);
					desplazamiento += bufferLoco->largoNombre;

					bufferLoco->listaCoordenadas = list_create();

					memcpy(&bufferLoco->listaCoordenadas->elements_count,
							miBuffer + desplazamiento, sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);

					int j;
					for (j = 0;
							j < bufferLoco->listaCoordenadas->elements_count;
							j++) {
						uint32_t auxiliar = 0;
						memcpy(&auxiliar, miBuffer + desplazamiento,
								sizeof(uint32_t));
						desplazamiento += sizeof(uint32_t);
						list_add(bufferLoco->listaCoordenadas, auxiliar);

					}

					printf("largo del mensaje :%d\n", bufferLoco->largoNombre);
					printf("posX %d\n", bufferLoco->posX);
					printf("posY %d\n", bufferLoco->posY);
					printf("cantidad de pokemons %d \n",
							bufferLoco->cantidadPokemons);
					printf("el mensaje recuperado de la cache es : %s\n",
							bufferLoco->nombrePokemon);
					printf("largo del mensaje %d", desplazamiento);

					enviarMensajeLocalized(bufferLoco->nombrePokemon,
							bufferLoco->listaCoordenadas, suscriptor->socket);

				} else { //si la particion no existe, es que el mensaje se borro.
					//habria que borrar el t_administrativo de la cola.

				}
			}

			break;
		}
		default: {
			printf("no se reconoce el mensaje o cola o suscriptor.\n");

		}
		}

	}

	free(bufferLoco);
	return mensaje;
}
t_administrativo* enviarMensajeASuscriptores(t_list* lista, t_paquete* mensaje) {
	t_suscriptor* suscriptorExistente = malloc(sizeof(t_suscriptor));
	t_administrativo* mensajeAdmin = malloc(sizeof(t_administrativo));
	mensajeAdmin->idMensaje = mensaje->buffer->idMensaje;
	mensajeAdmin->colaMensaje = mensaje->codigoOperacion;
	mensajeAdmin->suscriptoresEnviados = list_create();
	mensajeAdmin->suscriptoresRecibidos = list_create();
	int i;
	if (list_size(lista) > 0) {
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
						mensaje->buffer->idMensaje,
						suscriptorExistente->socket);
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
	}

	return mensajeAdmin;
}

void* administrarMensajes() {
	t_paquete* paquete;
	paquete = malloc(sizeof(t_paquete));
	printf("Bloqueado en el mutex\n");
//sem_wait(&bandejaCounter);
	pthread_mutex_lock(&bandejaMensajes_mutex);
	paquete = (t_paquete*) queue_pop(bandeja);
	pthread_mutex_unlock(&bandejaMensajes_mutex);
	printf(" Mi opCode es : %d,\n ", paquete->codigoOperacion);
	switch (paquete->codigoOperacion) {
	case SUSCRIBIRSE_NEW_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		suscriptor->codigoOperacion = MENSAJE_NEW_POKEMON;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = paquete->buffer->nombreProceso;
		suscriptor->socket = paquete->buffer->socket;
		verificarSuscriptor(suscriptor, NEW_POKEMON);
		printf("meti algo en la lista : ");
		log_info(logEntrega, "Se ha suscripto a la cola New.\n");
		break;
	}
	case SUSCRIBIRSE_APPEARED_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		suscriptor->codigoOperacion = MENSAJE_APPEARED_POKEMON;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = paquete->buffer->nombreProceso;
		suscriptor->socket = paquete->buffer->socket;
		verificarSuscriptor(suscriptor, APPEARED_POKEMON);
		log_info(logEntrega, "Se ha suscripto a la cola Appeared.\n");
		break;
	}
	case SUSCRIBIRSE_CATCH_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		suscriptor->codigoOperacion = MENSAJE_CATCH_POKEMON;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = paquete->buffer->nombreProceso;
		suscriptor->socket = paquete->buffer->socket;
		verificarSuscriptor(suscriptor, CATCH_POKEMON);
		log_info(logEntrega, "Se ha suscripto a la cola Catch.\n");
		break;
	}
	case SUSCRIBIRSE_CAUGHT_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		suscriptor->codigoOperacion = MENSAJE_CAUGHT_POKEMON;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = paquete->buffer->nombreProceso;
		suscriptor->socket = paquete->buffer->socket;
		verificarSuscriptor(suscriptor, CAUGHT_POKEMON);
		log_info(logEntrega, "Se ha suscripto a la cola Caught.\n");
		break;
	}
	case SUSCRIBIRSE_GET_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		suscriptor->codigoOperacion = MENSAJE_GET_POKEMON;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = paquete->buffer->nombreProceso;
		suscriptor->socket = paquete->buffer->socket;
		verificarSuscriptor(suscriptor, GET_POKEMON);
		log_info(logEntrega, "Se ha suscripto a la cola Get.\n");
		break;
	}
	case SUSCRIBIRSE_LOCALIZED_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		suscriptor->codigoOperacion = MENSAJE_LOCALIZED_POKEMON;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = paquete->buffer->nombreProceso;
		suscriptor->socket = paquete->buffer->socket;
		verificarSuscriptor(suscriptor, LOCALIZED_POKEMON);
		log_info(logEntrega, "Se ha suscripto a la cola Localized.\n");
		break;
	}
	case MENSAJE_NEW_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola New.\n");

		//if(mensaje es menor al tamaño disponible en cache DO...

		t_newPokemon* bufferLoco = malloc(sizeof(t_newPokemon));
		bufferLoco->sizeNombre = paquete->buffer->largoNombre - 1;
		bufferLoco->pokemon = paquete->buffer->nombrePokemon;
		bufferLoco->cantidadPokemons = paquete->buffer->cantidadPokemons;
		bufferLoco->posX = paquete->buffer->posX;
		bufferLoco->posY = paquete->buffer->posY;
		uint32_t sizeMensaje = sizeof(uint32_t) * 4 + bufferLoco->sizeNombre;
		int desplazamiento = 0;
		if (sizeMensaje <= brokerConf->tamanoMemoria) {

			t_administrativo* mensajeAdmin = enviarMensajeASuscriptores(
					NEW_POKEMON->lista, paquete);

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
			memcpy(buffer + desplazamiento, &bufferLoco->posX,
					sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);
			memcpy(buffer + desplazamiento, &bufferLoco->posY,
					sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);

			printf("el size antes de insertar en cache :%d.\n", sizeMensaje);

			printf("Antes de insertar en cache...\n");
			printf("Nombre:%s - PosX:%d - PosY:%d - Cantidad:%d \n",
					bufferLoco->pokemon, bufferLoco->posX, bufferLoco->posY,
					bufferLoco->cantidadPokemons);
			insertarMensajeEnCache(buffer, sizeMensaje,
					paquete->buffer->idMensaje);
			list_add(NEW_POKEMON->cola, mensajeAdmin);
			printf(" ENCOLE EN NEW : %s . \n", bufferLoco->pokemon);
		} else {
			printf(
					"tamaño del mensaje más grande que la memoria cache, no se puede alojar.");
		}

//		t_part particion;
//		desplazamiento = 0;
//		particion = obtenerMensaje(0);
//		printf(
//				"Particion Inicio:%d Particion Fin:%d Particion Size:%d Particion Estado:%d Particion Id:%d \n",
//				particion->inicio, particion->fin, particion->largo,
//				particion->estado, particion->id);
//
//		t_bufferOmnidata *buffer2 = malloc(sizeof(t_bufferOmnidata));
//
//		void* miBuffer = malloc(particion->largo);
//		printf("Rompo 1\n");
//		memcpy(miBuffer, cache + particion->inicio, particion->largo);
//
//		printf("Rompo 2\n");
//		memcpy(&buffer2->largoNombre, miBuffer + desplazamiento,
//				sizeof(uint32_t));
//		desplazamiento += sizeof(uint32_t);
//		buffer2->nombrePokemon = malloc(buffer2->largoNombre);
//		memcpy(buffer2->nombrePokemon, miBuffer + desplazamiento,
//				buffer2->largoNombre);
//		desplazamiento += buffer2->largoNombre;
//
//		memcpy(&buffer2->cantidadPokemons, miBuffer + desplazamiento,
//				sizeof(uint32_t));
//		desplazamiento += sizeof(uint32_t);
//		memcpy(&buffer2->posX, miBuffer + desplazamiento, sizeof(uint32_t));
//		desplazamiento += sizeof(uint32_t);
//		memcpy(&buffer2->posY, miBuffer + desplazamiento, sizeof(uint32_t));
//		desplazamiento += sizeof(uint32_t);
//
//		printf("largo del nombre :%d\n", buffer2->largoNombre);
//		printf("posX %d\n", buffer2->posX);
//		printf("posY %d\n", buffer2->posY);
//		printf("cantidad de pokemons %d \n", buffer2->cantidadPokemons);
//		printf("el mensaje recuperado de la cache es : %s\n",
//				buffer2->nombrePokemon);
//		printf("largo del mensaje %d", desplazamiento);
////		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
////		suscriptor->largoNombreProceso = 5;
////		suscriptor->nombreProceso = "team1";
////		suscriptor->socket = 5;

		break;
	}
	case MENSAJE_APPEARED_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Appeared.\n");

		t_appearedPokemon* bufferLoco = malloc(sizeof(t_appearedPokemon));
		bufferLoco->sizeNombre = paquete->buffer->largoNombre - 1;
		bufferLoco->pokemon = paquete->buffer->nombrePokemon;
		bufferLoco->posX = paquete->buffer->posX;
		bufferLoco->posY = paquete->buffer->posY;
		uint32_t sizeMensaje = sizeof(uint32_t) * 3 + bufferLoco->sizeNombre;
		int desplazamiento = 0;

		if (sizeMensaje <= brokerConf->tamanoMemoria) {
			t_administrativo* mensajeAdmin = enviarMensajeASuscriptores(
					APPEARED_POKEMON->lista, paquete);

			void* buffer = malloc(sizeMensaje);
			memcpy(buffer + desplazamiento, &bufferLoco->sizeNombre,
					sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);
			memcpy(buffer + desplazamiento, bufferLoco->pokemon,
					bufferLoco->sizeNombre);
			desplazamiento += bufferLoco->sizeNombre;
			memcpy(buffer + desplazamiento, &bufferLoco->posX,
					sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);
			memcpy(buffer + desplazamiento, &bufferLoco->posY,
					sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);

			insertarMensajeEnCache(buffer, sizeMensaje,
					paquete->buffer->idMensaje);

			list_add(APPEARED_POKEMON->cola, mensajeAdmin);
			printf("ENCOLE EN APPEARED : %s . \n", bufferLoco->pokemon);

		} else {
			printf(
					"tamaño del mensaje más grande que la memoria cache, no se puede alojar.");
		}
		break;
	}
	case MENSAJE_CATCH_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Catch.\n");

		t_catchPokemon* bufferLoco = malloc(sizeof(t_catchPokemon));
		bufferLoco->sizeNombre = paquete->buffer->largoNombre - 1;
		bufferLoco->pokemon = paquete->buffer->nombrePokemon;
		bufferLoco->posX = paquete->buffer->posX;
		bufferLoco->posY = paquete->buffer->posY;
		uint32_t sizeMensaje = sizeof(uint32_t) * 3 + bufferLoco->sizeNombre;
		int desplazamiento = 0;

		if (sizeMensaje <= brokerConf->tamanoMemoria) {
			t_administrativo* mensajeAdmin = enviarMensajeASuscriptores(
					CATCH_POKEMON->lista, paquete);
			void* buffer = malloc(sizeMensaje);
			memcpy(buffer + desplazamiento, &bufferLoco->sizeNombre,
					sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);
			memcpy(buffer + desplazamiento, bufferLoco->pokemon,
					bufferLoco->sizeNombre);
			desplazamiento += bufferLoco->sizeNombre;
			memcpy(buffer + desplazamiento, &bufferLoco->posX,
					sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);
			memcpy(buffer + desplazamiento, &bufferLoco->posY,
					sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);

			insertarMensajeEnCache(buffer, sizeMensaje,
					paquete->buffer->idMensaje);

			list_add(CATCH_POKEMON->cola, (void*) mensajeAdmin);
			printf("ENCOLE EN CATCH : %s . \n", bufferLoco->pokemon);
		} else {
			printf(
					"tamaño del mensaje más grande que la memoria cache, no se puede alojar.");
		}

		break;
	}
	case MENSAJE_CAUGHT_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Caught.\n");

		t_caughtPokemon* bufferLoco = malloc(sizeof(t_caughtPokemon));
		bufferLoco->booleano = paquete->buffer->boolean;
		uint32_t sizeMensaje = sizeof(uint32_t);
		int desplazamiento = 0;

		if (sizeMensaje <= brokerConf->tamanoMemoria) {

			t_administrativo* mensajeAdmin = enviarMensajeASuscriptores(
					CAUGHT_POKEMON->lista, paquete);

			void* buffer = malloc(sizeMensaje);
			memcpy(buffer + desplazamiento, &bufferLoco->booleano, sizeof(int));
			desplazamiento += sizeof(int);

			insertarMensajeEnCache(buffer, sizeMensaje,
					paquete->buffer->idMensaje);

			list_add(CAUGHT_POKEMON->cola, (void*) mensajeAdmin);
			printf("ENCOLE EN CAUGHT : %d . \n", bufferLoco->booleano);
		} else {
			printf(
					"tamaño del mensaje más grande que la memoria cache, no se puede alojar.");
		}
		break;
	}
	case MENSAJE_GET_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Get.\n");

		t_getPokemon* bufferLoco = malloc(sizeof(t_catchPokemon));
		bufferLoco->sizeNombre = paquete->buffer->largoNombre - 1;
		bufferLoco->pokemon = paquete->buffer->nombrePokemon;
		uint32_t sizeMensaje = sizeof(uint32_t) + bufferLoco->sizeNombre;
		int desplazamiento = 0;

		if (sizeMensaje <= brokerConf->tamanoMemoria) {
			t_administrativo* mensajeAdmin = enviarMensajeASuscriptores(
					GET_POKEMON->lista, paquete);
			void* buffer = malloc(sizeMensaje);
			memcpy(buffer + desplazamiento, &bufferLoco->sizeNombre,
					sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);
			memcpy(buffer + desplazamiento, bufferLoco->pokemon,
					bufferLoco->sizeNombre);
			desplazamiento += bufferLoco->sizeNombre;

			insertarMensajeEnCache(buffer, sizeMensaje,
					paquete->buffer->idMensaje);

			list_add(GET_POKEMON->cola, (void*) mensajeAdmin);
			printf("ENCOLE EN GET : %s . \n", bufferLoco->pokemon);
		} else {
			printf(
					"tamaño del mensaje más grande que la memoria cache, no se puede alojar.");
		}
		break;
	}
	case MENSAJE_LOCALIZED_POKEMON: {
		log_info(logEntrega, "Llego un mensaje nuevo a la cola Localized.\n");

		t_localizedPokemon* bufferLoco = malloc(sizeof(t_localizedPokemon));
		bufferLoco->sizeNombre = paquete->buffer->largoNombre - 1;
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

		if (sizeMensaje <= brokerConf->tamanoMemoria) {

			t_administrativo* mensajeAdmin = enviarMensajeASuscriptores(
					LOCALIZED_POKEMON->lista, paquete);

			void* buffer = malloc(sizeMensaje);
			memcpy(buffer + desplazamiento, &bufferLoco->sizeNombre,
					sizeof(uint32_t));
			desplazamiento += sizeof(uint32_t);
			memcpy(buffer + desplazamiento, bufferLoco->pokemon,
					bufferLoco->sizeNombre);
			desplazamiento += bufferLoco->sizeNombre;
			uint32_t cantidadCoordenadas =
					paquete->buffer->listaCoordenadas->elements_count;
			printf("Al serializar, cantidadCoordenadas=%d\n",
					cantidadCoordenadas);
			printf("Serializando CantidadCoordenadas=%d\n",
					cantidadCoordenadas);
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

			insertarMensajeEnCache(buffer, sizeMensaje,
					paquete->buffer->idMensaje);

			list_add(LOCALIZED_POKEMON->cola, (void*) mensajeAdmin);
			printf("ENCOLE EN LOCALIZED : %s . \n", bufferLoco->pokemon);
		} else {
			printf(
					"tamaño del mensaje más grande que la memoria cache, no se puede alojar.");
		}
		break;
	}
	case CONFIRMACION_ACK: {
		int encontreMensaje = buscarMensaje(paquete);//aca se puede hacer una validacion.
		break;
	}
	default: {
		printf("error de modulo, no se conoce quien envia paquetes\n");
	}
	}

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
				suscriptor->codigoOperacion = MENSAJE_NEW_POKEMON;
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
				suscriptor->codigoOperacion = MENSAJE_APPEARED_POKEMON;
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
				suscriptor->codigoOperacion = MENSAJE_GET_POKEMON;
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
				suscriptor->codigoOperacion = MENSAJE_CATCH_POKEMON;
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
				suscriptor->codigoOperacion = MENSAJE_CAUGHT_POKEMON;
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
				suscriptor->codigoOperacion = MENSAJE_LOCALIZED_POKEMON;
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

void generarDump(int signal) {
	if (signal == SIGUSR1) {
		printf("Received SIGUSR1!\n");

		//crearDump(); funcion que esta haciendo Pedro.
	}
}

//signal(SIGUSR1, my_handler);
