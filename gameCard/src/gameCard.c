#include "gameCard.h"

int main(void) {

	inicializarMutexGameCard();

	inicializar_logger();
	cargarConfigGameCard();

	contadorConexiones = 0;

	iniciarTallGrass();
	crear_bitmap();

	log_info(logger, "BLOCKS MAXIMOS: %d, BLOCK SIZE: %d y BLOCKS USADOS %d",
			g_blocks_maximos, g_block_size, g_blocks_usados);


	pthread_mutex_init(&bandejaMensajes_mutex, NULL);
	pthread_mutex_init(&recibir_mutex, NULL);
	sem_init(&bandejaCounter, 1, 0);

	bandejaMensajesGameCard = list_create();
	bandejaDeMensajesGameCard = queue_create();


	pthread_t hiloEscucha;
	pthread_create(&hiloEscucha, NULL, escucharConexionesGameCard, NULL);

	pthread_t hilo;
	pthread_create(&hilo, NULL, consumirMensajesGameCard, NULL);

	pthread_t suscripcion;
	pthread_create(&suscripcion,NULL,suscribirseABroker,NULL);

	for (;;) {

	}

	pthread_join(hiloEscucha, NULL);
	pthread_join(hilo, NULL);

	// LOGGEAR MENSAJE

	free(gameCardConfig);
	terminarProgramaGameCard();
	return EXIT_SUCCESS;

}

