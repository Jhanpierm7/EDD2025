#include <stdio.h>   
#include <stdlib.h>  
#include <stdbool.h> 


typedef enum { FEMENINO, MASCULINO, NO_BINARIO } Genero;

typedef struct Nodo {
    Genero genero;
    char apellido[50];
    struct Nodo* siguiente;
} Pasajero;

int g_capacidad_maxima = 0;
int g_limite_sobreventa = 0;
int g_tiquetes_vendidos = 0;
bool g_abordaje_iniciado = false;

// Lista de todos los tiquetes vendidos (Cola FIFO)
Pasajero* g_lista_venta_cabeza = NULL;
Pasajero* g_lista_venta_cola = NULL;

Pasajero* g_lista_abordados_cabeza = NULL;
Pasajero* g_lista_abordados_cola = NULL;

// --- PROTOTIPOS ---
void establecerCapacidad();
void venderTiquete();
void iniciarAbordaje();
void verAbordados();
void verNoAbordados();
void limpiarBufferEntrada();
void liberarMemoria(Pasajero* cabeza);
const char* generoAString(Genero g);
void mostrarMenu();

int main() {
    int opcion = 0;
    printf("--- SISTEMA DE GESTIÓN DE TIQUETES AÉREOS ---\n");
    establecerCapacidad(); 

    do {
        mostrarMenu();
        if (scanf("%d", &opcion) != 1) {
            printf("Error: Entrada no válida.\n");
            limpiarBufferEntrada();
            opcion = 0; 
            continue;
        }
        limpiarBufferEntrada();

        switch (opcion) {
            case 1: printf("Error: Capacidad ya establecida.\n"); break;
            case 2: venderTiquete(); break;
            case 3: iniciarAbordaje(); break;
            case 4: verAbordados(); break;
            case 5: verNoAbordados(); break;
            case 6: printf("Saliendo...\n"); break;
            default: printf("Opción no válida.\n"); break;
        }
        if (opcion != 6) {
             printf("\n(Presione Enter para continuar...)");
             getchar();
        }
    } while (opcion != 6);

    liberarMemoria(g_lista_venta_cabeza);
    liberarMemoria(g_lista_abordados_cabeza);
    printf("Memoria liberada. Adiós.\n");
    return EXIT_SUCCESS;
}

//  AUXILIARES 
void limpiarBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

const char* generoAString(Genero g) {
    return (g == FEMENINO) ? "Femenino" : ((g == MASCULINO) ? "Masculino" : "No Binario");
}

void liberarMemoria(Pasajero* cabeza) {
    Pasajero* actual = cabeza;
    while (actual != NULL) {
        Pasajero* siguiente_nodo = actual->siguiente;
        free(actual);
        actual = siguiente_nodo;
    }
}

void mostrarMenu() {
    printf("\n--- MENÚ (Cap: %d | Sobrev: %d | Vend: %d) ---\n", 
           g_capacidad_maxima, g_limite_sobreventa, g_tiquetes_vendidos);
    printf("Estado: %s\n", g_abordaje_iniciado ? "ABORDAJE INICIADO (Cerrado)" : "REGISTRO ABIERTO");
    printf("--------------------------------------\n");
    printf("2. Vender Tiquete\n");
    printf("3. Iniciar Abordaje\n");
    printf("4. Ver Abordados\n");
    printf("5. Ver No Abordados\n");
    printf("6. Salir\n");
    printf("Seleccione una opción: ");
}

// --- FUNCIONES DEL MENÚ ---
void establecerCapacidad() {
    do {
        printf("Ingrese la capacidad máxima del avión (> 0): ");
        if (scanf("%d", &g_capacidad_maxima) != 1) {
            limpiarBufferEntrada();
            g_capacidad_maxima = 0; 
        }
        limpiarBufferEntrada();
        if (g_capacidad_maxima <= 0) printf("Error: Debe ser positivo.\n");
    } while (g_capacidad_maxima <= 0);

    // Cálculo de sobreventa (110%)
    g_limite_sobreventa = (int)(g_capacidad_maxima * 1.10);
    if (g_limite_sobreventa == g_capacidad_maxima) g_limite_sobreventa++; // Asegurar al menos +1
    
    printf("Capacidad: %d, Límite Sobreventa: %d\n", g_capacidad_maxima, g_limite_sobreventa);
}

