#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Estructura del Nodo 
typedef struct Pasajero {
    int documento;               // clave de búsqueda (ABB)
    char destino[20];            // Timbíquí, Juanchaco, Tumaco, etc.
    int tipo_pasaje;             // 0 = se va , 1 = se va y regreso

    struct Pasajero *izq;
    struct Pasajero *der;

} Pasajero;

// Crear nodo
Pasajero* crearNodo(int documento, char destino[], int tipo_pasaje) {
    Pasajero* nuevo = (Pasajero*)malloc(sizeof(Pasajero));
    nuevo->documento = documento;
    strcpy(nuevo->destino, destino);
    nuevo->tipo_pasaje = tipo_pasaje;
    nuevo->izq = nuevo->der = NULL;
    return nuevo;
}

//Insertar en el ABB
Pasajero* insertar(Pasajero* raiz, int documento, char destino[], int tipo_pasaje) {
    if (raiz == NULL) {
        return crearNodo(documento, destino, tipo_pasaje);
    }

    if (documento < raiz->documento)
        raiz->izq = insertar(raiz->izq, documento, destino, tipo_pasaje);
    else if (documento > raiz->documento)
        raiz->der = insertar(raiz->der, documento, destino, tipo_pasaje);
    else
        printf("⚠ El documento %d ya existe. No se insertará.\n", documento);

    return raiz;
}

//Recorridos 
void inorden(Pasajero* raiz) {
    if (raiz != NULL) {
        inorden(raiz->izq);
        printf("Doc: %d | Destino: %s | Tipo: %s\n",
            raiz->documento,
            raiz->destino,
            raiz->tipo_pasaje == 0 ? "se va" : "se va y regreso");
        inorden(raiz->der);
    }
}

void preorden(Pasajero* raiz) {
    if (raiz != NULL) {
        printf("Doc: %d | Destino: %s | Tipo: %s\n",
            raiz->documento,
            raiz->destino,
            raiz->tipo_pasaje == 0 ? "se va " : "se va y regreso");
        preorden(raiz->izq);
        preorden(raiz->der);
    }
}

void postorden(Pasajero* raiz) {
    if (raiz != NULL) {
        postorden(raiz->izq);
        postorden(raiz->der);
        printf("Doc: %d | Destino: %s | Tipo: %s\n",
            raiz->documento,
            raiz->destino,
            raiz->tipo_pasaje == 0 ? "se va" : "se va  y regreso");
    }
}

//Contar Nodos
int contar(Pasajero* raiz) {
    if (raiz == NULL) return 0;
    return 1 + contar(raiz->izq) + contar(raiz->der);
}

//Buscar Minimo     
Pasajero* minimo(Pasajero* raiz) {
    while (raiz->izq != NULL)
        raiz = raiz->izq;
    return raiz;
}

//Eliminar nodo por el documento     
Pasajero* eliminar(Pasajero* raiz, int documento) {
    if (raiz == NULL) return raiz;

    if (documento < raiz->documento) {
        raiz->izq = eliminar(raiz->izq, documento);
    }
    else if (documento > raiz->documento) {
        raiz->der = eliminar(raiz->der, documento);
    }
    else {
        // Caso 1: sin hijos
        if (raiz->izq == NULL && raiz->der == NULL) {
            free(raiz);
            return NULL;
        }
        // Caso 2: un hijo
        else if (raiz->izq == NULL) {
            Pasajero* temp = raiz->der;
            free(raiz);
            return temp;
        }
        else if (raiz->der == NULL) {
            Pasajero* temp = raiz->izq;
            free(raiz);
            return temp;
        }
        // Caso 3: dos hijos
        Pasajero* temp = minimo(raiz->der);
        raiz->documento = temp->documento;
        strcpy(raiz->destino, temp->destino);
        raiz->tipo_pasaje = temp->tipo_pasaje;

        raiz->der = eliminar(raiz->der, temp->documento);
    }

    return raiz;
}

//Menu
void mostrarMenu() {
    printf("\n====== Sistema de Gestión de Tiquetes ======\n");
    printf("1. Registrar pasajero\n");
    printf("2. Mostrar pasajeros (Inorden)\n");
    printf("3. Mostrar pasajeros (Preorden)\n");
    printf("4. Mostrar pasajeros (Postorden)\n");
    printf("5. Contar pasajeros\n");
    printf("6. Eliminar pasajero\n");
    printf("7. Salir\n");
    printf("Seleccione una opcion: ");
}

//Main 
int main() {
    Pasajero* raiz = NULL;
    int op;

    do {
        mostrarMenu();
        scanf("%d", &op);

        if (op == 1) {
            int doc, tipo;
            char dest[20];

            printf("Documento: ");
            scanf("%d", &doc);

            printf("Destinos disponibles: Timbiqui, Juanchaco, Tumaco, Guapi\n");
            printf("Destino: ");
            scanf("%s", dest);

            printf("Tipo de pasaje (0 = se va, 1 = se va y regreso): ");
            scanf("%d", &tipo);

            raiz = insertar(raiz, doc, dest, tipo);
        }
        else if (op == 2) {
            printf("\n--- LISTADO INORDEN ---\n");
            inorden(raiz);
        }
        else if (op == 3) {
            printf("\n--- LISTADO PREORDEN ---\n");
            preorden(raiz);
        }
        else if (op == 4) {
            printf("\n--- LISTADO POSTORDEN ---\n");
            postorden(raiz);
        }
        else if (op == 5) {
            printf("Total pasajeros: %d\n", contar(raiz));
        }
        else if (op == 6) {
            int doc;
            printf("Documento a eliminar: ");
            scanf("%d", &doc);
            raiz = eliminar(raiz, doc);
        }

    } while (op != 7);

    return 0;
}