/*!
 * @file    menu.c
 * @brief   Gestion de conexion del cliente con el servidor y manejo del menu principal.
 * @author  Grupo 3
 * @date    18/12/2024
 * @details Este archivo implementa las funciones necesarias para:
 *          - Enlazar una conexion TCP con el servidor.
 *          - Exponer el menu principal del cliente.
 *          - Gestionar operaciones de inicio de sesion y registro.
 *          Dependencias:
 *          - menu.h: Declaraciones relacionadas con el menu del cliente.
 *          - canciones.h: Funciones relacionadas con canciones.
 *          - usuarios.h: Funciones relacionadas con la gestion de usuarios.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "menu.h"
#include "canciones.h"
#include "usuarios.h"

/*!
 * @brief   Enlaza conexion TCP con el servidor. Crea un socket, configura la direccion del
 *          servidor y establece una conexion.
 * @return  Retorna el descriptor del socket si la conexion es exitosa. 
 *          Retorna ERROR (-1) si ocurre un error.
*/
int conexion(void)
{
    int sock;
    struct sockaddr_in server_addr;
    
    // crear el socket.
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error al crear el socket.\n");
        return ERROR;
    }
    // configurar la direccion del servidor.
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        perror("Direccion IP invalida o no soportada.\n");
        close(sock);
        return ERROR;
    }
    // conectar con el servidor.
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error de conexion con el servidor.\n");
        close(sock);
        return ERROR;
    }

    return sock;
}

/*!
 * @brief   Muestra el menu principal del cliente y gestiona sus opciones.
 *          Presenta al cliente opciones como iniciar sesion, registrarse o salir.
 *          Se comunica con el servidor para validar credenciales y realizar operaciones.
 * @param sock Descriptor del socket de conexion con el servidor.
*/
void menu_cliente(int sock)
{
    int opcion;
    ssize_t bytes_received;
    char buffer[BUFFER_SIZE];
    Cuenta credencial;

    do
    {
        if ((opcion = op_menu()) == 3) // mostrar el menu de opciones, en caso de ser igual a 3, finalizamos.
        {
            printf("Desconectando...\n");
            break;  
        }
        // enviar usuario, contrasenia y opcion al servidor.
        ingresar_datos(credencial.usuario, credencial.contrasenia);
        snprintf(buffer, BUFFER_SIZE, "%d:%s:%s", opcion, credencial.usuario, credencial.contrasenia);
        if (send(sock, buffer, strlen(buffer), 0) < 0)
        {
            perror("Error al enviar datos de usuario.\n");
            break;
        }
        // recibir respuesta del servidor
        if ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) <= 0)
        {
            perror("Error al recibir respuesta del servidor.\n");
            break;
        }
        buffer[bytes_received] = '\0';
        if (opcion == 1) // iniciar sesion.
        {
            if (strcmp(buffer, EXITO) == 0)
            {
                printf("¡Bienvenido[%s]!\n", credencial.usuario);
                menu_canciones_cliente(sock, buffer);
                break;
            }
            printf("%s\n", buffer);
        } else if (opcion == 2) // registrar usuario.
        {
            printf("%s\n", buffer);
            if (strcmp(buffer, EXITO) == 0)
            {
                menu_canciones_cliente(sock, buffer);
                break;
            }
            if (strcmp(buffer, ERROR_GUARDAR) == 0)
            {
                break;
            }
        }
    } while (opcion != 3);

    // cerrar el socket.
    close(sock);
}

/*!
 * @brief   Muestra menu de opciones del cliente. 
 *          Presenta opciones disponibles al cliente (iniciar sesion, registrarse, salir).
 *          Valida que la opcion ingresada sea valida (1, 2 o 3).
 * @return  Opcion seleccionada por el cliente.
*/
int op_menu(void)
{
    int opcion = 0;

    printf("\nMenu de opciones.\n1. Iniciar sesion.\n2. Registrarse.\n3. Salir.\n");
    printf("Para seleccionar, ingrese valor correspondiente: ");
    while (opcion < 1 || opcion > 3)
    {
        scanf("%d", &opcion);
        while (getchar() != '\n'); // limpiamos buffer de entrada.
        if (opcion < 1 || opcion > 3)
        {
            printf("Opcion incorrecta. Intente nuevamente: \n");
        }
    }

    return opcion;
}