void venderTiquete() {
    if (g_abordaje_iniciado) {
        printf("Error: Abordaje iniciado. Venta cerrada.\n");
        return;
    }
    if (g_tiquetes_vendidos >= g_limite_sobreventa) {
        printf("Error: Límite de sobreventa alcanzado (%d).\n", g_limite_sobreventa);
        return;
    }

    Pasajero* nuevo = (Pasajero*)malloc(sizeof(Pasajero));
    if (nuevo == NULL) {
        printf("Error fatal: No hay memoria (malloc falló).\n");
        exit(EXIT_FAILURE);
    }

    printf("--- Nuevo Tiquete (%d/%d) ---\n", g_tiquetes_vendidos + 1, g_limite_sobreventa);
    int g_opcion = 0;
    do {
        printf("Género (1:F, 2:M, 3:NB): ");
        if (scanf("%d", &g_opcion) != 1) {
            g_opcion = 0;
            limpiarBufferEntrada();
        } else {
            limpiarBufferEntrada();
            if (g_opcion < 1 || g_opcion > 3) printf("Opción inválida.\n");
        }
    } while (g_opcion < 1 || g_opcion > 3);
    
    nuevo->genero = (Genero)(g_opcion - 1);
    printf("Ingrese el primer apellido: ");
    scanf("%49s", nuevo->apellido);
    limpiarBufferEntrada();
    nuevo->siguiente = NULL;

    if (g_lista_venta_cabeza == NULL) {
        g_lista_venta_cabeza = nuevo;
        g_lista_venta_cola = nuevo;
    } else {
        g_lista_venta_cola->siguiente = nuevo;
        g_lista_venta_cola = nuevo;
    }
    g_tiquetes_vendidos++;
    printf("Tiquete vendido a %s.\n", nuevo->apellido);
}

void iniciarAbordaje() {
    if (g_abordaje_iniciado) {
        printf("Error: Abordaje ya realizado.\n");
        return;
    }
    if (g_lista_venta_cabeza == NULL) {
        printf("No hay tiquetes vendidos.\n");
        return;
    }

    printf("--- INICIANDO ABORDAJE (Cap: %d) ---\n", g_capacidad_maxima);
    g_abordaje_iniciado = true;
    int contador_abordados = 0;

    // Mover nodos de lista_venta a lista_abordados
    while (g_lista_venta_cabeza != NULL && contador_abordados < g_capacidad_maxima) {
        // 1. Sacar de lista_venta (Dequeue)
        Pasajero* p = g_lista_venta_cabeza;
        g_lista_venta_cabeza = g_lista_venta_cabeza->siguiente;
        if (g_lista_venta_cabeza == NULL) g_lista_venta_cola = NULL;
        p->siguiente = NULL;

        // 2. Poner en lista_abordados (Enqueue)
        if (g_lista_abordados_cabeza == NULL) {
            g_lista_abordados_cabeza = p;
            g_lista_abordados_cola = p;
        } else {
            g_lista_abordados_cola->siguiente = p;
            g_lista_abordados_cola = p;
        }
        
        contador_abordados++;
        printf("Abordando (%d/%d): %s, %s\n", contador_abordados, g_capacidad_maxima, 
               p->apellido, generoAString(p->genero));
    }
    printf("--- ABORDAJE FINALIZADO (Total: %d) ---\n", contador_abordados);
    if (g_lista_venta_cabeza != NULL) {
        printf("Quedaron pasajeros en espera (ver Opción 5).\n");
    }
}

void verAbordados() {
    printf("--- LISTA DE PASAJEROS ABORDADOS ---\n");
    if (g_lista_abordados_cabeza == NULL) {
        printf(g_abordaje_iniciado ? "Nadie abordó.\n" : "Abordaje no iniciado.\n");
        return;
    }
    Pasajero* temp = g_lista_abordados_cabeza;
    int i = 1;
    while (temp != NULL) {
        printf("  %d. Apellido: %-20s | Género: %s\n", 
               i, temp->apellido, generoAString(temp->genero));
        temp = temp->siguiente;
        i++;
    }
    printf("Total a bordo: %d\n", i - 1);
}

void verNoAbordados() {
    printf("--- LISTA DE PASAJEROS NO ABORDADOS (ESPERA) ---\n");
    if (g_lista_venta_cabeza == NULL) {
        printf(g_abordaje_iniciado ? "Nadie quedó en espera.\n" : "Abordaje no iniciado. (0 vendidos)\n");
        return;
    }
    Pasajero* temp = g_lista_venta_cabeza;
    int i = 1;
    while (temp != NULL) {
        printf("  %d. Apellido: %-20s | Género: %s\n", 
               i, temp->apellido, generoAString(temp->genero));
        temp = temp->siguiente;
        i++;
    }
    printf("Total en espera: %d\n", i - 1);
}