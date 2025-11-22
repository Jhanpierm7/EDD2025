# Árbol Binario de Búsqueda (ABB)
Este programa implementa un sistema para registrar pasajeros utilizando un 
Árbol Binario de Búsqueda (ABB). Cada pasajero se almacena mediante su 
documento como clave, junto con su destino y tipo de pasaje.

# El sistema permite:
- Registrar pasajeros  
- Mostrar pasajeros (inorden, preorden, postorden)  
- Contar pasajeros  
- Eliminar un pasajero  
- Mostrar el árbol estructurado  
- Salir del sistema  

## 🧱 Estructura del Nodo
Cada pasajero se guarda como un nodo del ABB:
- **documento** (clave de búsqueda)
- **destino** (Timbiquí, Juanchaco, Tumaco, etc.)
- **tipo_pasaje**  
  - 0 → solo se va
  - 1 → se va y regreso
- **izq / der** → hijos del ABB

## ⚙ Funciones principales

### `crearNodo()`
Crea un nodo nuevo asignando memoria y llenando sus datos.

### `insertar()`
Inserta un nuevo pasajero en el ABB según el documento.  
Coloca el dato en la izquierda si es menor, o derecha si es mayor.

### `inorden()`
Recorre el árbol en orden ascendente: