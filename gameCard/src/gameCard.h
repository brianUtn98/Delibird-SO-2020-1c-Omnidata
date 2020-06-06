#ifndef GAMECARD_GAMECARD_H_
#define GAMECARD_GAMECARD_H_
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include <pthread.h>
#include <../MiLibreria/utils/utils.h>
#include<../MiLibreria/utils/cliente.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <commons/process.h>


#define GAMECARD_CONFIG_PATH "gameCard.config"
#define GAMECARD_LOG_PATH "GAMECARD.log"

#define RUTA_METADATA_GENERAL "/Metadata/Metadata.bin"
#define RUTA_BITMAP_GENERAL "/Metadata/Bitmap.bin"


pthread_mutex_t lock;
struct stat st;

typedef struct {
	int tiempoReintentoConexion;
	int tiempoReintentoOperacion;
	char *puntoDeMontaje;
	char *ipBroker;
	int puertoBroker;

} t_GAMECARDConfig;

t_log *logger;
t_config *GAMECARDTConfig;
t_GAMECARDConfig *gameCardConfig;

int conexion;
static int maximo_block_creado=0;

void inicializar_logger(void);
void cargarConfigGameCard(void);

/**
 * @NAME: suscribirmeAColasBroker
 * @DESC: Suscribe al proceso GameCard a 3 colas del Broker
 * y espera una confirmacion de este ultimo.
 * Colas:
 *  - tNEW_POKEMON
 *  - tCATCH_POKEMON
 *  - tGET_POKEMON
 */
void suscribirmeAColasBroker();

/**
 * @NAME: iniciarTallGrass
 * @DESC: Crea dentro del filesystem los archivo generales
 * metadata y bitmap.
 */
void iniciarTallGrass();

char* crearBlock(int block, int x, int y, int cant);

void terminarPrograma();
void crearEscribirArchivo(char* rutaArchivo, char* stringAEscribir);
char* crearRutaArchivo(char* nombreArchivo);
void agregarNewPokemon(char* pokemon, t_list* l_coordenadas, int cantidad);
int catchPokemon(int mensajeID, char* pokemon, int posicionMapa);
int existePokemon(char* pokemon);
int archivoAbierto(char* rutaArchivo);

#endif /* GAMECARD_GAMECARD_H_ */

