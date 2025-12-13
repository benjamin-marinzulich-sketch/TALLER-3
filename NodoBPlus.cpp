#include "NodoBPlus.h"

// ====================== NodoBPlusBase ======================

NodoBPlusBase::NodoBPlusBase(int _orden, bool _es_hoja)
    : orden(_orden), es_hoja(_es_hoja), cantidad_claves(0), padre(nullptr)
{
    claves = new int[orden];
}

NodoBPlusBase::~NodoBPlusBase() {
    delete[] claves;
}

bool NodoBPlusBase::esHoja() const {
    return es_hoja;
}

int NodoBPlusBase::getCantidadClaves() const {
    return cantidad_claves;
}

int NodoBPlusBase::getClave(int index) const {
    return claves[index];
}

void NodoBPlusBase::insertarClaveEnPos(int clave, int pos) {
    claves[pos] = clave;
    
    cantidad_claves++;
}

void NodoBPlusBase::desplazarDerechaDesde(int pos) {
    for (int i = cantidad_claves; i > pos; i--)
        claves[i] = claves[i - 1];
}

// ---------------- parent / orden / counters ----------------

void NodoBPlusBase::setPadre(NodoBPlusBase* p) {
    padre = p;
}

NodoBPlusBase* NodoBPlusBase::getPadre() const {
    return padre;
}

int NodoBPlusBase::getOrden() const {
    return orden;
}

void NodoBPlusBase::incrementarCantidadClaves() {
    cantidad_claves++;
}

void NodoBPlusBase::setCantidadClaves(int c) {
    cantidad_claves = c;
}

// ====================== NodoBInterno ======================

NodoBInterno::NodoBInterno(int orden)
    : NodoBPlusBase(orden, false)
{
    punteros = new NodoBPlusBase*[orden + 1];
    for (int i = 0; i < orden + 1; i++)
        punteros[i] = nullptr;
}

NodoBInterno::~NodoBInterno() {
    delete[] punteros;
}

int NodoBInterno::buscar_siguiente(int clave) {
    int i = 0;
    while (i < cantidad_claves && clave >= claves[i])
        i++;
    return i;
}

NodoBPlusBase* NodoBInterno::getHijo(int i) {
    return punteros[i];
}

void NodoBInterno::setHijo(int i, NodoBPlusBase* hijo) {
    punteros[i] = hijo;
}

void NodoBInterno::desplazarHijosDerechaDesde(int pos) {
    // Desplazar punteros a hijos desde posición pos hacia la derecha
    // (order+1 punteros, no solo orden)
    for (int i = cantidad_claves + 1; i > pos; i--) {
        punteros[i] = punteros[i - 1];
    }
}
void NodoBInterno::setPadre(NodoBInterno* p) {
    padre = p;
}

NodoBInterno* NodoBInterno::getPadre() {
    return padre;
}


// ====================== NodoBHoja ======================

NodoBHoja::NodoBHoja(int orden)
    : NodoBPlusBase(orden, true), siguiente_hoja(nullptr)
{
    datos = new NodoGrafo*[orden];
    for (int i = 0; i < orden; i++)
        datos[i] = nullptr;
}

NodoBHoja::~NodoBHoja() {
    delete[] datos;
}

NodoGrafo* NodoBHoja::getDato(int i) {
    return datos[i];
}

void NodoBHoja::setDato(int i, NodoGrafo* d) {
    datos[i] = d;
}

void NodoBHoja::setSiguiente(NodoBHoja* nxt) {
    siguiente_hoja = nxt;
}

NodoBHoja* NodoBHoja::getSiguiente() {
    return siguiente_hoja;
}
void NodoBHoja::setPadre(NodoBInterno* p) {
    padre = p;
}

NodoBInterno* NodoBHoja::getPadre() {
    return padre;
}