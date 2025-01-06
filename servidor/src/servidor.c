/*!
 * @file    servidor.c
 * @brief   Programa principal del servidor para gestionar la conexion con los clientes y procesar sus solicitudes.
 * @author  Grupo 3
 * @date    18/12/2024
 * @details Contiene la funcion main del servidor servidor, que:
 *          - Verifica argumentos pasados al programa.
 *          - Establece conexion con los clientes mediante un socket.
 *          - Gestiona el bucle principal del servidor para procesar solicitudes de los clientes.
 *          Dependencias:
 *          - usuarios.h: Funciones relacionadas con la gestion de usuarios.
 *          - canciones.h: Funciones relacionadas con la gestion de canciones.
*/

#include <stdio.h>
#include <stdlib.h>
#include "usuarios.h"
#include "canciones.h"

/*!
 * @brief   Funcion principal del servidor.
 *          Esta funcion inicializa el servidor, verifica los argumentos pasados, establece 
 *          la conexion con los clientes y ejecuta el bucle principal para gestionar las solicitudes de los clientes.
 * @param cant_arg Cantidad de argumentos pasados al programa.
 * @param arg      Arreglo de cadenas con los argumentos. Se espera:
 *                 - arg[1]: Direccion IP del servidor.
 *                 - arg[2]: Puerto del servidor.
 * @return OK(0) si el servidor se ejecuta correctamente, ERROR(-1) si ocurre algun problema.
*/

// iniciar poniendo los argumentos del main 127.0.0.1 9090
int main(int cant_arg, char* arg[])
{
    int server_sock;
    if (cant_arg != 3)
    {
        printf("Cantidad de argumentos ingresados erronea.\n");
        return ERROR;
    }
    
    // abro socket y conecto con el cliente.
    if (conexion(&server_sock, arg[1], atoi(arg[2])) == ERROR)
    {
        return ERROR;
    }
    // ingresamos a bucle.
    menu_bucle_servidor(server_sock);

    return OK;
}
