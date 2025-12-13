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
//   BÚSQUEDA CON CONTADOR DE ACCESOS
// ======================================================   

static ArbolBPlus* global_arbol = nullptr;

void set_global_arbol(ArbolBPlus* a) {
    global_arbol = a;
}

// Búsqueda (conteo accesos)
NodoGrafo* buscar_nodo_grafo(int clave, int* accesos = nullptr) {
    if (!global_arbol) return nullptr;

    int cuenta = 0;

    NodoBPlusBase* actual = global_arbol->getRaiz();

    // Descender hasta hoja, contando accesos por nodo visitado
    while (!actual->esHoja()) {
        cuenta++; // acceso a un nodo interno
        NodoBInterno* interno = (NodoBInterno*)actual;
        int pos = interno->buscar_siguiente(clave);
        actual = interno->getHijo(pos);
    }

    // contar la hoja visitada
    cuenta++;

    NodoBHoja* hoja = (NodoBHoja*)actual;
    int n = hoja->getCantidadClaves();
    for (int i = 0; i < n; i++) {
        if (hoja->getClave(i) == clave) {
            if (accesos) *accesos = cuenta;
            return hoja->getDato(i);
        }
    }

    if (accesos) *accesos = cuenta;
    return nullptr;
}



// ======================================================
//   CREAR NODO
// ======================================================   
void crear_nodo(int id_padre) {
    if (!global_arbol) {
        cout << "Error: arbol no inicializado\n";
        return;
    }

    NodoGrafo* padre = buscar_nodo_grafo(id_padre);
    if (!padre) {
        cout << "Padre con id " << id_padre << " no encontrado\n";
        return;
    }

    if (!padre->es_directorio()) {
        cout << "El nodo " << id_padre << " no es un directorio\n";
        return;
    }

    static int next_id = 200;
    int nuevo_id = next_id++;

    NodoDirectorio* nuevo = new NodoDirectorio(nuevo_id);

    // Insertar en B+
    global_arbol->insertar(nuevo_id, nuevo);

    // Actualizar padre
    NodoDirectorio* pdir = (NodoDirectorio*)padre;
    pdir->agregar_hijo(nuevo_id);

    cout << "Nodo creado id=" << nuevo_id << " agregado como hijo de " << id_padre << "\n";
}


// ======================================================
//   EliMINAR ARCHIVO
// ======================================================   
void eliminar_archivo(int id_archivo, int id_directorio_padre) {
    if (!global_arbol) {
        cout << "Error: arbol no inicializado\n";
        return;
    }

    int acc = 0;
    NodoGrafo* archivo = buscar_nodo_grafo(id_archivo, &acc);
    if (!archivo) {
        cout << "Archivo id=" << id_archivo << " no encontrado en B+\n";
        return;
    }

    // Quitar padre
    archivo->eliminar_padre(id_directorio_padre);

    // Actualizar padre
    int acc2 = 0;
    NodoGrafo* padre = buscar_nodo_grafo(id_directorio_padre, &acc2);
    if (padre && padre->es_directorio()) {
        NodoDirectorio* pd = (NodoDirectorio*)padre;
        pd->eliminar_hijo(id_archivo);
    } else {
        cout << "Advertencia: padre id=" << id_directorio_padre << " no encontrado o no es directorio\n";
    }

    // Si sin padres -> eliminar del B+
    vector<int> padres_restantes = archivo->lista_padres();
    if (padres_restantes.empty()) {
        // Buscar hoja
        NodoBPlusBase* actual = global_arbol->getRaiz();
        while (!actual->esHoja()) {
            NodoBInterno* interno = (NodoBInterno*)actual;
            int pos = interno->buscar_siguiente(id_archivo);
            actual = interno->getHijo(pos);
        }
        NodoBHoja* hoja = (NodoBHoja*)actual;

        int n = hoja->getCantidadClaves();
        vector<int> claves_tmp;
        vector<NodoGrafo*> datos_tmp;
        for (int i = 0; i < n; i++) {
            int c = hoja->getClave(i);
            NodoGrafo* d = hoja->getDato(i);
            if (c == id_archivo) {
                continue;
            }
            claves_tmp.push_back(c);
            datos_tmp.push_back(d);
        }

        hoja->setCantidadClaves(0);
        for (size_t i = 0; i < claves_tmp.size(); ++i) {
            hoja->insertarClaveEnPos(claves_tmp[i], (int)i);
            hoja->setDato((int)i, datos_tmp[i]);
        }

        cout << "Archivo id=" << id_archivo << " eliminado del B+\n";
    } else {
        cout << "Archivo id=" << id_archivo << " referencia eliminada; padres restantes=" << padres_restantes.size() << "\n";
    }
}


