#include "gameCard.h"

void inicializar_logger()
{
	logger = log_create("GAMECARD.log", "GAMECARD", 1, LOG_LEVEL_TRACE);
	if(logger == NULL){
		perror("No se puso inicializar el logger\n");
		exit(1);
	}
}

void cargarConfigGameCard()
{
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

	log_info(logger,"- tiempoReintentoConexion=%d\n",gameCardConfig->tiempoReintentoConexion);
	log_info(logger,"- tiempoReintentoOperacion=%d\n",gameCardConfig->tiempoReintentoOperacion);
	log_info(logger,"- puertoBroker=%d\n",gameCardConfig->puertoBroker);
	log_info(logger,"- ipBroker=%s\n",gameCardConfig->ipBroker);
	log_info(logger,"- puntoDeMontaje=%s\n",gameCardConfig->puntoDeMontaje);

	log_info(logger, "- CONFIG IMPORTADA CON EXITO\n");
	return;
}

void suscribirmeAColasBroker()
{
	/*
	 * 2. Suscribirse a las colas del Broker
	 * 	a. Suscribirse a tNEW_POKEMON
	 * 	b. Suscribirse a tCATCH_POKEMON
	 * 	c. Suscribirse a tGET_POKEMON
	 *
	 * 3. Recibir confirmación
	 */

	log_info(logger, "SUSCRIPCION ACEPTADA - tNEW_POKEMON\n");
	log_info(logger, "SUSCRIPCION ACEPTADA - tCATCH_POKEMON\n");
	log_info(logger, "SUSCRIPCION ACEPTADA - tGET_POKEMON\n");
	return;
}

char* crearRutaArchivo(char* nombreArchivo)
{
	char* rutaArchivo = string_new();
	string_append(&rutaArchivo, gameCardConfig->puntoDeMontaje);
	string_append(&rutaArchivo, nombreArchivo);
	log_debug(logger, "Ruta creada: %s", rutaArchivo);
	return rutaArchivo;
}

char* crearRutaPokemon(char* nombrePokemon)
{
	char* rutaArchivo = string_new();
	string_append(&rutaArchivo, gameCardConfig->puntoDeMontaje);
	string_append(&rutaArchivo, "/Files/Pokemon/");
	string_append(&rutaArchivo, nombrePokemon);
	string_append(&rutaArchivo, "/Metadata.bin");
	return rutaArchivo;
}

void crearArchivo(char* rutaArchivo, char* stringAEscribir)
{
	FILE *fp = fopen(rutaArchivo, "w+");
	if(fp == NULL)
	{
		log_error(logger, "Error al crear archivo %s\n", rutaArchivo);
		exit(1);
	}
	txt_write_in_file(fp, stringAEscribir);
	txt_close_file(fp);

	log_info(logger, "ARCHIVO %s ACTUALIZADO\n", rutaArchivo);
	return;
}
void EscribirArchivo(char* rutaArchivo, char* stringAEscribir)
{
	FILE *fp = txt_open_for_append(rutaArchivo);
	if(fp == NULL)
	{
		log_error(logger, "Error al crear archivo %s\n", rutaArchivo);
		exit(1);
	}
	txt_write_in_file(fp, stringAEscribir);
	txt_close_file(fp);

	log_info(logger, "ARCHIVO %s ACTUALIZADO\n", rutaArchivo);
	return;
}

void terminarPrograma()
{
	log_destroy(logger);
	config_destroy(GAMECARDTConfig);
	free(gameCardConfig);
}

void iniciarTallGrass()
{
	 // Crear archivos Metadata general
	char* rutaMetadata = crearRutaArchivo(RUTA_METADATA_GENERAL);
	char* Linea1Metadata= "BLOCK_SIZE=64\n";
	char* Linea2Metadata= "BLOCKS=5192\n";
	char* Linea3Metadata= "MAGIC_NUMBER=TALL_GRASS\n";

	EscribirArchivo(rutaMetadata, Linea1Metadata);
	EscribirArchivo(rutaMetadata, Linea2Metadata);
	EscribirArchivo(rutaMetadata, Linea3Metadata);

	// Crear Bitmap general
	char* rutaBitmap = crearRutaArchivo(RUTA_BITMAP_GENERAL);
	crearArchivo(rutaBitmap, "\n");
	return;
}

int archivoAbierto(char* rutaArchivo)
{
	/*
	 *  true   1
	 *  false  0
	 */
	return 0;
}

