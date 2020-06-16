#include "broker.h"

int main(void) {

	inicializarLogger();
	cargarConfigBROKER();
	inicializarLoggerEntregable();
	log_info(logger, "Desde el main veo IP=%s y PUERTO=%d",
			brokerConf->ipBroker, brokerConf->puertoBroker);
	inicializarColasBroker();
	inicializarEstructuras();
	idMensajeUnico = 0;
	idMensajeCorrelativo = 0; // ver como se inicializa esto y como se usa

	//pedirMemoriaInicial();
	//manejarMemoria();

	pthread_mutex_init(&bandejaMensajes_mutex, NULL);
	pthread_mutex_init(&recibir_mutex, NULL);
	pthread_mutex_init(&asignarIdMensaje_mutex, NULL);
	sem_init(&bandejaCounter, 1, 0);
	//pthread_mutex_lock(&bandejaMensajes_mutex);

	bandejaDeMensajes = list_create();
	contadorDeMensajes = 0;
	bandeja = queue_create();

	pthread_t hiloEscucha;
	pthread_create(&hiloEscucha, NULL, escucharConexiones, NULL);

	pthread_t hilo;
	pthread_create(&hilo, NULL, administrarMensajes, NULL);

	pthread_join(hiloEscucha, NULL);

	//pthread_join(hilo, NULL);

	destruirColasBroker();
	free(brokerConf);
	return EXIT_SUCCESS;
}

