
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
    mqd_t qHandlerLlamadas;
    mqd_t qHandlerLinea;
    char buzonLinea[TAMANO_MENSAJES];
    char buffer[TAMANO_MENSAJES+1];

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
        fprintf("Linea[%d] esperando llamada\n",pid);
        sleep(rand() % 30 + 1);
        mq_send(qHandlerLlamadas,(const char *)&buffer,sizeof(buffer),NULL);
        mq_receive(qHandlerLinea, (char*)&buffer,sizeof(buffer),NULL);
        printf("Linea[%d] llamada finalizada: %s\n", pid, buffer);
    }
    
    //TODO

    return EXIT_SUCCESS;
}

