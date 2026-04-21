
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


	//TODO: Esquema según especificado en la práctica.

    // Define variables locales
    int shm_fd=obtener_var(LLAMADASESPERA);

    // Coge semáforos y memoria compartida
    sem_t *sem_telefonos = sem_open(TELEFONOS, 0);
    sem_t *sem_mutex     = sem_open(MUTEXESPERA, 0);
    sem_t *sem_linea     = sem_open(LINEAS, 0);
    if (sem_telefonos== SEM_FAILED||sem_mutex== SEM_FAILED ){
        perror("sem_open");
        exit(1);
    }
    // Se pone en estado de libre incrementando el número de teléfonos libres
    sem_post(sem_telefonos);
    while(1){
        // Mensaje de Espera
        printf("Teléfono [%d] en espera...\n",pid);
        sem_wait(sem_linea);
        sem_wait(sem_mutex);
            consultar_var(shm_fd,&valorEspera);
            modificar_var(shm_fd,valorEspera-1);
        sem_post(sem_mutex);
		//TODO: Aquí hay que realizar procesos

        // Mensaje de en conversacion
        printf("Teléfono [%d] en conversacion... Nº Llamadas en espera: %d\n",pid,valorEspera);
        
        // Espera en conversación
        sleep(rand() % 10 + 10);
        sem_post(sem_telefonos);
        //sem_post(sem_linea);
    }

    return EXIT_SUCCESS;
}