#include "proceso.h"
//Estructura de un nodo del arbol con informacion de proceso
Nodo *nuevoNodo(int *dato, int inicio, int fin, int pid)
{
    Nodo *nodo = (Nodo *)malloc(sizeof(Nodo));
    nodo->pid = pid;
    memcpy(nodo->dato, dato, sizeof(int) * 100);
    nodo->izq = NULL;
    nodo->der = NULL;
    nodo->inicio = inicio;
    nodo->fin = fin;
    return nodo;
}

// Crea una nueva cola para manejar los nodos
Cola *crearCola()
{
    Cola *cola = (Cola *)malloc(sizeof(Cola));
    cola->frente = NULL;
    cola->fondo = NULL;
    return cola;
}

//Añadir un nodo a la cola
void encolar(Cola *cola, Nodo *nodo)
{
    EncolarNodo *nuevoEncolarNodo = (EncolarNodo *)malloc(sizeof(EncolarNodo));
    nuevoEncolarNodo->nodo = nodo;
    nuevoEncolarNodo->siguiente = NULL;

    if (cola->fondo == NULL)
    {
        cola->frente = nuevoEncolarNodo;
        cola->fondo = nuevoEncolarNodo;
    }
    else
    {
        cola->fondo->siguiente = nuevoEncolarNodo;
        cola->fondo = nuevoEncolarNodo;
    }
}

//Retirar un nodo de la cola
Nodo *decolar(Cola *cola)
{
    if (cola->frente == NULL)
    {
        return NULL;
    }
    EncolarNodo *temp = cola->frente;
    Nodo *nodo = temp->nodo;

    cola->frente = cola->frente->siguiente;

    if (cola->frente == NULL)
    {
        cola->fondo = NULL;
    }

    free(temp);
    return nodo;
}
//Verificar si cola esta vacia
int colaVacia(Cola *cola)
{
    return cola->frente == NULL;
}

// Función para construir un árbol a partir de un array, con un número definido de procesos.
Nodo *crearArbol(int *arr, int tam, int numProcesos)
{
    if (tam == 0)// Si el tam es 0 no hay nada que procesar
    {
        return NULL;
    }
    Cola *cola = crearCola();

    int pidContador = 0;

    Nodo *raiz = nuevoNodo(arr, 0, tam - 1, pidContador++);
    encolar(cola, raiz);

    while (pidContador < numProcesos)
    {
        Nodo *actual = decolar(cola);
        int medio = (actual->inicio + actual->fin) / 2;

        if (actual->inicio <= medio)
        {
            Nodo *izq = nuevoNodo(arr, actual->inicio, medio, pidContador++);
            actual->izq = izq;
            encolar(cola, izq);
        }

        if (medio + 1 <= actual->fin)
        {
            Nodo *der = nuevoNodo(arr, medio + 1, actual->fin, pidContador++);
            actual->der = der;
            encolar(cola, der);
        }
    }
    free(cola);
    return raiz;
}
