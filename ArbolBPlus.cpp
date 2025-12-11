#include "ArbolBPlus.h"
#include <iostream>
using namespace std;

// ===================== Constructor ======================

ArbolBPlus::ArbolBPlus(int _orden)
    : orden(_orden)
{
    raiz = new NodoBHoja(orden); // al inicio solo existe una hoja
}

// ===================== BÚSQUEDA ======================

NodoBHoja* ArbolBPlus::buscarHoja(int clave) {

    NodoBPlusBase* actual = raiz;

    while (!actual->esHoja()) {
        NodoBInterno* interno = (NodoBInterno*)actual;
        int pos = interno->buscar_siguiente(clave);
        actual = interno->getHijo(pos);
    }

    return (NodoBHoja*)actual;
}

NodoGrafo* ArbolBPlus::buscar(int clave) {

    NodoBHoja* hoja = buscarHoja(clave);

    int n = hoja->getCantidadClaves();
    for (int i = 0; i < n; i++) {
        if (hoja->getClave(i) == clave) {
            return hoja->getDato(i);
        }
    }
    return nullptr;
}

// ===================== INSERCIÓN ======================

void ArbolBPlus::insertar(int clave, NodoGrafo* dato) {

    NodoBHoja* hoja = buscarHoja(clave);

    // Si aún hay espacio en la hoja
    if (hoja->getCantidadClaves() < orden) {
        insertarEnHoja(hoja, clave, dato);
        return;
    }

    // Si la hoja está llena
    splitHoja(hoja, clave, dato);
}

void ArbolBPlus::insertarEnHoja(NodoBHoja* hoja, int clave, NodoGrafo* dato) {

    int n = hoja->getCantidadClaves();
    int pos = 0;

    // buscar posición ordenada
    while (pos < n && hoja->getClave(pos) < clave)
        pos++;

    // desplazar
    hoja->desplazarDerechaDesde(pos);

    hoja->insertarClaveEnPos(clave, pos);
    hoja->setDato(pos, dato);

}


void ArbolBPlus::splitHoja(NodoBHoja* hoja, int clave, NodoGrafo* dato) {

    // 1. Crear nueva hoja
    NodoBHoja* nueva = new NodoBHoja(orden);

    int total = orden + 1;   // claves que estarán momentáneamente todas juntas
    int mitad = total / 2;

    // Arreglos temporales (sin STL)
    int* claves_temp = new int[total];
    NodoGrafo** datos_temp = new NodoGrafo*[total];

    // copiar lo que ya tenía la hoja
    for (int i = 0; i < orden; i++) {
        claves_temp[i] = hoja->getClave(i);
        datos_temp[i] = hoja->getDato(i);
    }

    // Insertar la nueva clave en el lugar correcto del arreglo temporal
    int pos = 0;
    while (pos < orden && claves_temp[pos] < clave)
        pos++;

    // desplazar
    for (int i = total - 1; i > pos; i--) {
        claves_temp[i] = claves_temp[i - 1];
        datos_temp[i] = datos_temp[i - 1];
    }

    claves_temp[pos] = clave;
    datos_temp[pos] = dato;

    // 2. Repartir claves entre hoja izquierda y hoja derecha
    hoja->incrementarCantidadClaves();

    nueva->incrementarCantidadClaves();


    // izquierda (hoja original)
    for (int i = 0; i < mitad; i++) {
        hoja->insertarClaveEnPos(claves_temp[i], i);
        hoja->setDato(i, datos_temp[i]);
    }
    hoja->setCantidadClaves(mitad);


    // derecha (nueva hoja)
    for (int i = mitad; i < total; i++) {
        int j = i - mitad;
        nueva->insertarClaveEnPos(claves_temp[i], j);
        nueva->setDato(j, datos_temp[i]);
    }
    nueva->setCantidadClaves(hoja->getCantidadClaves() - mitad);


    // 3. Ajustar enlaces entre hojas
    nueva->setSiguiente(hoja->getSiguiente());
    hoja->setSiguiente(nueva);

    // clave promovida = primera clave de la hoja nueva
    int clave_promovida = nueva->getClave(0);

    delete[] claves_temp;
    delete[] datos_temp;

    // 4. Caso especial: la hoja era la raíz
    if (hoja == raiz) {

        NodoBInterno* nueva_raiz = new NodoBInterno(orden);

        nueva_raiz->insertarClaveEnPos(clave_promovida, 0);
        nueva_raiz->setCantidadClaves(1);


        nueva_raiz->setHijo(0, hoja);
        hoja->setPadre(nueva_raiz);
        nueva_raiz->setHijo(1, nueva);
        nueva->setPadre(nueva_raiz);

        raiz = nueva_raiz;
        return;
    }

    // 5. Si no era la raíz → insertar clave en el padre
    NodoBPlusBase* actual = raiz;
    NodoBInterno* padre = nullptr;

    // Buscar el padre de la hoja
    while (!actual->esHoja()) {
        NodoBInterno* interno = (NodoBInterno*)actual;
        int pos_hijo = interno->buscar_siguiente(clave);
        padre = interno;
        actual = interno->getHijo(pos_hijo);
    }

    // Insertar clave promovida en el padre
    if (padre->getCantidadClaves() < orden) {

        int pos_i = padre->buscar_siguiente(clave_promovida);
        padre->desplazarDerechaDesde(pos_i);

        padre->insertarClaveEnPos(clave_promovida, pos_i);

        padre->setHijo(pos_i + 1, nueva);
        nueva->setPadre(padre);
        return;
    }

    // si padre está lleno → split del nodo interno
    splitInterno(padre, clave_promovida);
}

NodoBInterno* ArbolBPlus::splitInterno(NodoBInterno* interno, int& clave_median){
    int orden = interno->getOrden();
    int total = interno->getCantidadClaves();
    int mitad = total / 2;  

    // Crear el nuevo nodo interno derecho
    NodoBInterno* nuevoInterno = new NodoBInterno(orden);

    // La clave que sube al padre
    clave_median = interno->getClave(mitad);

    // Copiar claves desde mitad+1 hacia adelante al nuevo interno
    int idxNuevo = 0;
    for (int i = mitad + 1; i < total; i++) {
        nuevoInterno->insertarClaveEnPos(interno->getClave(i), idxNuevo);
        nuevoInterno->incrementarCantidadClaves();
        idxNuevo++;
    }

    // Copiar punteros
    // Punteros 0..mitad van al nodo original
    // Punteros mitad+1..total van al nuevo nodo
    for (int i = mitad + 1; i <= total; i++) {
        nuevoInterno->setHijo(i - (mitad + 1), interno->getHijo(i));
        interno->getHijo(i)->setPadre(nuevoInterno);
    }

    // Ajustar cantidad claves del nodo original (izquierdo)
    interno->setCantidadClaves(mitad);

    return nuevoInterno;
}
