#pragma once
#include "NodoGrafo.h"

class NodoBPlusBase {
protected:
    int* claves;    
    int orden;       
    bool es_hoja;   

public:
    NodoBPlusBase(int orden, bool es_hoja);
    virtual ~NodoBPlusBase();

    bool esHoja() const { return es_hoja; }

 
    virtual int buscar_siguiente(int clave) = 0;
};

// ------------------------------------------------------------

class NodoBInterno : public NodoBPlusBase {
private:
    NodoBPlusBase** punteros;  

public:
    NodoBInterno(int orden);
    ~NodoBInterno();

    int buscar_siguiente(int clave) override;
};

// ------------------------------------------------------------

class NodoBHoja : public NodoBPlusBase {
private:
    NodoGrafo** datos;     
    NodoBHoja* siguiente;   

public:
    NodoBHoja(int orden);
    ~NodoBHoja();

    int buscar_siguiente(int clave) override; 
    NodoGrafo* obtenerDato(int idx);
};
