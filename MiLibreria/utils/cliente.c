#include "cliente.h"
#include "utils.h"
void conectarseAServidor(char *IP,int PUERTO) {
	struct sockaddr_in dirServer;
	dirServer.sin_family= AF_INET;
	dirServer.sin_addr.s_addr=inet_addr("127.0.0.1");
	dirServer.sin_port=htons(PUERTO);

	int cliente=socket(AF_INET,SOCK_STREAM,0);
	if(connect(cliente, (void*)&dirServer,sizeof(dirServer)) != 0){
		perror("No se pudo conectar");
		exit(1);
	}
	printf("Se pudo conectar al servidor");
	while(1){
	char mensaje[1000];
	printf("\nIngrese el mensaje que quiere mandar: ");
	scanf("%s",mensaje);
	/*int longMensaje=strlen(mensaje);
	send(cliente,longMensaje,4,0);*/
	send(cliente,mensaje,strlen(mensaje),0);
	}
	close(cliente);
	return;
}
