/* =========================================================
 * main.c - Punto de entrada de BitQuest
 * Rol 4: Integra el menu de UI con el motor de juego de Leo/Joao
 *
 * Flujo:
 *   1. Habilitar colores ANSI en consola
 *   2. Mostrar menu principal
 *   3. Jugar (bucle_principal de Leo/Joao)
 *   4. Mostrar resumen del nivel (con datos del struct Jugador)
 *   5. Mostrar resumen final con puntaje calculado en NASM
 * ========================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

#include "juego.h"  /* Struct Jugador + inicializar_juego + bucle_principal */
#include "ui.h"     /* Menus, HUD, pantallas de resultado + extern NASM */

/* ---------------------------------------------------------
 * mapa_prueba esta definido en juego.c (Leo).
 * Cuando Rodrigo integre los mapas reales de 60x60 esto
 * se reemplazara por los mapas oficiales.
 * --------------------------------------------------------- */
extern char mapa_prueba[];

/* Tamanio del mapa de prueba actual (5x5 = 25 celdas) */
#define MAPA_CELDAS_PRUEBA 25

int main(void) {
    /* Activar colores ANSI en la terminal de Windows */
    habilitar_colores_consola();

    /* Contadores globales para el resumen final */
    int monedas_acumuladas   = 0;
    int total_monedas_global = 0;
    int pasos_acumulados     = 0;
    int niveles_completados  = 0;

    /*
     * Calculamos las celdas libres al inicio con la funcion NASM
     * (Funcion obligatoria 5: contar_celdas_libres).
     * Se muestra antes de que el jugador empiece.
     */
    int celdas_libres = contar_celdas_libres(mapa_prueba, MAPA_CELDAS_PRUEBA);

    /* ---- Bucle del menu principal ---- */
    int opcion;
    while (1) {
        opcion = mostrar_menu_inicio();

        /* Salir del juego */
        if (opcion == MENU_SALIR) {
            limpiar_pantalla();
            printf(COLOR_CIAN "  Hasta luego, explorador!\n" COLOR_RESET);
            break;
        }

        /* Mostrar instrucciones y volver al menu */
        if (opcion == MENU_INSTRUCCIONES) {
            mostrar_instrucciones();
            continue;
        }

        /* ---- Opcion: Jugar ---- */

        /*
         * Contamos las monedas del nivel usando la funcion NASM
         * (Funcion obligatoria 1: contar_caracteres).
         * Esto cumple el requisito de NO hardcodear el total de monedas.
         */
        int total_monedas_nivel = contar_caracteres(mapa_prueba,
                                                    MAPA_CELDAS_PRUEBA, 'M');

        limpiar_pantalla();
        printf(COLOR_AMARILLO COLOR_BOLD "  Preparando nivel...\n" COLOR_RESET);
        printf("\n");
        printf("  " COLOR_CIAN "Celdas libres en el mapa: " COLOR_BOLD "%d" COLOR_RESET "\n",
               celdas_libres);
        printf("  " COLOR_AMARILLO "Monedas disponibles:      " COLOR_BOLD "%d" COLOR_RESET "\n",
               total_monedas_nivel);
        printf("\n  Presiona cualquier tecla para iniciar...\n");
        _getch();

        /* Inicializamos al jugador en su posicion de inicio */
        Jugador jugador;
        inicializar_juego(&jugador);

        /*
         * bucle_principal es el motor de logica de Leo.
         * Se encarga de movimiento, colisiones, recoleccion de
         * objetos y termina cuando el jugador llega a la salida (E)
         * o presiona Q.
         */
        bucle_principal(&jugador);

        /* ---- Nivel terminado: acumulamos estadisticas ---- */
        niveles_completados++;
        monedas_acumuladas   += jugador.monedas;
        total_monedas_global += total_monedas_nivel;
        pasos_acumulados     += jugador.pasos;

        /* Pantalla de resumen del nivel */
        mostrar_nivel_completado(niveles_completados,
                                 jugador.monedas,
                                 total_monedas_nivel,
                                 jugador.pasos);

        /*
         * Calculamos el puntaje con la funcion NASM
         * (Funcion obligatoria 3: calcular_puntaje).
         * Formula: (monedas * 100) - (pasos * 2) + (niveles * 500)
         */
        long long puntaje = calcular_puntaje((long long)monedas_acumuladas,
                                             (long long)pasos_acumulados,
                                             (long long)niveles_completados);

        /* Al completar 3 niveles: victoria y resumen final */
        if (niveles_completados >= 3) {
            mostrar_pantalla_victoria();
            mostrar_resumen_final(monedas_acumuladas,
                                  total_monedas_global,
                                  pasos_acumulados,
                                  niveles_completados,
                                  (int)puntaje);
            break;
        }

        /*
         * Mientras no lleguen a 3 niveles, volvemos al menu.
         * (Rodrigo integrara los mapas reales mas adelante)
         */
    }

    return 0;
}
