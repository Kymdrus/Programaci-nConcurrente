
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
    
    char buzonLinea[TAMANO_MENSAJES];
    char buffer[TAMANO_MENSAJES+1];
    sprintf(buzonLinea,"%s",argv[1]);
    mqd_t qHandlerLlamadas=mq_open(BUZON_LLAMADAS,O_WRONLY);
    mqd_t qHandlerLinea=mq_open(buzonLinea,O_RDONLY);
    // Inicia Random
    srand(pid);
   
    // Verifica los parámetros
    if (argc != 2)
    {
        fprintf(stderr, "Error. Usa: ./exec/linea <cola_linea_llamante>.\n");
        exit(EXIT_FAILURE);
    }
            sprintf(buzonLinea,"%s",argv[1]);

    while(1){
        //falta primer paso
        //
        printf("Linea[%d] esperando llamada\n",pid);
        sleep(rand() % 30 + 1);
        mq_send(qHandlerLlamadas,(const char *)&buffer,sizeof(buffer),0);
        mq_receive(qHandlerLinea, (char*)&buffer,sizeof(buffer),NULL);
        printf("Linea[%d] llamada finalizada: %s\n", pid, buffer);
    }
    
    //TODO

    return EXIT_SUCCESS;
}

