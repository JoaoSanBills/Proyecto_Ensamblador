#ifndef JUEGO_H
#define JUEGO_H

#include <stdbool.h>
#include "mapas.h"

//funciones del motor
void inicializar_juego(EstadoJuego* estado);
void bucle_principal(EstadoJuego* estado);

#endif
