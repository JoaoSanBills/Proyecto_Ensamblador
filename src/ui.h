#ifndef UI_H
#define UI_H

/* =========================================================
 * ui.h - Interfaz de UI/UX para BitQuest
 * Rol 4: Sistema de menus, HUD y pantallas de resultado
 * ========================================================= */

/* Codigos de escape ANSI (requiere habilitar_colores_consola()) */
#define COLOR_RESET    "\033[0m"
#define COLOR_BOLD     "\033[1m"
#define COLOR_ROJO     "\033[31m"
#define COLOR_VERDE    "\033[32m"
#define COLOR_AMARILLO "\033[33m"
#define COLOR_AZUL     "\033[34m"
#define COLOR_MAGENTA  "\033[35m"
#define COLOR_CIAN     "\033[36m"
#define COLOR_BLANCO   "\033[37m"

/* Opciones del menu principal */
#define MENU_JUGAR         1
#define MENU_INSTRUCCIONES 2
#define MENU_SALIR         3

/* =========================================================
 * Declaraciones de funciones NASM
 * Las primeras 2 vienen de Leo (Rol 2), las 3 restantes
 * vienen de nuestro bitquest.asm (Rol 4).
 * Se declaran aqui para que main.c tenga un solo include.
 * ========================================================= */

/* Las funciones NASM ahora se importan a traves de mapas.h */
#include "mapas.h"

/*
 * habilitar_colores_consola
 * Activa el modo VT100 en la terminal de Windows para que
 * los codigos ANSI de color funcionen correctamente.
 */
void habilitar_colores_consola(void);

/*
 * limpiar_pantalla
 * Limpia la consola usando el comando del sistema.
 */
void limpiar_pantalla(void);

/*
 * mostrar_menu_inicio
 * Muestra el menu principal interactivo con las opciones:
 *   1 - Jugar
 *   2 - Instrucciones
 *   3 - Salir
 * Retorna: MENU_JUGAR, MENU_INSTRUCCIONES o MENU_SALIR
 */
int mostrar_menu_inicio(void);

/*
 * mostrar_instrucciones
 * Muestra la pantalla de instrucciones con los controles
 * y las reglas basicas del juego.
 */
void mostrar_instrucciones(void);

/*
 * mostrar_hud
 * Imprime el HUD debajo de la ventana 20x20 durante el juego.
 * Parametros:
 *   nivel         - numero del nivel actual (1, 2 o 3)
 *   monedas       - monedas recolectadas hasta ahora
 *   total_monedas - total de monedas del nivel (calculado por NASM)
 *   tiene_llave   - 1 si el jugador tiene la llave, 0 si no
 *   pasos         - pasos realizados en el nivel actual
 */
void mostrar_hud(int nivel, int monedas, int total_monedas,
                 int tiene_llave, int pasos);

/*
 * mostrar_nivel_completado
 * Pantalla de resumen al terminar un nivel.
 * Parametros:
 *   nivel         - numero del nivel completado
 *   monedas       - monedas recolectadas en ese nivel
 *   total_monedas - total de monedas del nivel
 *   pasos         - pasos realizados en ese nivel
 */
void mostrar_nivel_completado(int nivel, int monedas,
                              int total_monedas, int pasos);

/*
 * mostrar_resumen_final
 * Pantalla de resumen global al terminar los 3 niveles.
 * Parametros:
 *   monedas_total        - suma de todas las monedas recolectadas
 *   total_monedas_global - suma de todas las monedas de los 3 niveles
 *   pasos_total          - suma de todos los pasos
 *   niveles_completados  - cantidad de niveles terminados
 *   puntaje              - puntaje calculado por la funcion NASM
 */
void mostrar_resumen_final(int monedas_total, int total_monedas_global,
                           int pasos_total, int niveles_completados,
                           int puntaje);

/*
 * mostrar_pantalla_victoria
 * Pantalla de felicitacion al completar el juego.
 * Se muestra antes del resumen final.
 */
void mostrar_pantalla_victoria(void);

#endif /* UI_H */
