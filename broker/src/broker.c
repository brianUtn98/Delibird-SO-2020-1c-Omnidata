#include "broker.h"

int main(void) {

	int contador = 0;
	int flag = 1;
	inicializarLogger();
	cargarConfigBROKER();
	inicializarColasBroker();
	iniciarServidor(brokerConf->ipBroker, brokerConf->puertoBroker);



	pthread_t *tid[contador];

	while (flag) {

		pthread_create(tid[contador], NULL, (void*) administrarColas,
		NULL);
		contador++;
	}


	free(brokerConf);
	return EXIT_SUCCESS;
}
