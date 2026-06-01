#include <stdio.h>
#include "juego.h"

int main() {
    Jugador jugador;
    
    printf("--- BitQuest ---\n");
    
    inicializar_juego(&jugador);
    bucle_principal(&jugador);
    
    return 0;
}
