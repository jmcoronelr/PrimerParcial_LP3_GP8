#include "proceso.h"
// Función merge para combinar dos sub-arreglos
void mergeArrays(Nodo *izq, Nodo *der, Nodo *result)
{
    int *array = malloc(sizeof(int) * 100);
    int j = 0;
    // Copia los elementos del sub-arreglo izquierdo al arreglo temporal
    for (int i = izq->inicio; i <= izq->fin; i++)
    {
        array[j] = izq->dato[i];
        j++;
    }
    // Copia los elementos del sub-arreglo derecho al arreglo temporal
    for (int i = der->inicio; i <= der->fin; i++)
    {
        array[j] = der->dato[i];
        j++;
    }
    int m = (j - 1) / 2;
    salto();
    merge(array, 0, m, j - 1);
    int b = 0;
    // Copia los elementos ordenados del arreglo temporal al nodo resultado
    for (int a = result->inicio; a <= result->fin; a++)
    {
        result->dato[a] = array[b];
        b++;
    }
    // Imprime información sobre el proceso de combinación
    printf("============================\n");
    printf("Proceso %d lista ordenada:", izq->pid);
    imprimirArray2(izq->dato, izq->inicio, izq->fin);
    salto();

    printf("Proceso %d lista ordenada:", der->pid);
    imprimirArray2(der->dato, der->inicio, der->fin);
    salto();

    printf("Proceso %d: ", result->pid);
    printf("lista izquierda: ");
    imprimirArray2(izq->dato, izq->inicio, izq->fin);
    printf(" lista derecha: ");
    imprimirArray2(der->dato, der->inicio, der->fin);
    printf(" => ");
    imprimirArray2(result->dato, result->inicio, result->fin);
    salto();
    free(array);
}
//Salto de linea
void salto()
{
    printf("\n");
}
//Imprimir arreglo
void imprimirArray(int *arr, int l, int r)
{
    for (int i = l; i < r; i++)
    {
        printf("%d,", arr[i]);
    }
    printf("%d", arr[r]);
}
//Imprimir arreglo entre llaves
void imprimirArray2(int *arr, int l, int r)
{
    printf(" ");
    for (int i = l; i < r; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("%d}", arr[r]);
}
//Funcion mergesort
void merge(int arr[], int l, int m, int r)
{

    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}
// Función para calcular la altura del árbol de procesos
int calcularAltura(int n_procesos)
{
    return (int)(log2(n_procesos + 1));
}
// Función para imprimir espacios en blanco según el nivel del árbol
void imprimirEspacios(int nivel, int n_proceso)
{
    for (int k = nivel - n_proceso; k > 0; k--)
    {
        printf("\t");
    }
}
// Función para imprimir un esquema visual del árbol de procesos
void esquemaArbol(int n_procesos, int l, int r, int *array)
{
    int profundidad = calcularAltura(n_procesos);
    int num_proceso = 0;
    printf("====esquema de arbol====\n");
    for (int j = 0; j < profundidad; j++)
    {
        int cant_nodos = pow(2, j);
        for (int i = 0; i < cant_nodos; i++)
        {
            imprimirEspacios(profundidad, j);
            printf("Proceso: %d", num_proceso);
            num_proceso++;
        }
        printf("\n");
        int cant_elementos = r / cant_nodos;
        int inicio = 0;
        for (int i = 0; i < cant_nodos; i++)
        {
            imprimirEspacios(profundidad, j);
            int fin = inicio + cant_elementos;
            if (fin > r)
                fin = r;
            imprimirArray(array, inicio, fin);
            if (cant_nodos >= 4)
            {
                for (int i = 0; i < cant_nodos / 4; i++)
                {
                    imprimirEspacios(profundidad, j);
                }
            }

            inicio = fin + 1;
        }
        salto();
    }
    salto();
}
// Función merge sort adaptado para nodos de árbol de procesos
void mergeSortProceso(Nodo *nodo, int num_processes)
{
    if (nodo == NULL)
    {
        return;
    }

    if (nodo->inicio < nodo->fin - 1)
    {
        int m = nodo->inicio + (nodo->fin - nodo->inicio) / 2;
        if (num_processes <= 1)
        {

            mergeSort(nodo->dato, nodo->inicio, nodo->fin);
        }
        else
        {
            int half_processes = num_processes / 2;
            int fdI[2], fdD[2]; // Pipes para comunicación con hijos
            if (pipe(fdI) == -1 || pipe(fdD) == -1)
            {
                perror("Error al crear los pipes");
                exit(EXIT_FAILURE);
            }

            Nodo *izq = (Nodo *)malloc(sizeof(Nodo));
            Nodo *der = (Nodo *)malloc(sizeof(Nodo));

            // Asignar datos iniciales a izq y der
            memcpy(izq->dato, nodo->izq->dato, sizeof(int) * 100);
            izq->inicio = nodo->inicio;
            izq->fin = m;
            izq->izq = nodo->izq->izq;
            izq->pid = nodo->izq->pid;
            izq->der = nodo->izq->der;

            memcpy(der->dato, nodo->der->dato, sizeof(int) * 100);
            der->inicio = m + 1;
            der->fin = nodo->fin;
            der->der = nodo->der->der;
            der->izq = nodo->der->izq;
            der->pid = nodo->der->pid;
            // Crear proceso hijo izquierdo
            pid_t hijo_izq = fork();
            if (hijo_izq == 0)
            {                  // Proceso hijo izquierdo
                close(fdI[0]); // Solo escritura
                mergeSortProceso(izq, half_processes);
                write(fdI[1], izq, sizeof(Nodo));
                close(fdI[1]); // Cerrar después de escribir
                exit(EXIT_SUCCESS);
            }

            // Crear proceso hijo derecho
            pid_t hijo_der = fork();
            if (hijo_der == 0)
            { // Proceso hijo derecho

                close(fdD[0]); // Solo escritura
                mergeSortProceso(der, half_processes);
                write(fdD[1], der, sizeof(Nodo));
                close(fdD[1]); // Cerrar después de escribir
                exit(EXIT_SUCCESS);
            }
            // Leer datos del hijo izquierdo
            read(fdI[0], izq, sizeof(Nodo));

            // Leer datos del hijo derecho
            read(fdD[0], der, sizeof(Nodo));

            // Proceso padre
            close(fdI[1]); // Solo lectura
            close(fdD[1]); // Solo lectura

            wait(NULL); // Esperar a que terminen los hijos
            wait(NULL);
            mergeArrays(izq, der, nodo); // Mezclar resultados

            free(izq);
            free(der);
        }
    }
}
// Función para imprimir los mapeos de datos a procesos
void imprimirMapeos(int *arr, int limite, int numProcesos)
{
    int profundidad = calcularAltura(numProcesos);
    int num_proceso = 0;

    printf("===mapeos===\n");
    for (int j = 0; j < profundidad; j++)
    {
        int cant_procesos = pow(2, j);
        int cant_elementos = limite / cant_procesos;
        int inicio = 0;
        for (int i = 0; i < cant_procesos; i++)
        {
            printf("Proceso %d: ", num_proceso);
            int fin = inicio + cant_elementos;
            if (fin > limite)
                fin = limite;
            imprimirArray(arr, inicio, fin);
            inicio = fin + 1;
            num_proceso++;
            salto();
        }
    }
}
// Función para imprimir los procesamientos de datos en cada proceso
void imprimirProcesamientos(Nodo *root)
{
    if (root == NULL)
    {
        return;
    }

    imprimirProcesamientos(root->izq);
    imprimirProcesamientos(root->der);
    printf("Proceso %d: ", root->pid);
    imprimirArray(root->dato, root->inicio, root->fin);
    printf("\n");
}
