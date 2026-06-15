// ui.c - todo lo de la consola, colores y mensajitos

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include "ui.h"

// ---- helpers de print ----

// linea normalita
static void separador_simple(void) {
    printf(COLOR_CIAN "=================================" COLOR_RESET "\n");
}

// linea mas gruesa pa pantallas importantes
static void separador_doble(void) {
    printf(COLOR_AMARILLO "=================================================" COLOR_RESET "\n");
}

// hacer que el vato presione una tecla pa seguir
static void esperar_tecla(void) {
    printf("\n  " COLOR_BLANCO "Presiona cualquier tecla para continuar..." COLOR_RESET "\n");
    _getch();
}

// ---- Funciones publicas ----

void habilitar_colores_consola(void) {
    // activa los codigos ansi en el cmd de windows o si no salen puros caracteres raros
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void limpiar_pantalla(void) {
    system("cls");
}

// menu principal chido

// dibuja el nombre del juego chido con ascii
// lo pongo aparte para usarlo en todos los menus
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
    int seleccion = 0; // 0=Jugar, 1=Instrucciones, 2=Salir

    while (1) {
        limpiar_pantalla();
        dibujar_titulo();
        separador_simple();
        printf("\n");

        // marcar la opcion en donde esta el jugador con flechitas
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

        // capturar teclado (windows es raro y las flechas mandan 2 bytes de golpe)
        char tecla = _getch();

        if (tecla == 0 || (unsigned char)tecla == 224) {
            // leer el segundo byte de la flechita
            char tecla2 = _getch();
            if (tecla2 == 72)      // flecha arriba
                seleccion = (seleccion - 1 + 3) % 3;
            else if (tecla2 == 80) // flecha abajo
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
            // enter pa confirmar la opcion
            return seleccion + 1;
        }
    }
}

// pantallita de como jugar

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
    printf("  Completa todos los niveles para ganar!\n");
    printf("\n");

    separador_doble();
    esperar_tecla();
}

// pintar el hud en la pantalla

void mostrar_hud(int nivel, int monedas, int total_monedas,
                 int tiene_llave, int pasos) {
    separador_simple();

    // nivel actual
    printf("  " COLOR_CIAN "Nivel: " COLOR_BOLD "%d" COLOR_RESET, nivel);

    // cuanta lana trae
    printf("   " COLOR_AMARILLO "Monedas: " COLOR_BOLD "%d/%d" COLOR_RESET, monedas, total_monedas);

    // ya encontro la llave?
    if (tiene_llave)
        printf("   " COLOR_CIAN "Llave: " COLOR_VERDE COLOR_BOLD "SI" COLOR_RESET);
    else
        printf("   " COLOR_CIAN "Llave: " COLOR_ROJO "No" COLOR_RESET);

    // cuantos pasos dio
    printf("   " COLOR_BLANCO "Pasos: %d" COLOR_RESET "\n", pasos);

    separador_simple();
}

// le mostramos esto cuando pasa el nivel

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

// pantalla epica de victoria

void mostrar_pantalla_victoria(void) {
    limpiar_pantalla();

    printf("\n\n");
    printf(COLOR_AMARILLO COLOR_BOLD);
    printf("  *  *  *  *  *  *  *  *  *  *  *  *  *\n");
    printf("\n");
    printf("        FELICIDADES, GANASTE\n");
    printf("\n");
    printf("  *  *  *  *  *  *  *  *  *  *  *  *  *\n");
    printf(COLOR_RESET "\n");

    printf("  Has completado " COLOR_VERDE COLOR_BOLD "todos los niveles" COLOR_RESET " de BitQuest.\n");
    printf("  Eres un verdadero " COLOR_CIAN "Explorador de Matrices" COLOR_RESET "!\n");
    printf("\n");

    esperar_tecla();
}

// el resumen perron del final

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

    // pintar el puntaje dependiendo de que tan cabron lo hizo
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
