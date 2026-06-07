bits 64
default rel
global contar_caracteres, movimiento_valido, calcular_puntaje, detectar_objeto, celdas_libres
section .text
contar_caracteres:      ;USAR PARA CONTAR MONEDAS !!!!!!NO PONER EL CONTADOR DIRECTAMENTE!!!!!!!
    ;RCX = Direccion base del mapa (puntero de 64 bits), es como el i del bucle
    ;EDX = Numero total de celdas
    ;R8B = caracter a buscar (1 byte = char, por eso es B)
    xor eax, eax    ;eax es el acumulador
    xor r9, r9      ;r9 sera el indice contador del bucle
    
    cmp edx, 0
    je .fin

    .bucle:
        mov r9b, [rcx]  ;tenemos el byte actual del mapa y lo guardamos en r9b

        cmp r9b, r8b    ;comparamos los caracteres
        jne .siguiente ;Si NO son iguales, no incrementamos y saltamos

        inc eax     ;sumamos 1
    .siguiente:
        inc rcx     ;es como incrementar el i, avanzamos una posicion
        dec edx     ;comienza en 3600, pero ya checamos 1, entonces restamos
        jnz .bucle  ;revismaos, si edx NO es 0 continuamos el ciclo
    .fin:
        ret

movimiento_valido:
    ;RCX = Direccion base del mapa (puntero de 64 bits), es como el i del bucle
    ;EDX = Numero total de columnas
    ;R8D = Nueva fila propuesta para el jugador (donde quiere moverse, 32 bits = D)
    ;R9D = Nueva columna propuesta para el jugador (donde quiere moverse, 32 bits = D)

    xor eax, eax
    xor r10d, r10d

    ;Realizamos -> INDICE PLANO = (Nfila x TotColumnas) + Ncolumna
    ;Esto pq estamos en la memoria RAM, arreglo unidimensional
    imul r8d, edx
    add r8d, r9d
    mov r10d, r8d
    movsxd r10, r10d
    ;Nos posicionamos en la posicion exacta en la que debemos estar
    mov al, [rcx + r10]

    cmp al, '#'
    je .block

    mov eax, 1
    jmp .fin2

    .block:
        mov eax, 0
    .fin2:
        ret

calcular_puntaje:
    ;ECX = Monedas recolectadas
    ;EDX = Monedas totales
    ;R8D = Pasos realizados
    ;R9D = Niveles completados (32 bits = D)
    ;Puntaje (EAX) = (recolectadas*100/totales) + (niveles*300) - (pasos*5)
    xor eax, eax
    xor r10d, r10d

    mov r10d, edx
    mov eax, ecx
    imul eax, 100

    cdq          ;va a tomar lo de eax y extender su signo edx, extender el signo solamente es dejar la parte de edx con 1(-) o 0(+)
    mov ecx, r10d
    idiv ecx    ;resultado en eax, residuo en edx
    
    imul r9d, 300
    add eax, r9d

    imul r8d, 5
    sub eax, r8d

    cmp eax, 0
    jle .negativo
    jmp .fin3

    .negativo:
        mov eax, 0
    .fin3:
        ret

detectar_objeto:
    ;RCX = Direccion inicial del mapa
    ;EDX = Tot de columnas
    ;R8D = Fila a revisar
    ;R9D = Columna a revisar (32 bits = D)
    ;[rsp + 40] = Caracter a buscar (1 byte = B), llega por la Pila
    xor eax, eax

    mov r10b, [rsp + 40]

    ;Realizamos -> INDICE PLANO = (Nfila x TotColumnas) + Ncolumna
    imul r8d, edx
    add r8d, r9d

    movsxd r8, r8d  ;Extendemos el indice de 32 (r8d) a 64 (r8)
    mov al, [rcx + r8]
    cmp al, r10b
    je .detectado

    mov eax, 0
    ret
    .detectado:
        mov eax, 1
        ret

celdas_libres:
    ;RCX = Direccion inicial del mapa
    ;EDX = Tot de celdas
    xor eax, eax
    xor r8, r8    ;indice contador del bucle

    cmp edx, 0
    je .fin4

    .loop:
        mov r8b, [rcx]
        cmp r8b, '.'
        jne .siguiente

        inc eax
        .siguiente:
            inc rcx
            dec edx
            jnz .loop
    .fin4:
        ret