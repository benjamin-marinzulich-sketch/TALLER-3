#pragma once
#include "NodoBPlus.h"
#include "NodoGrafo.h"

class ArbolBPlus {

private:
    NodoBPlusBase* raiz;
    int orden;

public:
    ArbolBPlus(int _orden);

    void insertar(int clave, NodoGrafo* dato);
    NodoGrafo* buscar(int clave);

private:
    NodoBHoja* buscarHoja(int clave);
    void insertarEnHoja(NodoBHoja* hoja, int clave, NodoGrafo* dato);

    // Splits
    void splitHoja(NodoBHoja* hoja, int clave, NodoGrafo* dato);
    NodoBInterno* splitInterno(NodoBInterno* interno, int& clave_median);

};
