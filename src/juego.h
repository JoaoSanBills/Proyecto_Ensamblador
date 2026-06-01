#ifndef JUEGO_H
#define JUEGO_H

#include <stdbool.h>

//jugador
typedef struct {
    int fila;
    int columna;
    int monedas;
    bool tiene_llave;
    int pasos;
} Jugador;

//funciones del motor
void inicializar_juego(Jugador* jugador);
void bucle_principal(Jugador* jugador);

#endif
