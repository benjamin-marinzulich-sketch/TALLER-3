#pragma once

#include "NodoGrafo.h"

class NodoBPlusBase {
protected:
    int* claves;
    int cantidad_claves;
    int orden;
    bool es_hoja;
    NodoBPlusBase* padre; // puntero al padre (puede ser interno)
    

public:
    NodoBPlusBase(int _orden, bool _es_hoja);
    virtual ~NodoBPlusBase();

    bool esHoja() const;
    int getCantidadClaves() const;
    int getClave(int index) const;

    // helpers para manejo de padre y orden
    void setPadre(NodoBPlusBase* p);
    NodoBPlusBase* getPadre() const;
    int getOrden() const;

    // helpers para manejar claves (implementación simple)
    void insertarClaveEnPos(int clave, int pos);
    void incrementarCantidadClaves();
    void decrementarCantidadClaves() { cantidad_claves--; }
    void setCantidadClaves(int c);
    void desplazarDerechaDesde(int pos);
};

class NodoBInterno : public NodoBPlusBase {
private:
    NodoBPlusBase** punteros;

    NodoBInterno* padre;

public:
    NodoBInterno(int orden);
    ~NodoBInterno();

    // firma consistente: recibe la clave a buscar
    int buscar_siguiente(int clave);
    NodoBPlusBase* getHijo(int i);
    void setHijo(int i, NodoBPlusBase* hijo);
    void desplazarHijosDerechaDesde(int pos);  // desplaza punteros a hijos
    void setPadre(NodoBInterno*);
    NodoBInterno* getPadre();
};

class NodoBHoja : public NodoBPlusBase {
private:
    NodoGrafo** datos;
    NodoBHoja* siguiente_hoja;

    NodoBInterno* padre;


public:
    NodoBHoja(int orden);
    ~NodoBHoja();

    NodoGrafo* getDato(int i);
    void setDato(int i, NodoGrafo* d);

    void setSiguiente(NodoBHoja* nxt);
    NodoBHoja* getSiguiente();
    void setPadre(NodoBInterno*);
    NodoBInterno* getPadre();

};
