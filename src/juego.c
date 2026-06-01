#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>
#include "juego.h"

void procesar_entrada(Jugador* jugador, bool* jugando) {
    if (_kbhit()) {
        char tecla = _getch();
        switch (tecla) {
            case 'w': case 'W':
                jugador->fila--;
                jugador->pasos++;
                break;
            case 's': case 'S':
                jugador->fila++;
                jugador->pasos++;
                break;
            case 'a': case 'A':
                jugador->columna--;
                jugador->pasos++;
                break;
            case 'd': case 'D':
                jugador->columna++;
                jugador->pasos++;
                break;
            case 'q': case 'Q':
                *jugando = false;
                break;
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
            printf("Jugador en -> Fila: %d, Columna: %d | Pasos: %d\n", jugador->fila, jugador->columna, jugador->pasos);
            ultima_fila = jugador->fila;
            ultima_columna = jugador->columna;
        }
        Sleep(10); //para evitar que se consuma aca cabron el CPU
    }
    printf("Saliendo del bucle de juego.\n");
}
