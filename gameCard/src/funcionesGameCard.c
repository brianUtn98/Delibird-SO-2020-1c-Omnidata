#include "gameCard.h"

void inicializar_logger() {
	logger = log_create("GAMECARD.log", "GAMECARD", 1, LOG_LEVEL_TRACE);
	if (logger == NULL) {
		perror("No se puso inicializar el logger\n");
		exit(1);
	}
}

void inicializarLoggerEntregable() {
	printf("Voy a crear un logger %s\n", gameCardConfig->LOG_FILE);

	logEntrega = log_create(gameCardConfig->LOG_FILE,
			gameCardConfig->nombreProceso, 1, LOG_LEVEL_TRACE);
	if (logEntrega == NULL) {
		perror("No se pudo inicializar el logger para la entrega\n");
	}
}

void cargarConfigGameCard() {
	gameCardConfig = (t_GAMECARDConfig*) malloc(sizeof(t_GAMECARDConfig));

	GAMECARDTConfig = config_create(GAMECARD_CONFIG_PATH);
	if (GAMECARDTConfig == NULL) {
		perror("Error al abrir el archivo de configuracion");
		log_error(logger, "- NO SE PUDO IMPORTAR LA CONFIGURACION");
		exit(2);
	}

	gameCardConfig->tiempoReintentoConexion = config_get_int_value(
			GAMECARDTConfig, "TIEMPO_DE_REINTENTO_CONEXION");
	gameCardConfig->tiempoReintentoOperacion = config_get_int_value(
			GAMECARDTConfig, "TIEMPO_DE_REINTENTO_OPERACION");
	gameCardConfig->ipBroker = string_duplicate(
			config_get_string_value(GAMECARDTConfig, "IP_BROKER"));
	gameCardConfig->puertoBroker = config_get_int_value(GAMECARDTConfig,
			"PUERTO_BROKER");
	gameCardConfig->puntoDeMontaje = string_duplicate(
			config_get_string_value(GAMECARDTConfig,
					"PUNTO_MONTAJE_TALLGRASS"));
	gameCardConfig->ipGameCard = string_duplicate(
			config_get_string_value(GAMECARDTConfig, "IP_GAMECARD"));
	gameCardConfig->puertoGameCard = config_get_int_value(GAMECARDTConfig,
			"PUERTO_GAMECARD");
	gameCardConfig->nombreProceso = string_duplicate(
			config_get_string_value(GAMECARDTConfig, "NOMBRE_PROCESO"));
	gameCardConfig->magicnumber = string_duplicate(
			config_get_string_value(GAMECARDTConfig, "MAGIC_NUMBER"));

	gameCardConfig->blocksCantidad = config_get_int_value(GAMECARDTConfig,
			"BLOCKS");
	gameCardConfig->blocksSize = config_get_int_value(GAMECARDTConfig,
			"BLOCK_SIZE");
	gameCardConfig->LOG_FILE = string_duplicate(config_get_string_value(GAMECARDTConfig,
			"LOG_FILE"));

	log_info(logger, "- tiempoReintentoConexion=%d\n",
			gameCardConfig->tiempoReintentoConexion);
	log_info(logger, "- tiempoReintentoOperacion=%d\n",
			gameCardConfig->tiempoReintentoOperacion);
	log_info(logger, "- puertoBroker=%d\n", gameCardConfig->puertoBroker);
	log_info(logger, "- ipBroker=%s\n", gameCardConfig->ipBroker);
	log_info(logger, "- puntoDeMontaje=%s\n", gameCardConfig->puntoDeMontaje);
	log_info(logger, "- ipGameCard=%s\n", gameCardConfig->ipGameCard);
	log_info(logger, "- puertoGameCard\n", gameCardConfig->puertoGameCard);
	log_info(logger, "- nombreProceso\n", gameCardConfig->nombreProceso);

	log_info(logger, "- blocksCantidad=%d\n", gameCardConfig->blocksCantidad);
	log_info(logger, "- blocksSize=%d\n", gameCardConfig->blocksSize);

	log_info(logger, "- CONFIG IMPORTADA CON EXITO\n");
	return;
}

