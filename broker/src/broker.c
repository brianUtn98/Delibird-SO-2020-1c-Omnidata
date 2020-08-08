#include "broker.h"

int main(void) {

	//printf(" Mi ip es : %s \n", getIp());

	inicializarLogger();
	cargarConfigBROKER();
	inicializarLoggerEntregable();
	//log_info(logger, "Desde el main veo IP=%s y PUERTO=%d",
		//	brokerConf->ipBroker, brokerConf->puertoBroker);
	inicializarSemaforos();
	inicializarColasBroker();
	idMensajeUnico = 0;

	idMensajeCorrelativo = 0; //esto no se si se usa...
	signal(SIGUSR1, generarDump);

	iniciarCache();

	bandejaDeMensajes = list_create();

	contadorDeMensajes = 0;
	bandeja = queue_create();
	//bandejaSuscriptores = queue_create();//esto no se usa

	pthread_t hiloEscucha;
	pthread_create(&hiloEscucha, NULL, escucharConexiones, NULL);
	pthread_detach(hiloEscucha);

	pthread_t hilo;
	pthread_create(&hilo, NULL, consumirMensajes, NULL);

	pthread_join(hiloEscucha, NULL);
	pthread_join(hilo, NULL);

	destruirColasBroker();
	free(brokerConf);
	return EXIT_SUCCESS;
}

