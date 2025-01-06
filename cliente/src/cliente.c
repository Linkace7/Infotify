/*!
 * @file    cliente.c
 * @brief   Programa cliente que establece conexion con el servidor y gestiona el menu del cliente.
 * @author  Grupo 3
 * @date    18/12/2024
 * @details Este programa se encarga de:
 *          - Establecer una conexion con el servidor.
 *          - Ingresar al menu principal del cliente una vez conectados.
 *          Dependencias:
 *          - menu.h: Funciones relacionadas con el menu del cliente.
 *          - canciones.h: Funciones relacionadas con la gestion de canciones.
*/

#include <stdio.h>
#include "menu.h"
#include "canciones.h"

/*!
 * @brief   Funcion principal del cliente.
 * Esta funcion inicializa el programa cliente:
 * - Establece la conexion con el servidor mediante la funcion conexion().
 * - Si la conexion es exitosa, llama a menu_cliente() para gestionar las operaciones del cliente.
 * - Si ocurre un error durante la conexion, retorna un codigo de error.
 * @return OK (0) si el programa finaliza correctamente, ERROR (-1) si ocurre un error.
*/

int main(void)
{
    int sock;

    // establecemos conexion con el servidor.
    if ((sock = conexion()) == ERROR)
    {
        return ERROR;
    }
    printf("Conectado al servidor en %s:%d\n", SERVER_IP, SERVER_PORT);
    // ingresamos al bucle del cliente.
    menu_cliente(sock);

    return OK;
}