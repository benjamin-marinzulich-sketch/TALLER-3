#include <iostream>
#include <string>
#include "NodoGrafo.h" 
#include "NodoBPlus.h" 
#include "ArbolBPlus.h"

using namespace std;

// ======================================================
//   DECLARACIÓN DE FUNCIONES 
// ======================================================

void insertar_nodo_grafo(int clave, NodoGrafo* nodo_grafo, ArbolBPlus* arbol) {
    arbol->insertar(clave, nodo_grafo);
}

// ======================================================
//   Mostrar recorrido por hojas (para debug)
// ======================================================


// ======================================================
//                       MAIN
// ======================================================
int main() {
    cout << "=hola";
    // Árbol B+ de orden 4 (3 claves por nodo)
    ArbolBPlus arbol(4);

    // Muchas claves para forzar splits fuertes
    int claves[] = {
        50, 20, 70, 10, 30, 60, 80,
        25, 27, 29, 26
    };

    int N = sizeof(claves) / sizeof(int);

    cout << "=== INSERTANDO " << N << " CLAVES ===\n";

    for (int i = 0; i < N; i++) {

        int clave = claves[i];
        cout << "\n>>> Insertando clave " << clave << "\n";

        // Crear nodo grafo simple
        NodoGrafo* nodo = new NodoArchivo(clave, 100, 1);

        // Insertar usando tu función global
        insertar_nodo_grafo(clave, nodo, &arbol);

    }

    cout << "\n=== INSERCIÓN COMPLETADA ===\n";

    return 0;
}
















































