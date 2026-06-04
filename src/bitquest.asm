; =========================================================
; bitquest.asm - Rutinas en NASM de 64 bits para BitQuest
; Convencion de llamadas Windows x64:
;   RCX = primer argumento
;   RDX = segundo argumento
;   R8  = tercer argumento
;   RAX = valor de retorno
; =========================================================

bits 64
default rel

global contar_caracteres

section .text

; ---------------------------------------------------------
; contar_caracteres
; Cuenta cuantas veces aparece un caracter en el mapa.
; Parametros:
;   RCX = direccion base del mapa (puntero char*)
;   EDX = numero total de celdas (filas * columnas)
;   R8B = caracter a buscar (1 byte)
; Retorna:
;   EAX = cantidad de veces que aparece el caracter
; ---------------------------------------------------------
contar_caracteres:
    xor eax, eax        ; eax = contador de coincidencias (retorno)
    xor r9d, r9d        ; r9d = indice auxiliar

    cmp edx, 0
    je  .fin            ; si no hay celdas, retornar 0

.bucle:
    mov r9b, [rcx]      ; cargamos el byte actual del mapa
    cmp r9b, r8b        ; comparamos con el caracter buscado
    jne .siguiente      ; si no coincide, pasamos al siguiente

    inc eax             ; coincidencia: sumamos 1 al contador

.siguiente:
    inc rcx             ; avanzamos al siguiente byte del mapa
    dec edx             ; reducimos el contador de celdas restantes
    jnz .bucle          ; si quedan celdas, repetimos

.fin:
    ret