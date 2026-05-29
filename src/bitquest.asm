bits 64
default rel
global contar_caracteres, 
section.text
;RCX = Direccion base del mapa (puntero de 64 bits), es como el i del bucle
;EDX = Numero total de celdas
;R8B = caracter a buscar (1 byte = char, por eso es B)
contar_caracteres:
    xor eax, eax    ;eax es el acumulador
    xor r9, r9      ;r9 sera el indice contador del bucle
    
    cmp edx, 0
    je .fin

    .bucle:
        mov r9b, [rcx]  ;tenemos el byte actual del mapa y lo guardamos en r9b

        cmp r9b, r8b    ;comparamos los caracteres
        jne .siguiente: ;Si NO son iguales, no incrementamos y saltamos

        inc eax     ;sumamos 1
    .siguiente:
        inc rcx     ;es como incrementar el i, avanzamos una posicion
        dec edx     ;comienza en 3600, pero ya checamos 1, entonces restamos
        jnz .ciclo  ;revismaos, si edx NO es 0 continuamos el ciclo
    .fin:
        ret