void suscribirmeAColasBroker() {
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
void setearVarGlobalesBlocks() {
	char* ruta_metadata = string_new();
	string_append(&ruta_metadata, gameCardConfig->puntoDeMontaje);
	string_append(&ruta_metadata, RUTA_METADATA_GENERAL);

	char buff2[255];
	FILE *fp = fopen(ruta_metadata, "r");

	//BLOCK_SIZE=64
	fscanf(fp, "%s", buff2);
	char* linea_size_blocks = string_duplicate(buff2);
	char** linea_size_blocks_array = string_split(linea_size_blocks, "=");
	string_trim(linea_size_blocks_array);
	sscanf(linea_size_blocks_array[1], "%d", &g_block_size);
	if (g_block_size == 0) {
		log_error(logger, "No se pudo cargar la metadata BLOCK_SIZE");
	}

	//BLOCKS=5192
	fscanf(fp, "%s", buff2);
	char* linea_blocks = string_duplicate(buff2);
	fclose(fp);

	char** linea_blocks_array = string_split(linea_blocks, "=");
	string_trim(linea_blocks_array);
	sscanf(linea_blocks_array[1], "%d", &g_blocks_maximos);
	if (g_blocks_maximos == 0) {
		log_error(logger, "No se pudo cargar la metadata BLOCKS");
	}

	g_blocks_usados = 0;

	free(ruta_metadata);
	free(linea_blocks);
	free(linea_size_blocks);
	return;
}

char* crearRutaArchivo(char* nombreArchivo) {
	char* rutaArchivo = string_new();
	string_append(&rutaArchivo, gameCardConfig->puntoDeMontaje);
	string_append(&rutaArchivo, nombreArchivo);
	return rutaArchivo;
}

char* crearRutaPokemon(char* nombrePokemon) {
	char* rutaArchivo = string_new();
	string_append(&rutaArchivo, gameCardConfig->puntoDeMontaje);
	string_append(&rutaArchivo, "/Files/Pokemon/");
	string_append(&rutaArchivo, nombrePokemon);
	string_append(&rutaArchivo, "/Metadata.bin");
	return rutaArchivo;
}

void escribir_archivo(char* rutaArchivo, char* stringAEscribir) {
	FILE *fp = txt_open_for_append(rutaArchivo);
	int indice = fileno(fp);
	flock(indice, LOCK_EX);

	if (fp == NULL) {
		log_error(logger, "Error al crear archivo %s\n", rutaArchivo);
		exit(1);
	}
	txt_write_in_file(fp, stringAEscribir);
	flock(indice, LOCK_UN);
	txt_close_file(fp);

	log_debug(logger, "ARCHIVO %s ACTUALIZADO\n", rutaArchivo);
	return;
}

void terminarProgramaGameCard() {
	log_destroy(logger);
	config_destroy(GAMECARDTConfig);
	free(gameCardConfig);
}

void crearCarpeta(char* ruta) {
	struct stat st = { 0 };
	if (stat(ruta, &st) == -1) {
		//No tengo idea si los permisos 0777 son seguros

		pthread_mutex_lock(&mutex_crear_carpeta);
		int exito = mkdir(ruta, 0777);
		pthread_mutex_unlock(&mutex_crear_carpeta);
		if (exito == -1) {
			log_error(logger, "No se pudo crear la carpeta: %s", ruta);
		}
	}
}

void iniciarTallGrass() {
	crearCarpeta(gameCardConfig->puntoDeMontaje);

	char* ruta_metadata = string_new();
	string_append(&ruta_metadata, gameCardConfig->puntoDeMontaje);
	string_append(&ruta_metadata, "/Metadata");
	crearCarpeta(ruta_metadata);

	char* ruta_blocks = string_new();
	string_append(&ruta_blocks, gameCardConfig->puntoDeMontaje);
	string_append(&ruta_blocks, "/Blocks");
	crearCarpeta(ruta_blocks);

	char* ruta_files = string_new();
	string_append(&ruta_files, gameCardConfig->puntoDeMontaje);
	string_append(&ruta_files, "/Files");
	crearCarpeta(ruta_files);

	char* ruta_pokemon = string_new();
	string_append(&ruta_pokemon, gameCardConfig->puntoDeMontaje);
	string_append(&ruta_pokemon, "/Files");
	string_append(&ruta_pokemon, "/Pokemon");
	crearCarpeta(ruta_pokemon);

	char* rutaMetadata = crearRutaArchivo(RUTA_METADATA_GENERAL);
	char* rutaBitmap = crearRutaArchivo(RUTA_BITMAP_GENERAL);

	// Creamos tall-grass/Metadata/Metadata.bin
	log_debug(logger, "Estoy antes de cargar");
	if (access(rutaMetadata, F_OK) == -1) {
		char* linea_metadata = string_new();
		string_append(&linea_metadata, "BLOCK_SIZE=");
		string_append(&linea_metadata, string_itoa(gameCardConfig->blocksSize));
		string_append(&linea_metadata, "\n");
		string_append(&linea_metadata, "BLOCKS=");
		string_append(&linea_metadata,
				string_itoa(gameCardConfig->blocksCantidad));
		string_append(&linea_metadata, "\n");
		string_append(&linea_metadata, "MAGIC_NUMBER=TALL_GRASS\n");

		escribir_archivo(rutaMetadata, linea_metadata);
		free(linea_metadata);

	}

	setearVarGlobalesBlocks();
	crearTodosLosBloquesEnFS();

	free(rutaBitmap);
	free(ruta_metadata);
	free(ruta_blocks);
	free(ruta_files);
	free(ruta_pokemon);
	free(rutaMetadata);
	return;
}

int archivoAbierto(char* rutaArchivo) {
	/*
	 *  true   1
	 *  false  0
	 */
	return 0;
}

int existePokemon(char* rutaPokemon) {
	/*
	 *  true   1
	 *  false  0
	 */
	if (access(rutaPokemon, F_OK) != -1) {
		// Existe el pokemon
		return 1;
	} else {
		// No existe el pokemon
		return 0;
	}
}

int agregarLineaBlock(int block, int x, int y, int cant) {
	char* rutaBlocks = string_new();
	string_append(&rutaBlocks, gameCardConfig->puntoDeMontaje);
	string_append(&rutaBlocks, "/Blocks/");
	string_append(&rutaBlocks, string_itoa(block));
	string_append(&rutaBlocks, ".bin");

	char* str_block = string_new();
	string_append(&str_block, string_itoa(x));
	string_append(&str_block, "-");
	string_append(&str_block, string_itoa(y));
	string_append(&str_block, "=");
	string_append(&str_block, string_itoa(cant));
	string_append(&str_block, "\n");

	escribir_archivo(rutaBlocks, str_block);
	int size = strlen(str_block);
	free(str_block);

	log_info(logger, "Block %s creado - size %d", rutaBlocks, size);
	free(rutaBlocks);
	return size;
}

void crearTodosLosBloquesEnFS() {

	for (int i = 0; i <= (g_blocks_maximos - 1); i++) {
		char* rutaBlocks = string_new();

		string_append(&rutaBlocks, gameCardConfig->puntoDeMontaje);
		string_append(&rutaBlocks, "/Blocks/");
		string_append(&rutaBlocks, string_itoa(i + 1));
		string_append(&rutaBlocks, ".bin");

		escribir_archivo(rutaBlocks, "");
		log_info(logger, "Block %s creado", rutaBlocks);
		free(rutaBlocks);
	}
	return;
}

t_paquete* obtenerPokemon(char* pokemon) {
	t_posicion *XY = malloc(sizeof(t_posicion));
	t_list *CoordXY = list_create();
	t_paquete *bufferLoco;
	bufferLoco = malloc(sizeof(t_paquete));
	bufferLoco->buffer = malloc(sizeof(t_bufferOmnidata));

	bufferLoco->buffer->listaCoordenadas = list_create();
	int cantidad = 0;
	char* rutaPokemon = crearRutaPokemon(pokemon);

	char* carpetaPokemon = string_new();
	string_append(&carpetaPokemon, gameCardConfig->puntoDeMontaje);
	string_append(&carpetaPokemon, "/Files/Pokemon/");
	string_append(&carpetaPokemon, pokemon);

	if (existePokemon(rutaPokemon) == 1) //NO EXISTE EL POKEMON
			{

		log_info(logger, "existe el pokemon");

		char buff[255];
		FILE *fp = fopen(rutaPokemon, "r");
		// Linea OPEN
		fscanf(fp, "%s", buff);
		// Pasamos la primera linea
		fscanf(fp, "%s", buff);
		// Linea SIZE
		fscanf(fp, "%s", buff);
		char* ln_size_actual = string_duplicate(buff);
		char** size_array = string_split(ln_size_actual, "=");
		int int_size_actual;
		string_trim(size_array);
		sscanf(size_array[1], "%d", &int_size_actual);
		free(ln_size_actual);

		fscanf(fp, "%s", buff);
		char** block_array = string_split(buff, "=");
		string_trim(block_array);

		fclose(fp);

		// restamos los []

		char* newln2 = string_new();
		string_append(&newln2, "BLOCKS=[");

		char aux[100];
		char** array_strings = string_get_string_as_array(block_array[1]);
		// Itearamos para saber si existen las mismas coordenadas en el filesystem
		// Si es el caso, aumentamos solo la cantidad dentro del block y terminamos.

		while (*array_strings != NULL) {

			char buff2[255];

			strcpy(aux, *array_strings);

			char* a_ruta = string_duplicate(aux);
			char* a_newln2 = string_duplicate(aux);

			char* ruta = string_new();
			string_append(&ruta, gameCardConfig->puntoDeMontaje);
			string_append(&ruta, "/Blocks/");
			//string_append(&ruta,array_strings[i]);
			string_append(&ruta, a_ruta);
			string_append(&ruta, ".bin");

			string_append(&newln2, a_newln2);
			string_append(&newln2, ",");

			FILE *fp_block = fopen(ruta, "r");

			// POSX
			fscanf(fp_block, "%s", buff2);
			printf("1: %s\n", buff2);
			char* s_x = strdup(buff2);
			char** block_arrayx = string_split(s_x, "=");
			int int_x;
			string_trim(block_arrayx);
			sscanf(block_arrayx[1], "%d", &int_x);
			XY->x = int_x;

			//list_add(CoordXY,(void*)int_x);
			//block_arrayx[1]

			//POSY
			fscanf(fp_block, "%s", buff2);
			printf("2: %s\n", buff2);
			char* s_y = string_duplicate(buff2);
			char** block_arrayy = string_split(s_y, "=");
			int int_y;
			string_trim(block_arrayy);
			sscanf(block_arrayy[1], "%d", &int_y);
			XY->y = int_y;
			//list_add(CoordXY,(void*)int_y);
			list_add(CoordXY, (void*) XY);

			//CANTIDAD
			fscanf(fp_block, "%s", buff2);
			printf("3: %s\n", buff2);
			fscanf(fp_block, "%s", buff2);
			char* s_cant = string_duplicate(buff2);
			char** block_arraycant = string_split(s_cant, "=");
			int int_cant;
			string_trim(block_arraycant);
			sscanf(block_arraycant[1], "%d", &int_cant);

			cantidad = +int_cant;

			fclose(fp_block);
			array_strings++;

			free(s_cant);
			free(s_y);
			free(s_x);
			free(ruta);

		}
		free(block_array);

		bufferLoco->buffer->listaCoordenadas = list_duplicate(CoordXY);
		bufferLoco->buffer->nombrePokemon = pokemon;
		bufferLoco->buffer->cantidadPokemons = cantidad;
		list_destroy(CoordXY);

	}

	return bufferLoco;
}

char* crearRutaCarpetaPokemon(char* pokemon) {
	char* carpetaPokemon = string_new();
	string_append(&carpetaPokemon, gameCardConfig->puntoDeMontaje);
	string_append(&carpetaPokemon, "/Files/Pokemon/");
	string_append(&carpetaPokemon, pokemon);
	return carpetaPokemon;
}

int crearPokemonDesdeCero(char* pokemon, int x, int y, int cantidad) {
	printf("Se agregan %d %s en (%d, %d)\n", cantidad, pokemon, x, y);
	char* rutaPokemon = crearRutaPokemon(pokemon);
	char* rutaCarpetaPokemon = crearRutaCarpetaPokemon(pokemon);
	crearCarpeta(rutaCarpetaPokemon);

	pthread_mutex_lock(&mutex_cant_blockers);
	g_blocks_usados++;
	pthread_mutex_unlock(&mutex_cant_blockers);

	int size = agregarLineaBlock(g_blocks_usados, x, y, cantidad);
	char* linea1Metadata = string_new();

	string_append(&linea1Metadata, "OPEN=N");
	string_append(&linea1Metadata, "\n");
	string_append(&linea1Metadata, "DIRECTORY=N\n");
	string_append(&linea1Metadata, "SIZE=");
	string_append(&linea1Metadata, string_itoa(size));
	string_append(&linea1Metadata, "\n");
	string_append(&linea1Metadata, "BLOCKS=[");
	string_append(&linea1Metadata, string_itoa(g_blocks_usados));
	string_append(&linea1Metadata, "]");
	string_append(&linea1Metadata, "\n");

	escribir_archivo(rutaPokemon, linea1Metadata);
	free(rutaCarpetaPokemon);
	free(linea1Metadata);
	free(rutaPokemon);
	return g_blocks_usados;
}
int usarBloqueActual(char* nro_bloque, int x, int y, int cantidad) {
	log_debug(logger, "Estoy adentro de usarBloqueActual");
	char* rutaBlocks = string_new();
	string_append(&rutaBlocks, gameCardConfig->puntoDeMontaje);
	string_append(&rutaBlocks, "/Blocks/");
	string_append(&rutaBlocks, nro_bloque);
	string_append(&rutaBlocks, ".bin");

	char* nueva_line = string_new();
	string_append(&nueva_line, string_itoa(x));
	string_append(&nueva_line, "-");
	string_append(&nueva_line, string_itoa(y));
	string_append(&nueva_line, "=");
	string_append(&nueva_line, string_itoa(cantidad));
	string_append(&nueva_line, "\n");

	log_debug(logger, "ruta Blocks %s", rutaBlocks);
	int tamanoNuevaLinea = strlen(nueva_line);
	int tamanoArchivo = tamanioBloque(rutaBlocks);
	free(rutaBlocks);
	free(nueva_line);

	log_debug(logger, "El bloque %s es de tamaño %d y g_block_size %d",
			nro_bloque, tamanoArchivo, g_block_size);
	if (tamanoArchivo + tamanoNuevaLinea < g_block_size) {
		return 1;
	} else {
		return 0;
	}
}
void actualizarSizePokemon(int nueva_cantidad, char* rutaPokemon) {
	FILE *fp = fopen(rutaPokemon, "r+");
	fseek(fp, 24, SEEK_SET);
	char* aux = string_new();
	string_append(&aux, string_itoa(nueva_cantidad));
	string_append(&aux, "\n");
	fputs(aux, fp);
	fclose(fp);
	free(aux);
	return;
}

void actualizarBloquesPokemon(char* rutaPokemon, int nro_bloque) {
	FILE *fp = fopen(rutaPokemon, "r+");
	fseek(fp, -2, SEEK_END);
	char* aux = string_new();

	string_append(&aux, ",");
	string_append(&aux, string_itoa(nro_bloque));
	string_append(&aux, "]");
	string_append(&aux, "\n");

	fputs(aux, fp);
	fclose(fp);
	free(aux);
	return;
}
void actualizarBloquesPokemonParaCatch(char* rutaPokemon, int tamanioCantidad) {
	FILE *fp = fopen(rutaPokemon, "r+");
	fseek(fp, -3 - tamanioCantidad, SEEK_END);
	char* aux = string_new();

	string_append(&aux, " ");

	fputs(aux, fp);
	fclose(fp);
	free(aux);
	return;
}
void agregarNewPokemon(char* pokemon, int x, int y, int cantidad) {

	char* rutaPokemon = crearRutaPokemon(pokemon);

	if (existePokemon(rutaPokemon) == 0) {
		//NO EXISTE EL POKEMON
		log_debug(logger, "No existe el %s en (%d, %d) y cantidad %d", pokemon,
				x, y, cantidad);
		int posicion = crearPokemonDesdeCero(pokemon, x, y, cantidad);
		actualizarBitMapen1(posicion);
		log_debug(logger, "Actualize el bitmap en la posicion %d", posicion);
		free(rutaPokemon);
		return;
	} else { // EXISTE EL POKEMON

		log_info(logger, "SE ENCONTRO EL POKEMON %s EN TAILGRASS", pokemon);
		int flag = 1;

		char* open = "OPEN=Y";

		while (flag) {

			char buff[255];
			FILE *fp = fopen(rutaPokemon, "r+");
			int indice = fileno(fp);
			flock(indice, LOCK_EX);

			fscanf(fp, "%s", buff);
			char* estado = string_duplicate(buff);

			if (strcmp(estado, open) == 0) {
				log_error(logger, "El archivo %s ya esta abierto", rutaPokemon);
				flock(indice, LOCK_UN);
				sleep(gameCardConfig->tiempoReintentoOperacion);
			} else {
				flock(indice, LOCK_UN);
				//ArchivoEnUso(rutaPokemon, pokemon);
				sleep(1);

				log_info(logger,
						"Cerramos el achivo %s para que no se pueda usar %s",
						pokemon, rutaPokemon);
				fseek(fp, 5, SEEK_SET);
				fputs("Y", fp);

				// Linea directory
				fscanf(fp, "%s", buff);
				// Linea SIZE
				fscanf(fp, "%s", buff);
				char* ln_size_actual = string_duplicate(buff);
				char** size_array = string_split(ln_size_actual, "=");
				int int_size_actual;
				string_trim(size_array);
				sscanf(size_array[1], "%d", &int_size_actual);
				free(ln_size_actual);

				// LEO BLOCKS=[1,2,3,4]
				fscanf(fp, "%s", buff);
				char** block_array = string_split(buff, "=");
				string_trim(block_array);

				fclose(fp);

				// Me quedo con [1,2,3,4]
				char** array_strings = string_get_string_as_array(
						block_array[1]);

				// Itearamos para saber si existen las mismas coordenadas en el filesystem
				// Si es el caso, aumentamos solo la cantidad dentro del block y terminamos.

				int resultado = chequearCoordenadasBlock(array_strings,
						cantidad, x, y);

				if (resultado == 0) {
					log_info(logger,
							"Se encontraron las posicones(%d,%d) para %s y se sumo la cantidad",
							x, y, pokemon);

					//actualizarBitMapen1(g_blocks_usados);

					ArchivoAbiertoParaUso(rutaPokemon, pokemon);
					flag = 0;
					free(array_strings);
					return;

				}

				char aux[30];

				while (*array_strings != NULL) {
					printf("array_strings %s\n", *array_strings);
					strcpy(aux, *array_strings);

					array_strings++;
				}

				log_debug(logger, "Que tiene el aux:%sa", aux);
				int usar = usarBloqueActual(aux, x, y, cantidad);

				if (usar == 1) {
					log_info(logger,
							"Se agregara una nueva linea al bloque numero %s para pokemon %s",
							aux, pokemon);

					int t_nueva_linea = agregarLineaAlFinalDelBloque(aux, x, y,
							cantidad);
					int nuevo_size = int_size_actual + t_nueva_linea;

					ArchivoAbiertoParaUso(rutaPokemon, pokemon);
					actualizarSizePokemon(nuevo_size, rutaPokemon);
					flag = 0;
					free(rutaPokemon);
					//free(array_strings);
					return;
				} else {
					log_info(logger,
							"Se necesita un nuevo bloque para %s (%d,%d)",
							pokemon, x, y);
					int t_nueva_linea = escribirPokemonOBuscarBloqueLibre(x, y,
							cantidad);
					int nuevo_size = int_size_actual + t_nueva_linea;
					actualizarSizePokemon(nuevo_size, rutaPokemon);
					actualizarBloquesPokemon(rutaPokemon, g_blocks_usados);
					ArchivoAbiertoParaUso(rutaPokemon, pokemon);

					//free(array_strings);
					free(rutaPokemon);
					return;
				}
			}
		}
	}
}

int catchPokemon(char* pokemon, int x, int y) {
	char* rutaPokemon = crearRutaPokemon(pokemon);

	if (existePokemon(rutaPokemon) == 0) {
		log_error(logger, "El pokemon %s no existe", pokemon);
		free(rutaPokemon);
		return -1;
	} else {
		log_info(logger,
				"Existe el pokemon %s en el FS. Buscando las coordenadas..",
				pokemon);

		int flag = 1;
		char* open = "OPEN=Y";

		while (flag) {
			char buff[255];
			FILE *fp = fopen(rutaPokemon, "r+");
			int indice = fileno(fp);
			flock(indice, LOCK_EX);

			fscanf(fp, "%s", buff);
			char* estado = string_duplicate(buff);

			if (strcmp(estado, open) == 0) {
				log_error(logger, "El archivo %s ya esta abierto", rutaPokemon);
				flock(indice, LOCK_UN);
				sleep(gameCardConfig->tiempoReintentoOperacion);
			} else {

				flock(indice, LOCK_UN);
				sleep(1);

				fseek(fp, 5, SEEK_SET);
				fputs("Y", fp);

				sleep(1);

				// Linea directory
				fscanf(fp, "%s", buff);

				// Linea size
				fscanf(fp, "%s", buff);
				char* ln_size_actual = string_duplicate(buff);
				char** size_array = string_split(ln_size_actual, "=");
				int int_size_actual;
				string_trim(size_array);
				sscanf(size_array[1], "%d", &int_size_actual);

				// LEO BLOCKS=[1,2,3,4]
				fscanf(fp, "%s", buff);
				char** block_array = string_split(buff, "=");
				string_trim(block_array);

				fclose(fp);

				// Me quedo con [1,2,3,4]
				char** array_strings = string_get_string_as_array(
						block_array[1]);

				int existe = existenPosicionesyReducir(array_strings,
						rutaPokemon, x, y, pokemon);

				if (existe == 0) {
					log_info(logger,
							"Existen las posiciones (%d,%d) para el pokemon %s y se disminuyo la cantidad",
							x, y, pokemon);
					ArchivoAbiertoParaUso(rutaPokemon, pokemon);
					flag = 0;
					return 1;
				} else {
					log_error(logger, "No existen %s en (%d,%d)", pokemon, x,
							y);
					ArchivoAbiertoParaUso(rutaPokemon, pokemon);
					flag = 0;
					return -1;
				}
			}
		}
	}

	return 0;

}

int existenPosicionesyReducir(char** array_strings, char* rutaPokemon, int x,
		int y, char* pokemon) {
	char** array_strings2 = array_strings;
	char aux[64];
	char* newln2 = string_new();
	string_append(&newln2, "BLOCKS=[");
	int i = 0;
	while (*array_strings != NULL) {

		int mismaposicion = 0;
		strcpy(aux, *array_strings);

		char buff2[30];
		char* a_ruta = string_duplicate(aux);

		char* ruta = string_new();
		string_append(&ruta, gameCardConfig->puntoDeMontaje);
		string_append(&ruta, "/Blocks/");
		string_append(&ruta, a_ruta);
		string_append(&ruta, ".bin");

		FILE *fp_block = fopen(ruta, "r+");
		if (!fp_block) {
			log_error(logger, "No se pudo abrir el archivo  %s", ruta);
		}
		int linea_nro = 0;

		// Iteramos hasta que sea el final del file
		while (fscanf(fp_block, "%s", buff2) != EOF) {

			linea_nro++;
			char* s_x = strdup(buff2);
			char** block_arrayx = string_split(s_x, "-");
			char** block_YCant = string_split(block_arrayx[1], "=");

			string_trim(block_arrayx);
			string_trim(block_YCant);

			int int_x;
			sscanf(block_arrayx[0], "%d", &int_x);

			int int_y;
			sscanf(block_YCant[0], "%d", &int_y);

			int int_cant;
			sscanf(block_YCant[1], "%d", &int_cant);

			int cantidad_actualizada;
			if (int_cant == 1) {
				cantidad_actualizada = 0;
			} else {
				cantidad_actualizada = int_cant - 1;
			}

			if (x == int_x) {
				mismaposicion++;
			}
			if (int_y == y) {
				mismaposicion++;
			};

			if (mismaposicion == 2)	// Se encontraron las coordenadas.
					{
				if (cantidad_actualizada == 0) {
					log_debug(logger,
							"Se eliminara la linea porque no quedan pokemons en (%d,%d)",
							x, y);
					fclose(fp_block);

					FILE *fptr1, *fptr2;
					int ctr = 0;

					int MAX = 256;
					char str[256], temp[] = "temp.txt";

					fptr1 = fopen(ruta, "r");
					if (!fptr1) {
						log_error(logger,
								"No se pudo abrir el archivo %s para el catch",
								ruta);
					}

					fptr2 = fopen(temp, "w");
					if (!fptr2) {
						log_error(logger,
								"No se pudo abrir el archivo %s para el catch",
								ruta);
					}
					while (!feof(fptr1)) {
						strcpy(str, "\0");
						fgets(str, MAX, fptr1);
						if (!feof(fptr1)) {
							ctr++;
							if (ctr != linea_nro) {
								fprintf(fptr2, "%s", str);
							}
						}
					}
					fclose(fptr1);
					fclose(fptr2);

					remove(ruta);
					rename(temp, ruta);

					char* ruta2 = string_new();
					string_append(&ruta2, gameCardConfig->puntoDeMontaje);
					string_append(&ruta2, "/Blocks/");
					string_append(&ruta2, a_ruta);
					string_append(&ruta2, ".bin");

					int tamanio = tamanioBloque(ruta2);

					char aux2[30];
					int total = 0;
					char* ruta3[1024];
					int w = 0;
					printf("total es %d\n", total);

					while (*array_strings2 != NULL) {
						printf("array_strings %s\n", *array_strings2);
						strcpy(aux2, *array_strings2);
						char* a_ruta = string_duplicate(aux2);
						ruta3[w] = string_new();
						string_append(&ruta3[w],
								gameCardConfig->puntoDeMontaje);
						string_append(&ruta3[w], "/Blocks/");
						string_append(&ruta3[w], a_ruta);
						string_append(&ruta3[w], ".bin");
						printf("total es %s\n", ruta3[w]);
						total = total + tamanioBloque(ruta3[w]);
						printf("total es %d\n", total);

						w++;
						array_strings2++;
					}
					printf("total es %d\n", total);

					//eliminarBloqueDeMetadata();

					if (tamanio == 0) {
						string_append(&newln2, "");
						actualizarSizePokemon(total, rutaPokemon);
						//actualizarBloquesPokemonParaCatch(rutaPokemon,int_cant);
						ModificarBlock(rutaPokemon, pokemon, newln2);
					}

					free(s_x);
					free(ruta);

					return 0;
				} else {
					// Disminuimos 1 en la cantidad

					int posicion = strlen(string_itoa(int_cant)) * (-1);
					fseek(fp_block, posicion, SEEK_CUR);

					char* texto = string_new();
					string_append(&texto, string_itoa(cantidad_actualizada));
					string_append(&texto, "\n");

					fputs(texto, fp_block);

					fclose(fp_block);

					free(texto);
					free(s_x);
					free(ruta);

					return 0;
				}
			}

			free(s_x);
		}

		string_append(&newln2, array_strings[i]);
		array_strings++;
		i++;
		free(ruta);
		fclose(fp_block);
	}
	return -1;

}

void* recvMensajesGameCard(void* socketCliente) {
	int socket = *(int*) socketCliente;

	t_paquete* bufferLoco = malloc(sizeof(t_paquete));
	int flag = 1;
	while (flag) {
		bufferLoco = recibirMensaje(socket);
		if (bufferLoco != NULL) {
			pthread_mutex_lock(&mutex_bandejaGameCard);
			queue_push(bandejaDeMensajesGameCard, (void*) bufferLoco);
			pthread_mutex_unlock(&mutex_bandejaGameCard);
			sem_post(&bandejaCounter);
		} else
			flag = 0;
	}
	free(bufferLoco);
	return NULL;
}

void* procesarMensajeGameCard() {

	t_paquete* bufferLoco = malloc(sizeof(t_paquete));

	pthread_mutex_lock(&mutex_bandejaGameCard);
	bufferLoco = (t_paquete*) queue_pop(bandejaDeMensajesGameCard);
	pthread_mutex_unlock(&mutex_bandejaGameCard);

//Creo una lista de ID_MESAJES

//bufferLoco->buffer->idMensajeCorrelativo;
//ListIdMensajes =list_create();
//list_add(ListIdMensajes, (void*)bufferLoco->buffer->idMensajeCorrelativo);

	switch (bufferLoco->codigoOperacion) {
	case MENSAJE_NEW_POKEMON: {

		agregarNewPokemon(bufferLoco->buffer->nombrePokemon,
				bufferLoco->buffer->posX, bufferLoco->buffer->posY,
				bufferLoco->buffer->cantidadPokemons);

		int socketBroker;

		socketBroker = crearConexion(gameCardConfig->ipBroker,
				gameCardConfig->puertoBroker,
				gameCardConfig->tiempoReintentoConexion);

		if (socketBroker > 0) {

			enviarMensajeBrokerAppeared(bufferLoco->buffer->nombrePokemon,
					bufferLoco->buffer->posX, bufferLoco->buffer->posY,
					bufferLoco->buffer->idMensajeCorrelativo, socketBroker);
		}

		printf("Temine de agregar Pokemon, Esperando otro mensaje \n");

		break;
	}
	case MENSAJE_GET_POKEMON: {

		printf("ENTRE POR GET_POKEMON Envio LOCALIZED al BROKER \n");

		printf("Id mensaje = %d, Id correlativo =%d\n",
				bufferLoco->buffer->idMensaje,
				bufferLoco->buffer->idMensajeCorrelativo);
		int idCorrelativo = bufferLoco->buffer->idMensaje;
		//bufferLoco = obtenerPokemon(bufferLoco->buffer->nombrePokemon);
		bufferLoco = obtenerCoordenadasPokemon(
				bufferLoco->buffer->nombrePokemon);
		bufferLoco->buffer->idMensajeCorrelativo = idCorrelativo;

		if (list_is_empty(bufferLoco->buffer->listaCoordenadas)) {
			log_info(logger, "No esxiste el pokemon %s en mi file systen",
					bufferLoco->buffer->nombrePokemon);
			free(bufferLoco);
			return NULL;
		}

		int socketBroker;

		socketBroker = crearConexion(gameCardConfig->ipBroker,
				gameCardConfig->puertoBroker,
				gameCardConfig->tiempoReintentoConexion);

		if (socketBroker > 0) {

			//enviarMensajeLocalized(bufferLoco->buffer->nombrePokemon,bufferLoco->buffer->listaCoordenadas,socketBroker);
			enviarMensajeLocalizedId(bufferLoco->buffer->nombrePokemon,
					bufferLoco->buffer->listaCoordenadas,
					bufferLoco->buffer->idMensajeCorrelativo, socketBroker);

		}

		break;
	}

	case MENSAJE_CATCH_POKEMON: {
		log_info(logger, "CATCH %s (%d, %d)", bufferLoco->buffer->nombrePokemon,
				bufferLoco->buffer->posX, bufferLoco->buffer->posY);
		int resultado = catchPokemon(bufferLoco->buffer->nombrePokemon,
				bufferLoco->buffer->posX, bufferLoco->buffer->posY);
		int socketBroker;

		socketBroker = crearConexion(gameCardConfig->ipBroker,
				gameCardConfig->puertoBroker,
				gameCardConfig->tiempoReintentoConexion);

		if (resultado == -1) {
			log_error(logger, "No se pudo atrapar");
			enviarMensajeBrokerCaught(bufferLoco->buffer->idMensaje, 0,
					socketBroker);
		} else {
			if (socketBroker > 0) {
				enviarMensajeBrokerCaught(bufferLoco->buffer->idMensaje, 1,
						socketBroker);
			}
		}
		break;

	}
	default: {
		break;
	}

		printf("Fuera del switch \n");
	}

//}
	printf("Termino el hilo procesoProcesarMensaje \n");
	free(bufferLoco);
	return NULL;
}

void inicializarMutexGameCard() {
	pthread_mutex_init(&mutex_bandejaGameCard, NULL);
	pthread_mutex_init(&mutex_crear_carpeta, NULL);
	pthread_mutex_init(&mutex_cant_blockers, NULL);
	pthread_mutex_init(&bandejaDeMensajesGameCardSuscripcion, NULL);
	sem_init(&contadorBandejaGameCard, 1, 0);
	return;
}

void* handlerGameCard(void* socketDelCliente) {
	recvMensajesGameCard(socketDelCliente);
	return NULL;
}

void *iniciarConexionBroker(void *arg) { //esta es una funcion que va a recibir el pthread_create
	socketBroker = crearConexion(gameCardConfig->ipBroker,
			gameCardConfig->puertoBroker,
			gameCardConfig->tiempoReintentoConexion);
	arg = (void*) socket;
	return NULL;
}

void ArchivoEnUso(char* rutaPokemon, char* pokemon) {
	printf("Entramos a ArchivoEnUso para ruta %s \n", rutaPokemon);
	char buffer[100];
	FILE* fptr1 = fopen(rutaPokemon, "w+");

	while (fscanf(fptr1, "%s", buffer) != EOF) {
		printf("Abrimos el archivo %s\n", rutaPokemon);
		printf("%s\n", buffer);
	}
	/*

	 fscanf(fptr1, "%s", buffer);
	 log_debug(logger,"Que tiene el archivo %s: %s", rutaPokemon, buffer);
	 //flock(indice, LOCK_EX);


	 printf("Despues del lock para el indice %d\n", indice);
	 fseek(fptr1, 6, SEEK_SET);
	 fputs("Y", fptr1);
	 printf("Termine de escribir la Y \n");

	 fseek(fptr1, 0, SEEK_SET);
	 fscanf(fptr1, "%s", buffer);
	 log_debug(logger,"Segunda vez, que tiene el archivo %s: %s", rutaPokemon, buffer);
	 */
	fclose(fptr1);
	return;
	/*
	 FILE *fptr2 = fopen(dir_pokemon, "w+");

	 int lno_size_blocks = 1;
	 int linectr = 0;

	 log_info(logger, "Metadata En USO %d: %s", lno_size_blocks, linea_size1);

	 while (!feof(fptr1)) {
	 strcpy(str, "\0");
	 fgets(str, MAX, fptr1);
	 if (!feof(fptr1)) {
	 linectr++;
	 if (linectr == lno_size_blocks) {
	 fprintf(fptr2, "%s", linea_size1);
	 } else {
	 fprintf(fptr2, "%s", str);
	 }
	 }
	 }

	 fclose(fptr2);
	 remove(rutaPokemon);

	 rename(dir_pokemon, rutaPokemon);

	 free(linea_size1);

	 free(dir_pokemon);

	 return;
	 */
}

void ArchivoAbiertoParaUso(char* rutaPokemon, char* pokemon) {
	FILE* fptr1 = fopen(rutaPokemon, "r+");
	//int indice = fileno(fptr1);
	//flock(indice, LOCK_EX);
	fseek(fptr1, 5, SEEK_SET);
	fputs("N", fptr1);
	//flock(indice, LOCK_UN);
	fclose(fptr1);
	return;
	/*
	 fseek(fp_block, tamanioCantidad, SEEK_CUR);
	 char* dir_pokemon = string_new();
	 string_append(&dir_pokemon, gameCardConfig->puntoDeMontaje);
	 string_append(&dir_pokemon, "/Files/Pokemon/");
	 string_append(&dir_pokemon, pokemon);
	 string_append(&dir_pokemon, "temp.txt");

	 char* linea_size2 = string_new();

	 string_append(&linea_size2, "OPEN=N");

	 string_append(&linea_size2, "\n");

	 int MAX = 256;
	 char str[MAX];

	 FILE *fptr1 = fopen(rutaPokemon, "r");
	 FILE *fptr2 = fopen(dir_pokemon, "w+");

	 int lno_size_blocks = 1;
	 int linectr = 0;

	 log_info(logger, "Linea a ser escrita en la posicion %d: %s",
	 lno_size_blocks, linea_size2);

	 while (!feof(fptr1)) {
	 strcpy(str, "\0");
	 fgets(str, MAX, fptr1);
	 if (!feof(fptr1)) {
	 linectr++;
	 if (linectr == lno_size_blocks) {
	 fprintf(fptr2, "%s", linea_size2);
	 } else {
	 fprintf(fptr2, "%s", str);
	 }
	 }
	 }
	 fclose(fptr1);
	 fclose(fptr2);
	 remove(rutaPokemon);

	 rename(dir_pokemon, rutaPokemon);

	 free(linea_size2);

	 free(dir_pokemon);

	 return;*/
}

void* hiloAgregarPokemon(void* bufferLoco1) {

	printf("SOY UN HILO AUXILIAR\n");

	t_paquete* bufferLoco2 = (t_paquete*) bufferLoco1;

	agregarNewPokemon(bufferLoco2->buffer->nombrePokemon,
			bufferLoco2->buffer->posX, bufferLoco2->buffer->posY,
			bufferLoco2->buffer->cantidadPokemons);

	return NULL;
}

void* auxiliar2(void* bufferLoco) {

	pthread_mutex_lock(&mutex_bandejaGameCard);
	printf("AGREGUE A LA COLA\n");
	queue_push(bandejaDeMensajesGameCard, (void*) bufferLoco);
	pthread_mutex_unlock(&mutex_bandejaGameCard);
//sem_post(&contadorBandejaGameCard);
	sem_post(&bandejaCounter);

	return NULL;

}

void* auxiliar3(void* bufferLoco) {

	sem_wait(&contadorBandejaGameCard);

	pthread_mutex_lock(&mutex_bandejaGameCard);
	bufferLoco = (t_paquete*) queue_pop(bandejaDeMensajesGameCard);
	pthread_mutex_unlock(&mutex_bandejaGameCard);
	if (bufferLoco != NULL) {

	} else {
		printf("ESTA VACIA \n");
	}

	return NULL;
}

void* escucharConexionesGameCard() {
	pthread_t threadId[MAX_CONEXIONES];

	int contadorConexiones = 0;
	int socketDelCliente[MAX_CONEXIONES];
	struct sockaddr direccionCliente;
	unsigned int tamanioDireccion = sizeof(direccionCliente);

	int servidor = initServer(gameCardConfig->ipGameCard,
			gameCardConfig->puertoGameCard);

	log_info(logger, "ESCUCHANDO CONEXIONES");
	log_info(logger, "iiiiIIIII!!!");

	while (1) {

		socketDelCliente[contadorConexiones] = accept(servidor,
				(void*) &direccionCliente, &tamanioDireccion);

		if (socketDelCliente >= 0) {

			if ((pthread_create(&threadId[contadorConexiones], NULL,
					recvMensajesGameCard,
					(void*) &socketDelCliente[contadorConexiones])) < 0) {
				log_info(logger, "No se pudo crear el hilo");

			} else {
				log_info(logger, "Handler asignado\n");
				tamanioDireccion = 0;

			}
		} else {
			log_info(logger, "Falló al aceptar conexión");
		}

		contadorConexiones++;

	}
	return NULL;
}

void* consumirMensajesGameCard() {

	while (1) {
		pthread_t hilito;
		sem_wait(&bandejaCounter);
		//pthread_mutex_lock(&bandejaMensajes_mutex);
		pthread_create(&hilito, NULL, procesarMensajeGameCard, NULL);
		//pthread_mutex_unlock(&bandejaMensajes_mutex);

	}

	return NULL;
}

void* suscribirseNewPokemon() {

	int socketBroker = crearConexion(gameCardConfig->ipBroker,
			gameCardConfig->puertoBroker,
			gameCardConfig->tiempoReintentoConexion);

	pthread_mutex_t mutexRecibir; //este semaforo no lo entiendo muy bien, pero funciona, sin él se rompe todo.
	pthread_mutex_init(&mutexRecibir, NULL);
	suscribirseNew(gameCardConfig->nombreProceso, 0, socketBroker);

	t_paquete *bufferLoco;
	bufferLoco = malloc(sizeof(t_paquete));
	int flag = 1;
	printf("Esperando por un nuevo mensaje AppearedPokemon...\n");

	while (flag) {

		pthread_mutex_lock(&mutexRecibir);
		bufferLoco = recibirMensaje(socketBroker);

		if (bufferLoco != NULL) {
			enviarAck(bufferLoco->buffer->nombrePokemon,
					bufferLoco, socketBroker);
			pthread_mutex_lock(&bandejaDeMensajesGameCardSuscripcion);
			queue_push(bandejaDeMensajesGameCard, bufferLoco); //falta mutex y contador
			pthread_mutex_unlock(&bandejaDeMensajesGameCardSuscripcion);
			pthread_mutex_unlock(&mutexRecibir);
			sem_post(&bandejaCounter);

		} else {

			liberarConexion(socketBroker);
			socketBroker = 0;
			while (socketBroker <= 0) {
				pthread_mutex_unlock(&mutexRecibir);
				sleep(gameCardConfig->tiempoReintentoConexion);

				socketBroker = crearConexionSinReintento(
						gameCardConfig->ipBroker, gameCardConfig->puertoBroker);
			}

			suscribirseNew(gameCardConfig->nombreProceso, 0, socketBroker);
		}
	}
	return NULL;
}
void* suscribirseGetPokemon() {

	int socketBroker = crearConexion(gameCardConfig->ipBroker,
			gameCardConfig->puertoBroker,
			gameCardConfig->tiempoReintentoConexion);

	pthread_mutex_t mutexRecibir; //este semaforo no lo entiendo muy bien, pero funciona, sin él se rompe todo.
	pthread_mutex_init(&mutexRecibir, NULL);
	suscribirseGet(gameCardConfig->nombreProceso, 0, socketBroker);

	t_paquete *bufferLoco;
	bufferLoco = malloc(sizeof(t_paquete));
	int flag = 1;
	printf("Esperando por un nuevo mensaje LocalizedPokemon...\n");

	while (flag) {

		pthread_mutex_lock(&mutexRecibir);
		bufferLoco = recibirMensaje(socketBroker);

		if (bufferLoco != NULL) {

			enviarAck(bufferLoco->buffer->nombrePokemon,
					bufferLoco, socketBroker);

			pthread_mutex_lock(&bandejaDeMensajesGameCardSuscripcion);
			queue_push(bandejaDeMensajesGameCard, bufferLoco); //falta mutex y contador
			pthread_mutex_unlock(&bandejaDeMensajesGameCardSuscripcion);
			pthread_mutex_unlock(&mutexRecibir);
			sem_post(&bandejaCounter);

		} else {

			liberarConexion(socketBroker);
			socketBroker = 0;
			while (socketBroker <= 0) {
				pthread_mutex_unlock(&mutexRecibir);
				sleep(gameCardConfig->tiempoReintentoConexion);

				socketBroker = crearConexionSinReintento(
						gameCardConfig->ipBroker, gameCardConfig->puertoBroker);
			}

			suscribirseGet(gameCardConfig->nombreProceso, 0, socketBroker);
		}
	}
	return NULL;
}
void* suscribirseCatchPokemon() {

	int socketBroker = crearConexion(gameCardConfig->ipBroker,
			gameCardConfig->puertoBroker,
			gameCardConfig->tiempoReintentoConexion);

	pthread_mutex_t mutexRecibir; //este semaforo no lo entiendo muy bien, pero funciona, sin él se rompe todo.
	pthread_mutex_init(&mutexRecibir, NULL);
	suscribirseCatch(gameCardConfig->nombreProceso, 0, socketBroker);

	t_paquete *bufferLoco;
	bufferLoco = malloc(sizeof(t_paquete));
	int flag = 1;
	printf("Esperando por un nuevo mensaje CatchPokemon...\n");

	while (flag) {

		pthread_mutex_lock(&mutexRecibir);
		bufferLoco = recibirMensaje(socketBroker);

		if (bufferLoco != NULL) {
			enviarAck(bufferLoco->buffer->nombrePokemon,
					bufferLoco, socketBroker);

			pthread_mutex_lock(&bandejaDeMensajesGameCardSuscripcion);
			queue_push(bandejaDeMensajesGameCard, bufferLoco); //falta mutex y contador
			pthread_mutex_unlock(&bandejaDeMensajesGameCardSuscripcion);
			pthread_mutex_unlock(&mutexRecibir);
			sem_post(&bandejaCounter);

		} else {

			liberarConexion(socketBroker);
			socketBroker = 0;
			while (socketBroker <= 0) {
				pthread_mutex_unlock(&mutexRecibir);
				sleep(gameCardConfig->tiempoReintentoConexion);

				socketBroker = crearConexionSinReintento(
						gameCardConfig->ipBroker, gameCardConfig->puertoBroker);
			}

			suscribirseCatch(gameCardConfig->nombreProceso, 0, socketBroker);
		}
	}
	return NULL;
}
void* suscribirseABroker() {

	pthread_t hilo1;
	pthread_t hilo2;
	pthread_t hilo3;

	pthread_create(&hilo1, NULL, (void*) suscribirseNewPokemon, NULL);
	pthread_create(&hilo2, NULL, (void*) suscribirseGetPokemon, NULL);
	pthread_create(&hilo3, NULL, (void*) suscribirseCatchPokemon, NULL);

	return NULL;
}

t_bitarray* crear_bitmap() {
	char* rutaBitmap = crearRutaArchivo(RUTA_BITMAP_GENERAL);
	int cantidadDeBloques = g_blocks_maximos / 8;

	int max = 0;
	if (access(rutaBitmap, F_OK) != -1) {
		// Existe el archivo Bitmap
		for (int i = 0; i < cantidadDeBloques; i++) {
			char* ruta = string_new();
			string_append(&ruta, gameCardConfig->puntoDeMontaje);
			string_append(&ruta, "/Blocks/");
			string_append(&ruta, string_itoa(i + 1));
			string_append(&ruta, ".bin");
			int tamanio = tamanioBloque(ruta);
			free(ruta);
			if (tamanio <= 0) {
				max = i;
				break;
			}
		}
		g_blocks_usados = max;
		int fd = open(rutaBitmap, O_CREAT | O_RDWR, 0664);

		ftruncate(fd, cantidadDeBloques);
		if (fd == -1) {
			log_error(logger, "No se pudo abrir el bitmap");
			perror("open file");
			exit(1);
		}
		char* bmap = mmap(NULL, cantidadDeBloques, PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0);
		if (bmap == MAP_FAILED) {
			perror("mmap");
			close(fd);
			exit(1);
		}
		t_bitarray* bitmap = bitarray_create_with_mode(bmap, cantidadDeBloques,
				LSB_FIRST);

		log_info(logger, "El tamano del bloque es de %d bits",
				bitarray_get_max_bit(bitmap));

		msync(bmap, sizeof(bitmap), MS_SYNC);
		munmap(bmap, cantidadDeBloques);

		close(fd);
		free(rutaBitmap);
		return bitmap;

	} else {
		// NO Existe el archivo Bitmap

		g_blocks_usados = 0;
		int fd = open(rutaBitmap, O_CREAT | O_RDWR, 0664);

		ftruncate(fd, cantidadDeBloques);
		if (fd == -1) {
			log_error(logger, "No se pudo abrir el bitmap");
			perror("open file");
			exit(1);
		}
		char* bmap = mmap(NULL, cantidadDeBloques, PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0);
		if (bmap == MAP_FAILED) {
			perror("mmap");
			close(fd);
			exit(1);
		}
		t_bitarray* bitmap = bitarray_create_with_mode(bmap, cantidadDeBloques,
				LSB_FIRST);

		log_info(logger, "El tamano del bloque es de %d bits",
				bitarray_get_max_bit(bitmap));
		size_t tope = bitarray_get_max_bit(bitmap);
		for (int i = 0; i < tope; i++) {
			bitarray_clean_bit(bitmap, i);
		}
		msync(bmap, sizeof(bitmap), MS_SYNC);
		munmap(bmap, cantidadDeBloques);

		close(fd);
		free(rutaBitmap);
		return bitmap;
	}
}

void* ModificarBlock(char* rutaPokemon, char* pokemon, char* newln2) {

	char* dir_pokemon = string_new();
	string_append(&dir_pokemon, gameCardConfig->puntoDeMontaje);
	string_append(&dir_pokemon, "/Files/Pokemon/");
	string_append(&dir_pokemon, pokemon);
	string_append(&dir_pokemon, "temp.txt");

	string_append(&newln2, "]");
	string_append(&newln2, "\n");

	int MAX = 256;
	char str[MAX];

	FILE *fptr1 = fopen(rutaPokemon, "r");
	FILE *fptr2 = fopen(dir_pokemon, "w+");

	int lno_blocks = 4;

	int linectr = 0;

	log_info(logger, "Linea a ser escrita en la posicion %d: %s", lno_blocks,
			newln2);

	while (!feof(fptr1)) {
		strcpy(str, "\0");
		fgets(str, MAX, fptr1);
		if (!feof(fptr1)) {
			linectr++;
			if (linectr == lno_blocks) {
				fprintf(fptr2, "%s", newln2);

			} else {
				fprintf(fptr2, "%s", str);
			}
		}
	}
	fclose(fptr1);
	fclose(fptr2);
	remove(rutaPokemon);

	rename(dir_pokemon, rutaPokemon);

	free(newln2);
	free(dir_pokemon);

	return NULL;
}

void actualizarBitMapen1(int blockUsado) {

	char* rutaBitmap = crearRutaArchivo(RUTA_BITMAP_GENERAL);

	int cantidadDeBloques = g_blocks_maximos / 8;

	int fd = open(rutaBitmap, O_CREAT | O_RDWR, 0664);

	if (fd == -1) {
		perror("open file");
		exit(1);
	}

	ftruncate(fd, cantidadDeBloques);

	char* bmap = mmap(NULL, cantidadDeBloques, PROT_READ | PROT_WRITE,
	MAP_SHARED, fd, 0);

	if (bmap == MAP_FAILED) {
		perror("mmap");
		close(fd);
		exit(1);
	}

	t_bitarray* bitmap = bitarray_create_with_mode(bmap, cantidadDeBloques,
			LSB_FIRST);

	bitarray_set_bit(bitmap, blockUsado);

	if (bitarray_test_bit(bitmap, blockUsado) == 1) {
		log_debug(logger, "Le asigne 1 a la posicicon %d del bitarray",
				blockUsado);
	}

	msync(bmap, sizeof(bitmap), MS_SYNC);
	munmap(bmap, cantidadDeBloques);

	close(fd);

	/*
	 bitarray_test_bit (te devuelve el valor del bit en la posición n)
	 bitarray_set_bit (te setea el valor del bit en la posición n en 1)
	 bitarray_clean_bit (te setea el valor del bit en la posición n en 0)

	 */
	free(rutaBitmap);
	return;

}

int chequearCoordenadasBlock(char** array_strings, int cantidad, int x, int y) {
	char aux[64];
	while (*array_strings != NULL) {

		int mismaposicion = 0;
		strcpy(aux, *array_strings);

		char buff2[30];
		char* a_ruta = string_duplicate(aux);

		char* ruta = string_new();
		string_append(&ruta, gameCardConfig->puntoDeMontaje);
		string_append(&ruta, "/Blocks/");
		string_append(&ruta, a_ruta);
		string_append(&ruta, ".bin");

		FILE *fp_block = fopen(ruta, "r+");
		if (!fp_block) {
			log_error(logger, "No se pudo abrir el archivo  %s", ruta);
		}

		// Iteramos hasta que sea el final del file
		while (fscanf(fp_block, "%s", buff2) != EOF) {
			char* s_x = strdup(buff2);
			char** block_arrayx = string_split(s_x, "-");
			char** block_YCant = string_split(block_arrayx[1], "=");

			//["2","3=10"]

			//["3","10"]

			string_trim(block_arrayx);
			string_trim(block_YCant);

			int int_x;
			sscanf(block_arrayx[0], "%d", &int_x);

			int int_y;
			sscanf(block_YCant[0], "%d", &int_y);

			int int_cant;

			sscanf(block_YCant[1], "%d", &int_cant);

			int cantidad_actualizada = cantidad + int_cant;
			if (x == int_x) {

				mismaposicion++;
			}

			if (int_y == y) {
				mismaposicion++;
			};

			if (mismaposicion == 2)	// Se encontraron las coordenadas.
					{

				int posicion = strlen(string_itoa(int_cant)) * (-1);
				fseek(fp_block, posicion, SEEK_CUR);

				char* texto = string_new();
				string_append(&texto, string_itoa(cantidad_actualizada));
				string_append(&texto, "\n");

				fputs(texto, fp_block);

				fclose(fp_block);

				free(texto);
				free(s_x);
				free(ruta);

				return 0;
			}
		}
		array_strings++;
		free(ruta);
		fclose(fp_block);
	}
	return -1;
}

int tamanioBloque(char* ruta) {

	FILE* archivoBloque = fopen(ruta, "r+");

	int c;
	int count = 0;
	for (c = getc(archivoBloque); c != EOF; c = getc(archivoBloque)) {
		count = count + 1;
	}
	fclose(archivoBloque);
	return count;
}
/*
 void actualizarBitMapen0(int blockUsado) {

 printf("Estoy en actualizarBitMapen0\n");
 char* rutaBitmap = crearRutaArchivo(RUTA_BITMAP_GENERAL);

 int cantidadDeBloques = 1024 / 8;

 int fd = open(rutaBitmap, O_CREAT | O_RDWR, 0664);

 if (fd == -1) {
 perror("open file");
 exit(1);
 }

 ftruncate(fd, cantidadDeBloques);

 char* bmap = mmap(NULL, cantidadDeBloques, PROT_READ | PROT_WRITE,
 MAP_SHARED, fd, 0);
 if (bmap == MAP_FAILED) {
 perror("mmap");
 close(fd);
 exit(1);
 }
 t_bitarray* bitmap = bitarray_create_with_mode(bmap, cantidadDeBloques,
 LSB_FIRST);

 bitarray_clean_bit(bitmap, blockUsado);
 if (bitarray_test_bit(bitmap, blockUsado) == 0) {
 log_debug(logger, "le asigne 1 a la posicicon %d al bit ARRaY", blockUsado);
 }
 msync(bmap, sizeof(bitmap), MS_SYNC);
 munmap(bmap, cantidadDeBloques);
 close(fd);

 bitarray_test_bit (te devuelve el valor del bit en la posición n)
 bitarray_set_bit (te setea el valor del bit en la posición n en 1)
 bitarray_clean_bit (te setea el valor del bit en la posición n en 0)



 }*/

int BuscarEspacioBitMap() {

	log_debug(logger, "Buscando un lugar vacio en BuscandoBitmap");

	char* rutaBitmap = crearRutaArchivo(RUTA_BITMAP_GENERAL);

	int cantidadDeBloques = g_blocks_maximos / 8;

	int fd = open(rutaBitmap, O_CREAT | O_RDWR, 0664);

	if (fd == -1) {
		perror("open file");
		exit(1);
	}

	ftruncate(fd, cantidadDeBloques);

	char* bmap = mmap(NULL, cantidadDeBloques, PROT_READ | PROT_WRITE,
	MAP_SHARED, fd, 0);

	if (bmap == MAP_FAILED) {
		perror("mmap");
		close(fd);
		exit(1);
	}

	t_bitarray* bitmap = bitarray_create_with_mode(bmap, cantidadDeBloques,
			LSB_FIRST);

	int posicion;

	for (int recorrido = 1; recorrido <= g_blocks_maximos; recorrido++) {
		if (bitarray_test_bit(bitmap, recorrido) == 0) {
			posicion = recorrido;
			break;
		}
	}

	log_debug(logger, "Posicion %d libre en el bitmap", posicion);

	msync(bmap, sizeof(bitmap), MS_SYNC);
	munmap(bmap, cantidadDeBloques);

	close(fd);

	free(rutaBitmap);

	return posicion;

}
int agregarLineaAlFinalDelBloque(char* bloque, int x, int y, int cantidad) {
	char* rutaBlocks = string_new();
	string_append(&rutaBlocks, gameCardConfig->puntoDeMontaje);
	string_append(&rutaBlocks, "/Blocks/");
	string_append(&rutaBlocks, bloque);
	string_append(&rutaBlocks, ".bin");

	char* nueva_linea = string_new();
	string_append(&nueva_linea, string_itoa(x));
	string_append(&nueva_linea, "-");
	string_append(&nueva_linea, string_itoa(y));
	string_append(&nueva_linea, "=");
	string_append(&nueva_linea, string_itoa(cantidad));
	string_append(&nueva_linea, "\n");

	int tamanoNuevaLinea = strlen(nueva_linea);
	escribir_archivo(rutaBlocks, nueva_linea);
	free(nueva_linea);
	free(rutaBlocks);
	return tamanoNuevaLinea;
}

int escribirPokemonOBuscarBloqueLibre(int x, int y, int cantidad) {

	int lugarlibre = BuscarEspacioBitMap();

	if (lugarlibre == -1) {
		log_error(logger, "No hay lugar en el bitmap)");
		return -1;

	}

	log_debug(logger, "lugarlibre %d, g_blocks_usados %d", lugarlibre,
			g_blocks_usados);

	pthread_mutex_lock(&mutex_cant_blockers);
	g_blocks_usados = lugarlibre;
	pthread_mutex_unlock(&mutex_cant_blockers);

	char* rutaBlocks = string_new();
	string_append(&rutaBlocks, gameCardConfig->puntoDeMontaje);
	string_append(&rutaBlocks, "/Blocks/");
	string_append(&rutaBlocks, string_itoa(g_blocks_usados));
	string_append(&rutaBlocks, ".bin");

	char* nueva_line = string_new();
	string_append(&nueva_line, string_itoa(x));
	string_append(&nueva_line, "-");
	string_append(&nueva_line, string_itoa(y));
	string_append(&nueva_line, "=");
	string_append(&nueva_line, string_itoa(cantidad));
	string_append(&nueva_line, "\n");

	int tamanoNuevaLinea = strlen(nueva_line);

	escribir_archivo(rutaBlocks, nueva_line);
	free(rutaBlocks);
	free(nueva_line);

	return tamanoNuevaLinea;

}

t_paquete* obtenerCoordenadasPokemon(char* pokemon) {
	t_posicion *XY = malloc(sizeof(t_posicion));
	t_list *CoordXY = list_create();

	t_paquete *bufferLoco;
	bufferLoco = malloc(sizeof(t_paquete));
	bufferLoco->buffer = malloc(sizeof(t_bufferOmnidata));

	bufferLoco->buffer->listaCoordenadas = list_create();
	int cantidad = 0;
	char* rutaPokemon = crearRutaPokemon(pokemon);

	char* carpetaPokemon = string_new();
	string_append(&carpetaPokemon, gameCardConfig->puntoDeMontaje);
	string_append(&carpetaPokemon, "/Files/Pokemon/");
	string_append(&carpetaPokemon, pokemon);

	if (existePokemon(rutaPokemon) == 1) //NO EXISTE EL POKEMON
			{

		log_info(logger, "existe el pokemon");

		char buff[255];
		FILE *fp = fopen(rutaPokemon, "r");
		// Linea OPEN
		fscanf(fp, "%s", buff);
		// Pasamos la primera linea
		fscanf(fp, "%s", buff);
		// Linea SIZE
		fscanf(fp, "%s", buff);
		char* ln_size_actual = string_duplicate(buff);
		char** size_array = string_split(ln_size_actual, "=");
		int int_size_actual;
		string_trim(size_array);
		sscanf(size_array[1], "%d", &int_size_actual);
		free(ln_size_actual);

		fscanf(fp, "%s", buff);
		char** block_array = string_split(buff, "=");
		string_trim(block_array);

		fclose(fp);

		// restamos los []

		char* newln2 = string_new();
		string_append(&newln2, "BLOCKS=[");

		char aux[100];
		char** array_strings = string_get_string_as_array(block_array[1]);
		// Itearamos para saber si existen las mismas coordenadas en el filesystem
		// Si es el caso, aumentamos solo la cantidad dentro del block y terminamos.

		while (*array_strings != NULL) {

			strcpy(aux, *array_strings);

			char buff2[30];
			char* a_ruta = string_duplicate(aux);

			char* ruta = string_new();
			string_append(&ruta, gameCardConfig->puntoDeMontaje);
			string_append(&ruta, "/Blocks/");
			string_append(&ruta, a_ruta);
			string_append(&ruta, ".bin");

			FILE *fp_block = fopen(ruta, "r+");
			if (!fp_block) {
				log_error(logger, "No se pudo abrir el archivo  %s", ruta);
			}

			// Iteramos hasta que sea el final del file
			while (fscanf(fp_block, "%s", buff2) != EOF) {
				char* s_x = strdup(buff2);
				char** block_arrayx = string_split(s_x, "-");
				char** block_YCant = string_split(block_arrayx[1], "=");

				//["2","3=10"]

				//["3","10"]

				string_trim(block_arrayx);
				string_trim(block_YCant);

				int int_x;
				sscanf(block_arrayx[0], "%d", &int_x);
				//agrego la coordenada x a la lista [(x,)]
				XY->x = int_x;

				int int_y;
				sscanf(block_YCant[0], "%d", &int_y);
				//agrego la coordenada x a la lista [(x,)]
				XY->y = int_y;
				list_add(CoordXY, (void*) XY);
				int int_cant;

				sscanf(block_YCant[1], "%d", &int_cant);
				cantidad = +int_cant;

			}
			array_strings++;

			free(ruta);

		}

		free(block_array);

		bufferLoco->buffer->listaCoordenadas = list_duplicate(CoordXY);
		bufferLoco->buffer->nombrePokemon = pokemon;
		bufferLoco->buffer->cantidadPokemons = cantidad;

		list_destroy(CoordXY);

	}

	return bufferLoco;
}
