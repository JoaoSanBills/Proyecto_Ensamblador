#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

#include "juego.h"  /* bucle_principal + EstadoJuego */
#include "ui.h"     /* Menus, HUD, pantallas de resultado + extern NASM */
#include "mapas.h"  /* mapas_cargar_nivel */

int main(void) {
    // activamos los colores en la consola
    habilitar_colores_consola();

    // loop infinito del menu principal wey
    int opcion;
    while (1) {
        opcion = mostrar_menu_inicio();

        // checar si el usuario le dio a salir
        if (opcion == MENU_SALIR) {
            limpiar_pantalla();
            printf(COLOR_CIAN "  Hasta luego, nos vemos pronto\n" COLOR_RESET);
            break;
        }

        // mostrar el manual de instrucciones y regresar
        if (opcion == MENU_INSTRUCCIONES) {
            mostrar_instrucciones();
            continue;
        }

        // opcion 1: jugar la campana completa
        EstadoJuego estado;
        inicializar_juego(&estado);

        int jugando_campana = 1;
        while (jugando_campana && estado.niveles_completados < NUM_LEVELS) {
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

            // mandamos llamar al loop del juego de joao
            bucle_principal(&estado);

            // si aprieta Q o pisa trampa el bucle termina, aca checamos si de verdad llego a la salida
            // para no darle el gane si se murio o se salio
            char c = mapa_obtener_celda(&estado, estado.pos_jugador.fila, estado.pos_jugador.col);
            if (c == CELDA_SALIDA) {
                // paso el nivel con exito
                estado.niveles_completados++;
                
                // mostrar la pantallita de stats del nivel
                mostrar_nivel_completado(estado.niveles_completados,
                                         estado.monedas_recogidas,
                                         estado.monedas_total,
                                         estado.pasos);
            } else {
                // el wey se rindio o cayo en trampa
                jugando_campana = 0;
            }
        }

        // si ya se rifo todos los niveles le mostramos la pantalla final
        if (estado.niveles_completados >= NUM_LEVELS) {
            // usar la funcion de nasm para calcular los puntos finales
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
