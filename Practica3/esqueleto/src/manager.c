#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <mqueue.h>
#include <definitions.h>
int g_telefonosProcesses = 0;
int g_lineasProcesses = 0;
struct TProcess_t *g_process_telefonos_table;
struct TProcess_t *g_process_lineas_table;
mqd_t qHandlerLlamadas;
mqd_t qHandlerLineas[NUMLINEAS];
 
void crear_buzones(){
    char Identificador[32];
    struct mq_attr mqAttr;
    mqAttr.mq_flags   = 0;
    mqAttr.mq_maxmsg  = 10;
    mqAttr.mq_msgsize = TAMANO_MENSAJES + 1;  // +1 por el \0
    mqAttr.mq_curmsgs = 0;
    for(int i=0; i<NUMLINEAS;i++){
       
        snprintf(Identificador, sizeof(Identificador), "%s%d", BUZON_LINEAS, i);
        qHandlerLineas[i] = mq_open(Identificador, O_CREAT|O_RDWR,0644,&mqAttr);
        if(qHandlerLineas[i] == -1){
            perror("mq_open linea");
            exit(EXIT_FAILURE);
        }
    }
    qHandlerLlamadas = mq_open(BUZON_LLAMADAS,O_CREAT|O_RDWR,0644,&mqAttr);
}
void liberar_recursos(){
    char Identificador[32];
     for (int i = 0; i < NUMLINEAS; i++)
    {
        sprintf(Identificador, "%s%d", BUZON_LINEAS, i);
        mq_close(qHandlerLineas[i]);
        mq_unlink(BUZON_LINEAS);
    }
    mq_close(qHandlerLlamadas);
    mq_unlink(BUZON_LLAMADAS);
    free(g_process_telefonos_table);
    free(g_process_lineas_table);
    printf("[MANAGER] Terminacion del programa (todos los procesos terminados)");
}
void terminar_procesos(void){
    printf("-------------------[MANAGER] Terminar con cualquier proceso pendiente ejecutándose-------------------");
   for(int i = 0; i < g_telefonosProcesses; i++){
        kill(g_process_telefonos_table[i].pid, SIGTERM);
    }
    // Esperar a que mueran
    for(int i = 0; i < g_telefonosProcesses; i++){  
        waitpid(g_process_telefonos_table[i].pid, NULL, 0);
    }
}
void manejador_senhal(int sign){
    for(int i = 0; i <  g_lineasProcesses; i++){
        if(strcmp(g_process_lineas_table[i].clase, CLASE_LINEA) == 0){
            waitpid(g_process_lineas_table[i].pid, NULL, 0);
        }
        
    }
    terminar_procesos();
    liberar_recursos();
    exit(0);
}

void instalar_manejador_senhal(){
    signal(SIGINT,manejador_senhal);
}


void iniciar_tabla_procesos(int n_procesos_telefono, int n_procesos_linea){
     printf("[Manager] Número de teléfonos creados %d\n", n_procesos_telefono );
    g_process_telefonos_table = malloc(n_procesos_telefono * sizeof(struct TProcess_t));
    printf("[Manager] Número de teléfonos creados %d\n", n_procesos_linea );
    g_process_lineas_table    = malloc(n_procesos_linea * sizeof(struct TProcess_t));
}
void lanzar_proceso_telefono(const int indice_tabla){
        char Cola[32];
         snprintf(Cola, sizeof(Cola), "%s%d", BUZON_LINEAS, indice_tabla);
       pid_t pid_hijo=fork();
        if(pid_hijo<0){
            perror("fork");
            exit(1);
        }if (pid_hijo==0){
            execl(RUTA_TELEFONO,CLASE_TELEFONO,Cola,NULL);
            perror("execl");
            exit(1);
        }
        g_process_telefonos_table[indice_tabla].pid = pid_hijo;
        g_process_telefonos_table[indice_tabla].clase = CLASE_TELEFONO;
}
void lanzar_proceso_linea(const int indice_tabla){
    pid_t pid_hijo=fork();
    char Cola[32];
    snprintf(Cola, sizeof(Cola), "%s%d", BUZON_LINEAS, indice_tabla);
        if(pid_hijo<0){
        perror("fork");
        exit(1);
    }if (pid_hijo==0){
        execl(RUTA_LINEA,CLASE_LINEA,Cola,NULL);
        perror("execl");
        exit(1);
    }
    g_process_lineas_table[indice_tabla].pid = pid_hijo;
    g_process_lineas_table[indice_tabla].clase = CLASE_LINEA;
    g_lineasProcesses++;
}
void crear_procesos(int numTelefonos, int numLineas){
       int i=0;
    for(i=0;i<numTelefonos;i++){
        lanzar_proceso_telefono(i);
        
    }
    sleep(1);
    for(i=0;i<numLineas;i++){
        lanzar_proceso_linea(i);
    }
}


void esperar_procesos(){
        for(int i = 0; i <  g_lineasProcesses; i++){
    if(strcmp(g_process_lineas_table[i].clase, CLASE_LINEA) == 0){
        waitpid(g_process_lineas_table[i].pid, NULL, 0);
        printf("[Manager] proceso LÍNEA TERMINADO %d\n", g_process_lineas_table[i].pid);
    }
}
}


void terminar_procesos_especificos(struct TProcess_t *process_table, int process_num){
        kill(process_table[process_num].pid, SIGTERM);

}





int main(int argc, char *argv[])
{
    // Define variables locales

    // Creamos los buzones
    crear_buzones();

    // Manejador de Ctrl-C
    instalar_manejador_senhal();
 
    // Crea Tabla para almacenar los pids de los procesos
    iniciar_tabla_procesos(NUMTELEFONOS,NUMLINEAS);

    // Tenemos todo
    // Lanzamos los procesos
    crear_procesos(NUMTELEFONOS,NUMLINEAS);
  
    // Esperamos a que finalicen las lineas
    esperar_procesos();

    // Matamos los telefonos y cualquier otro proceso restante
    terminar_procesos();


    // Finalizamos Manager
    printf("\n[MANAGER] Terminacion del programa (todos los procesos terminados).\n");
    liberar_recursos();

    return EXIT_SUCCESS;
}

//TODO
