#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>
#include "juego.h"

// firma de la funcion en NASM (hecha por Joao)
extern int movimiento_valido(char* mapa, int columnas, int nueva_fila, int nueva_columna);

// Mapa dummy de 5x5 para probar colisiones antes de tener el mapa oficial
char mapa_prueba[25] = {
    '#', '#', '#', '#', '#',
    '#', '.', 'M', '.', '#',
    '#', '.', '#', 'K', '#',
    '#', '.', 'D', 'E', '#',
    '#', '#', '#', '#', '#'
};

void procesar_entrada(Jugador* jugador, bool* jugando) {
    if (_kbhit()) {
        char tecla = _getch();
        int nueva_fila = jugador->fila;
        int nueva_columna = jugador->columna;

        switch (tecla) {
            case 'w': case 'W': nueva_fila--; break;
            case 's': case 'S': nueva_fila++; break;
            case 'a': case 'A': nueva_columna--; break;
            case 'd': case 'D': nueva_columna++; break;
            case 'q': case 'Q': 
                *jugando = false; 
                return;
            default:
                return; // Ignorar cualquier otra tecla
        }

        // Llamamos a la funcion de NASM hecha por Joao para validar el movimiento en nuestro mapa dummy de 5x5
        if (movimiento_valido(mapa_prueba, 5, nueva_fila, nueva_columna) == 1) {
            int indice = nueva_fila * 5 + nueva_columna;
            char objeto = mapa_prueba[indice];
            //si es puerta pero no trae la llave, lo ponemos como si fuera pared, no se registra movimiento
            if (objeto == 'D' && !jugador->tiene_llave) {
                return;
            }
            //interacciones
            if (objeto == 'M') {
                jugador->monedas++;
                mapa_prueba[indice] = '.'; //registra que se recoge la moneda
            } else if (objeto == 'K') {
                jugador->tiene_llave = true;
                mapa_prueba[indice] = '.'; //registra que se recoge la llave
            } else if (objeto == 'D' && jugador->tiene_llave) { //si tiene la llave y es la puerta
                mapa_prueba[indice] = '.'; //registra que se abre la puerta
            } else if (objeto == 'E') { //llega a la salida
                printf("\nLlegaste a la salida. Nivel terminado.\n");
                *jugando = false; //cambia el estado
            }
            //registra movimientos
            jugador->fila = nueva_fila;
            jugador->columna = nueva_columna;
            jugador->pasos++;
        }
    }
}

void inicializar_juego(Jugador* jugador) {
    jugador->fila = 1;  //esta posicion inicial es asi de que x ahorita
    jugador->columna = 1;
    jugador->monedas = 0;
    jugador->tiene_llave = false;
    jugador->pasos = 0;
}

void bucle_principal(Jugador* jugador) {
    bool jugando = true;
    int ultima_fila = -1;
    int ultima_columna = -1;
    printf("Inicia motor de logica y fisicas\n");
    printf("Presiona W A S D para moverte, o Q para salir.\n");
    while(jugando) {
        procesar_entrada(jugador, &jugando);
        if (jugador->fila != ultima_fila || jugador->columna != ultima_columna) {
            printf("Jugador en -> Fila: %d, Columna: %d | Pasos: %d | Monedas: %d | Llave: %s\n", 
                jugador->fila, jugador->columna, jugador->pasos, jugador->monedas, 
                jugador->tiene_llave ? "Si" : "No");
            ultima_fila = jugador->fila;
            ultima_columna = jugador->columna;
        }
        Sleep(10); //para evitar que se consuma aca cabron el CPU
    }
    printf("Saliendo del bucle de juego.\n");
}