int existePokemon(char* rutaPokemon)
{
	/*
	 *  true   1
	 *  false  0
	 */

	if( access( rutaPokemon, F_OK ) != -1 ) {
	    // Existe el pokemon
		return 1;
	} else {
	    // No existe el pokemon
		return 0;
	}
}

char* crearBlock(int block, int x, int y, int cant)
{
	log_debug(logger, "estoy dentro del crear block");
	char c_block[20];
	sprintf(c_block, "%d", block);
	log_debug(logger, "block=%s\n", c_block);

	char c_x[5];
	sprintf(c_x, "%d", x);

	char c_y[5];
	sprintf(c_y, "%d", y);

	char c_cant[5];
	sprintf(c_cant, "%d", cant);

	char* rutaBlocks = string_new();
	string_append(&rutaBlocks,"/Blocks/");
	string_append(&rutaBlocks,c_block);
	string_append(&rutaBlocks,".bin");
	char* ruta=crearRutaArchivo(rutaBlocks);

	char* posicionX = string_new();
	string_append(&posicionX,"X=");
	string_append(&posicionX,c_x);
	string_append(&posicionX,"\n");

	char* posicionY = string_new();
	string_append(&posicionY,"Y=");
	string_append(&posicionY,c_y);
	string_append(&posicionY,"\n");

	char* cantidad = string_new();
	string_append(&cantidad,"CANTIDAD=");
	string_append(&cantidad,c_cant);
	string_append(&cantidad,"\n");

	crearArchivo(ruta, posicionX);
	EscribirArchivo(ruta, posicionY);
	EscribirArchivo(ruta, cantidad);
	return ruta;
}
void agregarNewPokemon(char* pokemon, t_list* l_coordenadas, int cantidad)
{
	char* rutaPokemon=crearRutaPokemon(pokemon);

	if(existePokemon(rutaPokemon) == 0) //NO EXISTE EL POKEMON
	{
		//mutex
		maximo_block_creado++;
		//mutex

		void* x=list_get(l_coordenadas, 0);
		void* y=list_get(l_coordenadas, 1);

		//mutex
		char* rutaBlock=crearBlock(maximo_block_creado, ((int)x), ((int)y),cantidad);
		//mutex

		stat(rutaBlock, &st);
		int size = st.st_size;

		char* linea1Metadata=string_new();
		string_append(&linea1Metadata,"DIRECTORY=N\n");
		string_append(&linea1Metadata,"SIZE=");
		string_append(&linea1Metadata,(char*)&size);
		string_append(&linea1Metadata,"\n");
		string_append(&linea1Metadata,"BLOCKS=");
		string_append(&linea1Metadata,(char*)&maximo_block_creado);
		string_append(&linea1Metadata,"\n");
		string_append(&linea1Metadata,"OPEN=Y");

		//pthread_mutex_lock(&lock);
		crearArchivo(rutaPokemon,linea1Metadata);
		//pthread_mutex_unlock(&lock);

	}else{ // EXISTE EL POKEMON

		if(archivoAbierto(rutaPokemon) == 1)
		{
			/*
			De alguna forma hay que finalizar el hilo
			reintentar la operacion despues de un tiempo
			definido por configuracion.
			*/
			perror("El archivo esta siendo usado");
			exit(1);
		}
	}

	/*
	conectarse al Broker y mandar mensaje a la cola APPEARED_POKEMON
	-ID del mensaje recibido
	-Pokemon
	-Posicion del mapa
	*/

	return;
}

int catchPokemon(int mensajeID, char* pokemon, int posicionMapa)
{
	if(existePokemon(pokemon) == 0)
	{
		perror("El pokemon no existe");
		log_error(logger, "- El pokemon %s no existe", pokemon);
		exit(1);
	}
	if(archivoAbierto(pokemon) == 0)
	{
			/*
			De alguna forma hay que finalizar el hilo
			reintentar la operacion despues de un tiempo
			definido por configuracion.
			*/
		perror("El archivo esta siendo usado");
		exit(1);
	}
	/* Verificar si las posicion ya existen dentro del archivo del pokemon.
	 * En caso negativo informar error
	 *
	 * Si la cantidad es 1 eliminar la linea. En caso contrario decrementar.
	 *
	 * Conectarse al Broker y enviar el mensaje indicando
	 * el resultado correcto.
	 *
	 * Usar la cola de mensajes CAUGHT_POKEMON indicando
	 * -ID del mensaje
	 * -Resultado
	 *
	 */
	return 0;
}
