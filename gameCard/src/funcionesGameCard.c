#include "gameCard.h"

void inicializarLogger() {
	//crea el logger
	logger=log_create("logs/GAMECARD.log","GAMECARD",1,LOG_LEVEL_TRACE);

}

void cargarConfigGameCard() {
	//carga la configuracion
	GAMECARDTConfig = config_create(GAMECARD_CONFIG_PATH);

	if(GAMECARDTConfig ==NULL){
		perror("Error al abrir el archivo de configuracion");
		log_error(logger, "- NO SE PUDO IMPORTAR LA CONFIGURACION");
		exit(1);
	}

	t_GAMECARDConfig *gameCardConfig = malloc(sizeof(t_GAMECARDConfig));
	gameCardConfig->tiempoReintentoConexion=config_get_int_value(GAMECARDTConfig,"TIEMPO_DE_REINTENTO_CONEXION");
	gameCardConfig->tiempoReintentoOperacion=config_get_int_value(GAMECARDTConfig,"TIEMPO_DE_REINTENTO_OPERACION");
	gameCardConfig->ipBroker=config_get_string_value(GAMECARDTConfig,"IP_BROKER");
	gameCardConfig->puertoBroker=config_get_string_value(GAMECARDTConfig,"PUERTO_BROKER");
	gameCardConfig->puntoDeMontaje=config_get_string_value(GAMECARDTConfig,"PUNTO_MONTAJE_TALLGRASS");


	free(gameCardConfig);
}
