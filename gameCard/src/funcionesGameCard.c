#include "gameCard.h"

void inicializar_logger() {
	//crea el logger
	if((logger=log_create("logs/GAMECARD.log","GAMECARD",1,LOG_LEVEL_TRACE)) == NULL){
	perror("No se puso inicializar el logger\n");
	exit(1);
	}
}

void cargarConfigGameCard() {
	//carga la configuracion
	GAMECARDTConfig = config_create(GAMECARD_CONFIG_PATH);

	if(GAMECARDTConfig ==NULL){
		perror("Error al abrir el archivo de configuracion");
		log_error(logger, "- NO SE PUDO IMPORTAR LA CONFIGURACION");
		exit(2);
	}

	t_GAMECARDConfig *gameCardConfig = malloc(sizeof(t_GAMECARDConfig));


	gameCardConfig->tiempoReintentoConexion=config_get_int_value(GAMECARDTConfig,"TIEMPO_DE_REINTENTO_CONEXION");
	gameCardConfig->tiempoReintentoOperacion=config_get_int_value(GAMECARDTConfig,"TIEMPO_DE_REINTENTO_OPERACION");
	gameCardConfig->ipBroker=config_get_string_value(GAMECARDTConfig,"IP_BROKER");
	gameCardConfig->puertoBroker=config_get_string_value(GAMECARDTConfig,"PUERTO_BROKER");
	gameCardConfig->puntoDeMontaje=config_get_string_value(GAMECARDTConfig,"PUNTO_MONTAJE_TALLGRASS");

	config_destroy(gameCardConfig);
	//free(gameCardConfig);
}
//TODO
void* serializar_paquete(){

}
//Copiado del TP0
int crear_conexion(char *ip,char *puerto){
		struct addrinfo hints;
		struct addrinfo *server_info;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;

		getaddrinfo(ip, puerto, &hints, &server_info);

		int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

		if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
			printf("error");

		freeaddrinfo(server_info);

		return socket_cliente;
}
//TODO
void enviar_mensaje(char *mensaje,int socket){

}
//TODO
char* recibir_mensaje(int socket){

}
//TODO
void liberar_conexion(int socket){

}

//TODO
void terminar_programa(int socket,t_log* logger,t_config* config){

}
