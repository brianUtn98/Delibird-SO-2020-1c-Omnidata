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
	//log_info(logger, "TAMANO_MEMORIA=%d", brokerConf->tamanoMemoria);
	brokerConf->tamanoMinimoParticion = config_get_int_value(BROKERTConfig,
			"TAMANO_MINIMO_PARTICION");
	//log_info(logger, "TAMANO_MINIMO_PARTICION=%d",brokerConf->tamanoMinimoParticion);
	brokerConf->algoritmoMemoria = string_duplicate(
			config_get_string_value(BROKERTConfig, "ALGORITMO_MEMORIA"));
	//log_info(logger, "ALGORITMO_MEMORIA=%s", brokerConf->algoritmoMemoria);
	brokerConf->algoritmoReemplazo = string_duplicate(
			config_get_string_value(BROKERTConfig, "ALGORITMO_REEMPLAZO"));
	//log_info(logger, "ALGORITMO_REEMPLAZO=%s", brokerConf->algoritmoReemplazo);
	brokerConf->algoritmoParticionLibre = string_duplicate(
			config_get_string_value(BROKERTConfig,
					"ALGORITMO_PARTICION_LIBRE"));
	//log_info(logger, "ALGORITMO_PARTICION_LIBRE=%s",brokerConf->algoritmoParticionLibre);
	brokerConf->ipBroker = string_duplicate(
			config_get_string_value(BROKERTConfig, "IP_BROKER"));
	//log_info(logger, "IP_BROKER=%s", brokerConf->ipBroker);
	brokerConf->puertoBroker = config_get_int_value(BROKERTConfig,
			"PUERTO_BROKER");
	//log_info(logger, "PUERTO_BROKER=%d", brokerConf->puertoBroker);
	brokerConf->frecuenciaCompactacion = config_get_int_value(BROKERTConfig,
			"FRECUENCIA_COMPACTACION");
	//log_info(logger, "FRECUENCIA_COMPACTACION=%d",brokerConf->frecuenciaCompactacion);
	brokerConf->logFile = string_duplicate(
			config_get_string_value(BROKERTConfig, "LOG_FILE"));
	//log_info(logger, "LOG_FILE=%s", brokerConf->logFile);

	//log_info(logger, "- CONFIGURACION IMPORTADA\n");

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

	//LOCALIZED_PRUEBA = list_create();

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
// uso debugCache para mostrar cosas de la cache en pantalla mientras desarrollo.
// el que quiera que no le aparezcan, que la ponga en 0
//

