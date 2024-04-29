#include "proceso.h"

int main(int argc, char *argv[])
{
    // Verifica si se pasaron suficientes argumentos al programa
    if (argc < 3)
    {
        printf("Uso: %s <numero de procesos> <enteros separados por coma>\n", argv[0]);
        return 1; // Retorna error si no hay suficientes argumentos
    }

    // Convierte el primer argumento de entrada en un número entero que indica el número de procesos
    int num_procesos = atoi(argv[1]);
    // Segundo argumento que contiene los enteros separados por comas
    char *numeros = argv[2];
    // Convertir la cadena de entrada en un arreglo de enteros
    int arr[100];
    int a = 0;
    char *token = strtok(numeros, ",");// Divide la cadena en tokens según las comas
    
    
    while (token != NULL)
    {
        arr[a++] = atoi(token);
        token = strtok(NULL, ",");
    }

    // Crea el árbol de procesos basado en el arreglo y el número de procesos
    Nodo *raiz = crearArbol(arr, a , num_procesos);
    
    // Imprime un esquema visual del árbol de procesos y sus divisiones
    printf("----------------------------------------\n");
    esquemaArbol(num_procesos, 0, a - 1, raiz->dato);

    // Imprime los mapeos de rangos de datos a los procesos
    imprimirMapeos(raiz->dato, a - 1, num_procesos);
    
    // Decide si ejecutar un ordenamiento de fusión simple o uno distribuido entre procesos
    if (num_procesos == 1)
    {
        // Si sólo hay un proceso, usa el merge sort clásico
        mergeSort(raiz->dato, 0, a - 1);
    }
    else
    {
        // Si hay múltiples procesos, usa el merge sort adaptado para procesos múltiples
        mergeSortProceso(raiz, num_procesos);
    }
    printf("----------------------------------------\n");

    return 0;
}