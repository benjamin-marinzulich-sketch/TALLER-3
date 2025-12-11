#include "Grafo.h"
#include <iostream>

using namespace std;

Grafo::Grafo() {}

Grafo::~Grafo() {
    for (auto n : nodos)
        delete n;
}


// ================= Crear nodos =================

int Grafo::crear_directorio() {
    int id = nodos.size();
    nodos.push_back(new NodoDirectorio(id));
    return id;
}

int Grafo::crear_archivo(int tamano, int tipo) {
    int id = nodos.size();
    nodos.push_back(new NodoArchivo(id, tamano, tipo));
    return id;
}

// ================= Relaciones =================

void Grafo::agregar_contenido(int id_directorio, int id_hijo) {
    NodoGrafo* d = obtener_nodo(id_directorio);
    NodoGrafo* h = obtener_nodo(id_hijo);

    if (!d || !h) return;

    if (!d->es_directorio()) {
        cout << "Error: Solo los directorios pueden contener elementos.\n";
        return;
    }

    ((NodoDirectorio*)d)->agregar_hijo(id_hijo);
    h->agregar_padre(id_directorio);
}
// ================= BFS =================

void Grafo::bfs(int inicio) {
    queue<int> q;
    vector<bool> visitado(nodos.size(), false);

    q.push(inicio);
    visitado[inicio] = true;

    while (!q.empty()) {
        int actual = q.front();
        q.pop();

        cout << "Visita: " << actual << "\n";

        NodoGrafo* n = obtener_nodo(actual);
        for (int h : n->lista_hijos()) {
            if (!visitado[h]) {
                visitado[h] = true;
                q.push(h);
            }
        }
    }
}

// ================= DFS =================

void Grafo::dfs(int inicio) {
    stack<int> s;
    vector<bool> visitado(nodos.size(), false);

    s.push(inicio);

    while (!s.empty()) {
        int actual = s.top();
        s.pop();

        if (visitado[actual]) continue;
        visitado[actual] = true;

        cout << "Visita: " << actual << "\n";

        NodoGrafo* n = obtener_nodo(actual);
        auto hijos = n->lista_hijos();

        for (int i = hijos.size() - 1; i >= 0; i--)
            s.push(hijos[i]);
    }
}

// ================= Imprimir estructura =================

void Grafo::imprimir_estructura(int raiz) {
    queue<int> q;
    q.push(raiz);

    while (!q.empty()) {
        int actual = q.front();
        q.pop();

        NodoGrafo* n = obtener_nodo(actual);

        cout << "Nodo " << actual
            << (n->es_directorio() ? " [Directorio]" : " [Archivo]")
            << "\n";

        for (int h : n->lista_hijos()) {
            cout << "   Contiene -> " << h << "\n";
            q.push(h);
        }
    }
}

NodoGrafo* Grafo::obtener_nodo(int id) {
    if (id < 0 || id >= (int)nodos.size()) return nullptr;
    return nodos[id];
}