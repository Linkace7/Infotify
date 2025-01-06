/*!
 * @file    canciones.c
 * @brief   Gestion de canciones en el cliente: listado, filtrado y pasaje de canciones desde el servidor.
 * @author  Grupo 3
 * @date    18/12/2024
 * @details Este archivo implementa las funciones necesarias para:
 *          - Mostrar el menu de canciones del cliente.
 *          - Listar canciones disponibles en el servidor.
 *          - Filtrar canciones por artista o genero.
 *          - Solicitar y descargar canciones desde el servidor.
*/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "canciones.h"
#include <stdlib.h>

/*!
 * @brief   Muestra menu de canciones y gestiona las opciones seleccionadas.
 *          Permite listar canciones, filtrarlas o pedirlas. Envia las opciones seleccionadas
 *          al servidor y maneja las respuestas correspondientes.
 * @param sock   Descriptor del socket de conexion con el servidor.
 * @param buffer Buffer utilizado para enviar y recibir datos.
*/
void menu_canciones_cliente(int sock, char* buffer)
{
    int opcion = op_menu_canciones(); // mostrar el menu de opciones.

    while (opcion != 4)
    {
        //enviamos opcion al servidor. 
        snprintf(buffer, BUFFER_SIZE, "%d", opcion);
        if (send(sock, buffer, strlen(buffer), 0) < 0)
        {
            perror("Error al enviar opcion elegida.\n");
            break;
        }
        // derivamos opcion seleccionada.
        if (opcion == 1)
        {
            if (listar_cliente(sock, buffer) == ERROR)
            {
                break;
            }
        } else if (opcion == 2)
        {
            if (menu_filtrar_cliente(sock, buffer) == ERROR)
            {
                break;
            }
        } else if (opcion == 3)
        {
            if (escuchar_cancion_cliente(sock, buffer) == ERROR)
            {
                break;
            }
        }
        opcion = op_menu_canciones();
    }

    printf("Programa finalizado.\n");
}

/*!
 * @brief   Muestra las opciones del menu de canciones.
 *          Las opciones incluyen listar canciones, filtrarlas por artista o genero, o pedir una cancion.
 *          Valida que la opcion ingresada sea valida.
 * @return  Opcion seleccionada por el cliente.
*/
int op_menu_canciones(void)
{
    int opcion = 0;

    printf("\nMenu de opciones.\n1. Listar canciones.\n2. Filtrar canciones.\n3. Escuchar cancion.\n4. Salir.\n");
    printf("Para seleccionar, ingrese valor correspondiente: ");
    while (opcion < 1 || opcion > 4)
    {
        scanf("%d", &opcion);
        while (getchar() != '\n'); // limpiamos buffer de entrada.
        if (opcion < 1 || opcion > 4)
        {
            printf("Opcion incorrecta. Intente nuevamente: \n");
        }
    }

    return opcion;
}

/*!
 * @brief   Lista las canciones disponibles en el servidor.
 *          Envia una solicitud al servidor para obtener el listado de canciones y muestra los resultados en pantalla.
 * @param sock   Descriptor del socket de conexion con el servidor.
 * @param buffer Buffer utilizado para recibir datos.
 * @return OK(0) si el listado es exitoso, ERROR(-1) si ocurre un error.
*/
int listar_cliente(int sock, char* buffer)
{
    ssize_t bytes_received;

    printf("\nLista de canciones. \nNo - Tema - Artista - Album - Genero - Anio\n");
    while(1)
    {
        // recibir listado del servidor.
        if ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) <= 0)
        {
            perror("Error al recibir listado.\n");
            return ERROR;
        }
        buffer[bytes_received] = '\0'; // otorgamos fin de linea al buffer. 
        if (strcmp(buffer, FIN) == 0) // verificamos si lo que se recibio es senial de fin. 
        {
            printf("\n");
            break;
        }
        printf("%s", buffer);
    }

    return OK;
}

/*!
 * @brief   Envia una opcion de filtro al servidor y solicita canciones filtradas.
 *          Esta funcion permite al cliente seleccionar un criterio de filtrado
 *          y envia esta opcion al servidor. Luego llama a filtrar_cliente() para manejar los resultados
 *          recibidos desde el servidor.
 * @param sock   Descriptor del socket de conexion con el servidor.
 * @param buffer Buffer utilizado para enviar y recibir datos.
 * @return OK(0) si la operacion es exitosa, ERROR(-1) si ocurre un error.
*/
int menu_filtrar_cliente(int sock, char* buffer)
{
    int opcion = op_filtrar();

    // enviar opcion al servidor.
    snprintf(buffer, BUFFER_SIZE, "%d", opcion);
    if (send(sock, buffer, strlen(buffer), 0) < 0)
    {
        perror("Error al enviar opcion elegida. \n");
        return ERROR;
    }
    
    return filtrar_cliente(sock, buffer);
}

/*!
 * @brief   Muestra las opciones de filtrado al cliente.
 *          Presenta un menu con las opciones disponibles para filtrar canciones (por artista o por genero)
 *          y valida que la entrada sea correcta.
 * @return  La opcion seleccionada por el cliente (1 para artista, 2 para genero).
*/
int op_filtrar(void)
{
    int opcion = 0;

    printf("\nOpciones para filtrar.\n1. Por artista.\n2. Por genero.\n");
    printf("Para seleccionar, ingrese valor correspondiente: ");
    while (opcion != 1 && opcion != 2)
    {
        scanf("%d", &opcion);
        while (getchar() != '\n'); // limpiamos buffer de entrada. 
        if (opcion != 1 && opcion != 2)
        {
            printf("Opcion incorrecta. Intente nuevamente:\n");
        }
    }

    return opcion;
}

