
#include <stdio.h>   // Para printf, scanf, getchar, EOF, NULL
#include <stdlib.h>  // Para malloc, free, exit, EXIT_SUCCESS, EXIT_FAILURE
#include <stdbool.h> // Para el tipo bool, true, false




 * @brief 
typedef enum {
    FEMENINO,
    MASCULINO,
    NO_BINARIO
} Genero;

/**
 * @brief Estructura del Nodo, que representa a un Pasajero.
 * Contiene los datos del pasajero y el puntero al siguiente nodo.
 */
typedef struct Nodo {
    Genero genero;
    char apellido[50]; 
    
    struct Nodo* siguiente; 
} Pasajero;

// Estado del avión
int g_capacidad_maxima = 0;
int g_limite_sobreventa = 0;
int g_tiquetes_vendidos = 0;
bool g_abordaje_iniciado = false;


// Lista 1: Todos los tiquetes vendidos 
Pasajero* g_lista_venta_cabeza = NULL;
Pasajero* g_lista_venta_cola = NULL;

// Lista 2: Pasajeros que lograron abordar 
Pasajero* g_lista_abordados_cabeza = NULL;
Pasajero* g_lista_abordados_cola = NULL;


// Funciones
void mostrarMenu();
void limpiarBufferEntrada();
const char* generoAString(Genero g);
void liberarMemoria(Pasajero* cabeza);

// Funciones principales del menú
void establecerCapacidad();
void venderTiquete();
void iniciarAbordaje();
void verAbordados();
void verNoAbordados();


int main() {
    int opcion = 0;

    printf("--- SISTEMA DE GESTIÓN DE TIQUETES AÉREOS ---\n");
    
    
    establecerCapacidad();

    // 2. Bucle principal del menú
    do {
        mostrarMenu();
        
        // Validar que la entrada sea un número
        if (scanf("%d", &opcion) != 1) {
            printf("\nError: Entrada no válida. Debe ingresar un número (1-6).\n");
            limpiarBufferEntrada(); // Limpiar la entrada errónea (ej. 'abc')
            opcion = 0; // Resetear opción para evitar salida accidental
            continue;   // Saltar al siguiente ciclo del bucle
        }
        limpiarBufferEntrada(); // Limpiar el '\n' después del número

        switch (opcion) {
            case 1:
                printf("Error: La capacidad solo se puede establecer una vez al inicio.\n");
                break;
            case 2:
                venderTiquete();
                break;
            case 3:
                iniciarAbordaje();
                break;
            case 4:
                verAbordados();
                break;
            case 5:
                verNoAbordados();
                break;
            case 6:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
                break;
        }
        
        if (opcion != 6) {
             printf("\nPresione Enter para continuar...");
            
             getchar(); 
        }

    } while (opcion != 6);

    // 3. Liberar toda la memoria dinámica antes de salir
    printf("Liberando memoria de pasajeros no abordados...\n");
    liberarMemoria(g_lista_venta_cabeza);
    printf("Liberando memoria de pasajeros abordados...\n");
    liberarMemoria(g_lista_abordados_cabeza);

    printf("Programa finalizado limpiamente.\n");
    return EXIT_SUCCESS;
}

// --- IMPLEMENTACIÓN DE FUNCIONES AUXILIARES ---


 * @brief
void mostrarMenu() {
   
    printf("\n--- MENÚ PRINCIPAL ---\n");
    printf("Capacidad: %d | Sobreventa (110%%): %d | Vendidos: %d\n", 
           g_capacidad_maxima, g_limite_sobreventa, g_tiquetes_vendidos);
    
    if (g_abordaje_iniciado) {
        printf("Estado: ABORDAJE INICIADO (Venta cerrada)\n");
    } else {
        printf("Estado: REGISTRO ABIERTO\n");
    }
    printf("--------------------------------------\n");
    printf("1. Establecer Capacidad (Ya realizado)\n");
    printf("2. Vender Tiquete\n");
    printf("3. Iniciar Abordaje\n");
    printf("4. Ver Pasajeros Abordados\n");
    printf("5. Ver Pasajeros No Abordados (en espera)\n");
    printf("6. Salir\n");
    printf("--------------------------------------\n");
    printf("Seleccione una opción: ");
}


 * @brief 
