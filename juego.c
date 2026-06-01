#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "juego.h"

void inicializar_juego(Jugador* jugador) {
    jugador->fila = 1;  //esta posicion inicial es asi de que x ahorita
    jugador->columna = 1;
    jugador->monedas = 0;
    jugador->tiene_llave = false;
    jugador->pasos = 0;
}

void bucle_principal(Jugador* jugador) {
    bool jugando = true;
    
    printf("Iniciando Motor de Logica y Fisicas...\n");
    
    while(jugando) {
        //lectura del teclado
        //simulamos que se rompe al teclear algo
        printf("Bucle de juego corriendo. Presiona Enter para salir de la prueba...\n");
        getchar();
        jugando = false; 
    }
    
    printf("Saliendo del bucle de juego.\n");
}
