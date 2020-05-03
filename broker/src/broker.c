#include "broker.h"

int main(void) {

	int contador = 0;
	int flag = 1;
	inicializarLogger();
	cargarConfigBROKER();
	log_info(logger,"Desde el main veo IP=%s y PUERTO=%d",brokerConf->ipBroker,brokerConf->puertoBroker);
	inicializarColasBroker();
	iniciarServidor(brokerConf->ipBroker, brokerConf->puertoBroker);


	//Hay que modificar el iniciarServidor, porque el broker nunca llega hasta acá.
	pthread_t *tid[contador];

	while (flag) {

		pthread_create(tid[contador], NULL, (void*) administrarColas,
		NULL);//aca tengo que recibir el suscriptor y la cola donde se quiere suscribir
		contador++;
	}


	free(brokerConf);
	return EXIT_SUCCESS;
}