void limpiarBufferEntrada() {
    int c;
   
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Convierte un valor enum Genero a una cadena de texto (const char*).
 * @param g El valor del enum Genero.
 * @return Una cadena de texto descriptiva.
 */
const char* generoAString(Genero g) {
    switch (g) {
        case FEMENINO:   return "Femenino";
        case MASCULINO:  return "Masculino";
        case NO_BINARIO: return "No Binario";
        default:         return "Desconocido";
    }
}


 * @brief 
 * @param cabeza 
void liberarMemoria(Pasajero* cabeza) {
    Pasajero* actual = cabeza;
    Pasajero* siguiente_nodo = NULL;
    int contador = 0;

    while (actual != NULL) {
        siguiente_nodo = actual->siguiente; // 1. Guardar referencia al siguiente
        free(actual);                       // 2. Liberar el nodo actual
        actual = siguiente_nodo;            // 3. Moverse al siguiente
        contador++;
    }
    
    if (contador > 0) {
        printf("Se liberaron %d nodos de la memoria.\n", contador);
    }
}

// --- IMPLEMENTACIÓN DE FUNCIONES DEL MENÚ ---

/**
 * @brief Opción 1: Establece la capacidad inicial del avión y
 * calcula el límite de sobreventa (110%).
 */
void establecerCapacidad() {
    int capacidad = 0;
    do {
        printf("Por favor, ingrese la capacidad máxima de pasajeros del avión (debe ser > 0): ");
        
        if (scanf("%d", &capacidad) != 1) {
            printf("Entrada inválida. Debe ser un número.\n");
            limpiarBufferEntrada();
            continue; // Vuelve al inicio del bucle
        }
        limpiarBufferEntrada();
        
        if (capacidad <= 0) {
            printf("Error: La capacidad debe ser un número positivo.\n");
        }
    } while (capacidad <= 0);

    g_capacidad_maxima = capacidad;
    
   
    int extra = (g_capacidad_maxima * 10) / 100;
    if ((g_capacidad_maxima * 10) % 100 > 0) {
        extra++;
    }
    
    // Si 10% da 0, y la capacidad es positiva, asignamos al menos 1 extra.
    if (extra == 0 && g_capacidad_maxima > 0) {
        extra = 1; 
    }

    g_limite_sobreventa = g_capacidad_maxima + extra;
    
    printf("Capacidad máxima establecida en: %d\n", g_capacidad_maxima);
    printf("Límite de sobreventa (10%% extra) establecido en: %d\n", g_limite_sobreventa);
}

/**
 * @brief Opción 2: Vende un nuevo tiquete.
 * Pide memoria dinámica (malloc) para un nuevo pasajero
 * y lo añade al final de la lista de ventas (cola).
 */
void venderTiquete() {
    // Restricción 1: No vender si el abordaje inició
    if (g_abordaje_iniciado) {
        printf("Error: El proceso de abordaje ya ha comenzado. No se pueden vender más tiquetes.\n");
        return;
    }

    // Restricción 2: No vender si se alcanzó el límite de sobreventa
    if (g_tiquetes_vendidos >= g_limite_sobreventa) {
        printf("Error: Se ha alcanzado el límite máximo de sobreventa (%d tiquetes).\n", g_limite_sobreventa);
        printf("No se pueden vender más tiquetes.\n");
        return;
    }

    
    Pasajero* nuevo_pasajero = (Pasajero*)malloc(sizeof(Pasajero));
    if (nuevo_pasajero == NULL) {
        // ¡Error el sistema no tiene memoria.
        printf("Error fatal: No se pudo asignar memoria (malloc falló).\n");
        printf("El programa se cerrará.\n");
        exit(EXIT_FAILURE); // Salir si no hay memoria
    }

    // 2. Solicitar datos
    printf("--- Nuevo Tiquete (%d/%d) ---\n", g_tiquetes_vendidos + 1, g_limite_sobreventa);
    
    // Género 
    int g_opcion = 0;
    do {
        printf("Seleccione género (1: Femenino, 2: Masculino, 3: No Binario): ");
        if (scanf("%d", &g_opcion) != 1) {
            printf("Entrada inválida. Intente de nuevo.\n");
            g_opcion = 0; // Resetear opción si no es número
            limpiarBufferEntrada();
        } else {
            limpiarBufferEntrada();
            if (g_opcion < 1 || g_opcion > 3) {
                printf("Opción inválida. Debe ser 1, 2 o 3.\n");
            }
        }
    } while (g_opcion < 1 || g_opcion > 3);

    // Asignar género 
    nuevo_pasajero->genero = (Genero)(g_opcion - 1);

    // Apellido
    printf("Ingrese el primer apellido: ");
    scanf("%49s", nuevo_pasajero->apellido);
    limpiarBufferEntrada(); // Limpiar 
    
    nuevo_pasajero->siguiente = NULL;

    // 3. Añadir el nodo a la lista de ventas 
    if (g_lista_venta_cabeza == NULL) {
        // La lista está vacía
        g_lista_venta_cabeza = nuevo_pasajero;
        g_lista_venta_cola = nuevo_pasajero;
    } else {
        // Añadir al final de la lista
        g_lista_venta_cola->siguiente = nuevo_pasajero;
        
        g_lista_venta_cola = nuevo_pasajero; 
    }

    // 4. Actualizar estado
    g_tiquetes_vendidos++;
    printf("\n¡Tiquete vendido exitosamente a %s!\n", nuevo_pasajero->apellido);
    printf("Total vendidos: %d (Límite: %d)\n", g_tiquetes_vendidos, g_limite_sobreventa);
}


 * @brief Opción 3: Inicia el proceso de abordaje.
 * Mueve nodos desde la lista de "ventas" a la lista de "abordados",
 * la capacidad máxima (100%).
/
void iniciarAbordaje() {
    if (g_abordaje_iniciado) {
        printf("Error: El proceso de abordaje ya fue realizado.\n");
        return;
    }

    if (g_lista_venta_cabeza == NULL) {
        printf("No hay tiquetes vendidos. No se puede iniciar el abordaje.\n");
        return;
    }

    printf("--- INICIANDO ABORDAJE ---\n");
    printf("Abordarán los primeros %d pasajeros en orden de compra.\n", g_capacidad_maxima);
    
    // ¡Bloqueo! Esta es la restricción clave.
    g_abordaje_iniciado = true; 
    
    int contador_abordados = 0;

 
    while (g_lista_venta_cabeza != NULL && contador_abordados < g_capacidad_maxima) {
        
        // 1. Tomar el primer pasajero de la lista de venta (FIFO - Dequeue)
        Pasajero* pasajero_a_bordar = g_lista_venta_cabeza;

        // 2. Mover la cabeza de la lista de venta al siguiente
        g_lista_venta_cabeza = g_lista_venta_cabeza->siguiente;

        // 3. Si la lista de venta queda vacía, actualizar también la cola
        if (g_lista_venta_cabeza == NULL) {
            g_lista_venta_cola = NULL;
        }

        // 4. Limpiar el puntero "siguiente" del pasajero que movemos
        // (Buena práctica al mover nodos entre listas)
        pasajero_a_bordar->siguiente = NULL;

        // 5. Añadir el pasajero a la lista de abordados (FIFO - Enqueue)
        if (g_lista_abordados_cabeza == NULL) {
            // Es el primer pasajero en abordar
            g_lista_abordados_cabeza = pasajero_a_bordar;
            g_lista_abordados_cola = pasajero_a_bordar;
        } else {
            // Añadir al final de la lista de abordados
            g_lista_abordados_cola->siguiente = pasajero_a_bordar;
            g_lista_abordados_cola = pasajero_a_bordar; // Actualizar cola
        }

        // 6. Actualizar contador y reportar
        contador_abordados++;
        printf("Abordando (%d/%d): Apellido: %s, Género: %s\n",
               contador_abordados, g_capacidad_maxima, 
               pasajero_a_bordar->apellido, generoAString(pasajero_a_bordar->genero));
    }

    printf("--- ABORDAJE FINALIZADO ---\n");
    printf("Total pasajeros abordados: %d\n", contador_abordados);

   
    if (g_lista_venta_cabeza != NULL) {
        printf("Quedaron pasajeros en lista de espera (no abordados).\n");
        printf("Use la opción 5 para verlos.\n");
    } else if (g_tiquetes_vendidos > contador_abordados) {
         // Este caso no debería ocurrir si la lógica es correcta, pero es un seguro
         printf("Error de lógica: Quedaron pasajeros pero la lista de espera está vacía.\n");
    } else {
        printf("Todos los pasajeros con tiquete lograron abordar.\n");
    }
}

/**
 * @brief 
 */
void verAbordados() {
    printf("--- LISTA DE PASAJEROS ABORDADOS ---\n");

    if (g_lista_abordados_cabeza == NULL) {
        if (g_abordaje_iniciado) {
            printf("Nadie logró abordar el avión.\n");
        } else {
            printf("El proceso de abordaje aún no ha comenzado (Opción 3).\n");
        }
        return;
    }

    Pasajero* temp = g_lista_abordados_cabeza;
    int i = 1;
    while (temp != NULL) {}
        printf("  %d. Apellido: %-20s | Género: %s\n", 
               i, temp->apellido, generoAString(temp->genero));
        temp = temp->siguiente;
        i++;
    }
    printf("--------------------------------------\n");
    printf("Total a bordo: %d\n", i - 1);
}


  @brief 
 
 
void verNoAbordados() {
    printf("--- LISTA DE PASAJEROS NO ABORDADOS (EN ESPERA) ---\n");

    if (g_lista_venta_cabeza == NULL) {
        if (g_abordaje_iniciado) {
            // El abordaje ya ocurrió y la lista de espera quedó vacía
            printf("No quedaron pasajeros en lista de espera.\n");
        } else {
            
            printf("El abordaje no ha iniciado. Esta lista muestra los pasajeros\n");
            printf("que NO lograron abordar DESPUÉS de la Opción 3.\n");
            
            if (g_tiquetes_vendidos > 0) {
                 printf("Actualmente hay %d tiquetes vendidos esperando abordar.\n", g_tiquetes_vendidos);
            } else {
                 printf("No hay tiquetes vendidos.\n");
            }
        }
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
    printf("--------------------------------------\n");
    printf("Total en espera (no abordaron): %d\n", i - 1);
}