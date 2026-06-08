; =========================================================
; bitquest.asm - Rutinas NASM de 64 bits (Rol 4 - Joshua)
;
; NOTA: contar_caracteres y movimiento_valido estan en el
; bitquest.asm de Leo (Rol 2). Este archivo implementa las
; 3 funciones NASM restantes para completar las 5 obligatorias.
;
; Convencion de llamadas Windows x64:
;   Arg 1 -> RCX   Arg 2 -> RDX
;   Arg 3 -> R8    Arg 4 -> R9
;   Arg 5 -> [RSP+40] (dentro de la funcion, RSP apunta al ret addr)
;   Retorno -> RAX
; =========================================================

bits 64
default rel

global calcular_puntaje
global detectar_objeto
global contar_celdas_libres

section .text

; ---------------------------------------------------------
; calcular_puntaje
; Calcula el puntaje final del jugador.
; Formula: (monedas * 100) - (pasos * 2) + (niveles * 500)
;
; Parametros:
;   RCX = monedas recolectadas (total acumulado)
;   RDX = pasos realizados     (total acumulado)
;   R8  = niveles completados
; Retorna:
;   RAX = puntaje calculado (puede ser negativo si muchos pasos)
; ---------------------------------------------------------
calcular_puntaje:
    imul rcx, 100       ; monedas * 100
    imul rdx, 2         ; pasos * 2
    imul r8,  500       ; niveles * 500

    mov  rax, rcx       ; rax = monedas * 100
    sub  rax, rdx       ; rax -= pasos * 2
    add  rax, r8        ; rax += niveles * 500
    ret

; ---------------------------------------------------------
; detectar_objeto
; Detecta si existe un objeto especifico en una celda del mapa.
;
; Parametros:
;   RCX       = direccion base del mapa (char*)
;   EDX       = numero de columnas del mapa
;   R8D       = fila que se desea revisar
;   R9D       = columna que se desea revisar
;   [RSP+40]  = caracter del objeto a buscar (char, 5to arg)
; Retorna:
;   EAX = 1 si el objeto esta en esa celda, 0 si no
; ---------------------------------------------------------
detectar_objeto:
    ; Calculamos el indice plano: fila * columnas + columna
    imul r8d, edx       ; r8d = fila * columnas
    add  r8d, r9d       ; r8d = indice plano

    movsx r8, r8d       ; extendemos a 64 bits para usar como offset

    ; Leemos el caracter en la celda del mapa
    movzx eax, byte [rcx + r8]

    ; Leemos el 5to argumento (objeto buscado) desde la pila
    ; RSP+0  = direccion de retorno
    ; RSP+8  a RSP+32 = shadow space de los 4 args en registro
    ; RSP+40 = 5to argumento
    movzx r10d, byte [rsp + 40]

    ; Comparamos el caracter de la celda con el buscado
    cmp  al, r10b
    je   .encontrado

    xor  eax, eax       ; no coincide -> retornar 0
    ret

.encontrado:
    mov  eax, 1         ; coincide -> retornar 1
    ret

; ---------------------------------------------------------
; contar_celdas_libres
; Cuenta cuantas celdas de camino libre '.' hay en el mapa.
;
; Parametros:
;   RCX = direccion base del mapa (char*)
;   EDX = numero total de celdas (filas * columnas)
; Retorna:
;   EAX = cantidad de celdas libres encontradas
; ---------------------------------------------------------
contar_celdas_libres:
    xor eax, eax        ; eax = contador (retorno)

    cmp edx, 0
    je  .fin            ; si no hay celdas, retornar 0

.bucle:
    movzx r9d, byte [rcx]   ; cargar byte actual del mapa

    cmp  r9b, '.'           ; es camino libre?
    jne  .siguiente         ; no -> saltamos

    inc  eax                ; si -> incrementamos contador

.siguiente:
    inc  rcx                ; avanzamos al siguiente byte
    dec  edx                ; reducimos celdas por revisar
    jnz  .bucle             ; si quedan celdas, continuamos

.fin:
    ret