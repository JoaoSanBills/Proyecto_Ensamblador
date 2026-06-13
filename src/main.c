#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>

#include "juego.h"  /* bucle_principal + EstadoJuego */
#include "mapas.h"  /* mapas_cargar_nivel */

void habilitar_colores_consola(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

int main(void) {
    /* Activar colores ANSI en la terminal de Windows */
    habilitar_colores_consola();

    EstadoJuego estado;
    inicializar_juego(&estado);

    int jugando_campana = 1;
    while (jugando_campana && estado.niveles_completados < 3) {
        mapa_cargar_nivel(&estado, estado.niveles_completados);

        /* Ejecutar el motor de logica para el nivel cargado */
        bucle_principal(&estado);

        char c = mapa_obtener_celda(&estado, estado.pos_jugador.fila, estado.pos_jugador.col);
        if (c == CELDA_SALIDA) {
            /* Nivel completado con exito */
            estado.niveles_completados++;
        } else {
            /* El jugador se rindio (Q) */
            jugando_campana = 0;
        }
    }

    system("cls");
    if (estado.niveles_completados >= 3) {
        printf("¡Has completado todos los niveles!\n");
        int puntaje = calcular_puntaje(estado.monedas_recogidas_global,
                                       estado.monedas_total_global,
                                       estado.pasos_global,
                                       estado.niveles_completados);
        printf("Puntaje Final: %d\n", puntaje);
    } else {
        printf("Juego terminado.\n");
    }
    
    printf("Presiona cualquier tecla para salir...\n");
    _getch();

    return 0;
}
