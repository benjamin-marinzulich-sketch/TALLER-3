#pragma once
#include <vector>
using namespace std;

class NodoGrafo {
protected:
    int id;
    vector<int> padres; 

public:
    NodoGrafo(int _id);

    int get_id() const;
    void agregar_padre(int padre_id);
    void eliminar_padre(int padre_id);

    virtual bool es_directorio() = 0;
    virtual vector<int> lista_hijos(); 
    vector<int> lista_padres();
};

// ---------------------------------------

class NodoDirectorio : public NodoGrafo {
private:
    vector<int> hijos;

public:
    NodoDirectorio(int _id);

    bool es_directorio() override { return true; }
    void agregar_hijo(int hijo_id);
    vector<int> lista_hijos() override;
    void eliminar_hijo(int hijo_id);
};

// ---------------------------------------

class NodoArchivo : public NodoGrafo {
private:
    int tamano;
    int tipo; // 0=Imagen; 1=Documento; 2=Ejecutable; 3=Video; 4=Comprimido

public:
    NodoArchivo(int _id, int _tamano, int _tipo);

    bool es_directorio() override { return false; }
};
