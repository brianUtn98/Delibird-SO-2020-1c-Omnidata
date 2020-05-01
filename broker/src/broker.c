#include "broker.h"

int main(void) {

	int contador = 0;
	int flag = 1;
	inicializarLogger();
	cargarConfigBROKER();
	inicializarColasBroker();
	crearConexion(brokerConf->ipBroker, brokerConf->puertoBroker,10);///este 10 lo puse sin saber porque.


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
