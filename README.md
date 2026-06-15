# BitQuest: Explorador de Matrices con C y NASM

Proyecto Final de Lenguaje Ensamblador (Cuarto Semestre). 
Este proyecto es un videojuego de laberintos de consola desarrollado usando programación híbrida en C y Ensamblador de 64 bits (NASM). 

## Características Principales (Extras Implementados)
- **4 Niveles Completos** (incluyendo el nivel final "El Abismo")
- **Colores en Consola** para mejorar la experiencia visual
- **Mecánicas Avanzadas**: Llaves, Puertas, Monedas y Trampas ('X')
- **Bloques ASCII Extendidos** (Caracter 219) para renderizar paredes sólidas en vez de '#'
- Funciones críticas (validación de movimiento y conteo) delegadas a rutinas en Ensamblador puro.

## Cómo compilar y jugar

1. Abre la terminal (CMD o PowerShell).
2. Entra a la carpeta del código fuente:
   ```cmd
   cd src
   ```
3. Ejecuta el archivo por lotes (batch) que preparamos para compilar C y NASM al mismo tiempo:
   ```cmd
   .\compilar.bat
   ```
4. Finalmente, para arrancar el juego solo ejecuta el ejecutable generado:
   ```cmd
   .\BitQuest.exe
   ```

*Nota: Asegúrate de tener instalado GCC (MinGW-w64) y NASM, y que ambos estén en las variables de entorno de tu compu.*
