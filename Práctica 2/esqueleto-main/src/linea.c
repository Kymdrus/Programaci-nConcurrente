
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <definitions.h>
#include <memoriaI.h>
#include <semaforoI.h>

// Modulo principal
int main(int argc,char *argv[]){
        
    // Define variables locales
    int pid = getpid();
    int valorEspera;
    int shm_fd=obtener_var(LLAMADASESPERA);
	//TODO: Esquema especificado en la práctica.
	
	
    // Coge semáforos y memoria compartida
    sem_t *sem_telefonos = sem_open(TELEFONOS, 0);
    sem_t *sem_mutex     = sem_open(MUTEXESPERA, 0);
    sem_t *sem_lineas = sem_open(LINEAS, 0);
    if (sem_telefonos== SEM_FAILED||sem_mutex== SEM_FAILED || sem_lineas==SEM_FAILED ){
        perror("sem_open");
        exit(1);
    }

    // Realiza una espera entre 1..60 segundos
    printf("Linea [%d] esperando llamada...\n",pid);
    sleep(rand() % 30 + 1);

    //Aumenta las llamadas en espera
    sem_wait(sem_mutex);
        consultar_var(shm_fd,&valorEspera);
        modificar_var(shm_fd,valorEspera+1);
    sem_post(sem_mutex);

    // Espera telefono libre
    printf("Linea [%d] esperando telefono libre...Nº Llamadas en espera: %d\n",pid,valorEspera);
    sem_wait(sem_telefonos);
    // Lanza la llamada
    printf("Linea [%d] desviando llamada a un telefono...\n",pid);
    sem_post(sem_lineas);
    return EXIT_SUCCESS;
}