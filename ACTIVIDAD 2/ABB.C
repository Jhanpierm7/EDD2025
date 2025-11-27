#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//   ESTRUCTURA DEL NODO
typedef struct Pasajero {
    int documento;               // clave de búsqueda (ABB)
    char destino[20];            // Timbiquí, Juanchaco, Tumaco, etc.
    int tipo_pasaje;             // 0 = Ida, 1 = Ida y regreso

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

//     INSERTAR EN EL ABB
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

//       MÉTODO BUSCAR
Pasajero* buscar(Pasajero* raiz, int documento) {
    if (raiz == NULL)
        return NULL;

    if (documento == raiz->documento)
        return raiz;

    if (documento < raiz->documento)
        return buscar(raiz->izq, documento);
    else
        return buscar(raiz->der, documento);
}

//       RECORRIDOS
void inorden(Pasajero* raiz) {
    if (raiz != NULL) {
        inorden(raiz->izq);
        printf("Doc: %d | Destino: %s | Tipo: %s\n",
            raiz->documento,
            raiz->destino,
            raiz->tipo_pasaje == 0 ? "Ida" : "Ida y regreso");
        inorden(raiz->der);
    }
}

void preorden(Pasajero* raiz) {
    if (raiz != NULL) {
        printf("Doc: %d | Destino: %s | Tipo: %s\n",
            raiz->documento,
            raiz->destino,
            raiz->tipo_pasaje == 0 ? "Ida" : "Ida y regreso");
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
            raiz->tipo_pasaje == 0 ? "Ida" : "Ida y regreso");
    }
}

//      CONTAR NODOS
int contar(Pasajero* raiz) {
    if (raiz == NULL) return 0;
    return 1 + contar(raiz->izq) + contar(raiz->der);
}

//      BUSCAR MÍNIMO
Pasajero* minimo(Pasajero* raiz) {
    while (raiz->izq != NULL)
        raiz = raiz->izq;
    return raiz;
}

//     ELIMINAR NODO POR DOCUMENTO
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

//         MENÚ
void mostrarMenu() {
    printf("\n====== Sistema de Gestión de Tiquetes ======\n");
    printf("1. Registrar pasajero\n");
    printf("2. Mostrar pasajeros (Inorden)\n");
    printf("3. Mostrar pasajeros (Preorden)\n");
    printf("4. Mostrar pasajeros (Postorden)\n");
    printf("5. Contar pasajeros\n");
    printf("6. Eliminar pasajero\n");
    printf("7. Buscar pasajero\n");
    printf("8. Salir\n");
    printf("Seleccione una opción: ");
}

//          MAIN
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

            printf("Tipo de pasaje (0 = Ida, 1 = Ida y regreso): ");
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

            Pasajero* p = buscar(raiz, doc);

            if (p == NULL) {
                printf("❌ El pasajero no existe.\n");
            } else {
                raiz = eliminar(raiz, doc);
                printf("✔ Pasajero eliminado correctamente.\n");
            }
        }
        else if (op == 7) {
            int doc;
            printf("Documento a buscar: ");
            scanf("%d", &doc);

            Pasajero* p = buscar(raiz, doc);

            if (p == NULL) {
                printf("❌ No se encontró ese pasajero.\n");
            } else {
                printf("✔ Pasajero encontrado:\n");
                printf("Doc: %d | Destino: %s | Tipo: %s\n",
                       p->documento,
                       p->destino,
                       p->tipo_pasaje == 0 ? "Ida" : "Ida y regreso");
            }
        }
        else if (op == 8) {
            printf("Saliendo...\n");
        }
        else {
            printf("Opción inválida. Intente de nuevo.\n");
        }
    } while (op != 8);

    return 0;
}
