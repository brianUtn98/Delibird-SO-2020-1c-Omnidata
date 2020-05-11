#include "gameCard.h"

void inicializar_logger() {
	//crea el logger
	logger = log_create("GAMECARD.log", "GAMECARD", 1, LOG_LEVEL_TRACE);
	if(logger == NULL){
	perror("No se puso inicializar el logger\n");
	exit(1);
	}
}

void cargarConfigGameCard() {
	gameCardConfig = (t_GAMECARDConfig*) malloc(sizeof(t_GAMECARDConfig));
	GAMECARDTConfig = config_create(GAMECARD_CONFIG_PATH);

	if(GAMECARDTConfig == NULL){
		perror("Error al abrir el archivo de configuracion");
		log_error(logger, "- NO SE PUDO IMPORTAR LA CONFIGURACION");
		exit(2);
	}

	gameCardConfig->tiempoReintentoConexion=config_get_int_value(GAMECARDTConfig,"TIEMPO_DE_REINTENTO_CONEXION");
	gameCardConfig->tiempoReintentoOperacion=config_get_int_value(GAMECARDTConfig,"TIEMPO_DE_REINTENTO_OPERACION");
	gameCardConfig->ipBroker=string_duplicate(config_get_string_value(GAMECARDTConfig,"IP_BROKER"));
	gameCardConfig->puertoBroker=config_get_int_value(GAMECARDTConfig,"PUERTO_BROKER");
	gameCardConfig->puntoDeMontaje=string_duplicate(config_get_string_value(GAMECARDTConfig,"PUNTO_MONTAJE_TALLGRASS"));

	log_info(logger,"tiempoReintentoConexion=%d",gameCardConfig->tiempoReintentoConexion);
	log_info(logger,"tiempoReintentoOperacion=%d",gameCardConfig->tiempoReintentoOperacion);
	log_info(logger,"puertoBroker=%d",gameCardConfig->puertoBroker);
	log_info(logger,"ipBroker=%s",gameCardConfig->ipBroker);
	log_info(logger,"puntoDeMontaje=%s",gameCardConfig->puntoDeMontaje);

	log_info(logger, "Configuracion importada con exito");
	return;

	//config_destroy(GAMECARDTConfig);
	//ver si devolvemos gameCardConfig para poder liberar el malloc que pedimos para el struct
	//no olvidarse
	//
	//free(gameCardConfig);
}
//TODO
//void* serializar_paquete(t_paquete *paquete,int* bytes){
//	//Serializa un paquete
//	int size_serializado = sizeof(op_code) + sizeof(int) + paquete->buffer->size;
//	void *buffer=malloc(size_serializado);
//	int desplazamiento=0;
//	memcpy(buffer+desplazamiento,&(paquete->codigo_operacion),sizeof(paquete->codigo_operacion));
//	desplazamiento+=sizeof(paquete->codigo_operacion);
//	memcpy(buffer+desplazamiento,&(paquete->buffer->size),sizeof(paquete->buffer->size));
//	desplazamiento+=sizeof(paquete->buffer->size);
//	memcpy(buffer+desplazamiento,paquete->buffer->stream,sizeof(paquete->buffer->size));
//
//	(*bytes)=size_serializado;
//	return buffer;
//}
//Copiado del TP0
//int crear_conexion(char *ip,char *puerto){
//		struct addrinfo hints;
//		struct addrinfo *server_info;
//
//		memset(&hints, 0, sizeof(hints));
//		hints.ai_family = AF_UNSPEC;
//		hints.ai_socktype = SOCK_STREAM;
//		hints.ai_flags = AI_PASSIVE;
//
//		getaddrinfo(ip, puerto, &hints, &server_info);
//
//		int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
//
//		if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
//			printf("error");
//
//		freeaddrinfo(server_info);
//
//		return socket_cliente;
//}

/*void enviar_mensaje(char *mensaje,int socket){
printf("Enviando mensaje %s con %d bytes",mensaje,strlen(mensaje) + 1);
t_paquete *paquete=malloc(sizeof(t_paquete));
paquete->codigo_operacion=MENSAJE;
paquete->buffer->stream=mensaje;
paquete->buffer->size=strlen(mensaje)+1;

int size_serializado;
void *serializado=serializar_paquete(paquete,&size_serializado);
send(socket,serializado,size_serializado,0);
free(serializado);
}*/

/*char* recibir_mensaje(int socket){
op_code codigo_operacion;
int buffer_size;
char *buffer;
recv(socket,&codigo_operacion,sizeof(codigo_operacion),0);
recv(socket,&buffer_size,sizeof(buffer_size),0);
buffer=malloc(buffer_size);
recv(socket,buffer,buffer_size,0);

	if(buffer[buffer_size - 1] != '\0'){
		printf("WARN - El buffer recibido no es un string\n");
	}
return buffer;
}*/

//void liberar_conexion(int socket){
//close(socket);
//}
void crearArchivo(char* puntoMontaje, char* nombreArchivo)
{
	//Crear el full path
	char *rutaArchivo = string_new();
	string_append(&rutaArchivo, puntoMontaje);
	string_append(&rutaArchivo, nombreArchivo);

	log_info(logger, "Archivo general por crear %s", rutaArchivo);

	FILE *fp = txt_open_for_append(rutaArchivo);
	if(fp == NULL)
	{
		perror("Error al crear archivo METADATA.bin");
		log_error(logger, "- Error al crear archivo %s", rutaArchivo);
		exit(1);
	}
	txt_write_in_file(fp, "BLOCK_SIZE=64\n");
	txt_write_in_file(fp, "BLOCKS=5192\n");
	txt_write_in_file(fp, "MAGIC_NUMBER=TALL_GRASS\n");
	txt_close_file(fp);

	log_info(logger, "Archivo %s cargado con exito", rutaArchivo);
	return;
}

void terminarPrograma(){
	log_destroy(logger);
	config_destroy(GAMECARDTConfig);
	free(gameCardConfig);
}
