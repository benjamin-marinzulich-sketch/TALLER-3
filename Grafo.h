#pragma once
#include <vector>
#include <queue>
#include <stack>
#include "NodoGrafo.h"

class Grafo {
private:
    std::vector<NodoGrafo*> nodos;

public:
    Grafo();
    ~Grafo();

    int crear_directorio();
    int crear_archivo(int tamano, int tipo);

    void agregar_contenido(int id_directorio, int id_hijo);

    NodoGrafo* obtener_nodo(int id);

    void bfs(int inicio);
    void dfs(int inicio);

    void imprimir_estructura(int raiz);
};