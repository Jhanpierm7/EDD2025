#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Pasajero {
    char tipo_doc[3];     // CC, TI, PA
    char num_doc[20];     // Número de documento
    char apellido[50];    // Primer apellido
    struct Pasajero* siguiente;
} Pasajero;

Pasajero* inicio = NULL;
int capacidad_max;
int limite_overbooking;
int pasajeros_registrados = 0;

void registrar_pasajero();
void mostrar_pasajeros();
void liberar_memoria();

int main() {
    printf("===== Sistema de Gestion de Pasajeros =====\n");
    printf("Ingrese la capacidad maxima del avion: ");
    scanf("%d", &capacidad_max);
    getchar(); // limpiar buffer

    limite_overbooking = capacidad_max + ceil(capacidad_max * 0.10);

    int opcion;
    do {
        printf("\n===== Menu Principal =====\n");
        printf("1. Registrar pasajero\n");
        printf("2. Mostrar lista de pasajeros\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar(); // limpiar buffer

        switch (opcion) {
            case 1:
                registrar_pasajero();
                break;
            case 2:
                mostrar_pasajeros();
                break;
            case 3:
                liberar_memoria();
                printf("Gracias por usar el sistema.\n");
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
        }
    } while (opcion != 3);

    return 0;
}

void registrar_pasajero() {
    if (pasajeros_registrados >= limite_overbooking) {
        printf("No se pueden registrar más pasajeros. Límite alcanzado.\n");
        return;
    }

    Pasajero* nuevo = (Pasajero*)malloc(sizeof(Pasajero));
    if (nuevo == NULL) {
        printf("Error al asignar memoria.\n");
        return;
    }

    // Tipo de documento
    int valido = 0;
    while (!valido) {
        printf("Ingrese el tipo de documento (CC, TI, PA): ");
        scanf("%2s", nuevo->tipo_doc);
        getchar(); // limpiar buffer del ENTER

        if (strcmp(nuevo->tipo_doc, "CC") == 0 || strcmp(nuevo->tipo_doc, "TI") == 0 || strcmp(nuevo->tipo_doc, "PA") == 0) {
            valido = 1;
        } else {
            printf("Tipo de documento invalido. Intente nuevamente.\n");
        }
    }

    // Número de documento
    printf("Ingrese el numero de documento: ");
    fgets(nuevo->num_doc, 20, stdin);
    nuevo->num_doc[strcspn(nuevo->num_doc, "\n")] = '\0';

    // Apellido
    printf("Ingrese el primer apellido del pasajero: ");
    fgets(nuevo->apellido, 50, stdin);
    nuevo->apellido[strcspn(nuevo->apellido, "\n")] = '\0';

    nuevo->siguiente = NULL;

    // Insertar al final
    if (inicio == NULL) {
        inicio = nuevo;
    } else {
        Pasajero* actual = inicio;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }

    pasajeros_registrados++;
    printf("Pasajero registrado correctamente. Total: %d/%d\n", pasajeros_registrados, limite_overbooking);
}

void mostrar_pasajeros() {
    if (inicio == NULL) {
        printf("No hay pasajeros registrados.\n");
        return;
    }

    printf("\n===== Lista de Pasajeros =====\n");
    Pasajero* actual = inicio;
    int i = 1;
    while (actual != NULL) {
        printf("%d. [%s] %s - Documento: %s\n", i, actual->tipo_doc, actual->apellido, actual->num_doc);
        actual = actual->siguiente;
        i++;
    }

    printf("\nCapacidad del avion: %d\n", capacidad_max);
    printf("Maximo permitido con overbooking (10%%): %d\n", limite_overbooking);
    printf("Pasajeros registrados: %d\n", pasajeros_registrados);
}

void liberar_memoria() {
    Pasajero* actual = inicio;
    while (actual != NULL) {
        Pasajero* temp = actual;
        actual = actual->siguiente;
        free(temp);
    }
    inicio = NULL;
    printf("Memoria liberada correctamente.\n");
}