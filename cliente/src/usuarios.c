/*!
 * @file    usuarios.c
 * @brief   Gestion de datos de usuario: ingreso de usuario y contrasenia.
 * @author  Grupo 3
 * @date    18/12/2024
 * @details Este archivo contiene la implementacion de las funciones relacionadas con la gestion de datos de usuario,
 *          como el ingreso de nombres de usuario y contrasenias con validaciones de longitud.
*/

#include "usuarios.h"
#include <stdio.h>
#include <string.h>

/*!
 * @brief   Solicita al usuario ingresar su nombre de usuario y contrasenia.
 *          Esta funcion solicita al usuario que ingrese un nombre de usuario y una contrasenia, cada uno limitado a 25 caracteres.
 *          Se asegura de limpiar el buffer de entrada despues de cada operacion para evitar problemas con entradas largas.
 * @param usuario     Puntero a la cadena donde se almacenara el nombre de usuario ingresado.
 * @param contrasenia Puntero a la cadena donde se almacenara la contrasenia ingresada.
*/
void ingresar_datos(char *usuario, char *contrasenia)
{
    printf("\nUsuario (maximo 25 caracteres): ");
    scanf("%25s", usuario); // limitar a 25 caracteres.
    while (getchar() != '\n');
    printf("Contrasenia (maximo 25 caracteres): ");
    scanf("%25s", contrasenia);
    while (getchar() != '\n');
}