void iniciarCacheBuddy() {
	debugCache = 0;  //cero es igual a nottrace <-> not cero es igual a trace
	debugFino = 0; // not cero and debugCache not cero show all fields
	verbose = -1; // si es true muestra el cache, a pesar de estar debugCache en 0
	debugTrace = 0; // las funciones se identifican

	if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {partPD = 0;partBS = -1;}
	else {partPD = -1;partBS = 0;	};
	if (strcmp(brokerConf->algoritmoReemplazo, "LRU") == 0) {reemFIFO = 0;reemLRU = -1;}
	else {reemFIFO = -1;reemLRU = 0;};
	if (strcmp(brokerConf->algoritmoParticionLibre, "BF") == 0) {seleFF = 0;seleBF = -1;}
	else {seleFF = -1;seleBF = 0;};
	if (brokerConf->frecuenciaCompactacion < 2)	cantidadMaximaConsolidaciones = 1;
	else cantidadMaximaConsolidaciones = brokerConf->frecuenciaCompactacion;

	cache = malloc(brokerConf->tamanoMemoria);
	t_partBuddy particionInicial;
	particionInicial.posicionParticion = 0;
	particionInicial.libre = 1;
	particionInicial.tamanio = brokerConf->tamanoMemoria;
	particionInicial.cola = -1;
	particionInicial.idMensaje = -1;
	particionInicial.tamanioMensaje = -1;
	particionInicial.contadorLRU = -1;
	t_partBuddy* particionInicialCreada = crearParticionBuddyMemoria(particionInicial);
	particionesEnMemoriaBuddy = list_create(); list_add(particionesEnMemoriaBuddy, particionInicialCreada);
	if (string_equals_ignore_case(brokerConf->algoritmoReemplazo, "FIFO"))
		colaMensajesMemoriaBuddy = queue_create();
	if (string_equals_ignore_case(brokerConf->algoritmoReemplazo, "LRU"))
		CONTADORLRUBUDDY = 0;
	char numeral[16] = "+123456789ABCDEF";
	char * pnumeral = numeral;
	for (int i = 0; i < brokerConf->tamanoMemoria; i += 1)
		cache[i] = '-';
	if (debugCache || verbose) {
		dumpCache();
		mostrarConfiguracion();
	}
}
//llamarla en la funcion main 01/08/2020
void iniciarCache() {

	debugCache = 0;  //cero es igual a nottrace <-> not cero es igual a trace
	debugFino = 0; // not cero and debugCache not cero show all fields
	verbose = -1; // si es true muestra el cache, a pesar de estar debugCache en 0
	debugTrace = 0; // las funciones se identifican

	if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {partPD = 0;partBS = -1;}
	else {partPD = -1;partBS = 0;	};
	if (strcmp(brokerConf->algoritmoReemplazo, "LRU") == 0) {reemFIFO = 0;reemLRU = -1;}
	else {reemFIFO = -1;reemLRU = 0;};
	if (strcmp(brokerConf->algoritmoParticionLibre, "BF") == 0) {seleFF = 0;seleBF = -1;}
	else {seleFF = -1;seleBF = 0;};
	if (brokerConf->frecuenciaCompactacion < 2)	cantidadMaximaConsolidaciones = 1;
	else cantidadMaximaConsolidaciones = brokerConf->frecuenciaCompactacion;

	instanteCache = 1; // para guardar el instante en que ocurre cada movimiento de la cache.
	cache = (char *) malloc(brokerConf->tamanoMemoria);
	consolidaciones = 0;

	if (debugCache) {
		printf(ACROJO "\n");

		printf("\n (Ci) debugCache is TRUE (set to NULL for not cacheTrace.)");
		printf(
				"\n (Ci) Tamano Cache=[%d] - Particion Minima=[%d] - Frecuencia Compactacion=[%d]",
				brokerConf->tamanoMemoria, brokerConf->tamanoMinimoParticion,
				brokerConf->frecuenciaCompactacion);
		printf(
				"\n (Ci) esquemaCache PD=[%d] BS=[%d] - algReemplazo FIFO=[%d] LRU=[%d] - selecPartLibre FF=[%d] BF=[%d]",
				partPD, partBS, reemFIFO, reemLRU, seleFF, seleBF);
		printf("\n (Ci) Iniciar CACHE");
		printf("\n (Ci) instante = %d", instanteCache);
		printf("\n (Ci) Memoria de la CACHE %X-%X Largo %d", cache,
				cache + brokerConf->tamanoMemoria - 1,
				(brokerConf->tamanoMemoria) * sizeof(*cache));
		printf("\n (Ci) Size of: Nodo = %d bytes\n",
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
	inicioCache->idMensaje = 0;
	inicioCache->cola = 0;
	inicioCache->sgte = NULL;
	inicioCache->ant = NULL;
	inicioCache->mayor = NULL;
	inicioCache->menor = NULL;

	instanteCache++; // instante de iniciado de la CACHE (es siempre 1).

	partFirst = inicioCache;
	partLast = inicioCache;
	partBig = inicioCache;
	partSmall = inicioCache;

	char numeral[16] = "+123456789ABCDEF";
	char * pnumeral = numeral;
	for (int i = 0; i < brokerConf->tamanoMemoria; i += 1)
		cache[i] = '-';
//		memcpy(cache + i, pnumeral, 1); // le pone valores iniciales a la cache (ojo multiplo 16).

//	if (debugCache ) {
//		printf(ACROJO "\n");
//
//		printf("\n (Ci) debugCache is TRUE (set to NULL for not cacheTrace.)");
//		printf("\n (Ci) Tamano Cache=[%d] - Particion Minima=[%d] - Frecuencia Compactacion=[%d]",
//				brokerConf->tamanoMemoria, brokerConf->tamanoMinimoParticion,
//				brokerConf->frecuenciaCompactacion);
//		printf("\n (Ci) esquemaCache PD=[%d] BS=[%d] - algReemplazo FIFO=[%d] LRU=[%d] - selecPartLibre FF=[%d] BF=[%d]",
//				partPD, partBS, reemFIFO, reemLRU, seleFF, seleBF);
//		printf("\n (Ci) Iniciar CACHE");
//		printf("\n (Ci) instante = %d", instanteCache);
//		printf("\n (Ci) Memoria de la CACHE %X-%X Largo %d", cache, cache + brokerConf->tamanoMemoria - 1,
//				(brokerConf->tamanoMemoria) * sizeof(*cache));
//		printf("\n (Ci) Size of: Nodo = %d bytes\n", sizeof(struct nodoListaCache));
//
//		printf("\n (dC) Pruebas Cache - Mostrar Cache Inicial");
//		printf("\n (dC) Particion:%d:%.3X-%.3X [L] %.4d Size:%.4db orden:%d", 0,
//				partFirst->inicio, partFirst->fin, partFirst->estado,
//				partFirst->largo, ASCEND);
//		printf("\n (dC) Particion:%d:%.3X-%.3X [L] %.4d Size:%.4db orden:%d", 0,
//				partLast->inicio, partLast->fin, partLast->estado,
//				partLast->largo, DESCEND);
//		printf("\n (dC) Particion:%d:%.3X-%.3X [L] %.4d Size:%.4db orden:%d", 0,
//				partSmall->inicio, partSmall->fin, partSmall->estado,
//				partSmall->largo, AGRANDA);
//
//		printf("\n (dC) Particion:%d:%.3X-%.3X [L] %.4d Size:%.4db orden:%d\n"ACRESET,
//				0, partBig->inicio, partBig->fin, partBig->estado,
//				partBig->largo, 4);
	if (debugCache || verbose){
		if (partPD) mostrarCache(partFirst, ASCEND);

		dumpCache();
		mostrarConfiguracion();
	}
}												// (Ci) fin Cache inicializacion

void mostrarConfiguracion(){

	printf(ACAZUL);
	printf("\nCACHE");
	printf(ACCIAN"\nTamano=["ACAMARILLO"%d"ACCIAN"]-Part.Minima=["ACAMARILLO"%d"ACCIAN"]-Frec.Compactacion=["ACAMARILLO"%d"ACCIAN
			"]-Consolidaciones=["ACROJO"%d"ACCIAN"]-Instante=["ACROJO"%d"ACCIAN"]",
			brokerConf->tamanoMemoria, brokerConf->tamanoMinimoParticion,brokerConf->frecuenciaCompactacion,
			consolidaciones, instanteCache);
		printf("\nEsquema=["ACAMARILLO"%s"ACCIAN"] Alg.Reemplazo=["ACAMARILLO"%s"ACCIAN"] SelecPartLibre["ACAMARILLO"%s"ACCIAN"]",
			obtenerConfiguracion(ESQUEMA), obtenerConfiguracion(REEMPLAZO), obtenerConfiguracion(LIBRE));
	printf("\n debug=["ACVERDE"%d"ACCIAN"] verbose=["ACVERDE"%d"ACCIAN"] trace=["ACVERDE"%d"ACCIAN"] fino=["ACVERDE"%d"ACCIAN"]",
			debugCache, verbose, debugTrace, debugFino);
	printf(ACRESET"\n");
}

char* obtenerConfiguracion(int parametro){
	//	if(debugTrace){printf(ACCIAN "\n (oC) obtenerConfiguracion"ACRESET);}

		switch (parametro) {
		case ESQUEMA: {
			if (partPD) return "PARTICIONES"; else return "BUDDY";
		}
		case REEMPLAZO: {
			if (reemFIFO) return "FIFO"; else return "LRU";
		}
		case LIBRE: {
			if (seleFF) return "FF"; else return "BF";
		}
		default: {
			return "NO CONFIGURADO";
		}
	}


}




t_part obtenerMensaje(int idMensaje) {
	if(debugTrace){printf(ACCIAN "\n (oM) obtenerMensaje"ACRESET"\n");}


	t_part partAux = partFirst;
	while (partAux) {
		if (partAux->idMensaje == idMensaje) {
			if (debugCache)

				printf("Encontre:%d", idMensaje);
			if (reemLRU) {partAux->instante = instanteCache; instanteCache++;}

			return partAux;
		}
		partAux = partAux->sgte;
	}
	if (debugCache)
		printf("No encontre:%d", idMensaje);
	return NULL;
}


void insertarMensajeEnCache(void* mensaje, int largo, int idMensaje, int cola) {
	if(debugTrace){printf(ACCIAN "\n (iMEC) insertarMensajeEnCache"ACRESET"\n");}


	if (debugCache)
		printf("(iMC) inserta Mensaje en Cache");

	t_part partAux;
	partAux = encontrarPartLibre(largo, ASCEND);
	if (debugCache)
		printf("busque particion libre %X", (partAux));

	while (!partAux) {
		partAux = elegirFifoVictima();
		liberarParticionDinamica(partAux);  // hasta aca.
		consolidacionDinamica(partAux);
		if (consolidaciones >= cantidadMaximaConsolidaciones) {
			compactacionDinamica();
			consolidaciones = 0;
		}

		partAux = encontrarPartLibre(largo, ASCEND);
	}

	insertarEnParticion(partAux, mensaje, largo, tamanoABuscar, idMensaje, cola);



	if (debugCache || verbose) {
		if (partPD) mostrarCache(partFirst,ASCEND);
		dumpCache();
		mostrarConfiguracion();
	}


}

void insertarMensajeEnCache2(void* mensaje, int largo, int idMensaje, int cola) {

	if (debugCache)
		log_debug(logger, "entre a insertarMensajeEnCache");

	log_error(logger, "(iMC) inserta Mensaje en Cache");
	int flag = 1;
	t_part partAux;
	//partAux = encontrarPartLibre(largo, ASCEND);
//	if (debugCache)
//		log_error(logger, "busque particion libre %X", (partAux));

	while (flag) {

		partAux = encontrarPartLibre(largo, ASCEND);
		if (debugCache)
			log_error(logger, "busque particion libre %X", (partAux));

		if (partAux) {
			flag = 0;
		} else {
			partAux = elegirFifoVictima();
			liberarParticionDinamica(partAux);
			consolidacionDinamica(partAux);
			if (consolidaciones >= cantidadMaximaConsolidaciones) {
				compactacionDinamica();
				consolidaciones = 0;
				flag = 0;
			}

		}
		insertarEnParticion(partAux, mensaje, largo, tamanoABuscar, idMensaje, cola);

//	while (!partAux) {
//		partAux = elegirFifoVictima();
//		liberarParticionDinamica(partAux);
//		consolidacionDinamica(partAux);
//		if (consolidaciones >= cantidadMaximaConsolidaciones) {
//			compactacionDinamica();
//			consolidaciones = 0;
//		}
//
//		partAux = encontrarPartLibre(largo, ASCEND);
//	}

//	insertarEnParticion(partAux, mensaje, largo, tamanoABuscar, idMensaje, cola);

		if (debugCache)
			mostrarCache(partFirst, ASCEND);
		else if (verbose)
			mostrarCache(partFirst, ASCEND);
	}
}
t_part encontrarPartLibre(int size, int orden) {

	if(debugTrace){printf(ACCIAN "\n");
	printf("\n (ePL) encontrarPartLibre"ACRESET"\n");}


	t_part partAux;
	if (seleFF)
		partAux = encontrarFirstFitPartLibre(size, ASCEND);
	else {
		partAux = encontrarFirstFitPartLibre(size, AGRANDA); /// es para probar, ESTA MAL>
	} //seleBF=-1;
	return partAux;
}

t_part encontrarFirstFitPartLibre(int size, int orden) {

	if(debugTrace){printf(ACCIAN "\n");
	printf("\n (eFFPL) encontrarFirsFitPartLibre"ACRESET"\n");}


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
		printf("(ePL)-Busco partLibre [size=%d] [requerido =%d] en la CACHE",
				size, tamanoABuscar);

	while (partAux != NULL) {
		posicion++;
		if ((partAux->estado == 0) && (partAux->largo >= tamanoABuscar)) {
			if (debugCache) {
				printf("(ePL)-Encontre partLibre");
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
		printf("(ePL) No hay partLibre donde quepa [%d]", size);
		dumpCache();
	}

	return NULL;
}

t_part elegirFifoVictima(void) {

	if(debugTrace){printf(ACCIAN "\n (eFV) elegirFifoVictima"ACRESET"\n");}


// debugCache = -1;

	if (debugCache)
		printf("Vamos a elegirFifoVictima");

	t_part partAux, partVict = NULL;
	int first = 0, posAux, posicion = 0;
//	partVict = partFirst;
	partAux = partFirst;
//	first = partAux->instante;
//	posAux = posicion + 1;

	while (partAux) {
		posicion++;
		if (partAux->estado) {
			if (!first) {
				first = partAux->instante;
				posAux = posicion;
				partVict = partAux;
			}
			if (first > partAux->instante) {
				first = partAux->instante;
				posAux = posicion;
				partVict = partAux;
			}

			if (posicion % 8 == 0)
				printf("\n");
			if (debugCache)
				printf("-{p%d}[i%d](f%d)-", posicion, partAux->instante, first);
		} else {
			if (debugCache)
				printf("-{p%d) libre ", posicion);
		}
		partAux = partAux->sgte;
	}
	if (debugCache) {
		printf("\n (eFV)-EncontreVictimaFifo");
		mostrarPart(partVict, posAux, first);
	}
//debugCache = 0;

	return partVict;
}

void liberarParticionDinamica(t_part nodo) {
	if(debugTrace){printf(ACCIAN "\n (lPD) liberarParticionDinamica"ACRESET"\n");}


//debugCache = -1;
	if (debugCache)
		printf("\n(lPD) Liberar Particion Dinamica de la cache\n");

	char menos = '-';
	char * pmenos = &menos;

//	if(debugCache) mostrarCache(partFirst,ASCEND);
	if (nodo->estado != 0) {
		removerListaCola(nodo); // ya lo saque de las colas de distribucion.
		nodo->estado = 0;	// particion libre.

		for (int i = nodo->inicio; i < nodo->inicio + nodo->largo; i += 1)
			memcpy(cache + i, pmenos, 1);

		log_info(logEntrega, "Eliminacion de una particion de Memoria: [%d]",
				nodo->inicio);

	}
	if (debugCache)
		mostrarCache(partFirst, ASCEND);
//debugCache = 0;

}

void consolidacionDinamica(t_part nodo) {
	if(debugTrace){printf(ACCIAN "\n (cD) consolidacionDinamica"ACRESET"\n");}



//debugCache = -1;

//debugFino=-1;
	if (debugCache)
		printf("\n(cDC) Consolidacion Dinamica de la cache");

	t_part partAux;
	// si corresponde consolidar hacia arriba.
	if (nodo->ant && nodo->ant->estado == 0) {

		partAux = nodo->ant;

		if (debugCache) {
			printf("\nConsolidaremos la de arriba");
//			printf("\n partAux[%Xh] nodo.ant[%Xh] nodo.inicio[%Xh] partAux.largo[%d] nodo.fin[%Xh] nodo.largo[%d] nodo.instante[%d]"
//					,partAux ,nodo->ant, nodo->inicio, partAux->largo, nodo->fin, nodo->largo, nodo->instante);
		}
		removerPartPorTamano(partAux);
//if (debugCache) {printf("\n partAux[%Xh] nodo.ant[%Xh] nodo.inicio[%Xh] partAux.largo[%d] nodo.fin[%Xh] nodo.largo[%d] nodo.instante[%d]"
//		,partAux ,nodo->ant, nodo->inicio, partAux->largo, nodo->fin, nodo->largo, nodo->inicio);}
		removerPartPorTamano(nodo);
//if (debugCache) {printf("\n partAux[%Xh] nodo.ant[%Xh] nodo.inicio[%Xh] partAux.largo[%d] nodo.fin[%Xh] nodo.largo[%d] nodo.instante[%d]"
//		,partAux ,nodo->ant, nodo->inicio, partAux->largo, nodo->fin, nodo->largo, nodo->inicio);}

		nodo->inicio -= partAux->largo;
//		nodo->fin += partAux->largo;
		nodo->largo += partAux->largo;
		nodo->instante = 0;
		nodo->idMensaje = 0;
		nodo->cola = 0;
		nodo->ant = partAux->ant;

		if (partAux->ant)
			partAux->ant->sgte = nodo; // en que no estuviera esta linea nos significo no poder mostrar :(

		if (partFirst == partAux)
			partFirst = nodo;

		if (debugCache)
			printf("\nconshaciarriba inicio[%Xh] fin[%Xh] largo[%d]",
					partAux->inicio, partAux->fin, partAux->largo);

		insertarPartPorTamano(nodo);
		free(partAux); // libera el nodo
		if (debugCache)
			printf("\n\n->\n\n");
		mostrarCache(partFirst, ASCEND);
		printf("\n\n->\n\n");
	}

	// si corresponde consolidar hacia abajo.
	if (nodo->sgte && nodo->sgte->estado == 0) {
		if (debugCache)
			printf("Consolidaremos la de abajo");
		partAux = nodo->sgte;
		removerPartPorTamano(partAux);
		removerPartPorTamano(nodo);
//		nodo->inicio += partAux->largo;
		nodo->fin += partAux->largo;
		nodo->largo += partAux->largo;
		nodo->instante = 0;
		nodo->idMensaje = 0;
		nodo->cola = 0;
		nodo->sgte = partAux->sgte;
		if (partLast == partAux)
			partLast = nodo;
		insertarPartPorTamano(nodo);
		free(partAux); // libera el nodo
		if (debugCache)
			mostrarCache(partFirst, ASCEND);
	}
	if (debugCache)
		mostrarCache(partSmall, ASCEND);

	consolidaciones++;

//debugCache = 0;
//debugFino=0;
}

void compactacionDinamica() {
	if(debugTrace){printf(ACCIAN "\n (CD) compactacionDinamica"ACRESET"\n");}


	if (debugCache)
		printf("\n(CDC) Compactacion Dinamica de la cache");

	log_info(logEntrega, "Ejecucion de Compactacion dinamica");

	int particion = 0, libres = 0, usadas = 0, corridas = 0, removidas = 0,
			insertadas = 0, finales = 0, tamano = 0, tamanoEnUso = 0,
			tamanoLibre = 0, correr = 0;

	t_part partAux = partFirst;
	t_part partSgte = partAux->sgte;
//	t_part partPaBorrar = partAux;

//	char igual = '=';
//	char * p_igual = &igual;

	while (partAux) {

		particion++;
		tamano += partAux->largo;

		if (debugCache) {
			printf("part:%d estado:%d ", particion, partAux->estado);
			mostrarCache(partFirst, ASCEND);
			dumpCache();
		}

		partSgte = partAux->sgte;

		if (partAux->estado) {
			usadas++;
			finales++;
			tamanoEnUso += partAux->largo;

			if (correr) {
				corridas++;

//				memcpy(cache + partAux->inicio - correr, cache + partAux->inicio, partAux->largo);
				for (int i = 0; i < partAux->largo; i++)
					cache[partAux->inicio - correr + i] = cache[partAux->inicio
							+ i];
				partAux->inicio -= correr;
				partAux->fin -= correr;

				if (debugCache) {
					printf("Corrida part:%d estado:%d ", particion,
							partAux->estado);
					mostrarCache(partFirst, ASCEND);
				}

			}

		} else {
			libres++;
			correr += partAux->largo;
			tamanoLibre += partAux->largo;
			removerPartPorTamano(partAux);
			removerPartPorOrden(partAux);
			free(partAux);
			removidas++;

			if (debugCache) {
				printf("Eliminada");/*mostrarCache(partFirst, ASCEND);*/
			}

		} /*if (debugCache) { mostrarCache(partFirst, ASCEND);}*/

		partAux = partSgte;
	}

	if (debugCache) {
		printf(
				"Recorri todas las part, ahora tengo que compactar en una, partAux [%X], partSgte [%x]",
				partAux, partSgte);
		printf("tamanoLibre:%d tamanoEnUso:%d tamanoMemoria:%d ", tamanoLibre,
				tamanoEnUso, brokerConf->tamanoMemoria);
		printf("partAnt[%x] partSgte[%x] partMayor[%s] partMenor[%s]",
				partFirst, partLast, partBig, partSmall);
	}
	if (tamanoLibre > 0) {
		insertadas++;
		finales++;

//		printf("antes malloc");
		partAux = malloc(sizeof(struct nodoListaCache));
//		printf("Despues malloc  partAux %X",(partAux) );

		partAux->inicio = tamanoEnUso;
		partAux->fin = brokerConf->tamanoMemoria - 1;
		partAux->largo = brokerConf->tamanoMemoria - tamanoEnUso;
		partAux->estado = 0;
		partAux->idMensaje = 0;
		partAux->cola = 0;
		partAux->instante = 0;
		partAux->sgte = NULL;
		partAux->ant = partLast;
		if (!partFirst)
			partFirst = partAux;
		if (partLast)
			partLast->sgte = partAux;
		partLast = partAux;
		if (!(partBig == NULL && partSmall == NULL))
			insertarPartPorTamano(partAux);
		else {
			partBig = partAux;
			partSmall = partAux;
		}

//		printf(" deberiamos rellenar al final  Inicio[%d] Largo[%d] ",partAux->inicio, partAux->largo);

//		char ce[2] = "CO";
//		char * p_ce = ce;
		for (int i = partAux->inicio; i < partAux->inicio + partAux->largo; i++)
			cache[i] = 'C';
//			memcpy(cache + i, p_ce, 1); // le pone C de compactacion

	}

	if (debugCache) {
		mostrarCache(partFirst, ASCEND);
	}
//		printf("Particiones<Eran:[%d]><Usadas:[%d]<>Libres:[%d]><Movidas:[%d]><Borradas:[%d]><Agregadas:[%d]><Quedan:[%d]>",

//				particion, usadas, libres, corridas, removidas, insertadas, finales);
//	}
//debugCache = 0;

}

//	  printf(ACROJO     "This text is RED!"     ACRESET "\n");
//	  printf(ACVERDE   "This text is GREEN!"   ACRESET "\n");
//	  printf(ACAMARILLO  "This text is YELLOW!"  ACRESET "\n");
//	  printf(ACAZUL    "This text is BLUE!"    ACRESET "\n");
//	  printf(ACMAGENTA "This text is MAGENTA!" ACRESET "\n");
//	  printf(ACCIAN    "This text is CYAN!"    ACRESET "\n");


void dumpCache(){
	if(debugTrace){printf(ACCIAN "\n (dC) dumpCache"ACRESET"\n");}


	char cartel[32] = "Contenido Dinamico de la CACHE";
	int col = 32;
	int largoCartel = strlen(cartel);
	printf(ACAZUL "\n");
	for (int i=0;i<14+(col*3-largoCartel)/2;i++) printf(" "); printf("%s", cartel);

	char simbolo[2] = "CH";
	char*psimbolo;
	psimbolo = simbolo;

	for (int i = 0; i < brokerConf->tamanoMemoria; i += col) {
		printf("\n              ");
		for (int j = 0; j < col; j++) {
			if (i + j == brokerConf->tamanoMemoria) {
				printf("\n");
				break;	printf(ACAZUL "\n(dDC) Dump Dinamico de la cache");

			}
			printf(" __");

			if (!((j + 1) % 16))
				printf("  ");
		}
		printf("\n[%4d]-[%4d]<", i, i + col - 1);
		for (int j = 0; j < col; j++) {
			if (i + j == brokerConf->tamanoMemoria) {
				printf(">\n");
				break;
			}
			psimbolo[0] = cache[j + i];
			if ((int) simbolo[0] > 31 && (int) simbolo[0] < 128)
				printf("|" ACAMARILLO" %c" ACAZUL "",
						*psimbolo);
			else
				printf("|  ");
			if (!((j + 1) % 16))
				printf("| ");

		}
		printf("|>\n");
		printf("              ");
		for (int j = 0; j < col; j++) {
			if (i + j == brokerConf->tamanoMemoria) {
				printf("\n");
				break;
			}
			printf("|--");
			if (!((j + 1) % 16))
				printf("| ");
		}

		printf("\n              ");
		for (int j = 0; j < col; j++) {
			if (i + j == brokerConf->tamanoMemoria) {
				printf(">\n");
				break;
			}
//			memcpy(psimbolo,cache[j+i],1);

			printf("|" ACVERDE "%2X" ACAZUL, cache[j + i]);
			if (!((j + 1) % 16))
				printf("| ");
		}
	}
	printf(ACRESET);
}
void liberarAdministrativo(t_administrativo* admin) {
	if(debugTrace){printf(ACCIAN "\n (lA) liberarAdministrativo"ACRESET"\n");}



	free(admin);
}

void removerListaCola(t_part nodo) {
	if(debugTrace){printf(ACCIAN "\n (rLC) removerListaCola"ACRESET"\n");}



	int i;
	t_administrativo* auxiliar;

	switch (nodo->cola) {
	case MENSAJE_NEW_POKEMON: {
		for (i = 0; i < list_size(NEW_POKEMON->cola); i++) {
			pthread_mutex_lock(&mutexQueueNew);
			auxiliar = (t_administrativo*) list_get(NEW_POKEMON->cola, i);
			if (nodo->idMensaje == auxiliar->idMensaje) {
				pthread_mutex_lock(&mutexQueueNew);

				list_remove_and_destroy_element(NEW_POKEMON->cola, i, free);

				break;
			}
			pthread_mutex_unlock(&mutexQueueNew);

		}

		break;
	}
	case MENSAJE_APPEARED_POKEMON: {
		for (i = 0; i < list_size(APPEARED_POKEMON->cola); i++) {
			pthread_mutex_lock(&mutexQueueAppeared);
			auxiliar = (t_administrativo*) list_get(APPEARED_POKEMON->cola, i);
			if (nodo->idMensaje == auxiliar->idMensaje) {
				pthread_mutex_lock(&mutexQueueAppeared);

				list_remove_and_destroy_element(APPEARED_POKEMON->cola, i,
						free);
				//pthread_mutex_unlock(&mutexQueueAppeared);

				break;
			}
			pthread_mutex_unlock(&mutexQueueAppeared);
		}
		break;
	}

	case MENSAJE_GET_POKEMON: {
		for (i = 0; i < list_size(GET_POKEMON->cola); i++) {
			pthread_mutex_lock(&mutexQueueGet);
			auxiliar = (t_administrativo*) list_get(GET_POKEMON->cola, i);
			if (nodo->idMensaje == auxiliar->idMensaje) {
				pthread_mutex_lock(&mutexQueueGet);

				list_remove_and_destroy_element(GET_POKEMON->cola, i, free);
				//pthread_mutex_unlock(&mutexQueueGet);
				break;
			}
			pthread_mutex_unlock(&mutexQueueGet);
		}
		break;
	}
	case MENSAJE_CATCH_POKEMON: {
		for (i = 0; i < list_size(CATCH_POKEMON->cola); i++) {
			pthread_mutex_lock(&mutexQueueCatch);
			auxiliar = (t_administrativo*) list_get(CATCH_POKEMON->cola, i);
			if (nodo->idMensaje == auxiliar->idMensaje) {
				pthread_mutex_lock(&mutexQueueCatch);

				list_remove_and_destroy_element(CATCH_POKEMON->cola, i, free);
				//pthread_mutex_unlock(&mutexQueueCatch);
				break;
			}
			pthread_mutex_unlock(&mutexQueueCatch);

		}
		break;
	}
	case MENSAJE_CAUGHT_POKEMON: {
		for (i = 0; i < list_size(CAUGHT_POKEMON->cola); i++) {
			pthread_mutex_lock(&mutexQueueCaught);
			auxiliar = (t_administrativo*) list_get(CAUGHT_POKEMON->cola, i);
			if (nodo->idMensaje == auxiliar->idMensaje) {
				pthread_mutex_lock(&mutexQueueCaught);

				list_remove_and_destroy_element(CAUGHT_POKEMON->cola, i, free);
			//	pthread_mutex_unlock(&mutexQueueCaught);
				break;
			}
			pthread_mutex_unlock(&mutexQueueCaught);

		}
		break;
	}
	case MENSAJE_LOCALIZED_POKEMON: {
		for (i = 0; i < list_size(LOCALIZED_POKEMON->cola); i++) {
			pthread_mutex_lock(&mutexQueueLocalized);
			auxiliar = (t_administrativo*) list_get(LOCALIZED_POKEMON->cola, i);
			if (nodo->idMensaje == auxiliar->idMensaje) {
				pthread_mutex_lock(&mutexQueueLocalized);

				list_remove_and_destroy_element(LOCALIZED_POKEMON->cola, i,
						free);
				//pthread_mutex_unlock(&mutexQueueLocalized);
				break;
			}
			pthread_mutex_unlock(&mutexQueueLocalized);

		}
		break;
	}
	default: {
		printf("No se reconoce la estructura del mensaje a eliminar.\n");

	}
	}
}

t_part encontrarPartMayor(int size, int orden) {

	if(debugTrace){printf(ACCIAN "\n (ePM) encontrarPartMayor"ACRESET"\n");}



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
		printf("(ePM)-Busco partMayor [%d] en la CACHE", size);

	while (partAux != NULL) {
		posicion++;
		if (partAux->largo >= size) {
			if (debugCache) {
				printf("(ePM)-Encontre partMayor");
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
		printf("(ePM) No hay partMayor que [%d]", size);
	return NULL;
}
//
void insertarEnParticion(t_part nodo, void * mensaje, int size, int alojamiento,
		int idMensaje, int cola) {
	if(debugTrace){printf(ACCIAN "\n (iEP) insertarEnParticion"ACRESET"\n");}


	int nodoJusto = 0;       // nodoJusto set to FALSE
	if (nodo->largo == alojamiento)
		nodoJusto = -1; // Si el hueco es igual a la particion, nodoJusto is TRUE

	if (!nodoJusto) {

		t_part partNueva = malloc(sizeof(struct nodoListaCache)); //, alojar la nueva part

		partNueva->inicio = nodo->inicio;
		partNueva->fin = nodo->inicio + alojamiento - 1;
		partNueva->largo = alojamiento;
		partNueva->estado = size;
		partNueva->idMensaje = idMensaje;
		partNueva->cola = cola;
		partNueva->instante = instanteCache; //despues usar LRU
		partNueva->sgte = nodo;
		partNueva->ant = nodo->ant;
		if (partNueva->ant)
			nodo->ant->sgte = partNueva;
		if (partFirst == nodo)
			partFirst = partNueva;
//	partLast nunca podria cambiar por una insertarEnParticion libre

//		for(int i=0;i<size;i++) cache[i+partNueva->inicio] = mensaje[i];

		memcpy(cache + partNueva->inicio, mensaje, size);

		log_info(logEntrega,
				"Almacenado de un mensaje dentro de la memoria: Posicion Inicio en cache: [%d]",
				partNueva->inicio);

		insertarPartPorTamano(partNueva);

		nodo->inicio += alojamiento;
		nodo->largo -= alojamiento;
		nodo->instante = instanteCache;
		nodo->ant = partNueva;

		if (debugCache) {

			printf("Mostrar Cache con Nodo Corregido en lista 1-2");
			mostrarCache(partFirst, ASCEND);
		}
		removerPartPorTamano(nodo);
		insertarPartPorTamano(nodo);

	} // nodo justo no hay partNueva
	else {                             //if(nodoJusto)
		nodo->estado = size;
		nodo->idMensaje = idMensaje;
		nodo->cola = cola;
		nodo->instante = instanteCache;
		memcpy(cache + nodo->inicio, mensaje, size);

		log_info(logEntrega,
				"Almacenado de un mensaje dentro de la memoria: Posicion Inicio en cache: [%d]",
				nodo->inicio);

	}
	instanteCache++;

}
char* obtenerNombreCola(int cola) {
//	if(debugTrace){printf(ACCIAN "\n (oNC) obtenerNombreCola"ACRESET);}

	switch (cola) {
	case MENSAJE_NEW_POKEMON: {
		return "NEW";
	}
	case MENSAJE_APPEARED_POKEMON: {
		return "APPEARED";
	}
	case MENSAJE_GET_POKEMON: {
		return "GET";
	}
	case MENSAJE_LOCALIZED_POKEMON: {
		return "LOCALIZED";
	}
	case MENSAJE_CATCH_POKEMON: {
		return "CATCH";
	}
	case MENSAJE_CAUGHT_POKEMON: {
		return "CAUGHT";
	}
	default: {
		return "NULL";
	}
	}
}

void mostrarPartEnBruto(t_part nodo) {  //solo llamarla si debugFino es TRUE
	if(debugTrace){printf(ACCIAN "\n (mPEB) mostrarPartenBruto"ACRESET);}

	printf("\n" ACMAGENTA);
	printf("\n(mEB)[Ini=%Xh][Fin=%Xh][Lar=%d][Est=%d][LRU=%d][Id=%d][cola=%3.3s]"ACAMARILLO"[Nod=%8X]"ACMAGENTA
		   "\n     Punteros[Ant=%8X][Sig=%8X]  punteros[Fst=%8X][Lst=%8X]"
		   "\n   empotrados[Men=%8X][May=%8X]  externos[Sml=%8X][Big=%8X]\n",

			nodo->inicio, nodo->fin, nodo->largo, nodo->estado, nodo->instante,
			nodo->idMensaje, obtenerNombreCola(nodo->cola), (nodo), (nodo->ant), nodo->sgte, partFirst,
			partLast, nodo->menor, nodo->mayor, partSmall, partBig);
	printf(ACRESET "\n");
}

void mostrarPart(t_part nodo, int part, int orden) {
	if(debugTrace){printf(ACCIAN "\n(mP) mostrarPart"ACRESET"\n");}


	if (nodo->estado != 0) {   // part ocupada
		printf(ACCIAN"\n(mPX) Particion:"ACMAGENTA"%.3d"ACCIAN":"ACMAGENTA"%.4X"ACCIAN
				"-"ACMAGENTA"%.4X"ACCIAN" ["ACMAGENTA"X"ACCIAN"] "ACMAGENTA"%.4d"ACCIAN
				" Size:"ACMAGENTA"%.4db"ACCIAN" LRU:<"ACMAGENTA"%d"ACCIAN"> Cola:<"ACMAGENTA
				"%3.3s"ACCIAN"> ID:<"ACMAGENTA"%d"ACCIAN"> orden:"ACMAGENTA"%d"ACCIAN,

				part, nodo->inicio, nodo->fin, nodo->estado, nodo->largo,
				nodo->instante, obtenerNombreCola(nodo->cola), nodo->idMensaje, orden);

	} else {    // part libre
		printf(ACCIAN"\n(mPL) Particion:"ACVERDE"%.3d"ACCIAN":"ACVERDE"%.4X"ACCIAN
				"-"ACVERDE"%.4X"ACCIAN" ["ACVERDE"L"ACCIAN"] "ACVERDE"%.4d"ACCIAN
				" Size:"ACVERDE"%.4db"ACCIAN"                         orden:"ACVERDE"%d"ACCIAN,

				part, nodo->inicio, nodo->fin, nodo->estado, nodo->largo, //
				orden);
	}

	if (debugFino)
		mostrarPartEnBruto(nodo);
	return;
}
void mostrarCache(t_part nodo, int orden) {
	if(debugTrace){printf(ACCIAN "\n (mC) mostrarCache"ACRESET"\n");}

	if (partBS) return;

	printf(ACVERDE "\nParticiones de la CACHE ordenadas desde la "ACROJO);
	switch (orden) {
			case ASCEND:
				printf("Primera "ACVERDE"hasta la "ACROJO"Ultima");
				break;
			case DESCEND:
				printf("Ultima "ACVERDE"hasta la "ACROJO"Primera");
				break;
			case AGRANDA:
				printf("Menor "ACVERDE"hasta la "ACROJO"Mayor");
				break;
			case ACHICA:
				printf("Mayor "ACVERDE"hasta la "ACROJO"Menor");
				break;
			default:
				printf("Primera "ACVERDE"hasta la "ACROJO"Ultima");
			}
	printf(ACCIAN".\n");


	int part = 0, partFree = 0, partUsed = 0, memTotal = 0, memFree = 0,
			memUsed = 0;
	;
	while (nodo != NULL /*&& part < 10*/) {
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
	printf(
			"\nMemoria-{"ACROJO"Total"ACCIAN":["ACROJO"%XH"ACCIAN"="ACROJO
			"%dbytes"ACCIAN"]}-{"ACMAGENTA"Ocupada"ACCIAN":["ACMAGENTA"%XH"ACCIAN
			"="ACMAGENTA"%dbytes"ACCIAN"]}-{"ACVERDE"Libre"ACCIAN":["ACVERDE
			"%XH"ACCIAN"="ACVERDE"%dbytes"ACCIAN"]}-{"ACAMARILLO"Configurada"ACCIAN
			":["ACAMARILLO"%XH"ACCIAN"="ACAMARILLO"%dbytes"ACCIAN"]}",
			memTotal, memTotal, memUsed, memUsed, memFree, memFree,
			brokerConf->tamanoMemoria, brokerConf->tamanoMemoria);

	printf( "\nParticiones<"ACROJO"Totales"ACCIAN":["ACROJO"%d"ACCIAN
			"]><"ACMAGENTA"Ocupadas"ACCIAN":["ACMAGENTA"%d"ACCIAN
			"]><"ACVERDE"Libres"ACCIAN":["ACVERDE"%d"ACCIAN"]>",

			part, partUsed, partFree);
}

void removerPartPorOrden(t_part nodo) {
	if(debugTrace){printf(ACCIAN "\n (rPPO) removerPartPorOrden"ACRESET"\n");}


	if (debugCache)
		printf("(rPPO removerPartPorOrden");
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
	if(debugTrace){printf(ACCIAN "\n (rPPT) removerPartPorTamano"ACRESET"\n");}


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
	if(debugTrace){printf(ACCIAN "\n (iPPT) insertarPartPorTamano"ACRESET"\n");}


	t_part partAux;
	if (debugCache) {
		printf(
				"\n(iPPT) al nodo que recibimos tenermos que insertarlo en la lista encolada por tamano");
		printf("\nparticion inicial:");
		mostrarPart(nodo, 9, AGRANDA);
		printf("\n");
	}

	if (partSmall->menor == NULL && partSmall->mayor == NULL) { // hay una sola particion y voy a agregar => pie o cabeza
		if (debugCache)
			printf(
					"\nSe trata de una Lista con un solo nodo. {NULL-[]-NULL} (voy a insertar pie o cabeza por tamano)\n");

		if (partSmall->largo < nodo->largo) {

			if (debugCache) {
				printf("\n               Es cabeza y voy a insertarla");
			}

			insertarCabezaPorTamano(nodo);

			if (debugCache) {
				printf(
						"\n               Ya Inserte como cabeza {NULL-[partSmall]-[nodo]-NULL}  ");

			}
		} else {
			if (debugCache) {
				printf("\n              Es pie y voy a insertarlo");
			}
			insertarPiePorTamano(nodo);

			if (debugCache) {
				printf(
						"\n               Ya Inserte como Pie {NUL-[nodo]-[partSmall]-NULL} ");

			}
		}

	} else {
		if (debugCache)
			printf(
					"\nSe trata de una Lista con mas de un nodo. NULL-[]-NULL (voy a insertar pie, cabeza o medio por tamano)"
							"\n            {NULL-[partSmall]-...-[medio]-...-[partBig]-NULL}"
							"\n         tengo que buscar el mayor para insertarlo \n");

		partAux = encontrarPartMayor(nodo->largo, AGRANDA); //buscamos donde meter el nodo.

		if (!partAux) {
			if (debugCache) {
				printf(
						"\n        No hay ninguno mayor => es cabeza y voy a insertarla");
			}

			insertarCabezaPorTamano(nodo);

			if (debugCache) {
				printf(
						"\n        Ya inserte como cabeza {NULL-[parSmall]-...-[medio]-...-[partBig]-[nodo]-NULL");
			}

		} else if (!partAux->menor) {
			if (debugCache)
				printf(
						"\n        Hay mayor y es el pie => es pie y voy a insertarlo");

			insertarPiePorTamano(nodo);

			if (debugCache)
				printf(
						"\n        Ya inserte como Pie {NULL-nodo-[parSmall]-...-[medio]-...-[partBig]-NULL");

		} else if (!partAux->mayor) {
			if (debugCache)
				printf(
						"\n        Hay mayor y es la cabeza => es medio y voy a insertarlo");

			insertarMedioPorTamano(nodo, partAux);

			if (debugCache)
				printf(
						"\n        Ya inserte como Medio {NULL-[parSmall]-...-[medio]-...-[nodo]-[partBig]-NULL");
		} else {
			if (debugCache)
				printf(
						"\n        Hay mayor y es un medio => es un medio y voy a insertarlo");

			insertarMedioPorTamano(nodo, partAux);

			if (debugCache)
				printf(
						"\n        Ya inserte como Medio {NULL-[parSmall]-...-[nodo]-[medio]-...-[partBig]-NULL");
		}

	}

	if (debugCache) {
		mostrarPart(nodo, 6, AGRANDA);
		mostrarCache(partSmall, AGRANDA);
	}
}

void insertarCabezaPorTamano(t_part nodo) {
	if(debugTrace){printf(ACCIAN "\n (iCPT) insertarCabezaPorTamano"ACRESET"\n");}


	nodo->mayor = NULL;
	nodo->menor = partBig;
	partBig->mayor = nodo;
	partBig = nodo;
}

void insertarPiePorTamano(t_part nodo) {
	if(debugTrace){printf(ACCIAN "\n (iPPT) insertarPiePorTamano"ACRESET"\n");}


	nodo->menor = NULL;
	nodo->mayor = partSmall;
	partSmall->menor = nodo;
	partSmall = nodo;
}

void insertarMedioPorTamano(t_part nodo, t_part medio) {
	if(debugTrace){printf(ACCIAN "\n (iMPT) insertarMedioPorTamano"ACRESET"\n");}


	medio->menor->mayor = nodo;
	nodo->menor = medio->menor;
	nodo->mayor = medio;
	medio->menor = nodo;
}

void removerPiePorTamano() {
	if(debugTrace){printf(ACCIAN "\n (rPPT) removerPiePorTamano"ACRESET"\n");}


	partSmall->mayor->menor = NULL;
	partSmall = partSmall->mayor;
}

void removerCabezaPorTamano() {
	if(debugTrace){printf(ACCIAN "\n (rCPT) removerCabezaPorTamano"ACRESET"\n");}


	partBig->menor->mayor = NULL;
	partBig = partBig->menor;
}

void removerMedioPorTamano(t_part medio) {
	if(debugTrace){printf(ACCIAN "\n (rMPT) removerMedioPorTamano"ACRESET"\n");}


	medio->menor->mayor = medio->mayor;
	medio->mayor->menor = medio->menor;
}

void verificarSuscriptor(t_suscriptor* suscriptor, t_cola* cola) { //esto es para ver si se reconecto o si es nuevo.
	t_suscriptor* suscriptorExistente; //= malloc(sizeof(t_suscriptor));
	int i = 0;
	int flag = 0;

	if (list_size(cola->lista) > 0) {
		for (i = 0; i < list_size(cola->lista); i++) {
			pthread_mutex_lock(&mutexSuscriptor);
			suscriptorExistente = (t_suscriptor*) list_get(cola->lista, i);
			pthread_mutex_unlock(&mutexSuscriptor);
			if ((strcmp(suscriptor->nombreProceso,
					suscriptorExistente->nombreProceso)) == 0) {
				pthread_mutex_lock(&mutexSuscriptor);
				list_replace(cola->lista, i, suscriptor); // a este le tengo que mandar los mensajes que no le envie antes.
				pthread_mutex_unlock(&mutexSuscriptor);
				flag = 1;
				enviarMensajeCacheadoAck(cola, suscriptor); //hay un solo case implementado hasta ahora.
				break;
			}
		}
	}
	if (flag == 0) {
		pthread_mutex_lock(&mutexSuscriptor);
		list_add(cola->lista, suscriptor);
		pthread_mutex_unlock(&mutexSuscriptor);
		printf(
				"estoy agregando al suscriptor a la lista y a punto de enviale un mensaje.\n");
		if (cola->cola > 0) {
			printf("en el if de que hay mensajes cacheados.\n");
			enviarMensajeCacheado(cola, suscriptor);

		}

	}
	//free(suscriptorExistente);
}

void enviarMensajeCacheadoAck(t_cola* cola, t_suscriptor* suscriptor) { //solo un case esta , falta probarlo.

	//t_list* listaAuxiliar = list_create();
	t_administrativo* adminAuxiliar;
	t_suscriptor* suscriptorAuxiliar;
	t_part particion;
	t_bufferOmnidata* bufferLoco = malloc(sizeof(t_bufferOmnidata));
	int desplazamiento = 0;

	int i;
	int j;
	if (list_size(cola->cola) > 0) {

		switch (suscriptor->codigoOperacion) {
		case MENSAJE_NEW_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {
				adminAuxiliar = (t_administrativo*) list_get(cola->cola, i);

				for (j = 0; j < list_size(adminAuxiliar->suscriptoresRecibidos);
						j++) {
					suscriptorAuxiliar = (t_suscriptor*) list_get(
							adminAuxiliar->suscriptoresRecibidos, j);
					if (strcmp(suscriptor->nombreProceso,
							suscriptorAuxiliar->nombreProceso) == 0) {
						printf(
								"ya envié el mensaje y recibi el ACK antes,suscriptor reconectado.");

					} else {
						particion = obtenerMensaje(adminAuxiliar->idMensaje);
						list_add(adminAuxiliar->suscriptoresEnviados,
								suscriptor);
						list_replace(cola->cola, i, adminAuxiliar);
						if (particion != 0) {

							void* miBuffer = malloc(particion->largo + 1); // es + 1 me parece.
							memcpy(miBuffer, cache + particion->inicio,
									particion->largo);

							memcpy(&bufferLoco->largoNombre,
									miBuffer + desplazamiento,
									sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

							bufferLoco->nombrePokemon = malloc(
									bufferLoco->largoNombre + 1);

							memcpy(bufferLoco->nombrePokemon,
									miBuffer + desplazamiento,
									bufferLoco->largoNombre);
							desplazamiento += bufferLoco->largoNombre;

							bufferLoco->nombrePokemon[bufferLoco->largoNombre] =
									'\0';

							memcpy(&bufferLoco->cantidadPokemons,
									miBuffer + desplazamiento,
									sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);
							memcpy(&bufferLoco->posX, miBuffer + desplazamiento,
									sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);
							memcpy(&bufferLoco->posY, miBuffer + desplazamiento,
									sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

							printf("largo del mensaje :%d\n",
									bufferLoco->largoNombre);
							printf("posX %d\n", bufferLoco->posX);
							printf("posY %d\n", bufferLoco->posY);
							printf("cantidad de pokemons %d \n",
									bufferLoco->cantidadPokemons);
							printf(
									"el mensaje recuperado de la cache es : %s\n",
									bufferLoco->nombrePokemon);
							printf("largo del mensaje %d", desplazamiento);

							enviarMensajeGameCardNewPokemon(
									bufferLoco->nombrePokemon, bufferLoco->posX,
									bufferLoco->posY,
									bufferLoco->cantidadPokemons,
									bufferLoco->idMensaje, suscriptor->socket);

//							enviarMensajeBrokerNew(bufferLoco->nombrePokemon,
//									bufferLoco->posX, bufferLoco->posY,
//									bufferLoco->cantidadPokemons,
//									suscriptor->socket);
						} else {
							log_error(logger,
									"Particion igual a 0, no encontre mensaje.");
							return;
						}
					}
				}
			}
			break;
		}
		case MENSAJE_APPEARED_POKEMON: {

			for (i = 0; i < list_size(cola->cola); i++) {
				adminAuxiliar = (t_administrativo*) list_get(cola->cola, i);

				for (j = 0; j < list_size(adminAuxiliar->suscriptoresRecibidos);
						j++) {
					suscriptorAuxiliar = (t_suscriptor*) list_get(
							adminAuxiliar->suscriptoresRecibidos, j);
					if (strcmp(suscriptor->nombreProceso,
							suscriptorAuxiliar->nombreProceso) == 0) {
						printf(
								"ya envié el mensaje y recibi el ACK antes,suscriptor reconectado.");

					} else {
						particion = obtenerMensaje(adminAuxiliar->idMensaje);
						list_add(adminAuxiliar->suscriptoresEnviados,
								suscriptor);
						list_replace(cola->cola, i, adminAuxiliar);
						if (particion != 0) {

							void* miBuffer = malloc(particion->largo + 1); // es + 1 me parece.
							memcpy(miBuffer, cache + particion->inicio,
									particion->largo);

							memcpy(&bufferLoco->largoNombre,
									miBuffer + desplazamiento,
									sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

							bufferLoco->nombrePokemon = malloc(
									bufferLoco->largoNombre + 1);

							memcpy(bufferLoco->nombrePokemon,
									miBuffer + desplazamiento,
									bufferLoco->largoNombre);
							desplazamiento += bufferLoco->largoNombre;

							bufferLoco->nombrePokemon[bufferLoco->largoNombre] =
									'\0';

							memcpy(&bufferLoco->posX, miBuffer + desplazamiento,
									sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);
							memcpy(&bufferLoco->posY, miBuffer + desplazamiento,
									sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

							printf("largo del mensaje :%d\n",
									bufferLoco->largoNombre);
							printf("posX %d\n", bufferLoco->posX);
							printf("posY %d\n", bufferLoco->posY);
							printf("cantidad de pokemons %d \n",
									bufferLoco->cantidadPokemons);
							printf(
									"el mensaje recuperado de la cache es : %s\n",
									bufferLoco->nombrePokemon);
							printf("largo del mensaje %d", desplazamiento);

							enviarMensajeBrokerAppearedId(
									bufferLoco->nombrePokemon, bufferLoco->posX,
									bufferLoco->posY, bufferLoco->idMensaje,
									bufferLoco->idMensajeCorrelativo,
									suscriptor->socket);

						} else {
							log_error(logger,
									"Particion igual a 0, no encontre mensaje.");
							return;
						}
					}
				}
			}

			break;
		}
		case MENSAJE_CATCH_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {
				adminAuxiliar = (t_administrativo*) list_get(cola->cola, i);

				for (j = 0; j < list_size(adminAuxiliar->suscriptoresRecibidos);
						j++) {
					suscriptorAuxiliar = (t_suscriptor*) list_get(
							adminAuxiliar->suscriptoresRecibidos, j);
					if (strcmp(suscriptor->nombreProceso,
							suscriptorAuxiliar->nombreProceso) == 0) {
						printf(
								"ya envié el mensaje y recibi el ACK antes,suscriptor reconectado.");

					} else {
						particion = obtenerMensaje(adminAuxiliar->idMensaje);
						list_add(adminAuxiliar->suscriptoresEnviados,
								suscriptor);
						list_replace(cola->cola, i, adminAuxiliar);
						if (particion != 0) {

							void* miBuffer = malloc(particion->largo + 1); // es + 1 me parece.
							memcpy(miBuffer, cache + particion->inicio,
									particion->largo);

							memcpy(&bufferLoco->largoNombre,
									miBuffer + desplazamiento,
									sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

							bufferLoco->nombrePokemon = malloc(
									bufferLoco->largoNombre + 1);

							memcpy(bufferLoco->nombrePokemon,
									miBuffer + desplazamiento,
									bufferLoco->largoNombre);
							desplazamiento += bufferLoco->largoNombre;

							bufferLoco->nombrePokemon[bufferLoco->largoNombre] =
									'\0';

							desplazamiento += sizeof(uint32_t);
							memcpy(&bufferLoco->posX, miBuffer + desplazamiento,
									sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);
							memcpy(&bufferLoco->posY, miBuffer + desplazamiento,
									sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

							printf("largo del mensaje :%d\n",
									bufferLoco->largoNombre);
							printf("posX %d\n", bufferLoco->posX);
							printf("posY %d\n", bufferLoco->posY);
							printf("cantidad de pokemons %d \n",
									bufferLoco->cantidadPokemons);
							printf(
									"el mensaje recuperado de la cache es : %s\n",
									bufferLoco->nombrePokemon);
							printf("largo del mensaje %d", desplazamiento);

							enviarMensajeGameCardCatch(
									bufferLoco->nombrePokemon, bufferLoco->posX,
									bufferLoco->posY, bufferLoco->idMensaje,
									suscriptor->socket);

						} else {
							log_error(logger,
									"Particion igual a 0, no encontre mensaje.");
							return;
						}
					}
				}
			}

			break;
		}
		case MENSAJE_CAUGHT_POKEMON: {

			for (i = 0; i < list_size(cola->cola); i++) {
				adminAuxiliar = (t_administrativo*) list_get(cola->cola, i);

				for (j = 0; j < list_size(adminAuxiliar->suscriptoresRecibidos);
						j++) {
					suscriptorAuxiliar = (t_suscriptor*) list_get(
							adminAuxiliar->suscriptoresRecibidos, j);
					if (strcmp(suscriptor->nombreProceso,
							suscriptorAuxiliar->nombreProceso) == 0) {
						printf(
								"ya envié el mensaje y recibi el ACK antes,suscriptor reconectado.");

					} else {
						particion = obtenerMensaje(adminAuxiliar->idMensaje);
						list_add(adminAuxiliar->suscriptoresEnviados,
								suscriptor);
						list_replace(cola->cola, i, adminAuxiliar);
						if (particion != 0) {

							void* miBuffer = malloc(particion->largo);
							memcpy(miBuffer, cache + particion->inicio,
									particion->largo);

							memcpy(&bufferLoco->boolean,
									miBuffer + desplazamiento,
									sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

//							printf("largo del mensaje :%d\n",
//									bufferLoco->largoNombre);
//							printf("posX %d\n", bufferLoco->posX);
//							printf("posY %d\n", bufferLoco->posY);
//							printf("cantidad de pokemons %d \n",
//									bufferLoco->cantidadPokemons);
//							printf(
//									"el mensaje recuperado de la cache es : %s\n",
//									bufferLoco->nombrePokemon);
//							printf("largo del mensaje %d", desplazamiento);

							enviarMensajeBrokerCaughtId(bufferLoco->idMensaje,
									bufferLoco->idMensajeCorrelativo,
									bufferLoco->boolean, suscriptor->socket);
						} else {
							log_error(logger,
									"Particion igual a 0, no encontre mensaje.");
							return;
						}
					}
				}
			}

			break;
		}
		case MENSAJE_GET_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {
				adminAuxiliar = (t_administrativo*) list_get(cola->cola, i);

				for (j = 0; j < list_size(adminAuxiliar->suscriptoresRecibidos);
						j++) {
					suscriptorAuxiliar = (t_suscriptor*) list_get(
							adminAuxiliar->suscriptoresRecibidos, j);
					if (strcmp(suscriptor->nombreProceso,
							suscriptorAuxiliar->nombreProceso) == 0) {
						printf(
								"ya envié el mensaje y recibi el ACK antes,suscriptor reconectado.");

					} else {
						particion = obtenerMensaje(adminAuxiliar->idMensaje);
						list_add(adminAuxiliar->suscriptoresEnviados,
								suscriptor);
						list_replace(cola->cola, i, adminAuxiliar);
						if (particion != 0) {

							void* miBuffer = malloc(particion->largo + 1); // es + 1 me parece.
							memcpy(miBuffer, cache + particion->inicio,
									particion->largo);

							memcpy(&bufferLoco->largoNombre,
									miBuffer + desplazamiento,
									sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

							bufferLoco->nombrePokemon = malloc(
									bufferLoco->largoNombre + 1);

							memcpy(bufferLoco->nombrePokemon,
									miBuffer + desplazamiento,
									bufferLoco->largoNombre);
							desplazamiento += bufferLoco->largoNombre;

							bufferLoco->nombrePokemon[bufferLoco->largoNombre] =
									'\0';

							printf("largo del mensaje :%d\n",
									bufferLoco->largoNombre);
//							printf("posX %d\n", bufferLoco->posX);
//							printf("posY %d\n", bufferLoco->posY);
//							printf("cantidad de pokemons %d \n",
//									bufferLoco->cantidadPokemons);
							printf(
									"el mensaje recuperado de la cache es : %s\n",
									bufferLoco->nombrePokemon);
							printf("largo del mensaje %d", desplazamiento);

							enviarMensajeGameCardGetPokemon(
									bufferLoco->nombrePokemon,
									bufferLoco->idMensaje, suscriptor->socket);
						} else {
							log_error(logger,
									"Particion igual a 0, no encontre mensaje.");
							return;
						}
					}
				}
			}

			break;
		}
		case MENSAJE_LOCALIZED_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {
				adminAuxiliar = (t_administrativo*) list_get(cola->cola, i);

				for (j = 0; j < list_size(adminAuxiliar->suscriptoresRecibidos);
						j++) {
					suscriptorAuxiliar = (t_suscriptor*) list_get(
							adminAuxiliar->suscriptoresRecibidos, j);
					if (strcmp(suscriptor->nombreProceso,
							suscriptorAuxiliar->nombreProceso) == 0) {
						printf(
								"ya envié el mensaje y recibi el ACK antes,suscriptor reconectado.");

					} else {
						particion = obtenerMensaje(adminAuxiliar->idMensaje);
						list_add(adminAuxiliar->suscriptoresEnviados,
								suscriptor);
						list_replace(cola->cola, i, adminAuxiliar);
						if (particion != 0) {

							void* miBuffer = malloc(particion->largo);
							memcpy(miBuffer, cache + particion->inicio,
									particion->largo);

							memcpy(&bufferLoco->largoNombre,
									miBuffer + desplazamiento,
									sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);
							bufferLoco->nombrePokemon = malloc(
									bufferLoco->largoNombre + 1);
							memcpy(bufferLoco->nombrePokemon,
									miBuffer + desplazamiento,
									bufferLoco->largoNombre);
							desplazamiento += bufferLoco->largoNombre;
							bufferLoco->nombrePokemon[bufferLoco->largoNombre] =
									'\0';
							bufferLoco->listaCoordenadas = list_create();

							memcpy(
									&bufferLoco->listaCoordenadas->elements_count,
									miBuffer + desplazamiento,
									sizeof(uint32_t));
							desplazamiento += sizeof(uint32_t);

							int j;
							for (j = 0;
									j
											< bufferLoco->listaCoordenadas->elements_count;
									j++) {
								uint32_t auxiliar = 0;
								memcpy(&auxiliar, miBuffer + desplazamiento,
										sizeof(uint32_t));
								desplazamiento += sizeof(uint32_t);
								list_add(bufferLoco->listaCoordenadas,
										(void*) auxiliar);

							}

							printf("largo del mensaje :%d\n",
									bufferLoco->largoNombre);
							printf("posX %d\n", bufferLoco->posX);
							printf("posY %d\n", bufferLoco->posY);
							printf("cantidad de pokemons %d \n",
									bufferLoco->cantidadPokemons);
							printf(
									"el mensaje recuperado de la cache es : %s\n",
									bufferLoco->nombrePokemon);
							printf("largo del mensaje %d", desplazamiento);

							enviarMensajeLocalizedId(bufferLoco->nombrePokemon,
									bufferLoco->listaCoordenadas,
									bufferLoco->idMensaje,
									bufferLoco->idMensajeCorrelativo,
									suscriptor->socket);
						} else {
							log_error(logger,
									"Particion igual a 0, no encontre mensaje.");
							return;
						}
					}
				}
			}

			break;
		}
		default: {
			printf("algo para el default, no entró en ningún switch.\n");
		}
		}

	}
}
void enviarMensajeCacheado(t_cola* cola, t_suscriptor* suscriptor) { //no hace falta que me devuelva algo, void me parece.
	t_part particion;
	t_partBuddy* particionBuddy;
	t_administrativo* mensaje;
	t_bufferOmnidata* bufferLoco = malloc(sizeof(t_bufferOmnidata));
	int desplazamiento = 0;
	int i;
	void* miBuffer;
	if (list_size(cola->cola) > 0) {

		printf("rompo1\n");

		switch (suscriptor->codigoOperacion) {
		case MENSAJE_NEW_POKEMON: {
			printf("rompo2\n");
			for (i = 0; i < list_size(cola->cola); i++) {
				printf("rompo3\n");
				pthread_mutex_lock(&mutexQueueNew);
				mensaje = (t_administrativo*) list_get(cola->cola, i);
				pthread_mutex_unlock(&mutexQueueNew);

				pthread_mutex_lock(&mutexSuscriptor);
				list_add(mensaje->suscriptoresEnviados, suscriptor);
				pthread_mutex_unlock(&mutexSuscriptor);

				pthread_mutex_lock(&mutexQueueNew);
				list_replace(cola->cola, i, mensaje);
				pthread_mutex_unlock(&mutexQueueNew);

				if (strcmp(brokerConf->algoritmoMemoria, "PARTICIONES") == 0) {
					pthread_mutex_lock(&mutexCache);
					particion = obtenerMensaje(mensaje->idMensaje);

					printf(
							"Particion Inicio:%d Particion Fin:%d Particion Size:%d Particion Estado:%d Particion Id:%d \n",
							particion->inicio, particion->fin, particion->largo,
							particion->estado, particion->idMensaje);
					if (particion != 0) {
						printf("rompo4\n");
						miBuffer = malloc(particion->largo);
						memcpy(miBuffer, cache + particion->inicio,
								particion->largo);

					} else {
						pthread_mutex_unlock(&mutexCache);
						log_error(logger,
								"Particion igual a 0, no encontre mensaje.");
						return;
					}
					pthread_mutex_unlock(&mutexCache);

				}
				if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {
					printf("estoy en buddy a punto de obtener un mensaje .\n");
					pthread_mutex_lock(&mutexCache);
					particionBuddy = obtenerMensajeBuddy(mensaje->idMensaje);

					if (particionBuddy != NULL) {

						printf("largo de la particion es : %d .\n",
								particionBuddy->tamanio);

						miBuffer = malloc(particionBuddy->tamanioMensaje);

						printf("largo del mensaje es : %d .\n",
								particionBuddy->tamanioMensaje);
						memcpy(miBuffer,
								cache
										+ particionBuddy->posicionParticion,
								particionBuddy->tamanioMensaje);

					}
					pthread_mutex_unlock(&mutexCache);

				}

//				if (particion == 0) { //si la particion no existe, es que el mensaje se borro.
//					//habria que borrar el t_administrativo de la cola.
//
//				}
				memcpy(&bufferLoco->largoNombre, miBuffer + desplazamiento,
						sizeof(uint32_t));
				desplazamiento += sizeof(uint32_t);

				bufferLoco->nombrePokemon = malloc(bufferLoco->largoNombre + 1);

				memcpy(bufferLoco->nombrePokemon, miBuffer + desplazamiento,
						bufferLoco->largoNombre);
				desplazamiento += bufferLoco->largoNombre;

				bufferLoco->nombrePokemon[bufferLoco->largoNombre] = '\0';

				memcpy(&bufferLoco->cantidadPokemons, miBuffer + desplazamiento,
						sizeof(uint32_t));
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

				printf("el socket es :%d\n", suscriptor->socket);
				printf("el id del mensaje es %d.\n", mensaje->idMensaje);

				enviarMensajeGameCardNewPokemon(bufferLoco->nombrePokemon,
						bufferLoco->posX, bufferLoco->posY,
						bufferLoco->cantidadPokemons, mensaje->idMensaje,
						suscriptor->socket);

//				enviarMensajeBrokerNew(bufferLoco->nombrePokemon,
//						bufferLoco->posX, bufferLoco->posY,
//						bufferLoco->cantidadPokemons, suscriptor->socket);

				printf("rompo enviarGameCardNew.\n");
			}
			break;
		}
		case MENSAJE_APPEARED_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {
				pthread_mutex_lock(&mutexQueueAppeared);
				mensaje = list_get(cola->cola, i);
				pthread_mutex_unlock(&mutexQueueAppeared);

				pthread_mutex_lock(&mutexSuscriptor);
				list_add(mensaje->suscriptoresEnviados, suscriptor);
				pthread_mutex_unlock(&mutexSuscriptor);

				pthread_mutex_lock(&mutexQueueAppeared);
				list_replace(cola->cola, i, mensaje);
				pthread_mutex_unlock(&mutexQueueAppeared);

				if (strcmp(brokerConf->algoritmoMemoria, "PARTICIONES") == 0) {
					pthread_mutex_lock(&mutexCache);
					particion = obtenerMensaje(mensaje->idMensaje);

					printf(
							"Particion Inicio:%d Particion Fin:%d Particion Size:%d Particion Estado:%d Particion Id:%d \n",
							particion->inicio, particion->fin, particion->largo,
							particion->estado, particion->idMensaje);
					if (particion != 0) {

						printf("rompo4\n");
						printf("Particion largo : %d\n", particion->largo);
						miBuffer = malloc(particion->largo); //Todo . Valgrind tira error!
						memcpy(miBuffer, cache + particion->inicio,
								particion->largo);

					} else {
						pthread_mutex_unlock(&mutexCache);
						log_error(logger,
								"Particion igual a 0, no encontre mensaje.");
						return;
					}
					pthread_mutex_unlock(&mutexCache);

				}
				if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {
					printf("estoy en buddy a punto de obtener un mensaje .\n");
					pthread_mutex_lock(&mutexCache);

					particionBuddy = obtenerMensajeBuddy(mensaje->idMensaje);

					if (particionBuddy != 0) {

						printf("largo de la particion es : %d .\n",
								particionBuddy->tamanioMensaje);

						miBuffer = malloc(particionBuddy->tamanioMensaje);

						printf("largo del mensaje es : %d .\n",
								particionBuddy->tamanioMensaje);
						memcpy(miBuffer,
								cache
										+ particionBuddy->posicionParticion,
								particionBuddy->tamanioMensaje);

					}
					pthread_mutex_unlock(&mutexCache);

				}
				if (particion == 0) { //si la particion no existe, es que el mensaje se borro.
					//habria que borrar el t_administrativo de la cola.

				}

				memcpy(&bufferLoco->largoNombre, miBuffer + desplazamiento,
						sizeof(uint32_t)); //Todo . Valgrind tira error!
				desplazamiento += sizeof(uint32_t);

				bufferLoco->nombrePokemon = malloc(bufferLoco->largoNombre + 1);
				memcpy(bufferLoco->nombrePokemon, miBuffer + desplazamiento,
						bufferLoco->largoNombre);
				desplazamiento += bufferLoco->largoNombre;

				bufferLoco->nombrePokemon[bufferLoco->largoNombre] = '\0';

				memcpy(&bufferLoco->posX, miBuffer + desplazamiento,
						sizeof(uint32_t)); //Todo . Valgrind tira error!
				desplazamiento += sizeof(uint32_t);
				memcpy(&bufferLoco->posY, miBuffer + desplazamiento,
						sizeof(uint32_t)); //Todo . Valgrind tira error!
				desplazamiento += sizeof(uint32_t);

				printf("largo del mensaje :%d\n", bufferLoco->largoNombre);
				printf("posX %d\n", bufferLoco->posX);
				printf("posY %d\n", bufferLoco->posY);
				printf("cantidad de pokemons %d \n",
						bufferLoco->cantidadPokemons); //Todo . Valgrind tira error!
				printf("el mensaje recuperado de la cache es : %s\n",
						bufferLoco->nombrePokemon);
				printf("largo del mensaje %d", desplazamiento);

				enviarMensajeBrokerAppearedId(bufferLoco->nombrePokemon,
						bufferLoco->posX, bufferLoco->posY,
						bufferLoco->idMensaje, bufferLoco->idMensajeCorrelativo,
						suscriptor->socket);

			}

			break;
		}
		case MENSAJE_CATCH_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {
				pthread_mutex_lock(&mutexQueueCatch);
				mensaje = list_get(cola->cola, i);
				pthread_mutex_unlock(&mutexQueueCatch);

				pthread_mutex_lock(&mutexSuscriptor);
				list_add(mensaje->suscriptoresEnviados, suscriptor);
				pthread_mutex_unlock(&mutexSuscriptor);

				pthread_mutex_lock(&mutexQueueCatch);
				list_replace(cola->cola, i, mensaje);
				pthread_mutex_unlock(&mutexQueueCatch);

				if (strcmp(brokerConf->algoritmoMemoria, "PARTICIONES") == 0) {
					pthread_mutex_lock(&mutexCache);

					particion = obtenerMensaje(mensaje->idMensaje);

					printf(
							"Particion Inicio:%d Particion Fin:%d Particion Size:%d Particion Estado:%d Particion Id:%d \n",
							particion->inicio, particion->fin, particion->largo,
							particion->estado, particion->idMensaje);
					if (particion != 0) {
						printf("rompo4\n");
						miBuffer = malloc(particion->largo);
						memcpy(miBuffer, cache + particion->inicio,
								particion->largo);

					} else {
						pthread_mutex_unlock(&mutexCache);
						log_error(logger,
								"Particion igual a 0, no encontre mensaje.");
						return;
					}
					pthread_mutex_unlock(&mutexCache);

				}
				if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {
					printf("estoy en buddy a punto de obtener un mensaje .\n");
					pthread_mutex_lock(&mutexCache);

					particionBuddy = obtenerMensajeBuddy(mensaje->idMensaje);

					if (particionBuddy != 0) {

						printf("largo de la particion es : %d .\n",
								particionBuddy->tamanioMensaje);

						miBuffer = malloc(particionBuddy->tamanioMensaje);

						printf("largo del mensaje es : %d .\n",
								particionBuddy->tamanioMensaje);
						memcpy(miBuffer,
								cache
										+ particionBuddy->posicionParticion,
								particionBuddy->tamanioMensaje);

					}
					pthread_mutex_unlock(&mutexCache);

				}
				if (particion == 0) { //si la particion no existe, es que el mensaje se borro.
					//habria que borrar el t_administrativo de la cola.

				}

				memcpy(&bufferLoco->largoNombre, miBuffer + desplazamiento,
						sizeof(uint32_t));
				desplazamiento += sizeof(uint32_t);
				bufferLoco->nombrePokemon = malloc(bufferLoco->largoNombre + 1);
				memcpy(bufferLoco->nombrePokemon, miBuffer + desplazamiento,
						bufferLoco->largoNombre);
				desplazamiento += bufferLoco->largoNombre;
				bufferLoco->nombrePokemon[bufferLoco->largoNombre] = '\0';

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

				enviarMensajeGameCardCatch(bufferLoco->nombrePokemon,
						bufferLoco->posX, bufferLoco->posY,
						bufferLoco->idMensaje, suscriptor->socket);

			}

			break;
		}
		case MENSAJE_CAUGHT_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {

				pthread_mutex_lock(&mutexQueueCaught);
				mensaje = list_get(cola->cola, i);
				pthread_mutex_unlock(&mutexQueueCaught);

				pthread_mutex_lock(&mutexSuscriptor);
				list_add(mensaje->suscriptoresEnviados, suscriptor);
				pthread_mutex_unlock(&mutexSuscriptor);

				pthread_mutex_lock(&mutexQueueCaught);
				list_replace(cola->cola, i, mensaje);
				pthread_mutex_unlock(&mutexQueueCaught);

				if (strcmp(brokerConf->algoritmoMemoria, "PARTICIONES") == 0) {
					pthread_mutex_lock(&mutexCache);

					particion = obtenerMensaje(mensaje->idMensaje);

					printf(
							"Particion Inicio:%d Particion Fin:%d Particion Size:%d Particion Estado:%d Particion Id:%d \n",
							particion->inicio, particion->fin, particion->largo,
							particion->estado, particion->idMensaje);
					if (particion != 0) {
						printf("rompo4\n");
						miBuffer = malloc(particion->largo);
						memcpy(miBuffer, cache + particion->inicio,
								particion->largo);

					} else {
						pthread_mutex_unlock(&mutexCache);
						log_error(logger,
								"Particion igual a 0, no encontre mensaje.");
						return;
					}
					pthread_mutex_unlock(&mutexCache);

				}
				if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {
					printf("estoy en buddy a punto de obtener un mensaje .\n");
					pthread_mutex_lock(&mutexCache);

					particionBuddy = obtenerMensajeBuddy(mensaje->idMensaje);

					if (particionBuddy != 0) {

						printf("largo de la particion es : %d .\n",
								particionBuddy->tamanioMensaje);

						miBuffer = malloc(particionBuddy->tamanioMensaje);

						printf("largo del mensaje es : %d .\n",
								particionBuddy->tamanioMensaje);
						memcpy(miBuffer,
								cache
										+ particionBuddy->posicionParticion,
								particionBuddy->tamanioMensaje);

					}
					pthread_mutex_unlock(&mutexCache);

				}
				if (particion == 0) { //si la particion no existe, es que el mensaje se borro.
					//habria que borrar el t_administrativo de la cola.

				}

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

				enviarMensajeBrokerCaughtId(bufferLoco->idMensaje,
						bufferLoco->idMensajeCorrelativo, bufferLoco->boolean,
						suscriptor->socket);

			}

			break;
		}
		case MENSAJE_GET_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {

				pthread_mutex_lock(&mutexQueueGet);
				mensaje = list_get(cola->cola, i);
				pthread_mutex_unlock(&mutexQueueGet);

				pthread_mutex_lock(&mutexSuscriptor);
				list_add(mensaje->suscriptoresEnviados, suscriptor);
				pthread_mutex_unlock(&mutexSuscriptor);

				pthread_mutex_lock(&mutexQueueGet);
				list_replace(cola->cola, i, mensaje);
				pthread_mutex_unlock(&mutexQueueGet);

				if (strcmp(brokerConf->algoritmoMemoria, "PARTICIONES") == 0) {
					pthread_mutex_lock(&mutexCache);

					particion = obtenerMensaje(mensaje->idMensaje);

					printf(
							"Particion Inicio:%d Particion Fin:%d Particion Size:%d Particion Estado:%d Particion Id:%d \n",
							particion->inicio, particion->fin, particion->largo,
							particion->estado, particion->idMensaje);
					if (particion != 0) {
						printf("rompo4\n");
						miBuffer = malloc(particion->largo);
						memcpy(miBuffer, cache + particion->inicio,
								particion->largo);

					} else {
						pthread_mutex_unlock(&mutexCache);
						log_error(logger,
								"Particion igual a 0, no encontre mensaje.");
						return;
					}
					pthread_mutex_unlock(&mutexCache);

				}
				if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {
					printf("estoy en buddy a punto de obtener un mensaje .\n");
					pthread_mutex_lock(&mutexCache);

					particionBuddy = obtenerMensajeBuddy(mensaje->idMensaje);

					if (particionBuddy != 0) {

						printf("largo de la particion es : %d .\n",
								particionBuddy->tamanioMensaje);

						miBuffer = malloc(particionBuddy->tamanioMensaje);

						printf("largo del mensaje es : %d .\n",
								particionBuddy->tamanioMensaje);
						memcpy(miBuffer,
								cache
										+ particionBuddy->posicionParticion,
								particionBuddy->tamanioMensaje);

					}
					pthread_mutex_unlock(&mutexCache);

				}
				if (particion == 0) { //si la particion no existe, es que el mensaje se borro.
					//habria que borrar el t_administrativo de la cola.

				}

				memcpy(&bufferLoco->largoNombre, miBuffer + desplazamiento,
						sizeof(uint32_t));
				desplazamiento += sizeof(uint32_t);
				bufferLoco->nombrePokemon = malloc(bufferLoco->largoNombre + 1);
				memcpy(bufferLoco->nombrePokemon, miBuffer + desplazamiento,
						bufferLoco->largoNombre);
				desplazamiento += bufferLoco->largoNombre;
				bufferLoco->nombrePokemon[bufferLoco->largoNombre] = '\0';
				printf("largo del mensaje :%d\n", bufferLoco->largoNombre);
				printf("posX %d\n", bufferLoco->posX);
				printf("posY %d\n", bufferLoco->posY);
				printf("cantidad de pokemons %d \n",
						bufferLoco->cantidadPokemons);
				printf("el mensaje recuperado de la cache es : %s\n",
						bufferLoco->nombrePokemon);
				printf("largo del mensaje %d", desplazamiento);

				enviarMensajeGameCardGetPokemon(bufferLoco->nombrePokemon,
						bufferLoco->idMensaje, suscriptor->socket);

			}

			break;
		}
		case MENSAJE_LOCALIZED_POKEMON: {
			for (i = 0; i < list_size(cola->cola); i++) {

				pthread_mutex_lock(&mutexQueueLocalized);
				mensaje = list_get(cola->cola, i);
				pthread_mutex_unlock(&mutexQueueLocalized);

				pthread_mutex_lock(&mutexSuscriptor);
				list_add(mensaje->suscriptoresEnviados, suscriptor);
				pthread_mutex_unlock(&mutexSuscriptor);

				pthread_mutex_lock(&mutexQueueLocalized);
				list_replace(cola->cola, i, mensaje);
				pthread_mutex_unlock(&mutexQueueLocalized);

				if (strcmp(brokerConf->algoritmoMemoria, "PARTICIONES") == 0) {
					pthread_mutex_lock(&mutexCache);

					particion = obtenerMensaje(mensaje->idMensaje);

					printf(
							"Particion Inicio:%d Particion Fin:%d Particion Size:%d Particion Estado:%d Particion Id:%d \n",
							particion->inicio, particion->fin, particion->largo,
							particion->estado, particion->idMensaje);
					if (particion != 0) {
						printf("rompo4\n");
						miBuffer = malloc(particion->largo);
						memcpy(miBuffer, cache + particion->inicio,
								particion->largo);

					} else {
						pthread_mutex_unlock(&mutexCache);
						log_error(logger,
								"Particion igual a 0, no encontre mensaje.");
						return;
					}
					pthread_mutex_unlock(&mutexCache);

				}
				if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {
					printf("estoy en buddy a punto de obtener un mensaje .\n");
					pthread_mutex_lock(&mutexCache);

					particionBuddy = obtenerMensajeBuddy(mensaje->idMensaje);

					if (particionBuddy != 0) {

						printf("largo de la particion es : %d .\n",
								particionBuddy->tamanioMensaje);

						miBuffer = malloc(particionBuddy->tamanioMensaje);

						printf("largo del mensaje es : %d .\n",
								particionBuddy->tamanioMensaje);
						memcpy(miBuffer,
								cache
										+ particionBuddy->posicionParticion,
								particionBuddy->tamanioMensaje);

					}
					pthread_mutex_unlock(&mutexCache);

				}
				if (particion == 0) { //si la particion no existe, es que el mensaje se borro.
					//habria que borrar el t_administrativo de la cola.

				}

				memcpy(&bufferLoco->largoNombre, miBuffer + desplazamiento,
						sizeof(uint32_t));
				desplazamiento += sizeof(uint32_t);
				bufferLoco->nombrePokemon = malloc(bufferLoco->largoNombre + 1);
				memcpy(bufferLoco->nombrePokemon, miBuffer + desplazamiento,
						bufferLoco->largoNombre);
				desplazamiento += bufferLoco->largoNombre;
				bufferLoco->nombrePokemon[bufferLoco->largoNombre] = '\0';
				bufferLoco->listaCoordenadas = list_create();

				memcpy(&bufferLoco->listaCoordenadas->elements_count,
						miBuffer + desplazamiento, sizeof(uint32_t));
				desplazamiento += sizeof(uint32_t);

				int j;
				for (j = 0; j < bufferLoco->listaCoordenadas->elements_count;
						j++) {
					uint32_t auxiliar = 0;
					memcpy(&auxiliar, miBuffer + desplazamiento,
							sizeof(uint32_t));
					desplazamiento += sizeof(uint32_t);
					list_add(bufferLoco->listaCoordenadas, (void*) auxiliar);

				}

				printf("largo del mensaje :%d\n", bufferLoco->largoNombre);
				printf("posX %d\n", bufferLoco->posX);
				printf("posY %d\n", bufferLoco->posY);
				printf("cantidad de pokemons %d \n",
						bufferLoco->cantidadPokemons);
				printf("el mensaje recuperado de la cache es : %s\n",
						bufferLoco->nombrePokemon);
				printf("largo del mensaje %d", desplazamiento);

				enviarMensajeLocalizedId(bufferLoco->nombrePokemon,
						bufferLoco->listaCoordenadas, bufferLoco->idMensaje,
						bufferLoco->idMensajeCorrelativo, suscriptor->socket);

			}

			break;
		}
		default: {
			printf("no se reconoce el mensaje o cola o suscriptor.\n");

		}
		}

	}

	free(bufferLoco);
	return;
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
		//for (i = 0; i < list_size(lista); i++) {
		//log_debug(logger, "LIST SIZE :%d", list_size(lista));
		switch (mensaje->codigoOperacion) {
		case MENSAJE_NEW_POKEMON: {
			for (i = 0; i < list_size(lista); i++) {
				suscriptorExistente = (t_suscriptor*) list_get(lista, i);
				log_debug(logger, "NOMBRE SUSCRIPTOR: %s",
						suscriptorExistente->nombreProceso);

				enviarMensajeGameCardNewPokemon(mensaje->buffer->nombrePokemon,
						mensaje->buffer->posX, mensaje->buffer->posY,
						mensaje->buffer->cantidadPokemons,
						mensaje->buffer->idMensaje,
						suscriptorExistente->socket);

//				enviarMensajeBrokerNew(mensaje->buffer->nombrePokemon,
//						mensaje->buffer->posX, mensaje->buffer->posY,
//						mensaje->buffer->cantidadPokemons, suscriptorExistente->socket);
				log_info(logEntrega, "Envío un mensaje al suscriptor : %s.\n",
						suscriptorExistente->nombreProceso);
				list_add(mensajeAdmin->suscriptoresEnviados,
						(void*) suscriptorExistente);
			}
			break;
		}
		case MENSAJE_APPEARED_POKEMON: {
			for (i = 0; i < list_size(lista); i++) {
				log_debug(logger, "ITERACION %d", i);
				suscriptorExistente = (t_suscriptor*) list_get(lista, i);
				log_debug(logger, "NOMBRE SUSCRIPTOR: %s",
						suscriptorExistente->nombreProceso);

				enviarMensajeBrokerAppearedId(mensaje->buffer->nombrePokemon,
						mensaje->buffer->posX, mensaje->buffer->posY,
						mensaje->buffer->idMensaje,
						mensaje->buffer->idMensajeCorrelativo,
						suscriptorExistente->socket);

//				enviarMensajeBrokerAppeared(mensaje->buffer->nombrePokemon,
//						mensaje->buffer->posX, mensaje->buffer->posY,
//						mensaje->buffer->idMensajeCorrelativo,
//						suscriptorExistente->socket);

				log_info(logEntrega, "Envío un mensaje al suscriptor : %s.\n",
						suscriptorExistente->nombreProceso);
				list_add(mensajeAdmin->suscriptoresEnviados,
						(void*) suscriptorExistente);
			}
			break;
		}
		case MENSAJE_GET_POKEMON: {
//			enviarMensajeBrokerGet(mensaje->buffer->nombrePokemon,
//					suscriptorExistente->socket);
			for (i = 0; i < list_size(lista); i++) {
				suscriptorExistente = (t_suscriptor*) list_get(lista, i);
				enviarMensajeGameCardGetPokemon(mensaje->buffer->nombrePokemon,
						mensaje->buffer->idMensaje,
						suscriptorExistente->socket);
				log_info(logEntrega, "Envío un mensaje al suscriptor : %s.\n",
						suscriptorExistente->nombreProceso);
				list_add(mensajeAdmin->suscriptoresEnviados,
						(void*) suscriptorExistente);
			}
			break;

		}
		case MENSAJE_CATCH_POKEMON: {
//				enviarMensajeBrokerCatch(mensaje->buffer->nombrePokemon,
//						mensaje->buffer->posX, mensaje->buffer->posY,
//						suscriptorExistente->socket);
			for (i = 0; i < list_size(lista); i++) {
				suscriptorExistente = (t_suscriptor*) list_get(lista, i);
				enviarMensajeGameCardCatch(mensaje->buffer->nombrePokemon,
						mensaje->buffer->posX, mensaje->buffer->posY,
						mensaje->buffer->idMensaje,
						suscriptorExistente->socket);
				log_info(logEntrega, "Envío un mensaje al suscriptor : %s.\n",
						suscriptorExistente->nombreProceso);
				list_add(mensajeAdmin->suscriptoresEnviados,
						(void*) suscriptorExistente);
			}
			break;
		}
		case MENSAJE_CAUGHT_POKEMON: {
			for (i = 0; i < list_size(lista); i++) {
				suscriptorExistente = (t_suscriptor*) list_get(lista, i);

				enviarMensajeBrokerCaughtId(mensaje->buffer->idMensaje,
						mensaje->buffer->idMensajeCorrelativo,
						mensaje->buffer->boolean, suscriptorExistente->socket);

//				enviarMensajeBrokerCaught(mensaje->buffer->idMensajeCorrelativo,
//						mensaje->buffer->boolean, suscriptorExistente->socket);

				log_info(logEntrega, "Envío un mensaje al suscriptor : %s.\n",
						suscriptorExistente->nombreProceso);
				list_add(mensajeAdmin->suscriptoresEnviados,
						(void*) suscriptorExistente);
			}
			break;
		}
		case MENSAJE_LOCALIZED_POKEMON: {
			for (i = 0; i < list_size(lista); i++) {
				suscriptorExistente = (t_suscriptor*) list_get(lista, i);
				enviarMensajeLocalizedId(mensaje->buffer->nombrePokemon,
						mensaje->buffer->listaCoordenadas,
						mensaje->buffer->idMensaje,
						mensaje->buffer->idMensajeCorrelativo,
						suscriptorExistente->socket);
				log_info(logEntrega, "Envío un mensaje al suscriptor : %s.\n",
						suscriptorExistente->nombreProceso);
				list_add(mensajeAdmin->suscriptoresEnviados,
						(void*) suscriptorExistente);
			}
			break;
		}
		default: {
			printf("error de mensaje o de suscriptor.\n");
		}
		}
		//}
	}

	return mensajeAdmin;
}

void* administrarMensajes() {
	t_paquete* paquete;
	paquete = malloc(sizeof(t_paquete));
	printf("Bloqueado en el mutex\n");
//sem_wait(&bandejaCounter);
//while(1){
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
		suscriptor->nombreProceso = string_duplicate(
				paquete->buffer->nombreProceso);
		suscriptor->socket = paquete->buffer->socket;
		verificarSuscriptor(suscriptor, NEW_POKEMON);
		printf("meti algo en la lista : ");
		log_debug(logger, "Nombre suscriptor: %s", suscriptor->nombreProceso);
		log_info(logEntrega, "Se ha suscripto a la cola New.\n");
		break;
	}
	case SUSCRIBIRSE_APPEARED_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		suscriptor->codigoOperacion = MENSAJE_APPEARED_POKEMON;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = string_duplicate(
				paquete->buffer->nombreProceso);
		suscriptor->socket = paquete->buffer->socket;
		verificarSuscriptor(suscriptor, APPEARED_POKEMON);
		log_debug(logger, "Nombre suscriptor: %s", suscriptor->nombreProceso);
		log_info(logEntrega, "Se ha suscripto a la cola Appeared.\n");
		break;
	}
	case SUSCRIBIRSE_CATCH_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		suscriptor->codigoOperacion = MENSAJE_CATCH_POKEMON;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = string_duplicate(
				paquete->buffer->nombreProceso);
		suscriptor->socket = paquete->buffer->socket;
		verificarSuscriptor(suscriptor, CATCH_POKEMON);
		log_debug(logger, "Nombre suscriptor: %s", suscriptor->nombreProceso);
		log_info(logEntrega, "Se ha suscripto a la cola Catch.\n");
		break;
	}
	case SUSCRIBIRSE_CAUGHT_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		suscriptor->codigoOperacion = MENSAJE_CAUGHT_POKEMON;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = string_duplicate(
				paquete->buffer->nombreProceso);
		suscriptor->socket = paquete->buffer->socket;
		verificarSuscriptor(suscriptor, CAUGHT_POKEMON);
		log_debug(logger, "Nombre suscriptor: %s", suscriptor->nombreProceso);
		log_info(logEntrega, "Se ha suscripto a la cola Caught.\n");
		break;
	}
	case SUSCRIBIRSE_GET_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		suscriptor->codigoOperacion = MENSAJE_GET_POKEMON;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = string_duplicate(
				paquete->buffer->nombreProceso);
		suscriptor->socket = paquete->buffer->socket;
		verificarSuscriptor(suscriptor, GET_POKEMON);
		log_debug(logger, "Nombre suscriptor: %s", suscriptor->nombreProceso);
		log_info(logEntrega, "Se ha suscripto a la cola Get.\n");
		break;
	}
	case SUSCRIBIRSE_LOCALIZED_POKEMON: {
		t_suscriptor* suscriptor = malloc(sizeof(t_suscriptor));
		suscriptor->codigoOperacion = MENSAJE_LOCALIZED_POKEMON;
		suscriptor->largoNombreProceso = paquete->buffer->largoNombreProceso;
		suscriptor->nombreProceso = string_duplicate(
				paquete->buffer->nombreProceso);
		suscriptor->socket = paquete->buffer->socket;
		verificarSuscriptor(suscriptor, LOCALIZED_POKEMON);
		log_debug(logger, "Nombre suscriptor: %s", suscriptor->nombreProceso);
		log_info(logEntrega, "Se ha suscripto a la cola Localized.\n");
		log_debug(logEntrega, "La cola LOCALIZED tiene %d suscriptores",
				list_size(LOCALIZED_POKEMON->lista));
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
			//pthread_mutex_lock(&mutexCache);

			if (strcmp(brokerConf->algoritmoMemoria, "PARTICIONES") == 0) {
				pthread_mutex_lock(&mutexCache);
				insertarMensajeEnCache(buffer, sizeMensaje,
						paquete->buffer->idMensaje, MENSAJE_NEW_POKEMON);
				pthread_mutex_unlock(&mutexCache);
			}
			if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {
				pthread_mutex_lock(&mutexCache);
				insertarMensajeEnCacheBuddy(buffer, sizeMensaje,
						paquete->buffer->idMensaje, MENSAJE_NEW_POKEMON);
				pthread_mutex_unlock(&mutexCache);
			}

			//	pthread_mutex_unlock(&mutexCache);
			pthread_mutex_lock(&mutexQueueNew);
			list_add(NEW_POKEMON->cola, mensajeAdmin);
			pthread_mutex_unlock(&mutexQueueNew);

			printf(" ENCOLE EN NEW : %s . \n", bufferLoco->pokemon);
		} else {
			printf(
					"tamaño del mensaje más grande que la memoria cache, no se puede alojar.");
			pthread_exit(NULL);
		}

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

			//pthread_mutex_lock(&mutexCache);
			log_error(logger, "Antres de insertar mensaje!!!!");

			if (strcmp(brokerConf->algoritmoMemoria, "PARTICIONES") == 0) {
				pthread_mutex_lock(&mutexCache);
				insertarMensajeEnCache(buffer, sizeMensaje,
						paquete->buffer->idMensaje, MENSAJE_APPEARED_POKEMON);
				pthread_mutex_unlock(&mutexCache);
			}
			if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {
				pthread_mutex_lock(&mutexCache);
printf(ACAMARILLO"\n\nAntes de insertarMensajeEnCacheBuddy"ACRESET"\n");
				insertarMensajeEnCacheBuddy(buffer, sizeMensaje,
						paquete->buffer->idMensaje, MENSAJE_APPEARED_POKEMON);
printf(ACAMARILLO"\n\nDespues de insertarMensajeEnCacheBuddy"ACRESET"\n");

				pthread_mutex_unlock(&mutexCache);

			}

			//pthread_mutex_unlock(&mutexCache);

			pthread_mutex_lock(&mutexQueueAppeared);
			list_add(APPEARED_POKEMON->cola, mensajeAdmin);
			pthread_mutex_unlock(&mutexQueueAppeared);
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

			//pthread_mutex_lock(&mutexCache);

			if (strcmp(brokerConf->algoritmoMemoria, "PARTICIONES") == 0) {
				pthread_mutex_lock(&mutexCache);
				insertarMensajeEnCache(buffer, sizeMensaje,
						paquete->buffer->idMensaje, MENSAJE_CATCH_POKEMON);
				pthread_mutex_unlock(&mutexCache);

			}
			if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {
				pthread_mutex_lock(&mutexCache);

				insertarMensajeEnCacheBuddy(buffer, sizeMensaje,
						paquete->buffer->idMensaje, MENSAJE_CATCH_POKEMON);
				pthread_mutex_unlock(&mutexCache);

			}

			//pthread_mutex_unlock(&mutexCache);
			pthread_mutex_lock(&mutexQueueCatch);
			list_add(CATCH_POKEMON->cola, (void*) mensajeAdmin);
			pthread_mutex_unlock(&mutexQueueCatch);
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

			//pthread_mutex_lock(&mutexCache);

			if (strcmp(brokerConf->algoritmoMemoria, "PARTICIONES") == 0) {
				pthread_mutex_lock(&mutexCache);

				insertarMensajeEnCache(buffer, sizeMensaje,
						paquete->buffer->idMensaje, MENSAJE_CAUGHT_POKEMON);
				pthread_mutex_unlock(&mutexCache);

			}
			if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {
				pthread_mutex_lock(&mutexCache);

				insertarMensajeEnCacheBuddy(buffer, sizeMensaje,
						paquete->buffer->idMensaje, MENSAJE_CAUGHT_POKEMON);
				pthread_mutex_unlock(&mutexCache);

			}

			//pthread_mutex_unlock(&mutexCache);
			pthread_mutex_lock(&mutexQueueCaught);
			list_add(CAUGHT_POKEMON->cola, (void*) mensajeAdmin);
			pthread_mutex_unlock(&mutexQueueCaught);
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

			//pthread_mutex_lock(&mutexCache);

			if (strcmp(brokerConf->algoritmoMemoria, "PARTICIONES") == 0) {
				pthread_mutex_lock(&mutexCache);

				insertarMensajeEnCache(buffer, sizeMensaje,
						paquete->buffer->idMensaje, MENSAJE_GET_POKEMON);
				pthread_mutex_unlock(&mutexCache);

			}
			if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {
				pthread_mutex_lock(&mutexCache);

				insertarMensajeEnCacheBuddy(buffer, sizeMensaje,
						paquete->buffer->idMensaje, MENSAJE_GET_POKEMON);
				pthread_mutex_unlock(&mutexCache);

			}

			//pthread_mutex_unlock(&mutexCache);
			pthread_mutex_lock(&mutexQueueGet);
			list_add(GET_POKEMON->cola, (void*) mensajeAdmin);
			pthread_mutex_unlock(&mutexQueueGet);
			printf("ENCOLE EN GET : %s . \n", bufferLoco->pokemon);
		} else {
			printf(
					"tamaño del mensaje más grande que la memoria cache, no se puede alojar.");
		}
		break;
	}
	case MENSAJE_LOCALIZED_POKEMON: {
		//	log_info(logEntrega, "Llego un mensaje nuevo a la cola Localized.\n");
		log_info(logger, "Localized: %s, Cantidad coordenadas:%d",
				paquete->buffer->nombrePokemon,
				list_size(paquete->buffer->listaCoordenadas));
		t_localizedPokemon* bufferLoco = malloc(sizeof(t_localizedPokemon));
		bufferLoco->sizeNombre = paquete->buffer->largoNombre - 1;
		bufferLoco->pokemon = paquete->buffer->nombrePokemon;
		bufferLoco->cantidadDePosiciones =
				paquete->buffer->listaCoordenadas->elements_count;
		bufferLoco->posiciones = list_create();
		bufferLoco->posiciones = list_duplicate(
				paquete->buffer->listaCoordenadas);
//		uint32_t sizeMensaje = sizeof(uint32_t)
//				* 2* paquete->buffer->listaCoordenadas->elements_count
//				+ bufferLoco->sizeNombre;
		uint32_t sizeMensaje = bufferLoco->sizeNombre + sizeof(uint32_t)
				+ sizeof(uint32_t)
				+ 2 * sizeof(uint32_t) * list_size(bufferLoco->posiciones);
		uint32_t desplazamiento = 0;

		log_debug(logger, "Antes de insertar mensaje en cache... Size %d ",
				sizeMensaje);
		if (sizeMensaje <= brokerConf->tamanoMemoria) {
			log_debug(logger, "Entre en el if x---x");

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

			pthread_mutex_lock(&mutexQueueLocalized);
			list_add(LOCALIZED_POKEMON->cola, (void*) mensajeAdmin);
			pthread_mutex_unlock(&mutexQueueLocalized);
//			list_add(LOCALIZED_PRUEBA, (void*) mensajeAdmin);
//			log_debug(logEntrega, "La cola de prueba tiene %d elementos",
//					list_size(LOCALIZED_PRUEBA));

			log_debug(logEntrega, "ENCOLE EN LOCALIZED : %s - IdMensaje: %d",
					bufferLoco->pokemon, mensajeAdmin->idMensaje);

			log_debug(logger, "Antes de insertar en cache!");
			//pthread_mutex_lock(&mutexCache);

			if (strcmp(brokerConf->algoritmoMemoria, "PARTICIONES") == 0) {
				pthread_mutex_lock(&mutexCache);

				insertarMensajeEnCache(buffer, sizeMensaje,
						paquete->buffer->idMensaje, MENSAJE_LOCALIZED_POKEMON);
				pthread_mutex_unlock(&mutexCache);

			}
			if (strcmp(brokerConf->algoritmoMemoria, "BS") == 0) {
				pthread_mutex_lock(&mutexCache);

				insertarMensajeEnCacheBuddy(buffer, sizeMensaje,
						paquete->buffer->idMensaje, MENSAJE_LOCALIZED_POKEMON);
				pthread_mutex_unlock(&mutexCache);

			}

			//	pthread_mutex_unlock(&mutexCache);
			log_debug(logger, "Ya inserte en cache");

			//	log_debug(logger,"LIST SIZE: %d",list_size(LOCALIZED_POKEMON->cola));

		} else {
			//printf(
			//	"tamaño del mensaje más grande que la memoria cache, no se puede alojar.");
			log_error(logger,
					"El tamano del mensaje (%d) es mas grande que la memoria cache, no se puede alojar",
					sizeMensaje);
		}
		break;
	}
	case CONFIRMACION_ACK: {
		log_info(logEntrega,
				"Recibi confirmación del suscriptor : %s , por el mensaje con ID : %d .\n",
				paquete->buffer->nombreProceso, paquete->buffer->idMensaje);
		int encontreMensaje = buscarMensaje(paquete); //aca se puede hacer una validacion.
		printf("Encontre mensaje %d\n", encontreMensaje);
		if (encontreMensaje == 0) {
			printf(
					"no encontré el mensaje que quiero ACK/marcar como confirmado. \n");
		}
		break;
	}
	default: {
		printf("error de modulo, no se conoce quien envia paquetes\n");
	}
	}