/*!
 * @brief   Solicita y envia un filtro para listar canciones especificas.
 *          Envia un filtro (por artista o genero) al servidor y recibe las canciones que cumplen con ese criterio.
 * @param sock   Descriptor del socket de conexion con el servidor.
 * @param buffer Buffer utilizado para enviar y recibir datos.
 * @return OK(0) si la operacion es exitosa, ERROR(-1) si ocurre un error.
*/
int filtrar_cliente(int sock, char* buffer)
{
    ssize_t bytes_received;

    while (1)
    {
        printf("Ingrese filtro: ");
        // leer entrada.
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
        {
            printf("Error al leer entrada. Intente nuevamente.\n");
            continue;
        }
        // eliminar salto de linea y validar entrada vacia.
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) == 0)
        {
            printf("Entrada vacia. Intente nuevamente.\n");
            continue;
        }
        break;    
    }
    // enviar filtro al servidor.
    if (send(sock, buffer, strlen(buffer), 0) < 0)
    {
        perror("Error al enviar filtro a servidor.\n");
        return ERROR;
    }
    printf("\nLista de canciones. \nNo - Tema - Artista - Album - Genero - Anio\n");
    while(1)
    {
        if ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) <= 0)
        {
            perror("Error al recibir listado.\n");
            return ERROR;
        }
        buffer[bytes_received] = '\0';
        if (strcmp(buffer, FIN) == 0) // verificamos si lo que se recibio es senial de fin. 
        {
            printf("\n");
            break;
        }
        printf("%s", buffer);
    }
    return OK;
}

/*!
 * @brief   Solicita y descarga una cancion desde el servidor.
 *          Envia al servidor el numero de la cancion solicitada, recibe los datos de la cancion y los guarda en un archivo local.
 * @param sock   Descriptor del socket de conexion con el servidor.
 * @param buffer Buffer utilizado para enviar y recibir datos.
 * @return OK(0) si la operacion es exitosa, ERROR(-1) si ocurre un error.
*/
int escuchar_cancion_cliente(int sock, char* buffer)
{
    int eleccion;
    ssize_t bytes_received;
    char cancion[50];
    FILE* archivo = NULL;
    char comando[100];

    while (1)
    {
        // solicitar numero de cancion.
        printf("Ingrese numero de cancion (para salir, ingrese 0): ");
        if (scanf("%d", &eleccion) != 1)
        {
            printf("Entrada invalida. Intente nuevamente.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        if (eleccion == 0) // salir si se elige 0.
        {
            if (send(sock, FIN, strlen(FIN), 0) < 0)
            {
                perror("Error al enviar senial de salida.\n");
                return ERROR;
            }
            return OK;
        }
        if (eleccion < 0) // validar que no sea negativo.
        {
            printf("Numero de cancion no puede ser negativo. Intente nuevamente.\n");
            continue;
        }
        snprintf(cancion, sizeof(cancion), "%d.mp3", eleccion); // Dar formato al nombre.
        if (access(cancion, F_OK) == 0) // Verificar si el archivo ya existe.
        {
            printf("Cancion ya en sistema.\n");
            continue;
        }
        strcpy(buffer, cancion);
        if (send(sock, buffer, strlen(buffer), 0) < 0) // Enviar numero de cancion al servidor.
        {
            perror("Error al enviar numero de cancion.\n");
            return ERROR;
        }
        // Recibir respuesta del servidor.
        if ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) <= 0)
        {
            perror("Error al recibir respuesta del servidor.\n");
            return ERROR;
        }
        buffer[bytes_received] = '\0';
        if (strcmp(buffer, FIN) == 0) // validar respuesta de existencia desde el servidor.
        {
            printf("Cancion inexistente.\n");
            return OK;
        }
        else if (strcmp(buffer, "ERROR") == 0)
        {
            printf("Error al abrir archivo en el servidor.\n");
            return ERROR;
        }
        if ((archivo = fopen(cancion, "wb")) == NULL)
        {
            perror("Error al crear archivo de cancion.\n");
            return ERROR;
        }
        while (1)
        {
            if ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) <= 0)
            {
                perror("Error al recibir datos del servidor.\n");
                fclose(archivo);
                return ERROR;
            }
            if (strncmp(buffer, FIN, strlen(FIN)) == 0 && bytes_received == strlen(FIN)) // Verificar si es el fin de la transmision.
            {
                printf("Descarga finalizada.\n");
                break;
            }
            if (fwrite(buffer, 1, bytes_received, archivo) != (size_t)bytes_received)
            {
                perror("Error al escribir en archivo.\n");
                fclose(archivo);
                return ERROR;
            }
        }
        break;
    }

    fclose(archivo);

    snprintf(comando, sizeof(comando), "mpg123 \"%s\"", cancion); // Reemplaza "mpg123" con el reproductor que prefieras
    if (system(comando) != 0) {
        printf("No se pudo reproducir la cancion. Verifique que tenga un reproductor instalado.\n");
    }
    return OK;

    
}