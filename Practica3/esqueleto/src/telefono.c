
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <mqueue.h>
#include <definitions.h>

// Modulo principal
int main(int argc,char *argv[]){

    // Define variables locales
    int pid = getpid();
    char buffer[TAMANO_MENSAJES+1];
    char buzonLinea[TAMANO_MENSAJES];

    sprintf(buzonLinea,"%s",argv[1]);
    mqd_t qHandlerLlamadas=mq_open(BUZON_LLAMADAS,O_WRONLY);
    mqd_t qHandlerLinea=mq_open(buzonLinea,O_RDONLY);
    //TODO

    srand(pid);

    //TODO
    while(1){
        printf("Telefono[%d] en espera...\n", pid);
        mq_receive(qHandlerLlamadas,(char*)buffer,sizeof(buffer),NULL);
        sleep(rand() % 11 + 10);
        mq_send(qHandlerLinea, (char*)&buffer,sizeof(buffer),0);
        mq_send(qHandlerLinea, FIN_CONVERSACION, strlen(FIN_CONVERSACION) + 1, 0);
        printf("Telefono[%d] ha colgado la llamada%s\n", pid,buzonLinea);
        mq_close(qHandlerLinea);
    }
    return EXIT_SUCCESS;
}