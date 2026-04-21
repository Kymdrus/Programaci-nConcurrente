#include <errno.h>
#include <linux/limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <definitions.h>
#include <memoriaI.h>
#include <semaforoI.h>

int g_telefonosProcesses = 0;
int g_lineasProcesses = 0;
struct TProcess_t *g_process_telefonos_table;
struct TProcess_t *g_process_lineas_table;

void procesar_argumentos(int argc, char *argv[], int *numTelefonos, int *numLineas){

    if (argc<3){
        fprintf(stderr,"Faltan argumentos, Le recuerdo que debe ser: <NombrePrograma> <nºtelefonos> <nºlineas>\n");
        exit(1);
    }
    *numTelefonos=atoi(argv[1]);
    *numLineas=atoi(argv[2]);
    if(*numTelefonos < 0 || *numLineas < 0){
        fprintf(stderr, "Error: los argumentos deben ser números positivos\n");
        exit(1);
    }
}
void liberar_recursos(){
 
    sem_unlink(TELEFONOS);
    sem_unlink(MUTEXESPERA);
    sem_unlink(LINEAS);
    destruir_var(LLAMADASESPERA);
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
void lanzar_proceso_telefono(const int indice_tabla){
     
    pid_t pid_hijo=fork();
        if(pid_hijo<0){
            perror("fork");
            exit(1);
        }if (pid_hijo==0){
            execl(RUTA_TELEFONO,CLASE_TELEFONO,NULL);
            perror("execl");
            exit(1);
        }
        g_process_telefonos_table[indice_tabla].pid = pid_hijo;
        g_process_telefonos_table[indice_tabla].clase = CLASE_TELEFONO;
        g_telefonosProcesses++;
}
void lanzar_proceso_linea(const int indice_tabla){
     pid_t pid_hijo=fork();
        if(pid_hijo<0){
            perror("fork");
            exit(1);
        }if (pid_hijo==0){
            execl(RUTA_LINEA,CLASE_LINEA,NULL);
            perror("execl");
            exit(1);
        }
        g_process_lineas_table[indice_tabla].pid = pid_hijo;
        g_process_lineas_table[indice_tabla].clase = CLASE_LINEA;
        g_lineasProcesses++;
    }

void iniciar_tabla_procesos(int n_procesos_telefono, int n_procesos_linea){
    printf("[Manager] Número de teléfonos creados %d\n", n_procesos_telefono );
   g_process_telefonos_table = malloc(n_procesos_telefono * sizeof(struct TProcess_t));
    printf("[Manager] Número de teléfonos creados %d\n", n_procesos_linea );
    g_process_lineas_table    = malloc(n_procesos_linea * sizeof(struct TProcess_t));
   
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
    int numTelefonos;
    int numLineas;

    // Procesa los argumentos y los guarda en las dos variables
    procesar_argumentos(argc,argv,&numTelefonos,&numLineas);

    // Creamos semáforos y memoria compartida
    crear_sem(MUTEXESPERA,1); crear_sem(TELEFONOS,0);crear_sem(LINEAS,0);
    crear_var(LLAMADASESPERA,0);  // No hay llamadas en espera

    // Manejador de Ctrl-C
    instalar_manejador_senhal();
 
    // Crea Tabla para almacenar los pids de los procesos
    iniciar_tabla_procesos(numTelefonos, numLineas);

    // Tenemos todo
    // Lanzamos los procesos
    crear_procesos(numTelefonos,numLineas);
  
    // Esperamos a que finalicen las lineas
    esperar_procesos();

    // Matamos los telefonos y cualquier otro proceso restante
    terminar_procesos();

    // Finalizamos Manager
    printf("\n[MANAGER] Terminacion del programa (todos los procesos terminados).\n");
    liberar_recursos();

    return EXIT_SUCCESS;
}

//TODO: Realizar todas las funciones necesarias.