#include "NodoGrafo.h"


// ===== NodoGrafo =====

NodoGrafo::NodoGrafo(int _id) : id(_id) {}

int NodoGrafo::get_id() const {
    return id;
}

void NodoGrafo::agregar_padre(int padre_id) {
    padres.push_back(padre_id);
}

void NodoGrafo::eliminar_padre(int padre_id) {
    for (auto it = padres.begin(); it != padres.end(); ) {
        if (*it == padre_id) it = padres.erase(it);
        else ++it;
    }
}

std::vector<int> NodoGrafo::lista_padres() {
    return padres;
}

std::vector<int> NodoGrafo::lista_hijos() {
    return std::vector<int>();  // vacío por defecto
}

// ===== NodoDirectorio =====

NodoDirectorio::NodoDirectorio(int _id) : NodoGrafo(_id) {}

void NodoDirectorio::agregar_hijo(int hijo_id) {
    hijos.push_back(hijo_id);
}

std::vector<int> NodoDirectorio::lista_hijos() {
    return hijos;
}

void NodoDirectorio::eliminar_hijo(int hijo_id) {
    for (auto it = hijos.begin(); it != hijos.end(); ) {
        if (*it == hijo_id) it = hijos.erase(it);
        else ++it;
    }
}

// ===== NodoArchivo =====

NodoArchivo::NodoArchivo(int _id, int _tamano, int _tipo)
    : NodoGrafo(_id), tamano(_tamano), tipo(_tipo) {}
