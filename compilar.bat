@echo off
setlocal enabledelayedexpansion

:: ============================================================
::  compilar.bat  -  Proyecto Ensamblador (C + NASM x64)
::  Uso: compilar.bat [nombre_salida]
::       Si no se pasa nombre, usa el nombre de la carpeta.
:: ============================================================

:: --- Compilador GCC de 64 bits (MSYS2 ucrt64) ---
set "GCC=C:\msys64\ucrt64\bin\gcc.exe"

:: --- Nombre del ejecutable de salida ---
for %%I in (.) do set "PROYECTO=%%~nxI"
if not "%~1"=="" set "PROYECTO=%~1"

:: --- Carpetas del proyecto ---
set "SRC_DIR=src"
set "OBJ_DIR=obj"
set "BIN_DIR=bin"

echo.
echo ============================================================
echo   Compilando: %PROYECTO%
echo ============================================================
echo.

:: --- Limpiar .obj anteriores ---
del /q "%OBJ_DIR%\*.obj" 2>nul

:: -------------------------------------------------------
:: 1. Compilar todos los .asm en src\ con NASM (win64)
:: -------------------------------------------------------
set "FOUND_ASM=0"
for %%F in ("%SRC_DIR%\*.asm") do (
    set "FOUND_ASM=1"
    echo [NASM]  %%~nxF
    nasm -f win64 "%%F" -o "%OBJ_DIR%\%%~nF.obj"
    if errorlevel 1 (
        echo.
        echo [ERROR] Fallo al compilar %%~nxF
        echo.
        exit /b 1
    )
)

:: -------------------------------------------------------
:: 2. Compilar todos los .c en src\ con GCC
:: -------------------------------------------------------
set "FOUND_C=0"
for %%F in ("%SRC_DIR%\*.c") do (
    set "FOUND_C=1"
    echo [GCC]   %%~nxF
    "%GCC%" -c "%%F" -o "%OBJ_DIR%\%%~nF.obj" -Wall -O2
    if errorlevel 1 (
        echo.
        echo [ERROR] Fallo al compilar %%~nxF
        echo.
        exit /b 1
    )
)

:: Verificar que haya algo que enlazar
if "%FOUND_ASM%"=="0" if "%FOUND_C%"=="0" (
    echo [AVISO] No se encontraron archivos .asm ni .c en %SRC_DIR%\
    exit /b 1
)

:: -------------------------------------------------------
:: 3. Enlazar todos los .obj en obj\ con GCC
:: -------------------------------------------------------
set "OBJS="
for %%F in ("%OBJ_DIR%\*.obj") do (
    set "OBJS=!OBJS! "%%F""
)

echo [LINK]  %BIN_DIR%\%PROYECTO%.exe
"%GCC%" !OBJS! -o "%BIN_DIR%\%PROYECTO%.exe"
if errorlevel 1 (
    echo.
    echo [ERROR] Fallo en el enlace final
    echo.
    exit /b 1
)

echo.
echo ============================================================
echo   Listo! Ejecutable: %BIN_DIR%\%PROYECTO%.exe
echo ============================================================
echo.

endlocal
pause