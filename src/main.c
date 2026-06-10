#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

#include "juego.h"  /* bucle_principal + EstadoJuego */
#include "ui.h"     /* Menus, HUD, pantallas de resultado + extern NASM */
#include "mapas.h"  /* mapas_cargar_nivel */

int main(void) {
    /* Activar colores ANSI en la terminal de Windows */
    habilitar_colores_consola();

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
        EstadoJuego estado;
        inicializar_juego(&estado);

        int jugando_campana = 1;
        while (jugando_campana && estado.niveles_completados < 3) {
            mapa_cargar_nivel(&estado, estado.niveles_completados);

            limpiar_pantalla();
            printf(COLOR_AMARILLO COLOR_BOLD "  Preparando nivel: %s...\n" COLOR_RESET, mapa_nombre_nivel(estado.nivel_actual));
            printf("\n");
            printf("  " COLOR_CIAN "Celdas libres en el mapa: " COLOR_BOLD "%d" COLOR_RESET "\n",
                   estado.celdas_libres);
            printf("  " COLOR_AMARILLO "Monedas disponibles:      " COLOR_BOLD "%d" COLOR_RESET "\n",
                   estado.monedas_total);
            printf("\n  Presiona cualquier tecla para iniciar...\n");
            _getch();

            /* Ejecutar el motor de logica para el nivel cargado */
            bucle_principal(&estado);

            /* Si el jugador decidio salir con Q, bucle_principal terminara y nosotros podemos verificar si no llego a la salida */
            // La unica forma oficial de pasar de nivel es que la pos del jugador sea la salida, o bien, validar si llego.
            // En procesar_entrada, cuando pisa 'E', *jugando = false; 
            // Podriamos checar si de verdad esta en la celda de salida:
            char c = mapa_obtener_celda(&estado, estado.pos_jugador.fila, estado.pos_jugador.col);
            if (c == CELDA_SALIDA) {
                /* Nivel completado con exito */
                estado.niveles_completados++;
                
                /* Pantalla de resumen del nivel */
                mostrar_nivel_completado(estado.niveles_completados,
                                         estado.monedas_recogidas,
                                         estado.monedas_total,
                                         estado.pasos);
            } else {
                /* El jugador se rindio (Q) */
                jugando_campana = 0;
            }
        }

        /* Al completar 3 niveles: victoria y resumen final */
        if (estado.niveles_completados >= 3) {
            /* Calculamos el puntaje con la funcion NASM */
            int puntaje = calcular_puntaje(estado.monedas_recogidas_global,
                                           estado.monedas_total_global,
                                           estado.pasos_global,
                                           estado.niveles_completados);

            mostrar_pantalla_victoria();
            mostrar_resumen_final(estado.monedas_recogidas_global,
                                  estado.monedas_total_global,
                                  estado.pasos_global,
                                  estado.niveles_completados,
                                  puntaje);
        }
    }

    return 0;
}
