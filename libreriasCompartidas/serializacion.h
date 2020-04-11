#ifndef SERIALIZACION_H_
#define SERIALIZACION_H_

#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <commons/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "sockets.h"
#include <dirent.h>

#define uint unsigned int

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

typedef enum t_protocolo {
	tAdministrativo = 1,
	tPath,
	tWrite,
	tRead,
	tDIR,
	tSuseCreate,
	tSuseWait,
	tSuseSignal,
	tSuseJoin,
	tSuseClose,
	tMuseInit,
	tMuseClose,
	tMuseAlloc,
	tMuseFree,
	tMuseGet,
	tMuseCpy,
	tMuseMap,
	tMuseSync,
	tMuseUnmap,
	tFinDeProtocolo //NO SACAR Y DEJAR A LO ULTIMO!!!
} t_protocolo;

typedef enum t_tipoOperacion{
	tGetattr = 1,
	tMkdir,
	tRmdir,
	tOpen,
	tRelease,
	tFlush,
	tReleaseDir,
	tOpenDir,
	tFinTipoOperacion //NO SACAR Y DEJAR A LO ULTIMO!!!
} t_tipoOperacion;

typedef enum t_tipoAdministrativo{
	tSuseScheduleNext = 1,
	tDamePid,
	tFinTipoAdministrativo //NO SACAR Y DEJAR A LO ULTIMO!!!
} t_tipoAdministrativo;

typedef enum {SC=1,SHC,EC} TIPO_CONSISTENCIA;

///////////////////////////////////////////FUSE - SAC - SACCLI//////////////////////////////////////

typedef struct t_path{
	char* path;
	//Acá podria ir el File Descriptor (fi->fh). Las operaciones que no lo usan (es rellenado luego) no lo mandarían. (ejemplo sac_open())
	int tipoOperacion;
} __attribute__((packed)) t_path;

typedef struct t_write{
	char* path;
	char* datos;
	off_t desplazamiento;
} __attribute__((packed)) t_write;

typedef struct t_read{
	char* path;
	off_t desplazamiento;
} __attribute__((packed)) t_read;

///////////////////////////////////////////MUSE - LIBMUSE//////////////////////////////////////////

typedef struct t_museAlloc{
	uint32_t pid;
	uint32_t tamanio;
} __attribute__((packed)) t_museAlloc;

typedef struct t_museCpy{
	uint32_t pid;
	uint32_t posicion;
	char* aCopiar;
	uint32_t cantidad;
	
} __attribute__((packed)) t_museCpy;

typedef struct t_museGet{
	uint32_t pid;
	uint32_t posicion;
	char* aCopiar;
	uint32_t cantidad;
	
} __attribute__((packed)) t_museGet;

//////////////////////////////////////////COMUNES A TODOS//////////////////////////////////////////

typedef struct t_administrativo{
	int16_t codigo;
	uint32_t valor;
} __attribute__((packed)) t_administrativo;

typedef struct infoAdminConexiones_s{
    t_log* log;
	int socketCliente;
} __attribute__((packed)) infoAdminConexiones_t;

typedef struct infoServidor_s{
    t_log* log;
	int puerto;
	char* ip;
} __attribute__((packed)) infoServidor_t;

/////////////////////////////////////////HILOLAY HACIA SUSE////////////////////////////////////////

typedef struct t_suseCreate{
	int PID;
	int TID;
} __attribute__((packed)) t_suseCreate;

typedef struct t_suseWait{
	uint PID;
	uint TID;
	char* idSemaforo;
} __attribute__((packed)) t_suseWait;

typedef struct t_suseSignal{
	uint PID;
	uint TID;
	char* idSemaforo;
} __attribute__((packed)) t_suseSignal;

typedef struct t_suseJoin{
	uint PID;
	uint TIDEsperado;
} __attribute__((packed)) t_suseJoin;

typedef struct t_suseClose{
	uint PID;
	uint TID;
} __attribute__((packed)) t_suseClose;

////////////////////////////////////////////SUSE HACIA HILOLAY//////////////////////////////////////



///////////////////////////////////////////FUNCIONES SERIALIZACION/////////////////////////////////

void* recibirPaquete(int fdCliente, int* tipoMensaje, int* tamanioMensaje, t_log *log);
void  enviarPaquete(int fdCliente, int tipoMensaje, void * mensaje, int tamanioMensaje, t_log *log);

void* serializar(int tipoMensaje, void* mensaje, int* tamanio);
void* deserializar(uint16_t tipoMensaje, void* mensaje);

void* serializarPath(void* path, int* tamanio);
t_path* deserializarPath(void* buffer);

void* serializarAdministrativo(void* administrativo, int* tamanio);
t_administrativo* deserializarAdministrativo(void* buffer);

void* serializarWrite(void* write, int* tamanio);
t_write* deserializarWrite(void* buffer);

void* serializarRead(void* read, int* tamanio);
t_read* deserializarRead(void* buffer);

void* serializarSuseCreate(t_suseCreate *suseCreate, int *tamanio);
t_suseCreate *deserializarSuseCreate(void *buffer);

void *serializarSuseWait(t_suseWait *suseWait, int *tamanio);
t_suseWait *deserializarSuseWait(void *buffer);

void *serializarSuseSignal(t_suseSignal *suseSignal, int *tamanio);
t_suseSignal *deserializarSuseSignal(void *buffer);

void *serializarSuseJoin(t_suseJoin *suseJoin, int *tamanio);
t_suseJoin *deserializarSuseJoin(void *buffer);

void *serializarSuseClose(t_suseClose *suseClose, int *tamanio);
t_suseClose *deserializarSuseClose(void *buffer);

///////////////////////FUNCIONES SERIALIZACION MUSE-LIBMUSE///////////////////////////////////

void* serializarMuseCpy(void* administrativo, int* tamanio);
t_museCpy* deserializarMuseCpy(void* buffer);

void* serializarMuseGet(void* administrativo, int* tamanio);
t_museCpy* deserializarMuseGet(void* buffer);

void* serializarMuseAlloc(void* administrativo, int* tamanio);
t_museAlloc* deserializarMuseAlloc(void* buffer);



#endif /* SERIALIZACION_H_ */