/* =========================================================
 * ui.c - Implementacion de UI/UX para BitQuest
 * Rol 4: Sistema de menus, HUD y pantallas de resultado
 * ========================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include "ui.h"

/* ---- Helpers internos ---- */

/* Imprime una linea decorativa de separacion */
static void separador_simple(void) {
    printf(COLOR_CIAN "=================================" COLOR_RESET "\n");
}

/* Imprime una linea doble decorativa para pantallas importantes */
static void separador_doble(void) {
    printf(COLOR_AMARILLO "=================================================" COLOR_RESET "\n");
}

/* Espera que el usuario presione cualquier tecla */
static void esperar_tecla(void) {
    printf("\n  " COLOR_BLANCO "Presiona cualquier tecla para continuar..." COLOR_RESET "\n");
    _getch();
}

/* ---- Funciones publicas ---- */

void habilitar_colores_consola(void) {
    /* Activa el procesamiento de secuencias VT100 en Windows 10+ */
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void limpiar_pantalla(void) {
    system("cls");
}

/* ---- Menu de inicio ---- */

/*
 * Dibuja el titulo ASCII art del juego en amarillo.
 * Separado en funcion propia para reutilizarlo en varias pantallas.
 */
static void dibujar_titulo(void) {
    printf(COLOR_AMARILLO COLOR_BOLD);
    printf("  +-+-+-+-+-+-+-+-+\n");
    printf("  |B|i|t|Q|u|e|s|t|\n");
    printf("  +-+-+-+-+-+-+-+-+\n");
    printf(COLOR_RESET);
    printf(COLOR_CIAN "  Explorador de Matrices con C y NASM\n" COLOR_RESET);
    printf("\n");
}

int mostrar_menu_inicio(void) {
    int seleccion = 0; /* 0=Jugar, 1=Instrucciones, 2=Salir */

    while (1) {
        limpiar_pantalla();
        dibujar_titulo();
        separador_simple();
        printf("\n");

        /* Resalta la opcion seleccionada con flecha y color */
        if (seleccion == 0)
            printf(COLOR_VERDE COLOR_BOLD " >> " COLOR_RESET COLOR_VERDE);
        else
            printf("    ");
        printf("[ 1 ]  Jugar" COLOR_RESET "\n");

        if (seleccion == 1)
            printf(COLOR_CIAN COLOR_BOLD " >> " COLOR_RESET COLOR_CIAN);
        else
            printf("    ");
        printf("[ 2 ]  Instrucciones" COLOR_RESET "\n");

        if (seleccion == 2)
            printf(COLOR_ROJO COLOR_BOLD " >> " COLOR_RESET COLOR_ROJO);
        else
            printf("    ");
        printf("[ 3 ]  Salir" COLOR_RESET "\n");

        printf("\n");
        separador_simple();
        printf("  " COLOR_BLANCO "W/S o flechas para navegar  |  Enter o numero para elegir" COLOR_RESET "\n");

        /* Lectura de tecla (en Windows las flechas envian 2 bytes: 0xE0 + codigo) */
        char tecla = _getch();

        if (tecla == 0 || (unsigned char)tecla == 224) {
            /* Segunda parte del codigo de flecha */
            char tecla2 = _getch();
            if (tecla2 == 72)      /* flecha arriba */
                seleccion = (seleccion - 1 + 3) % 3;
            else if (tecla2 == 80) /* flecha abajo */
                seleccion = (seleccion + 1) % 3;

        } else if (tecla == 'w' || tecla == 'W') {
            seleccion = (seleccion - 1 + 3) % 3;

        } else if (tecla == 's' || tecla == 'S') {
            seleccion = (seleccion + 1) % 3;

        } else if (tecla == '1') {
            return MENU_JUGAR;

        } else if (tecla == '2') {
            return MENU_INSTRUCCIONES;

        } else if (tecla == '3' || tecla == 'q' || tecla == 'Q') {
            return MENU_SALIR;

        } else if (tecla == '\r' || tecla == '\n') {
            /* Enter confirma la seleccion actual */
            return seleccion + 1;
        }
    }
}

/* ---- Pantalla de instrucciones ---- */

void mostrar_instrucciones(void) {
    limpiar_pantalla();
    dibujar_titulo();
    separador_doble();

    printf("\n");
    printf(COLOR_AMARILLO COLOR_BOLD "  CONTROLES\n" COLOR_RESET);
    printf("  " COLOR_CIAN "W" COLOR_RESET " o flecha arriba  ->  Mover hacia arriba\n");
    printf("  " COLOR_CIAN "A" COLOR_RESET " o flecha izq.   ->  Mover hacia la izquierda\n");
    printf("  " COLOR_CIAN "S" COLOR_RESET " o flecha abajo  ->  Mover hacia abajo\n");
    printf("  " COLOR_CIAN "D" COLOR_RESET " o flecha der.   ->  Mover hacia la derecha\n");
    printf("  " COLOR_ROJO "Q" COLOR_RESET "                 ->  Salir del juego\n");
    printf("\n");

    separador_simple();
    printf("\n");
    printf(COLOR_AMARILLO COLOR_BOLD "  SIMBOLOS DEL MAPA\n" COLOR_RESET);
    printf("  " COLOR_BLANCO "#" COLOR_RESET "  Pared        - No puedes atravesarla\n");
    printf("  " COLOR_BLANCO "." COLOR_RESET "  Camino libre - Puedes caminar aqui\n");
    printf("  " COLOR_VERDE "P" COLOR_RESET "  Jugador      - Tu posicion actual\n");
    printf("  " COLOR_AMARILLO "M" COLOR_RESET "  Moneda       - Recogela para sumar puntos\n");
    printf("  " COLOR_CIAN "K" COLOR_RESET "  Llave        - Necesaria para abrir la puerta\n");
    printf("  " COLOR_MAGENTA "D" COLOR_RESET "  Puerta       - Solo abre si tienes la llave\n");
    printf("  " COLOR_ROJO "E" COLOR_RESET "  Salida       - Llega aqui para completar el nivel\n");
    printf("\n");

    separador_simple();
    printf("\n");
    printf(COLOR_AMARILLO COLOR_BOLD "  OBJETIVO\n" COLOR_RESET);
    printf("  Recorre el laberinto, recoge la " COLOR_CIAN "llave" COLOR_RESET ",\n");
    printf("  abre la " COLOR_MAGENTA "puerta" COLOR_RESET " y llega a la " COLOR_ROJO "salida" COLOR_RESET ".\n");
    printf("  Recolecta todas las " COLOR_AMARILLO "monedas" COLOR_RESET " que puedas.\n");
    printf("  Completa los 3 niveles para ganar!\n");
    printf("\n");

    separador_doble();
    esperar_tecla();
}

/* ---- HUD en tiempo real ---- */

void mostrar_hud(int nivel, int monedas, int total_monedas,
                 int tiene_llave, int pasos) {
    separador_simple();

    /* Nivel */
    printf("  " COLOR_CIAN "Nivel: " COLOR_BOLD "%d" COLOR_RESET, nivel);

    /* Monedas */
    printf("   " COLOR_AMARILLO "Monedas: " COLOR_BOLD "%d/%d" COLOR_RESET, monedas, total_monedas);

    /* Estado de la llave */
    if (tiene_llave)
        printf("   " COLOR_CIAN "Llave: " COLOR_VERDE COLOR_BOLD "SI" COLOR_RESET);
    else
        printf("   " COLOR_CIAN "Llave: " COLOR_ROJO "No" COLOR_RESET);

    /* Pasos */
    printf("   " COLOR_BLANCO "Pasos: %d" COLOR_RESET "\n", pasos);

    separador_simple();
}

/* ---- Pantalla de nivel completado ---- */

void mostrar_nivel_completado(int nivel, int monedas,
                              int total_monedas, int pasos) {
    limpiar_pantalla();

    printf("\n\n");
    separador_doble();
    printf("\n");
    printf(COLOR_VERDE COLOR_BOLD "  *** NIVEL %d COMPLETADO ***\n" COLOR_RESET, nivel);
    printf("\n");
    printf("  " COLOR_AMARILLO "Monedas recolectadas: " COLOR_BOLD "%d / %d" COLOR_RESET "\n",
           monedas, total_monedas);
    printf("  " COLOR_BLANCO "Pasos realizados:      " COLOR_BOLD "%d" COLOR_RESET "\n", pasos);
    printf("\n");
    separador_doble();

    esperar_tecla();
}

/* ---- Pantalla de victoria ---- */

void mostrar_pantalla_victoria(void) {
    limpiar_pantalla();

    printf("\n\n");
    printf(COLOR_AMARILLO COLOR_BOLD);
    printf("  *  *  *  *  *  *  *  *  *  *  *  *  *\n");
    printf("\n");
    printf("        FELICIDADES, GANASTE!\n");
    printf("\n");
    printf("  *  *  *  *  *  *  *  *  *  *  *  *  *\n");
    printf(COLOR_RESET "\n");

    printf("  Has completado los " COLOR_VERDE COLOR_BOLD "3 niveles" COLOR_RESET " de BitQuest.\n");
    printf("  Eres un verdadero " COLOR_CIAN "Explorador de Matrices" COLOR_RESET "!\n");
    printf("\n");

    esperar_tecla();
}

/* ---- Resumen final del juego ---- */

void mostrar_resumen_final(int monedas_total, int total_monedas_global,
                           int pasos_total, int niveles_completados,
                           int puntaje) {
    limpiar_pantalla();

    printf("\n");
    separador_doble();
    printf("\n");
    printf(COLOR_VERDE COLOR_BOLD "  RESUMEN FINAL - BITQUEST\n" COLOR_RESET);
    printf("\n");

    printf("  " COLOR_CIAN "Juego completado\n" COLOR_RESET);
    printf("\n");
    printf("  " COLOR_AMARILLO "Monedas totales recolectadas: " COLOR_BOLD "%d / %d" COLOR_RESET "\n",
           monedas_total, total_monedas_global);
    printf("  " COLOR_BLANCO "Pasos totales:                " COLOR_BOLD "%d" COLOR_RESET "\n",
           pasos_total);
    printf("  " COLOR_VERDE "Niveles completados:          " COLOR_BOLD "%d" COLOR_RESET "\n",
           niveles_completados);

    printf("\n");
    separador_simple();
    printf("\n");

    /* Puntaje con color segun el valor */
    printf("  " COLOR_AMARILLO COLOR_BOLD "Puntaje final: ");
    if (puntaje >= 3000)
        printf(COLOR_VERDE COLOR_BOLD "%d" COLOR_RESET "\n", puntaje);
    else if (puntaje >= 1500)
        printf(COLOR_AMARILLO COLOR_BOLD "%d" COLOR_RESET "\n", puntaje);
    else
        printf(COLOR_BLANCO COLOR_BOLD "%d" COLOR_RESET "\n", puntaje);

    printf("\n");
    separador_doble();

    printf("\n  " COLOR_CIAN "Gracias por jugar BitQuest!" COLOR_RESET "\n\n");
    esperar_tecla();
}
