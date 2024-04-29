#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
int pid;
// Función que maneja todas las señales
void manejadorSenal(int signal) {
    switch (signal) {
        case SIGINT:
            printf("Señal SIGINT recibida (%d)\n", signal);
            break;
        case SIGALRM:
            printf("Señal SIGALRM recibida (%d)\n", signal);
            break;
        case SIGUSR1:
            printf("Señal SIGUSR1 recibida (%d)\n", signal);
            break;
        case SIGUSR2:
            printf("Señal SIGUSR2 recibida (%d)\n", signal);
            break;
        default:
            printf("Señal recibida: %d\n", signal);
            break;
    }
}
// Nos sirve para verificar el programa1
void modificarArchivo() {
    FILE *fileRead, *fileWrite;
    int num1, num2, num3;

    pid = getpid();
    fileRead = fopen("procesos.txt", "r");
    if (fileRead == NULL) {
        perror("Error al abrir el archivo para lectura");
        return;
    }

    fileWrite = fopen("temp.txt", "w");
    if (fileWrite == NULL) {
        perror("Error al abrir el archivo para escritura");
        fclose(fileRead);
        return;
    }

    while (fscanf(fileRead, "%d %d %d", &num1, &num2, &num3) != EOF) {
        fprintf(fileWrite, "%d %d %d\n", pid, num2, num3);
    }

    fclose(fileRead);
    fclose(fileWrite);

    remove("procesos.txt");
    rename("temp.txt", "procesos.txt");

}


int main() {
    modificarArchivo();
    // Registrar el manejador para todas las señales posibles
    struct sigaction sa;
    sa.sa_handler = manejadorSenal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    // Manejadores para señales especificas
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    // Manejador generico para las demas señales
    for (int i = 1; i < NSIG; i++) {
        if (i != SIGINT && i != SIGALRM && i != SIGUSR1 && i != SIGUSR2) {
            sigaction(i, &sa, NULL);
        }
    }

    // Mensaje inicial
    printf("Esperando señales... con PID: %d\n",pid);

    // Bucle infinito para mantener el programa en ejecución
    while (1) {
        pause();  
    }

    return 0;
}