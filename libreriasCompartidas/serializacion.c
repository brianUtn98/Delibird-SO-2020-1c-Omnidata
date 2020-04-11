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
#include "serializacion.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////SERIALIZACION PAQUETES//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void enviarPaquete(int fdCliente, int tipoMensaje, void *mensaje, int tamanioMensaje, t_log *log)
{

	int desplazamiento = 0;

	void *mensajeSerializado = serializar(tipoMensaje, mensaje, &tamanioMensaje);
	int tamanioTotal = sizeof(int) * 2 + tamanioMensaje;

	// printf("\nTamaño total del paquete serializado dentro del enviarPaquete: %d", tamanioTotal);
	// printf("\nTipo del paquete dentro del enviarPaquete: %d", tipoMensaje);

	void *buffer = malloc(tamanioTotal);
	memcpy(buffer + desplazamiento, &tipoMensaje, sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(buffer + desplazamiento, &tamanioMensaje, sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(buffer + desplazamiento, mensajeSerializado, tamanioMensaje);

	enviarPorSocket(fdCliente, buffer, tamanioTotal, log);

	free(buffer);
	free(mensajeSerializado);
}

void* recibirPaquete(int fdCliente, int* tipoMensaje, int* tamanioMensaje, t_log *log){

	int recibido = recibirPorSocket(fdCliente, tipoMensaje, sizeof(int), log);

	printf("\n--->Tipo de mensaje en recibir paquete: %d",*tipoMensaje);
	printf("\n--->Bytes recibidos en primera iteracion: %d",recibido);


	if (*tipoMensaje < 1 || *tipoMensaje > tFinDeProtocolo || recibido <= 0){
		return NULL;
	}

	recibido = recibirPorSocket(fdCliente, tamanioMensaje, sizeof(int), log);
	void* mensaje = malloc(*tamanioMensaje);

	printf("\n--->Bytes recibidos en segunda iteracion: %d",recibido);
	printf("\n--->Tamaño del paquete a recibir: %d",*tamanioMensaje);

	recibido = recibirPorSocket(fdCliente, mensaje, *tamanioMensaje, log);

 	printf("\n--->Bytes recibidos en tercera iteracion: %d",recibido);

	void* buffer = deserializar(*tipoMensaje, mensaje);

	free(mensaje);
	return buffer;

} // Recordar castear

void *serializar(int tipoMensaje, void *mensaje, int *tamanio)
{
	void *buffer;

	switch (tipoMensaje)
	{

	// case tHilolayInit:
	// {
	// 	//Creo que no hace falta hacer nada
	// 	break;
	// }

	case tSuseCreate:
	{
		buffer = serializarSuseCreate(mensaje, tamanio);
		break;
	}

	// case tSuseScheduleNext:
	// {
	// 	//buffer = serializarSuseScheduleNext(mensaje, tamanio);
	// 	buffer = serializarAdministrativo(mensaje, tamanio);
	// 	break;
	// }

	case tSuseWait:
	{
		buffer = serializarSuseWait(mensaje, tamanio);
		break;
	}

	// case tSuseWaitRetorno:
	// {
	// 	buffer = serializarSuseWaitRetorno(mensaje, tamanio);
	// 	break;
	// }

	case tSuseSignal:
	{
		buffer = serializarSuseSignal(mensaje, tamanio);
		break;
	}

	case tSuseJoin:
	{
		buffer = serializarSuseJoin(mensaje, tamanio);
		break;
	}

	case tSuseClose:
	{
		buffer = serializarSuseClose(mensaje, tamanio);
		break;
	}

	case tAdministrativo: {
			buffer = serializarAdministrativo(mensaje, tamanio);
			break;
		}

	case tPath:{
			buffer = serializarPath(mensaje, tamanio);
			break;
		}

	case tWrite:{
			buffer = serializarWrite(mensaje, tamanio);
			break;
		}

	case tRead:{
			buffer = serializarRead(mensaje, tamanio);
			break;
		}
	//MUSE-LIBMUSE
	case tMuseAlloc: {
			buffer = serializarMuseAlloc(mensaje, tamanio);
			break;
	}

	case tMuseCpy: {
		buffer = serializarMuseCpy(mensaje, tamanio);
		break;
	}

	case tMuseGet: {
			buffer = serializarMuseGet(mensaje, tamanio);
			break;
	}

	

	// case 0:
	// {
	// 	//printf("Desconexion. \n");
	// 	break;
	// }

	default:
	{
		//perror("Se recibio un mensaje que no esta en el protocolo.");
		abort();
		break;
	}
	}
	return buffer;
}

void *deserializar(uint16_t tipoMensaje, void *mensaje)
{

	void *buffer;

	switch (tipoMensaje)
	{

	case tSuseCreate:
	{
		buffer = deserializarSuseCreate(mensaje);
		break;
	}

	case tSuseWait:
	{
		buffer = deserializarSuseWait(mensaje);
		break;
	}

	case tSuseSignal:
	{
		buffer = deserializarSuseSignal(mensaje);
		break;
	}

	case tSuseJoin:
	{
		buffer = deserializarSuseJoin(mensaje);
		break;
	}

	case tSuseClose:
	{
		buffer = deserializarSuseClose(mensaje);
		break;
	}

	case tAdministrativo: {
				buffer = deserializarAdministrativo(mensaje);
				break;
			}

	case tPath:{
		buffer = deserializarPath(mensaje);
		break;
	}

	case tWrite:{
		buffer = deserializarWrite(mensaje);
		break;
	}

	case tRead:{
		buffer = deserializarRead(mensaje);
		break;
	}
	//MUSE-LIBMUSE
	case tMuseAlloc: {
				buffer = deserializarMuseAlloc(mensaje);
				break;
	}

	case tMuseCpy: {
		buffer = deserializarMuseCpy(mensaje);
		break;
	}

	case tMuseGet: {
		buffer = deserializarMuseGet(mensaje);
		break;
	}

	// case 0:
	// {
	// 	//printf("Desconexion. \n");
	// 	break;
	// }

	default:
	{
		//perror("Se recibio un mensaje que no esta en el protocolo.");
		abort();
		break;
	}
	}
	return buffer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////SERIALIZACION PAQUETES//////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////ADMINISTRATIVO////////////////////////////////////////////////////

void* serializarAdministrativo(void* administrativo, int* tamanio) {

	t_administrativo* unAdministrativo = (t_administrativo*) administrativo;

	*tamanio = sizeof(uint16_t) + sizeof(uint32_t);

	int desplazamiento = 0;

	void* paqueteAdministrativo = malloc(*tamanio);

	memcpy(paqueteAdministrativo + desplazamiento, &unAdministrativo->codigo, sizeof(uint16_t));
	desplazamiento += sizeof(uint16_t);

	memcpy(paqueteAdministrativo + desplazamiento, &unAdministrativo->valor, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	///////////////////////////////////////CASO DE PRUEBA SERIALIZACION ADMINISTRATIVO////////////////////////////////////////////

	printf("\n\nADMINISTRATIVO A SERIALIZAR: \n");
	printf("\nCodigo: %d", unAdministrativo->codigo);
	printf("\nValor: %d", unAdministrativo->valor);
	printf("\nTamaño del administrativo: %d", *tamanio);
	
	t_administrativo* administrativoDeserializado = deserializarAdministrativo(unAdministrativo);

	printf("\n\nADMINISTRATIVO DESERIALIZADO: \n");
	printf("\nCodigo: %d", administrativoDeserializado->codigo);
	printf("\nValor: %d\n", administrativoDeserializado->valor);

	free(administrativoDeserializado);

	///////////////////////////////////////FIN CASO DE PRUEBA SERIALIZACION ADMINISTRATIVO////////////////////////////////////////////

	return paqueteAdministrativo;

}

t_administrativo* deserializarAdministrativo(void* buffer) {

	int desplazamiento = 0;
	
	t_administrativo* unAdministrativo = malloc(sizeof(t_administrativo));

	memcpy(&unAdministrativo->codigo, buffer + desplazamiento, sizeof(uint16_t));
	desplazamiento += sizeof(uint16_t);

	memcpy(&unAdministrativo->valor, buffer + desplazamiento, sizeof(uint32_t));

	return unAdministrativo;

}

////////////////////////////////////////////////////PATH////////////////////////////////////////////////////

void* serializarPath(void* path, int* tamanio){

	t_path* unPath = (t_path*) path;
	
	int desplazamiento = 0;
	int tamanioPath = string_length(unPath->path);

	*tamanio = sizeof(int) * 2 + tamanioPath;

	void* pathSerializado = malloc(*tamanio);

	memcpy(pathSerializado + desplazamiento, &tamanioPath,sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(pathSerializado + desplazamiento, unPath->path,tamanioPath);
	desplazamiento += tamanioPath;

	memcpy(pathSerializado + desplazamiento, &unPath->tipoOperacion, sizeof(int));
	desplazamiento += sizeof(int);

	///////////////////////////////////////CASO DE PRUEBA SERIALIZACION PATH////////////////////////////////////////////

	// printf("\nPATH A SERIALIZAR: \n");
	// printf("\nPath: %s", unPath->path);
	// printf("\nTipo de Operacion: %d", unPath->tipoOperacion);
	// printf("\nTamaño del path: %d", *tamanio);
	
	// t_path* pathDeserializado = deserializarPath(pathSerializado);

	// printf("\n\nPATH DESERIALIZADO: \n");
	// printf("\nPath: %s", pathDeserializado->path);
	// printf("\nTipo de Operacion: %d", pathDeserializado->tipoOperacion);
	
	// free(pathDeserializado);

	///////////////////////////////////////FIN CASO DE PRUEBA SERIALIZACION PATH/////////////////////////////////////////

	return pathSerializado;
}

t_path* deserializarPath(void* buffer){

	t_path* unPath = malloc(sizeof(t_path));

	int desplazamiento = 0;
	int tamanioPath = 0;

	memcpy(&tamanioPath, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	char* bufferPath = malloc(tamanioPath+1);
	memcpy(bufferPath, buffer + desplazamiento, tamanioPath);
	bufferPath[tamanioPath] = '\0';
	desplazamiento += tamanioPath;

	unPath->path = string_new();
	string_append(&unPath->path, bufferPath);

	memcpy(&unPath->tipoOperacion, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	free (bufferPath);

	return unPath;
}

////////////////////////////////////////////////////WRITE////////////////////////////////////////////////////

void* serializarWrite(void* write, int* tamanio){

	t_write* unWrite = (t_write*) write;
	
	int desplazamiento = 0;

	int tamanioPath = string_length(unWrite->path);

	int tamanioDatos = string_length(unWrite->datos);

	*tamanio = tamanioPath + tamanioDatos + sizeof(int) * 2 + sizeof(off_t);

	void* writeSerializado = malloc(*tamanio);

	memcpy(writeSerializado + desplazamiento, &tamanioPath,sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(writeSerializado + desplazamiento, unWrite->path,tamanioPath);
	desplazamiento += tamanioPath;

	memcpy(writeSerializado + desplazamiento, &tamanioDatos,sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(writeSerializado + desplazamiento, unWrite->datos,tamanioDatos);
	desplazamiento += tamanioDatos;

	memcpy(writeSerializado + desplazamiento, &unWrite->desplazamiento, sizeof(off_t));
	desplazamiento += sizeof(off_t);

	///////////////////////////////////////CASO DE PRUEBA SERIALIZACION WRITE////////////////////////////////////////////

	printf("\nWRITE A SERIALIZAR: \n");
	printf("\nPath: %s", unWrite->path);
	printf("\nDatos: %s", unWrite->datos);
	printf("\nDesplazamiento: %d", unWrite->desplazamiento);
	printf("\nTamaño del Write: %d", *tamanio);
	
	t_write* writeDeserializado = deserializarWrite(writeSerializado);

	printf("\nPath: %s", writeDeserializado->path);
	printf("\nDatos: %s", writeDeserializado->datos);
	printf("\nDesplazamiento: %d", writeDeserializado->desplazamiento);
	printf("\nTamaño del Write: %d", *tamanio);
	
	free(writeDeserializado);

	///////////////////////////////////////FIN CASO DE PRUEBA SERIALIZACION PATH/////////////////////////////////////////

	return writeSerializado;
}

t_write* deserializarWrite(void* buffer){

	t_write* unWrite = malloc(sizeof(t_write));

	int desplazamiento = 0;

	int tamanioPath = 0;
	int tamanioDatos = 0;

	memcpy(&tamanioPath, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	char* bufferPath = malloc(tamanioPath+1);
	memcpy(bufferPath, buffer + desplazamiento, tamanioPath);
	bufferPath[tamanioPath] = '\0';
	desplazamiento += tamanioPath;

	unWrite->path = string_new();
	string_append(&unWrite->path, bufferPath);

	memcpy(&tamanioDatos, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	char* bufferDatos = malloc(tamanioDatos+1);
	memcpy(bufferDatos, buffer + desplazamiento, tamanioDatos);
	bufferDatos[tamanioDatos] = '\0';
	desplazamiento += tamanioDatos;

	unWrite->datos = string_new();
	string_append(&unWrite->datos, bufferDatos);

	memcpy(&unWrite->desplazamiento, buffer + desplazamiento, sizeof(off_t));
	desplazamiento += sizeof(off_t);

	free(bufferPath);
	free(bufferDatos);

	return unWrite;
}

////////////////////////////////////////////////////READ////////////////////////////////////////////////////

void* serializarRead(void* read, int* tamanio){

	t_read* unRead = (t_read*) read;
	
	int desplazamiento = 0;

	int tamanioPath = string_length(unRead->path);

	*tamanio = tamanioPath + sizeof(int)  + sizeof(off_t);

	void* readSerializado = malloc(*tamanio);

	memcpy(readSerializado + desplazamiento, &tamanioPath,sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(readSerializado + desplazamiento, unRead->path,tamanioPath);
	desplazamiento += tamanioPath;

	memcpy(readSerializado + desplazamiento, &unRead->desplazamiento, sizeof(off_t));
	desplazamiento += sizeof(off_t);

	///////////////////////////////////////CASO DE PRUEBA SERIALIZACION READ////////////////////////////////////////////

	printf("\nREAD A SERIALIZAR: \n");
	printf("\nPath: %s", unRead->path);
	printf("\nDesplazamiento: %d", unRead->desplazamiento);
	printf("\nTamaño del Read: %d", *tamanio);
	
	t_read* readDeserializado = deserializarRead(readSerializado);

	printf("\nPath: %s", readDeserializado->path);
	printf("\nDesplazamiento: %d", readDeserializado->desplazamiento);
	printf("\nTamaño del Write: %d", *tamanio);
	
	free(readDeserializado);

	///////////////////////////////////////FIN CASO DE PRUEBA SERIALIZACION PATH/////////////////////////////////////////

	return readSerializado;
}

t_read* deserializarRead(void* buffer){

	t_read* unRead = malloc(sizeof(t_read));

	int desplazamiento = 0;

	int tamanioPath = 0;

	memcpy(&tamanioPath, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	char* bufferPath = malloc(tamanioPath+1);
	memcpy(bufferPath, buffer + desplazamiento, tamanioPath);
	bufferPath[tamanioPath] = '\0';
	desplazamiento += tamanioPath;

	unRead->path = string_new();
	string_append(&unRead->path, bufferPath);

	memcpy(&unRead->desplazamiento, buffer + desplazamiento, sizeof(off_t));
	desplazamiento += sizeof(off_t);

	free (bufferPath);

	return unRead; 
    
}

///////////////////////////////////////////////////SUSECREATE///////////////////////////////////////////////
void* serializarSuseCreate(t_suseCreate *suseCreate, int *tamanio)
{
	t_suseCreate *unSuseCreate = (t_suseCreate *)suseCreate;

	int desplazamiento = 0;

	*tamanio = sizeof(int) * 2;

	void *suseCreateSerializado = malloc(*tamanio);

	memcpy(suseCreateSerializado + desplazamiento, &unSuseCreate->PID, sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(suseCreateSerializado + desplazamiento, &unSuseCreate->TID, sizeof(int));
	desplazamiento += sizeof(int);

	printf("\nCREATE A SERIALIZAR: \n");
	printf("\nPid: %d", unSuseCreate->PID);
	printf("\nTid: %d", unSuseCreate->TID);
	
	t_suseCreate* suseCreateDeserializado = deserializarSuseCreate(suseCreateSerializado);

	printf("\nPid: %d", suseCreateDeserializado->PID);
	printf("\nTid: %d", suseCreateDeserializado->TID);
	
	free(suseCreateDeserializado);

	return suseCreateSerializado;
}

t_suseCreate *deserializarSuseCreate(void *buffer)
{
	t_suseCreate *unSuseCreate = malloc(sizeof(t_suseCreate));

	int desplazamiento = 0;

	memcpy(&unSuseCreate->PID, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(&unSuseCreate->TID, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	return unSuseCreate;
}

/////////////////////////////////////////////////SUSEWAIT//////////////////////////////////////////////////

void *serializarSuseWait(t_suseWait *suseWait, int *tamanio)
{
	t_suseWait *unSuseWait = (t_suseWait *)suseWait;

	int desplazamiento = 0;
	int tamanioIdSemaforo = string_length(unSuseWait->idSemaforo);

	*tamanio = sizeof(uint) * 2 + sizeof(int) + tamanioIdSemaforo;

	void *suseWaitSerializado = malloc(*tamanio);

	memcpy(suseWaitSerializado + desplazamiento, &unSuseWait->PID, sizeof(uint));
	desplazamiento += sizeof(uint);

	memcpy(suseWaitSerializado + desplazamiento, &unSuseWait->TID, sizeof(uint));
	desplazamiento += sizeof(uint);

	memcpy(suseWaitSerializado + desplazamiento, &tamanioIdSemaforo, sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(suseWaitSerializado + desplazamiento, unSuseWait->idSemaforo, tamanioIdSemaforo);
	desplazamiento += tamanioIdSemaforo;

	return suseWaitSerializado;
}

t_suseWait *deserializarSuseWait(void *buffer)
{
	t_suseWait* unSuseWait = malloc(sizeof(t_suseWait));

	int desplazamiento = 0;
	int tamanioIdSemaforo = 0;

	memcpy(&unSuseWait->PID, buffer + desplazamiento, sizeof(uint));
	desplazamiento += sizeof(uint);

	memcpy(&unSuseWait->TID, buffer + desplazamiento, sizeof(uint));
	desplazamiento += sizeof(uint);

	memcpy(&tamanioIdSemaforo, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	char *bufferIdSemaforo = malloc(tamanioIdSemaforo + 1);
	memcpy(bufferIdSemaforo, buffer + desplazamiento, tamanioIdSemaforo);
	bufferIdSemaforo[tamanioIdSemaforo] = '\0';
	desplazamiento += tamanioIdSemaforo;

	unSuseWait->idSemaforo = string_new();

	string_append(&unSuseWait->idSemaforo, bufferIdSemaforo);

	free(bufferIdSemaforo);

	return unSuseWait;
}

////////////////////////////////////////////////SUSESIGNAL/////////////////////////////////////////////////

void *serializarSuseSignal(t_suseSignal *suseSignal, int *tamanio)
{
	t_suseSignal *unSuseSignal = (t_suseSignal *)suseSignal;

	int desplazamiento = 0;
	int tamanioIdSemaforo = string_length(unSuseSignal->idSemaforo);

	*tamanio = sizeof(uint) * 2 + sizeof(int) + tamanioIdSemaforo;

	void *suseSignalSerializado = malloc(*tamanio);

	memcpy(suseSignalSerializado + desplazamiento, &unSuseSignal->PID, sizeof(uint));
	desplazamiento += sizeof(uint);

	memcpy(suseSignalSerializado + desplazamiento, &unSuseSignal->TID, sizeof(uint));
	desplazamiento += sizeof(uint);

	memcpy(suseSignalSerializado + desplazamiento, &tamanioIdSemaforo, sizeof(int));
	desplazamiento += sizeof(int);

	memcpy(suseSignalSerializado + desplazamiento, unSuseSignal->idSemaforo, tamanioIdSemaforo);
	desplazamiento += tamanioIdSemaforo;

	return suseSignalSerializado;
}

t_suseSignal *deserializarSuseSignal(void *buffer)
{
	t_suseSignal* unSuseSignal = malloc(sizeof(t_suseSignal));

	int desplazamiento = 0;
	int tamanioIdSemaforo = 0;

	memcpy(&unSuseSignal->PID, buffer + desplazamiento, sizeof(uint));
	desplazamiento += sizeof(uint);

	memcpy(&unSuseSignal->TID, buffer + desplazamiento, sizeof(uint));
	desplazamiento += sizeof(uint);

	memcpy(&tamanioIdSemaforo, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);

	char *bufferIdSemaforo = malloc(tamanioIdSemaforo + 1);
	memcpy(bufferIdSemaforo, buffer + desplazamiento, tamanioIdSemaforo);
	bufferIdSemaforo[tamanioIdSemaforo] = '\0';
	desplazamiento += tamanioIdSemaforo;

	unSuseSignal->idSemaforo = string_new();

	string_append(&unSuseSignal->idSemaforo, bufferIdSemaforo);

	free(bufferIdSemaforo);

	return unSuseSignal;
}

///////////////////////////////////////////////SUSEJOIN////////////////////////////////////////////////////

void *serializarSuseJoin(t_suseJoin *suseJoin, int *tamanio)
{
	t_suseJoin *unSuseJoin = (t_suseJoin *)suseJoin;

	int desplazamiento = 0;

	*tamanio = sizeof(uint) * 2;

	void *suseJoinSerializado = malloc(*tamanio);

	memcpy(suseJoinSerializado + desplazamiento, &unSuseJoin->PID, sizeof(uint));
	desplazamiento += sizeof(uint);

	memcpy(suseJoinSerializado + desplazamiento, &unSuseJoin->TIDEsperado, sizeof(uint));
	desplazamiento += sizeof(uint);

	return suseJoinSerializado;
}

t_suseJoin *deserializarSuseJoin(void *buffer)
{
	t_suseJoin *unSuseJoin = malloc(sizeof(t_suseJoin));

	int desplazamiento = 0;

	memcpy(&unSuseJoin->PID, buffer + desplazamiento, sizeof(uint));
	desplazamiento += sizeof(uint);

	memcpy(&unSuseJoin->TIDEsperado, buffer + desplazamiento, sizeof(uint));
	desplazamiento += sizeof(uint);

	return unSuseJoin;
}

//////////////////////////////////////////////SUSECLOSE////////////////////////////////////////////////////

void *serializarSuseClose(t_suseClose *suseClose, int *tamanio)
{
	t_suseClose *unSuseClose = (t_suseClose *)suseClose;

	int desplazamiento = 0;

	*tamanio = sizeof(uint) * 2;

	void *suseCloseSerializado = malloc(*tamanio);

	memcpy(suseCloseSerializado + desplazamiento, &unSuseClose->PID, sizeof(uint));
	desplazamiento += sizeof(uint);

	memcpy(suseCloseSerializado + desplazamiento, &unSuseClose->TID, sizeof(uint));
	desplazamiento += sizeof(uint);

	return suseCloseSerializado;
}

t_suseClose *deserializarSuseClose(void *buffer)
{
	t_suseClose *unSuseClose = malloc(sizeof(t_suseClose));

	int desplazamiento = 0;

	memcpy(&unSuseClose->PID, buffer + desplazamiento, sizeof(uint));
	desplazamiento += sizeof(uint);	

	memcpy(&unSuseClose->TID, buffer + desplazamiento, sizeof(uint));
	desplazamiento += sizeof(uint);

	return unSuseClose;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// SERIALIZACION PAQUETES MUSE-LIBMUSE //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////MuseAlloc//////////////////////////////////////////////////

void* serializarMuseAlloc(void* administrativo, int* tamanio) {

	t_museAlloc* unAdministrativo = (t_museAlloc*) administrativo;

	*tamanio = 2* sizeof(uint32_t);

	printf("\n \n ******Tamanio alloc  %d",*tamanio);

	int desplazamiento = 0; 

	void* paqueteAdministrativo = malloc(*tamanio);

	memcpy(paqueteAdministrativo + desplazamiento, &unAdministrativo->pid, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(paqueteAdministrativo + desplazamiento, &unAdministrativo->tamanio, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return paqueteAdministrativo;

}

t_museAlloc* deserializarMuseAlloc(void* buffer) {

	int desplazamiento = 0;
	
	t_museAlloc* unAdministrativo = malloc(sizeof(t_museAlloc));

	memcpy(&unAdministrativo->pid, buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(&unAdministrativo->tamanio, buffer + desplazamiento, sizeof(uint32_t));

	return unAdministrativo;

}


///////////////////////////////////////////////////////MuseCpy//////////////////////////////////////////////////

void* serializarMuseCpy(void* administrativo, int* tamanio) {

	t_museCpy* unMcpy = (t_museCpy*) administrativo;

	*tamanio = sizeof(uint32_t) * 3 + unMcpy->cantidad ;

	printf("\n \n ******Tamanio museCpy %d",*tamanio);

	int desplazamiento = 0; 

	void* paqueteMcpy = malloc(*tamanio);

	memcpy(paqueteMcpy + desplazamiento, &unMcpy->pid, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(paqueteMcpy + desplazamiento, &unMcpy->posicion, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(paqueteMcpy + desplazamiento, &unMcpy->cantidad, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);


	memcpy(paqueteMcpy + desplazamiento, &unMcpy->aCopiar, unMcpy->cantidad );
	desplazamiento += unMcpy->cantidad;



	
	return paqueteMcpy;

}

t_museCpy* deserializarMuseCpy(void* buffer) {

	int desplazamiento = 0;
	
	t_museCpy* unMcpy = malloc(sizeof(t_museCpy));

	memcpy(&unMcpy->pid, buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(&unMcpy->posicion, buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(&unMcpy->cantidad, buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	
	unMcpy->aCopiar = malloc(sizeof(char) * unMcpy->cantidad);

	memcpy(unMcpy->aCopiar, buffer + desplazamiento, sizeof(char) * unMcpy->cantidad);


	return unMcpy;

}


///////////////////////////////////////////////////////MuseGet//////////////////////////////////////////////////

void* serializarMuseGet(void* administrativo, int* tamanio) {

	t_museGet* unMget = (t_museGet*) administrativo;

	*tamanio = sizeof(uint32_t) * 3 + unMget->cantidad ;

	printf("\n \n ******Tamanio unMget %d",*tamanio);

	int desplazamiento = 0; 

	void* paqueteMget = malloc(*tamanio);

	memcpy(paqueteMget + desplazamiento, &unMget->pid, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(paqueteMget + desplazamiento, &unMget->posicion, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(paqueteMget + desplazamiento, &unMget->cantidad, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);


	memcpy(paqueteMget + desplazamiento, &unMget->aCopiar, unMget->cantidad );
	desplazamiento += unMget->cantidad;
	
	return paqueteMget;

}

t_museCpy* deserializarMuseGet(void* buffer) {

	int desplazamiento = 0;
	
	t_museGet* unMcpy = malloc(sizeof(t_museGet));

	memcpy(&unMcpy->pid, buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(&unMcpy->posicion, buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	memcpy(&unMcpy->cantidad, buffer + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	
	unMcpy->aCopiar = malloc(sizeof(char) * unMcpy->cantidad);

	memcpy(unMcpy->aCopiar, buffer + desplazamiento, sizeof(char) * unMcpy->cantidad);


	return unMcpy;

}