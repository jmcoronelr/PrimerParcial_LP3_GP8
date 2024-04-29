#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <string.h>
#include <math.h>
#include <stdatomic.h>
typedef struct Nodo
{
    int dato[100];
    int pid;
    int inicio, fin;
    struct Nodo *izq;
    struct Nodo *der;
} Nodo;

typedef struct EncolarNodo
{
    Nodo *nodo;
    struct EncolarNodo *siguiente;
} EncolarNodo;

typedef struct Cola
{
    EncolarNodo *frente;
    EncolarNodo *fondo;
} Cola;

// Imprime un array separado por espacios
void imprimirArray(int *arr, int l, int r);

// Imprime un array separado por espacios envuelto en llaves {}
void imprimirArray2(int *arr, int l, int r);

// Realiza la mezcla para el ordenamiento del array
void merge(int arr[], int l, int m, int r);

// Realiza el ordenamiento mergeSort tradicional
void mergeSort(int arr[], int l, int r);

// Realiza el ordenamiento mergeSort utilizando procesos
void mergeSortProceso(Nodo *nodo, int numProcesos);

// Obtiene la altura de un arbol binario dado el numero de nodos
int calcularAltura(int numProcesos);

// Imprime el esquema del arbol del ordenamiento mergeSort
void esquemaArbol(int numProcesos, int l, int r, int *array);

// Funcion auxiliar para imprimir espacios
void imprimirEspacios(int nivel, int numProceso);

// Funcion auxiliar para impimir saltos de linea
void salto();

// Funcion encargada de realizar el mapeo de las sublistas de entrada de cada subproceso
void imprimirMapeos(int *arr, int tam, int numProcesos);

// void imprimirProcesamiento(int *arr, int tam, int numProcesos);

Nodo *nuevoNodo(int *dato, int inicio, int fin, int pid);
Cola *crearCola();
void encolar(Cola *cola, Nodo *nodo);
Nodo *decolar(Cola *cola);
int colaVacia(Cola *cola);
Nodo *crearArbol(int *arr, int tam, int numProcesos);
void inorderTraversal(Nodo *root);

void mergeArrays(Nodo *izq, Nodo *der, Nodo *result);
void imprimirProcesamientos(Nodo *root);