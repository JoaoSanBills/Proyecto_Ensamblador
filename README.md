# BitQuest: Explorador de Matrices con C y NASM

Videojuego de laberinto en consola desarrollado con programacion hibrida C + NASM de 64 bits.

## Requisitos

- NASM (ensamblador de 64 bits)
- GCC (compilador de C de 64 bits, por ejemplo MinGW-w64)
- Windows 10 o superior (para soporte de colores ANSI en consola)

## Como compilar y ejecutar

### Windows (recomendado)

1. Abre una terminal (CMD o PowerShell) en la carpeta `src/`.
2. Ejecuta el script de compilacion:

```
cd src
..\compilar.bat
```

O bien compila manualmente paso a paso:

```
nasm -f win64 bitquest.asm -o bitquest.obj
gcc main.c juego.c ui.c bitquest.obj -o BitQuest.exe
BitQuest.exe
```

### Linux

```
nasm -f elf64 bitquest.asm -o bitquest.o
gcc main.c juego.c ui.c bitquest.o -o BitQuest
./BitQuest
```

## Estructura del proyecto

```
Proyecto_Ensamblador/
├── src/
│   ├── main.c        - Funcion principal y control del flujo del juego
│   ├── juego.c       - Motor de logica: movimiento, colisiones, niveles
│   ├── juego.h       - Prototipos y struct Jugador
│   ├── ui.c          - Menus, HUD y pantallas de resultado (Rol 4)
│   ├── ui.h          - Prototipos de funciones de UI
│   ├── mapas.h       - Declaracion de los 3 mapas de 60x60
│   └── bitquest.asm  - Funciones obligatorias en NASM de 64 bits
├── compilar.bat      - Script de compilacion para Windows
└── README.md         - Este archivo
```

## Funciones NASM implementadas

| Funcion              | Descripcion                                       |
|----------------------|---------------------------------------------------|
| `contar_caracteres`  | Cuenta cuantas veces aparece un caracter en el mapa |
| `validar_movimiento` | Valida si una posicion puede ser ocupada por el jugador |
| `calcular_puntaje`   | Calcula el puntaje final con formula definida       |
| `detectar_objeto`    | Detecta si hay un objeto especifico en una celda    |
| `contar_celdas_libres` | Cuenta las celdas de camino libre en el mapa     |

## Controles

| Tecla | Accion                  |
|-------|-------------------------|
| W     | Mover hacia arriba      |
| A     | Mover hacia la izquierda|
| S     | Mover hacia abajo       |
| D     | Mover hacia la derecha  |
| Q     | Salir del juego         |

## Elementos del mapa

| Simbolo | Significado                              |
|---------|------------------------------------------|
| `#`     | Pared (no se puede atravesar)            |
| `.`     | Camino libre                             |
| `P`     | Jugador                                  |
| `M`     | Moneda (suma puntos al recolectarla)     |
| `K`     | Llave (permite abrir la puerta)          |
| `D`     | Puerta (solo abre con la llave)          |
| `E`     | Salida (termina el nivel)                |

## Equipo

- Rol 1 - Motor de Logica y Fisicas (C)
- Rol 2 - Especialista en Ensamblador (NASM)
- Rol 3 - Arquitecto de Niveles (C y Datos)
- Rol 4 - Desarrollador de UI/UX y Gestor de Entrega (C) - Joshua
