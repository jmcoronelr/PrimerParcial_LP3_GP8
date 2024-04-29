#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
//Almacenamos los datos del archivo proceso
typedef struct {
    int processId;
    int signal;
    int delay;
} ProcessInfo;

int main() {
    FILE *file;
    ProcessInfo proc; //pdi,delay,signal
    pid_t pid; //identificador del pid
    int status; //salida del proceso hijo

    //Abrir el archivo
    file = fopen("procesos.txt", "r");
    if (file == NULL) {
        perror("Error al abrir");
        return 1;
    }

    // Leer cada linea y hacer fork
    while (fscanf(file, "%d %d %d", &proc.processId, &proc.signal, &proc.delay) != EOF) {
        pid = fork();

        if (pid == -1) {
            // Error al hacer fork
            perror("fallo el fork");
            exit(1);
        } else if (pid == 0) {
            // Proceso hijo
            sleep(proc.delay);
            printf("Proceso hijo: Mandando señal %d al proceso %d despues de %d segundos\n",
                   proc.signal, proc.processId, proc.delay);
            //Mandar la señal al proceso
            kill(proc.processId, proc.signal);

            exit(0); // Terminar
        }
        // El padre sigue forkeando para cada linea
    }

    // El padre espera por todos los procesos hijos
    while ((pid = wait(&status)) > 0);

    fclose(file);
    return 0;
}
