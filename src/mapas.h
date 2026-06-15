#ifndef MAPAS_H
#define MAPAS_H

/* ============================================================
 *  mapas.h  -  BitQuest | Arquitecto de Niveles
 *  Sistema de carga, gestion de memoria y spawn de mapas.
 *
 *  Convencion de caracteres (segun requisitos del proyecto):
 *    '.'  ->  Camino libre (suelo transitable)
 *    '#'  ->  Pared (bloqueante)
 *    'M'  ->  Moneda (recogible, incrementa contador)
 *    'K'  ->  Llave  (permite abrir la puerta 'D')
 *    'D'  ->  Puerta (solo se cruza si el jugador tiene la llave)
 *    'E'  ->  Salida (termina el nivel al pisarla)
 *    'P'  ->  Punto de spawn del jugador
 * ============================================================ */

/* --- Dimensiones del mapa completo --- */
#define MAP_COLS     60
#define MAP_ROWS     60
#define MAP_SIZE     (MAP_COLS * MAP_ROWS)   /* 3600 bytes */
#define NUM_LEVELS   4

/* --- Dimensiones de la ventana visible --- */
#define VIS_COLS     20
#define VIS_ROWS     20

/* --- Caracteres del mapa --- */
#define CELDA_SUELO   '.'
#define CELDA_PARED   '#'
#define CELDA_MONEDA  'M'
#define CELDA_LLAVE   'K'
#define CELDA_PUERTA  'D'
#define CELDA_SALIDA  'E'
#define CELDA_SPAWN   'P'
#define CELDA_TRAMPA  'X'

/* --- Resultado de intentar moverse --- */
typedef enum {
    MOVE_OK              = 0,  /* movimiento valido, celda libre          */
    MOVE_PARED           = 1,  /* choco con una pared                     */
    MOVE_MONEDA          = 2,  /* recogio una moneda (M desaparece)       */
    MOVE_LLAVE           = 3,  /* recogio la llave (K desaparece)         */
    MOVE_SALIDA          = 4,  /* llego a la salida (nivel terminado)     */
    MOVE_PUERTA_ABIERTA  = 5,  /* cruzo la puerta (tenia llave)           */
    MOVE_PUERTA_CERRADA  = 6,  /* intento cruzar puerta sin llave         */
    MOVE_FUERA           = 7   /* intento salir del mapa                  */
} ResultadoMovimiento;

/* --- Coordenada (fila, columna) dentro del mapa --- */
typedef struct {
    int fila;
    int col;
} Coordenada;

/* --- Estado completo del juego --- */
typedef struct {
    /* Datos del mapa activo */
    char       celdas[MAP_SIZE];   /* buffer lineal 60x60 = 3600 bytes     */
    int        nivel_actual;       /* 0 = nivel 1, 1 = nivel 2, 2 = nivel 3 */

    /* Posiciones */
    Coordenada spawn;              /* posicion 'P' del nivel actual        */
    Coordenada pos_jugador;        /* posicion actual del jugador          */

    /* Inventario y estadisticas del nivel */
    int        monedas_total;      /* total monedas del nivel (via NASM)   */
    int        monedas_recogidas;  /* cuantas lleva recogidas              */
    int        tiene_llave;        /* 1 si el jugador cargo la llave       */
    int        pasos;              /* pasos dados en el nivel actual       */
    int        celdas_libres;      /* celdas '.' calculadas via NASM       */

    /* Acumulados del juego completo (para resumen final) */
    int        monedas_total_global;
    int        monedas_recogidas_global;
    int        pasos_global;
    int        niveles_completados;
} EstadoJuego;

/* ============================================================
 *  Declaraciones de funciones NASM (rutinas.asm)
 *  Convencion de llamada: Microsoft x64
 * ============================================================ */

/* Funcion 1: contar cuantas veces aparece un caracter en el mapa
 *   RCX = char* mapa   (puntero al inicio del arreglo)
 *   EDX = int total    (numero total de bytes a recorrer)
 *   R8B = char c       (caracter a contar)
 *   Retorna en EAX: numero de ocurrencias
 */
extern int contar_caracteres(const char *mapa, int total, char c);

/* Funcion 2: validar movimiento
 *   RCX = char* mapa
 *   RDX = int cols
 *   R8D = int fila
 *   R9D = int col
 *   Retorna en EAX: 1 si es valido (diferente de '#'), 0 si esta bloqueado
 */
extern int validar_movimiento(const char *mapa, int cols, int fila, int col);

/* Funcion 4: detectar objeto
 *   RCX = char* mapa
 *   RDX = int cols
 *   R8D = int fila
 *   R9D = int col
 *   [RSP+40] = char obj
 *   Retorna en EAX: 1 si se encontro, 0 si no
 */
extern int detectar_objeto(const char *mapa, int cols, int fila, int col, char obj);

/* Funcion 5: celdas libres
 *   RCX = char* mapa
 *   RDX = int total
 *   Retorna en EAX: cantidad de celdas libres '.'
 */
extern int contar_celdas_libres(const char *mapa, int total);

/* Funcion 3: calcular_puntaje
 *   ECX = int monedas
 *   EDX = int totales
 *   R8D = int pasos
 *   R9D = int niveles
 *   Retorna en EAX: puntaje
 */
extern int calcular_puntaje(int recolectadas, int totales, int pasos, int niveles);

/* ============================================================
 *  API publica del modulo de mapas (mapas.c)
 * ============================================================ */

/*
 * mapa_cargar_nivel
 *   Carga el nivel indicado (0-2) en estado->celdas.
 *   Calcula monedas_total usando contar_caracteres (NASM).
 *   Posiciona al jugador en 'P'. Resetea estadisticas del nivel.
 */
void mapa_cargar_nivel(EstadoJuego *estado, int nivel);

/*
 * mapa_obtener_celda
 *   Retorna el caracter en (fila, col).
 *   Retorna CELDA_PARED si las coordenadas estan fuera del mapa.
 */
char mapa_obtener_celda(const EstadoJuego *estado, int fila, int col);

/*
 * mapa_establecer_celda
 *   Escribe 'valor' en la celda (fila, col). No hace nada si es invalida.
 */
void mapa_establecer_celda(EstadoJuego *estado, int fila, int col, char valor);

/*
 * mapa_spawnear_jugador
 *   Reubica al jugador en estado->spawn (posicion 'P' del nivel).
 *   Llamar cuando el jugador deba volver al inicio del nivel.
 */
void mapa_spawnear_jugador(EstadoJuego *estado);

/* mapa_intentar_moverse delegada al motor de Leo */

/*
 * mapa_nombre_nivel
 *   Retorna el nombre del nivel como string de solo lectura.
 */
const char *mapa_nombre_nivel(int nivel);

#endif /* MAPAS_H */
