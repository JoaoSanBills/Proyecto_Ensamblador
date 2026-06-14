#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>
#include "juego.h"
#include "ui.h"

static void imprimir_ventana_mapa(const EstadoJuego *estado) {
    /* Calcular esquina superior izquierda de la ventana (centrada) */
    int inicio_f = estado->pos_jugador.fila - (VIS_ROWS / 2);
    int inicio_c = estado->pos_jugador.col  - (VIS_COLS / 2);

    /* Clamping (Asegurar limites del mapa) */
    if (inicio_f < 0) inicio_f = 0;
    if (inicio_c < 0) inicio_c = 0;
    
    if (inicio_f + VIS_ROWS > MAP_ROWS) {
        inicio_f = MAP_ROWS - VIS_ROWS;
    }
    if (inicio_c + VIS_COLS > MAP_COLS) {
        inicio_c = MAP_COLS - VIS_COLS;
    }

    printf("\n");
    for (int f = 0; f < VIS_ROWS; f++) {
        printf("    "); /* Margen izquierdo */
        for (int c = 0; c < VIS_COLS; c++) {
            int map_f = inicio_f + f;
            int map_c = inicio_c + c;

            /* Si estamos en la posicion exacta del jugador, imprimir la P */
            if (map_f == estado->pos_jugador.fila && map_c == estado->pos_jugador.col) {
                printf(COLOR_VERDE COLOR_BOLD "P" COLOR_RESET " ");
            } else {
                char celda = mapa_obtener_celda(estado, map_f, map_c);
                switch (celda) {
                    case CELDA_PARED:  printf(COLOR_BLANCO "%c%c" COLOR_RESET, 219, 219); break;
                    case CELDA_SUELO:  printf("." " "); break;
                    case CELDA_MONEDA: printf(COLOR_AMARILLO "M" COLOR_RESET " "); break;
                    case CELDA_LLAVE:  printf(COLOR_CIAN "K" COLOR_RESET " "); break;
                    case CELDA_PUERTA: printf(COLOR_MAGENTA "D" COLOR_RESET " "); break;
                    case CELDA_SALIDA: printf(COLOR_ROJO "E" COLOR_RESET " "); break;
                    case CELDA_TRAMPA: printf(COLOR_ROJO "X" COLOR_RESET " "); break;
                    case CELDA_SPAWN:  printf("." " "); break; /* P real ya se pinto */
                    default:           printf("%c ", celda); break;
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

void procesar_entrada(EstadoJuego* estado, bool* jugando) {
    if (_kbhit()) {
        unsigned char tecla = (unsigned char)_getch();
        int nueva_fila = estado->pos_jugador.fila;
        int nueva_columna = estado->pos_jugador.col;

        switch (tecla) {
            case 'w': case 'W': 
            case 72: // flecha arriba
                nueva_fila--; break;
            case 's': case 'S': 
            case 80: // flecha abajo
                nueva_fila++; break;
            case 'a': case 'A': 
            case 75: // flecha izquierda
                nueva_columna--; break;
            case 'd': case 'D': 
            case 77: // flecha derecha
                nueva_columna++; break;
            case 'q': case 'Q': 
                *jugando = false; 
                return;
            case 224: // prefijo de flechas especiales
                return; 
            default:
                return; // Ignorar cualquier otra tecla
        }

        // Llamamos a la funcion de NASM de Joao para validar el movimiento (paredes)
        if (validar_movimiento(estado->celdas, MAP_COLS, nueva_fila, nueva_columna) == 1) {
            
            // Detectar objetos usando las funciones de NASM de Joao
            // Puerta: detectar_objeto(..., 'D')
            if (detectar_objeto(estado->celdas, MAP_COLS, nueva_fila, nueva_columna, CELDA_PUERTA)) {
                if (!estado->tiene_llave) {
                    return; // Bloqueado
                } else {
                    mapa_establecer_celda(estado, nueva_fila, nueva_columna, CELDA_SUELO);
                }
            }

            // Moneda
            if (detectar_objeto(estado->celdas, MAP_COLS, nueva_fila, nueva_columna, CELDA_MONEDA)) {
                estado->monedas_recogidas++;
                estado->monedas_recogidas_global++;
                mapa_establecer_celda(estado, nueva_fila, nueva_columna, CELDA_SUELO);
            }

            // Llave
            if (detectar_objeto(estado->celdas, MAP_COLS, nueva_fila, nueva_columna, CELDA_LLAVE)) {
                estado->tiene_llave = 1;
                mapa_establecer_celda(estado, nueva_fila, nueva_columna, CELDA_SUELO);
            }

            // Trampa (Morir y reiniciar campana)
            if (detectar_objeto(estado->celdas, MAP_COLS, nueva_fila, nueva_columna, CELDA_TRAMPA)) {
                limpiar_pantalla();
                printf("\n\n" COLOR_ROJO "  *** CAISTE EN UNA TRAMPA ('X') ***\n" COLOR_RESET);
                printf("  El suelo se derrumba bajo tus pies...\n");
                printf("\n  Presiona cualquier tecla para volver al menu...\n");
                Sleep(500);
                while(_kbhit()) _getch(); // limpiar buffer
                _getch();
                *jugando = false; 
                // Actualiza pos para que la celda actual sea X y main.c sepa que perdio
                estado->pos_jugador.fila = nueva_fila;
                estado->pos_jugador.col = nueva_columna;
                return;
            }

            // Salida
            if (detectar_objeto(estado->celdas, MAP_COLS, nueva_fila, nueva_columna, CELDA_SALIDA)) {
                *jugando = false; // Nivel completado
            }

            // Actualiza posicion del jugador
            estado->pos_jugador.fila = nueva_fila;
            estado->pos_jugador.col = nueva_columna;
            estado->pasos++;
            estado->pasos_global++;
        }
    }
}

void inicializar_juego(EstadoJuego* estado) {
    // La inicialización global de estado ya se maneja en main.c
    // Esta función limpia acumulados si empezamos juego nuevo
    estado->monedas_total_global = 0;
    estado->monedas_recogidas_global = 0;
    estado->pasos_global = 0;
    estado->niveles_completados = 0;
}

void bucle_principal(EstadoJuego* estado) {
    bool jugando = true;
    int ultima_fila = -1;
    int ultima_columna = -1;
    int ultimas_monedas = -1;

    limpiar_pantalla();
    mostrar_hud(estado->nivel_actual + 1, estado->monedas_recogidas, estado->monedas_total, estado->tiene_llave, estado->pasos);
    imprimir_ventana_mapa(estado);

    while(jugando) {
        procesar_entrada(estado, &jugando);
        if (estado->pos_jugador.fila != ultima_fila || estado->pos_jugador.col != ultima_columna || estado->monedas_recogidas != ultimas_monedas) {
            
            limpiar_pantalla();
            mostrar_hud(estado->nivel_actual + 1, estado->monedas_recogidas, estado->monedas_total, estado->tiene_llave, estado->pasos);
            imprimir_ventana_mapa(estado);

            ultima_fila = estado->pos_jugador.fila;
            ultima_columna = estado->pos_jugador.col;
            ultimas_monedas = estado->monedas_recogidas;
        }
        Sleep(10); //para evitar que se consuma aca cabron el CPU
    }
}