//}
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
		printf("Case NEW\n");
		for (i = 0; i < list_size(NEW_POKEMON->cola); i++) {
			unAdmin = (t_administrativo*) list_get(NEW_POKEMON->cola, i);
			if (idMensaje == unAdmin->idMensaje) {
				suscriptor->codigoOperacion = MENSAJE_NEW_POKEMON;
				list_add(unAdmin->suscriptoresRecibidos, suscriptor);
				list_replace(NEW_POKEMON->cola, i, unAdmin);
				flag = 1;
				return flag;
			}

		}
		break;
	}
	case MENSAJE_APPEARED_POKEMON: {
		printf("Case APPEARED\n");
		for (i = 0; i < list_size(APPEARED_POKEMON->cola); i++) {
			unAdmin = (t_administrativo*) list_get(APPEARED_POKEMON->cola, i);
			if (idMensaje == unAdmin->idMensaje) {
				suscriptor->codigoOperacion = MENSAJE_APPEARED_POKEMON;
				list_add(unAdmin->suscriptoresRecibidos, suscriptor);
				list_replace(APPEARED_POKEMON->cola, i, unAdmin);
				flag = 1;
				return flag;
			}

		}
		break;
	}
	case MENSAJE_GET_POKEMON: {
		printf("Case GET\n");
		for (i = 0; i < list_size(GET_POKEMON->cola); i++) {
			unAdmin = (t_administrativo*) list_get(GET_POKEMON->cola, i);
			if (idMensaje == unAdmin->idMensaje) {
				suscriptor->codigoOperacion = MENSAJE_GET_POKEMON;
				list_add(unAdmin->suscriptoresRecibidos, suscriptor);
				list_replace(GET_POKEMON->cola, i, unAdmin);
				flag = 1;
				return flag;
			}

		}
		break;
	}
	case MENSAJE_CATCH_POKEMON: {
		printf("Case CATCH\n");
		for (i = 0; i < list_size(CATCH_POKEMON->cola); i++) {
			unAdmin = (t_administrativo*) list_get(CATCH_POKEMON->cola, i);
			if (idMensaje == unAdmin->idMensaje) {
				suscriptor->codigoOperacion = MENSAJE_CATCH_POKEMON;
				list_add(unAdmin->suscriptoresRecibidos, suscriptor);
				list_replace(CATCH_POKEMON->cola, i, unAdmin);
				flag = 1;
				return flag;
			}
		}
		break;
	}

	case MENSAJE_CAUGHT_POKEMON: {
		printf("Case CAUGHT\n");
		for (i = 0; i < list_size(CAUGHT_POKEMON->cola); i++) {
			unAdmin = (t_administrativo*) list_get(CAUGHT_POKEMON->cola, i);
			if (idMensaje == unAdmin->idMensaje) {
				suscriptor->codigoOperacion = MENSAJE_CAUGHT_POKEMON;
				list_add(unAdmin->suscriptoresRecibidos, suscriptor);
				list_replace(CAUGHT_POKEMON->cola, i, unAdmin);
				flag = 1;
				return flag;
			}
		}
		break;
	}
	case MENSAJE_LOCALIZED_POKEMON: {
		printf("Case LOCALIZED - %d\n", list_size(LOCALIZED_POKEMON->cola));
		for (i = 0; i < list_size(LOCALIZED_POKEMON->cola); i++) {
			unAdmin = (t_administrativo*) list_get(LOCALIZED_POKEMON->cola, i);
			log_debug(logger, "Comparando Id:%d con Id:%d", idMensaje,
					unAdmin->idMensaje);
			if (idMensaje == unAdmin->idMensaje) {
				log_debug(logger, "Encontre!");
				suscriptor->codigoOperacion = MENSAJE_LOCALIZED_POKEMON;
				list_add(unAdmin->suscriptoresRecibidos, suscriptor);
				list_replace(LOCALIZED_POKEMON->cola, i, unAdmin);
				flag = 1;
				return flag;
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
//				printf("recibi un mensaje con el nombre : %s .\n",
	//					bufferLoco->buffer->nombrePokemon);
				log_debug(logger,
						"Llego mensaje OpCode:%d - Nombre: %s - CantidadCoordenadas:%d",
						bufferLoco->codigoOperacion,
						bufferLoco->buffer->nombrePokemon,
						list_size(bufferLoco->buffer->listaCoordenadas));
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
		//	pthread_join(hilito,NULL);
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
				&direccionCliente, &tamanioDireccion);

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
	pthread_mutex_init(&mutexCache, NULL);
	sem_init(&bandejaCounter, 1, 0);
	sem_init(&bandejaSuscriptorCounter, 1, 0);

	pthread_mutex_init(&mutexQueueNew, NULL);
	pthread_mutex_init(&mutexQueueAppeared, NULL);
	pthread_mutex_init(&mutexQueueGet, NULL);
	pthread_mutex_init(&mutexQueueLocalized, NULL);
	pthread_mutex_init(&mutexQueueCatch, NULL);
	pthread_mutex_init(&mutexQueueCaught, NULL);
	pthread_mutex_init(&mutexSuscriptor, NULL);

}

void generarDump(int signal) {
	if (signal == SIGUSR1) {
		printf("Received SIGUSR1!\n");

//crearDump(); funcion que esta haciendo Pedro.
	}

	return;
}
//signal(SIGUSR1, my_handler);

///////////////////////////////////////////////////////////////BuddySystem///////////////////////////////////////////////////
void insertarMensajeEnCacheBuddy(void* mensaje, int largo, int idMensaje, int cola) {
//	int pop_debugCache = debugCache;
//	debugCache = -1;
//void * mensaje, int largo, int idMensaje, int cola
	printf(ACROJO"Adentro de insertarMensajeEnCacheBuddy"ACRESET"\n");

	if (largo <= brokerConf->tamanoMemoria) {
		printf(ACROJO"Luego del if largo en insertarMensajeEnCacheBuddy"ACRESET"\n");
		while (1) {
			printf(ACVERDE"En el while de insertar MensajeEnCacheBuddy"ACRESET"\n");
			if (almacenarMensajeBuddy(mensaje, largo, idMensaje, cola)){
				printf(ACAZUL"Pude almacenar en insertarMensajeEnCacheBuddy"ACRESET"\n");
				break;
				printf(ACAMARILLO"No pude almacenar en insertarMensajeEnCacheBuddy"ACRESET"\n");
			}
			else {
				printf(ACMAGENTA"Antes de eliminar en insertarMensajeEnCacheBuddy"ACRESET"\n");
				eliminarParticionBuddy();
				printf(ACBLANCO"Antes de consolidar en insertarMensajeEnCacheBuddy"ACRESET"\n");
				consolidarMemoriaBuddy();
				printf(ACCIAN"Despues de consolidar en insertarMensajeEnCacheBuddy"ACRESET"\n");
			}
			printf(ACROJO"Iterando en insertarMensajeEnCacheBuddy"ACRESET"\n");
		}
		if (debugCache || verbose) {
			/*mostrarCache(partFirst,ASCEND);*/
			dumpCache();
			mostrarConfiguracion();
		}

	}

//log_info(logger,"MENSAJE NO CACHEADO DEBIDO A QUE LA LONGITUD SUPERA EL TAMAÑO DE LA MEMORIA");
//	debugCache = pop_debugCache;
}

bool almacenarMensajeBuddy(void* mensaje, int largo, int idMensaje, int cola) {

	printf(ACROJO"Adentro de almacenarMensajeBuddy"ACRESET"\n");

	t_partBuddy* particion;

	printf(ACROJO"antes del if FF Adentro de almacenarMensajeBuddy"ACRESET"\n");
	printf(ACVERDE"algoritmoParticionLibre[%s]\n", brokerConf->algoritmoParticionLibre);
	if (string_equals_ignore_case(brokerConf->algoritmoParticionLibre, "FF")) printf(ACAMARILLO"Es FF True"ACRESET"\n\n\n\n");

	if (string_equals_ignore_case(brokerConf->algoritmoParticionLibre, "FF")){

		printf(ACROJO"Vamos a buscar primera particion Libre Adentro de almacenarMensajeBuddy"ACRESET"\n\n\n\n\n");



		particion = buscarPrimerParticionLibreBuddy(largo);
		printf(ACROJO"Encontramos la primera particion libre [%d]Adentro de almacenarMensajeBuddy"ACRESET"\n", particion);

	}
	else if (string_equals_ignore_case(brokerConf->algoritmoParticionLibre,"BF"))
		printf(ACAMARILLO"Buscamos la mejor particion libre Adentro de almacenarMensajeBuddy"ACRESET"\n");
		particion = buscarMejorParticionLibreBuddy(largo);
		printf(ACAMARILLO"Encontramos la mejor particion libre [%d]Adentro de almacenarMensajeBuddy"ACRESET"\n", particion);
	if (particion == NULL)
		return false;
//--------------------------------------------------------------------------
	int potenciaDeDosMasCercana = buscarPotenciaDeDosMasCercana(largo);

	while (particion->tamanio > potenciaDeDosMasCercana) {
		particion->tamanio = (particion->tamanio) / 2;
		agregarBuddy(particion);
	}

	int posicionParticion = particion->posicionParticion;
	particion = cargarDatosParticionBuddy(particion, mensaje, largo, idMensaje,
			cola);
	list_add(particionesEnMemoriaBuddy, particion);

	if (string_equals_ignore_case(brokerConf->algoritmoReemplazo, "FIFO")) {
		int* idMensaje = crearElementoColaMensajesMemoriaBuddy(
				particion->idMensaje);
		queue_push(colaMensajesMemoriaBuddy, idMensaje);
	}

	memcpy(cache + posicionParticion, mensaje, largo);

	char* nombreCola = obtenerNombreCola(particion->cola);
	log_info(logger, "MENSAJE %s ALMACENADO EN LA POSICION %d", nombreCola,
			particion->posicionParticion);

	free(mensaje);		//ver si hago este free

	return true;

}

t_partBuddy* buscarPrimerParticionLibreBuddy(uint32_t largo) {
	ordenarParticionesPorPosicionBuddy();

	bool particionLibre(void* particion) {
		t_partBuddy* particionCasteada = particion;
		if (largo < brokerConf->tamanoMinimoParticion)
			return (brokerConf->tamanoMinimoParticion
					<= (particionCasteada->tamanio) && particionCasteada->libre);
		else
			return (largo <= (particionCasteada->tamanio)
					&& particionCasteada->libre);
	}
	return list_remove_by_condition(particionesEnMemoriaBuddy, particionLibre);
}

t_partBuddy* buscarMejorParticionLibreBuddy(uint32_t largo) {
	int j=0;

printf(ACBLANCO"Estoy en BuscarMejorParticionLibreBuddy[%d] buscarMPLBuddy"ACRESET"\n",j);


	bool particionLibre(void* particion) {
		j++;
		t_partBuddy* particionCasteada = particion;

printf(ACVERDE"particionLibre[%d] j[%d] particion[%x] en buscarMPLBuddy"ACRESET"\n",particionLibre,j,particion);
printf(ACAMARILLO"largo [%d] brokerConf->tamanoMinimoParticion[%d]"ACRESET"\n",largo,brokerConf->tamanoMinimoParticion);

		if (largo < brokerConf->tamanoMinimoParticion)

			return (brokerConf->tamanoMinimoParticion
					<= (particionCasteada->tamanio) && particionCasteada->libre);
		else
printf(ACROJO"largo[%d] particionCasteada.tamanio[%d] partidionCasteada.libre[%d] expresionDelIf[%d]"ACRESET"\n",
	largo, particionCasteada->tamanio, particionCasteada->libre, (largo <= (particionCasteada->tamanio)	&& particionCasteada->libre));
printf(ACROJO"Return por largo["ACVERDE"%d"ACROJO"] tamanoMinimoParticion[%d] "ACCIAN"en buscarMPLBuddy j[%d]"ACRESET"\n",
		largo,brokerConf->tamanoMinimoParticion,j);

			return (largo <= (particionCasteada->tamanio)
					&& particionCasteada->libre);
	}
printf(ACAZUL"estoy arriba de particionesLibres"ACRESET"\n");
	t_list* particionesLibres = list_filter(particionesEnMemoriaBuddy,
			particionLibre);
	printf(ACCIAN"ya pase particionesLibres y voy al comparador"ACRESET"\n");

	bool comparadorParticionesLibres(void* particion1, void* particion2) {
		t_partBuddy* particion1Casteada = particion1;
		t_partBuddy* particion2Casteada = particion2;

printf(ACBLANCO"Return por tamanio < tamanio [%d] en BuscarMejorParticionLibreBuddy"ACRESET"\n");

		return (particion1Casteada->tamanio) < (particion2Casteada->tamanio);
	}
	printf(ACVERDE"ya pase el comparador y voy al sort "ACRESET"\n");

	list_sort(particionesLibres, comparadorParticionesLibres);

	printf(ACBLANCO"ya pase el sort y voy a remover las 0"ACRESET"\n");


	t_partBuddy* mejorParticionAuxiliar = list_remove(particionesLibres, 0);

	printf(ACCIAN"ya pase remover las 0 y voy a list_destroy"ACRESET"\n");
printf(ACROJO"mejorParticionAuxiliar["ACAMARILLO"%X"ACROJO"] "ACRESET"\n",mejorParticionAuxiliar);

if (!mejorParticionAuxiliar) return mejorParticionAuxiliar;    // agregado 04/08/2020

	list_destroy(particionesLibres);

	printf(ACVERDE"ya pase list_destroy y voy posicionMejorParticion"ACRESET"\n\n\n");
//  !!!

	int posicionMejorParticion = mejorParticionAuxiliar->posicionParticion;

	printf(ACROJO"ya pase posicionMejorParticion y voy a particion MismoIdMensaje"ACRESET"\n");


//	borrar_particion_buddy_memoria(mejorParticionAuxiliar);

	bool particionMismoIdMensaje(void* particion) {
		t_partBuddy* particionCasteada = particion;
		printf(ACAMARILLO"ya pase particionMismoIdMensaje y voy al return"ACRESET"\n");


printf(ACBLANCO"Return por particionCastead=particion en BuscarMejorParticionLibreBuddy"ACRESET"\n");

		return (particionCasteada->posicionParticion) == posicionMejorParticion;
	}
	t_partBuddy* mejorParticion = list_remove_by_condition(
			particionesEnMemoriaBuddy, particionMismoIdMensaje);
printf(ACBLANCO"Return por encontrar mejorParticion [%X] en BuscarMejorParticionLibreBuddy"ACRESET"\n", mejorParticion);

	return mejorParticion;

}

int buscarPotenciaDeDosMasCercana(uint32_t tamanio) {

	int potencia = 0;
	int exponente = 0;
	while (1) {

		potencia = pow(2, exponente);
		if (tamanio <= potencia) {
			return potencia;
		}
		exponente++;
	}
}

void agregarBuddy(t_partBuddy* particion) {

	t_partBuddy particionBuddy;
	particionBuddy.libre = true;
	particionBuddy.tamanio = particion->tamanio;
	particionBuddy.posicionParticion = particion->posicionParticion
			+ particion->tamanio;
	t_partBuddy* particionBuddyCreada = crearParticionBuddyMemoria(
			particionBuddy);
	list_add(particionesEnMemoriaBuddy, particionBuddyCreada);
}

t_partBuddy* crearParticionBuddyMemoria(t_partBuddy particion) {
	t_partBuddy* nuevaParticion = malloc(sizeof(t_partBuddy));

	nuevaParticion->posicionParticion = particion.posicionParticion;
	nuevaParticion->libre = particion.libre;
	nuevaParticion->tamanio = particion.tamanio;
	nuevaParticion->tamanioMensaje = particion.tamanioMensaje;
	nuevaParticion->idMensaje = particion.idMensaje;
	nuevaParticion->cola = particion.cola;
	nuevaParticion->contadorLRU = particion.contadorLRU;

	return nuevaParticion;
}

void borrarParticionBuddyMemoria(t_partBuddy* particion) {

	free(particion);
}

t_partBuddy* cargarDatosParticionBuddy(t_partBuddy* particion, void* mensaje,
		int largo, int idMensaje, int cola) {

	switch (cola) {
	case MENSAJE_NEW_POKEMON: {

		particion->cola = MENSAJE_NEW_POKEMON;
		particion->idMensaje = idMensaje;
		particion->libre = false;
		uint32_t tamanioMensaje = largo;
		particion->tamanioMensaje = tamanioMensaje;
		if (tamanioMensaje < brokerConf->tamanoMinimoParticion)
			particion->tamanio = tamanioMensaje
					< brokerConf->tamanoMinimoParticion;
		else
			particion->tamanio = buscarPotenciaDeDosMasCercana(
					particion->tamanioMensaje);
		CONTADORLRUBUDDY++;
		particion->contadorLRU = CONTADORLRUBUDDY;

		break;
	}
	case MENSAJE_APPEARED_POKEMON: {

		particion->cola = MENSAJE_APPEARED_POKEMON;
		particion->idMensaje = idMensaje;
		particion->libre = false;
		uint32_t tamanioMensaje = largo;
		particion->tamanioMensaje = tamanioMensaje;
		if (tamanioMensaje < brokerConf->tamanoMinimoParticion)
			particion->tamanio = tamanioMensaje
					< brokerConf->tamanoMinimoParticion;
		else
			particion->tamanio = buscarPotenciaDeDosMasCercana(
					particion->tamanioMensaje);
		CONTADORLRUBUDDY++;
		particion->contadorLRU = CONTADORLRUBUDDY;
		break;
	}
	case MENSAJE_GET_POKEMON: {
		particion->cola = MENSAJE_GET_POKEMON;
		particion->idMensaje = idMensaje;
		particion->libre = false;
		uint32_t tamanioMensaje = largo;
		particion->tamanioMensaje = tamanioMensaje;
		if (tamanioMensaje < brokerConf->tamanoMinimoParticion)
			particion->tamanio = brokerConf->tamanoMinimoParticion;
		else
			particion->tamanio = buscarPotenciaDeDosMasCercana(
					particion->tamanioMensaje);
		CONTADORLRUBUDDY++;
		particion->contadorLRU = CONTADORLRUBUDDY;
		break;
	}
	case MENSAJE_LOCALIZED_POKEMON: {

		particion->cola = MENSAJE_LOCALIZED_POKEMON;
		particion->idMensaje = idMensaje;
		particion->libre = false;
		uint32_t tamanioMensaje = largo;
		particion->tamanioMensaje = tamanioMensaje;
		if (tamanioMensaje < brokerConf->tamanoMinimoParticion)
			particion->tamanio = brokerConf->tamanoMinimoParticion;
		else
			particion->tamanio = buscarPotenciaDeDosMasCercana(
					particion->tamanioMensaje);
		CONTADORLRUBUDDY++;
		particion->contadorLRU = CONTADORLRUBUDDY;
		break;
	}
	case MENSAJE_CATCH_POKEMON: {
		particion->cola = MENSAJE_CATCH_POKEMON;
		particion->idMensaje = idMensaje;
		particion->libre = false;
		uint32_t tamanioMensaje = largo;
		particion->tamanioMensaje = tamanioMensaje;
		if (tamanioMensaje < brokerConf->tamanoMinimoParticion)
			particion->tamanio = brokerConf->tamanoMinimoParticion;
		else
			particion->tamanio = buscarPotenciaDeDosMasCercana(
					particion->tamanioMensaje);
		CONTADORLRUBUDDY++;
		particion->contadorLRU = CONTADORLRUBUDDY;
		break;
	}
	case MENSAJE_CAUGHT_POKEMON: {
		particion->cola = MENSAJE_CAUGHT_POKEMON;
		particion->idMensaje = idMensaje;
		particion->libre = false;
		uint32_t tamanioMensaje = largo;
		particion->tamanioMensaje = tamanioMensaje;
		if (tamanioMensaje < brokerConf->tamanoMinimoParticion)
			particion->tamanio = brokerConf->tamanoMinimoParticion;
		else
			particion->tamanio = particion->tamanioMensaje;
		CONTADORLRUBUDDY++;
		particion->contadorLRU = CONTADORLRUBUDDY;
		break;
	}
	}
	return particion;

}

t_list* sacarParticionesLibresBuddy() {
	bool particionLibre(void* particion) {
		t_partBuddy* particionCasteada = particion;
		return !(particionCasteada->libre);
	}
	return list_filter(particionesEnMemoriaBuddy, particionLibre);

}

void borrarElementoCola(uint32_t* elemento) {
	free(elemento);
}

void eliminarIdCola(uint32_t idMensaje, int idCola) {
	bool igualIdMensaje(void* elementoCola) {
		uint32_t* idMensajeCola = elementoCola;
		return *idMensajeCola == idMensaje;
	}
	switch (idCola) {
	case MENSAJE_NEW_POKEMON: {
		pthread_mutex_lock(&mutexQueueNew);
		list_remove_and_destroy_by_condition(NEW_POKEMON->cola, igualIdMensaje,
				(void*) borrarElementoCola);
		pthread_mutex_unlock(&mutexQueueNew);
		break;
	}
	case MENSAJE_APPEARED_POKEMON: {
		pthread_mutex_lock(&mutexQueueAppeared);
		list_remove_and_destroy_by_condition(APPEARED_POKEMON->cola,
				igualIdMensaje, (void*) borrarElementoCola);
		pthread_mutex_unlock(&mutexQueueAppeared);
		break;
	}
	case MENSAJE_GET_POKEMON: {
		pthread_mutex_lock(&mutexQueueGet);
		list_remove_and_destroy_by_condition(GET_POKEMON->cola, igualIdMensaje,
				(void*) borrarElementoCola);
		pthread_mutex_unlock(&mutexQueueGet);
		break;
	}
	case MENSAJE_LOCALIZED_POKEMON: {
		pthread_mutex_lock(&mutexQueueLocalized);
		list_remove_and_destroy_by_condition(LOCALIZED_POKEMON->cola,
				igualIdMensaje, (void*) borrarElementoCola);
		pthread_mutex_unlock(&mutexQueueLocalized);
		break;
	}
	case MENSAJE_CATCH_POKEMON: {
		pthread_mutex_lock(&mutexQueueCatch);
		list_remove_and_destroy_by_condition(CATCH_POKEMON->cola,
				igualIdMensaje, (void*) borrarElementoCola);
		pthread_mutex_unlock(&mutexQueueCatch);
		break;
	}
	case MENSAJE_CAUGHT_POKEMON: {
		pthread_mutex_lock(&mutexQueueCaught);
		list_remove_and_destroy_by_condition(CAUGHT_POKEMON->cola,
				igualIdMensaje, (void*) borrarElementoCola);
		pthread_mutex_unlock(&mutexQueueCaught);
		break;
	}
	}
}

void eliminarParticionBuddy() {

	if (string_equals_ignore_case(brokerConf->algoritmoReemplazo, "FIFO")) {
		int* idMensaje = queue_pop(colaMensajesMemoriaBuddy);
		int idMensajeAuxiliar = *idMensaje;
		borrarElementoColaMensajesMemoriaBuddy(idMensaje);

		void cambiarALibre(void* particion) {
			t_partBuddy* particionCasteada = particion;
			if ((particionCasteada->idMensaje) == idMensajeAuxiliar) {
				particionCasteada->libre = true;
				log_info(logger, "PARTICION ELIMINADA CUYA POSICION ES: %d",
						particionCasteada->posicionParticion);
				eliminarIdCola(particionCasteada->idMensaje,
						particionCasteada->cola);
			}

		}
		list_iterate(particionesEnMemoriaBuddy, cambiarALibre);
	} else if (string_equals_ignore_case(brokerConf->algoritmoReemplazo,
			"LRU")) {

		t_list* particionesOcupadas = sacarParticionesLibresBuddy();

		bool comparadorParticionesLibresPorLRU(void* particion1,
				void* particion2) {
			t_partBuddy* particion1Casteada = particion1;
			t_partBuddy* particion2Casteada = particion2;
			return (particion1Casteada->contadorLRU)
					< (particion2Casteada->contadorLRU);
		}
		list_sort(particionesOcupadas, comparadorParticionesLibresPorLRU);

		t_partBuddy* particionMenosUsada = list_get(particionesOcupadas, 0);
		int idMensaje = particionMenosUsada->idMensaje;
		list_destroy(particionesOcupadas);
		void cambiarALibre(void* particion) {
			t_partBuddy* particionCasteada = particion;
			if ((particionCasteada->idMensaje) == idMensaje) {
				particionCasteada->libre = true;
				log_info(logger, "PARTICION ELIMINADA CUYA POSICION ES: %d",
						particionCasteada->posicionParticion);
				eliminarIdCola(particionCasteada->idMensaje,
						particionCasteada->cola);
			}

		}
		list_iterate(particionesEnMemoriaBuddy, cambiarALibre);
	}

}

void consolidarMemoriaBuddy() {

	ordenarParticionesPorPosicionBuddy();
	int sizeLista = list_size(particionesEnMemoriaBuddy);
	int index = 0;
	int indexAdyacente = index + 1;
	while (indexAdyacente < sizeLista) {
		t_partBuddy* particion = list_get(particionesEnMemoriaBuddy, index);
		t_partBuddy* particionAdyacente = list_get(particionesEnMemoriaBuddy,
				indexAdyacente);

		if (particion->libre && particionAdyacente->libre) {

			if (particion->tamanio == particionAdyacente->tamanio) {
				log_info(logger,
						"PARTICION CON POSICION %d Y PARTICION CO POSICION %d CONSOLIDADAS",
						particion->posicionParticion,
						particionAdyacente->posicionParticion);
				particion->tamanio += particionAdyacente->tamanio;
				int posicion = particionAdyacente->posicionParticion;
				particionAdyacente = removerPorPosicionBuddy(posicion);

				borrarParticionBuddyMemoria(particionAdyacente);

				sizeLista = list_size(particionesEnMemoriaBuddy);
				index--;
				indexAdyacente--;
			}

		} else if (particion->tamanio == particionAdyacente->tamanio) {
			index++;
			indexAdyacente++;
		}
		index++;
		indexAdyacente++;
	}
}

int* crearElementoColaMensajesMemoriaBuddy(int idMensaje) {
	int* newIdMensaje = malloc(sizeof(int));
	*newIdMensaje = idMensaje;
	return newIdMensaje;
}

void borrarElementoColaMensajesMemoriaBuddy(int* idMensaje) {
	free(idMensaje);
}

t_partBuddy* removerPorPosicionBuddy(int posicion) {
	bool compararPorId(void* particion) {
		t_partBuddy* particionCasteada = particion;
		return particionCasteada->posicionParticion == posicion;
	}

	return list_remove_by_condition(particionesEnMemoriaBuddy, compararPorId);
}

void ordenarParticionesPorPosicionBuddy() {
	bool comparadorParticionesPorPosicion(void* particion1, void* particion2) {
		t_partBuddy* particion1Casteada = particion1;
		t_partBuddy* particion2Casteada = particion2;
		return (particion1Casteada->posicionParticion)
				< (particion2Casteada->posicionParticion);
	}
	list_sort(particionesEnMemoriaBuddy, comparadorParticionesPorPosicion);
}

t_partBuddy* obtenerMensajeBuddy(int idMensaje) {
	bool particionIgualID(void* particion) {
		t_partBuddy* particionCasteada = particion;

		return (particionCasteada->idMensaje == idMensaje)
				&& !(particionCasteada->libre);
	}

	return (t_partBuddy*) list_find(particionesEnMemoriaBuddy, particionIgualID);//castee
}