// ======================================================
//                       MAIN
// ======================================================
// --- Helpers de entrada ---
static int read_int(const string &prompt) {
    while (true) {
        cout << prompt;
        string line;
        if (!std::getline(cin, line)) return 0;
        try {
            size_t pos;
            int v = stoi(line, &pos);
            if (pos != line.size()) throw invalid_argument("extra");
            return v;
        } catch (...) {
            cout << "Entrada invalida. Intentalo de nuevo.\n";
        }
    }
}

static int read_choice(int minv, int maxv) {
    while (true) {
        int v = read_int("Elige una opcion: ");
        if (v >= minv && v <= maxv) return v;
        cout << "Opcion fuera de rango. Intenta otra vez.\n";
    }
}

int main() {
    ArbolBPlus* arbol = new ArbolBPlus(4); 
    set_global_arbol(arbol);

    cout << "Bienvenido. ¿Que desea hacer?\n";

    while (true) {
        cout << "\nMenu:\n";
        cout << "1) Insertar nodo\n";
        cout << "2) Buscar nodo\n";
        cout << "3) Crear nodo (hijo)\n";
        cout << "4) Eliminar archivo (refcount)\n";
        cout << "5) Listar hijos de directorio\n";
        cout << "6) Salir\n";

        int opt = read_choice(1, 6);

        if (opt == 6) break;

        if (opt == 1) {
            int id = read_int("Id para insertar: ");
            cout << "Tipo: 1=directorio, 2=archivo\n";
            int tipo = read_choice(1,2);
            if (tipo == 1) {
                NodoDirectorio* nd = new NodoDirectorio(id);
                insertar_nodo_grafo(id, nd, arbol);
                cout << "Directorio insertado con id=" << id << "\n";
            } else {
                int tam = read_int("Tamaño: ");
                int ttip = read_int("Tipo (0=Imagen,1=Documento,2=Ejecutable,3=Video,4=Comprimido): ");
                NodoArchivo* nf = new NodoArchivo(id, tam, ttip);
                insertar_nodo_grafo(id, nf, arbol);
                cout << "Archivo insertado con id=" << id << "\n";
                cout << "¿Agregar padre ahora? (0=no, 1=si): ";
                string ans; getline(cin, ans);
                if (!ans.empty() && ans[0] == '1') {
                    int pid = read_int("Id del directorio padre: ");
                    int a=0;
                    NodoGrafo* p = buscar_nodo_grafo(pid, &a);
                    if (p && p->es_directorio()) {
                        nf->agregar_padre(pid);
                        ((NodoDirectorio*)p)->agregar_hijo(id);
                        cout << "Padre actualizado.\n";
                    } else cout << "Padre no encontrado o no es directorio.\n";
                }
            }
        } else if (opt == 2) {
            int id = read_int("Id a buscar: ");
            int acc = 0;
            NodoGrafo* r = buscar_nodo_grafo(id, &acc);
            cout << "Accesos B+: " << acc << "\n";
            if (r) {
                cout << "Encontrado id=" << r->get_id() << ", es_directorio=" << (r->es_directorio()?"si":"no") << "\n";
                if (r->es_directorio()) {
                    auto h = ((NodoDirectorio*)r)->lista_hijos();
                    cout << "Hijos: "; for (int v: h) cout << v << " "; cout << "\n";
                } else {
                    auto p = r->lista_padres();
                    cout << "Padres: "; for (int v: p) cout << v << " "; cout << "\n";
                }
            } else cout << "No encontrado.\n";

        } else if (opt == 3) {
            int pid = read_int("Id del directorio padre: ");
            crear_nodo(pid);

        } else if (opt == 4) {
            int fid = read_int("Id del archivo a eliminar (referencia): ");
            int pid = read_int("Id del directorio padre que elimina la referencia: ");
            eliminar_archivo(fid, pid);

        } else if (opt == 5) {
            int pid = read_int("Id del directorio: ");
            int a=0;
            NodoGrafo* p = buscar_nodo_grafo(pid, &a);
            if (p && p->es_directorio()) {
                auto h = ((NodoDirectorio*)p)->lista_hijos();
                cout << "Hijos de " << pid << ": "; for (int v: h) cout << v << " "; cout << "\n";
            } else cout << "Directorio no encontrado.\n";
        }
    }

    cout << "Saliendo.\n";
    return 0;
}
















































