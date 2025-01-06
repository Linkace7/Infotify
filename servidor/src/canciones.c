/*!
 * @file    canciones.c
 * @brief   Gestion de canciones en el servidor: listado, filtrado, y envio de canciones a clientes conectados al sistema.
 * @author  Grupo 3
 * @date    18/12/2024
 * @details Este archivo implementa las funciones necesarias para:
 *          - Mostrar un menu de opciones al cliente.
 *          - Listar las canciones disponibles.
 *          - Filtrar canciones por artista o genero.
 *          - Enviar canciones solicitadas por los clientes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <unistd.h>
#include "canciones.h"

/*!
 * @brief   Menu principal del servidor para gestionar opciones de canciones.
 *          Recibe las opciones seleccionadas por el cliente y llama a las funciones correspondientes
 *          para listar, filtrar o enviar canciones.
 * @param client_sock Descriptor del socket del cliente conectado.
 * @param buffer      Buffer utilizado para enviar y recibir datos.
*/
void menu_canciones_servidor(int client_sock, char* buffer)
{
    int opcion, estado;
    ssize_t bytes_received;

    do
    {
        // recibir opcion del cliente.
        if ((bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0)) < 0)
        {
            perror("Error al recibir opcion del cliente.\n");
            break;
        } else if (bytes_received == 0)
        {
            printf("Conexion finalizada por el cliente.\n");
            break;
        }
        buffer[bytes_received] = '\0'; // otorgamos fin de linea al buffer.
        opcion = atoi(buffer);
        switch (opcion) // procesamos opcion elegida.
        {
            case 1:
                printf("Opcion seleccionada: Listar canciones.\n");
                estado = listar_servidor(client_sock, buffer);
                break;
            case 2:
                printf("Opcion seleccionada: Filtrar canciones.\n");
                estado = menu_filtrar_servidor(client_sock, buffer);
                break;
            case 3:
                printf("Opcion seleccionada: Escuchar cancion.\n");
                estado = escuchar_cancion_servidor(client_sock, buffer);
                break;
            default:
                printf("Opcion incorrecta recibida.\n");
                estado = ERROR;
                break;
        }
    } while (estado == OK);
}

/*!
 * @brief   Lista las canciones disponibles en el servidor.
 *          Envia el listado de canciones al cliente conectado. Lee las canciones desde un archivo CSV
 *          y las envia linea por linea.
 * @param client_sock Descriptor del socket del cliente.
 * @param buffer      Buffer utilizado para enviar datos.
 * @return OK(0) si el listado es exitoso, ERROR(-1) si ocurre algun problema.
*/
int listar_servidor(int client_sock, char* buffer)
{
    int i;
    int cont = 1;
    char* token = NULL;
    char linea[LINEA_MAX];
    FILE *canciones = fopen("media.csv", "r");
    if (canciones == NULL)
    {
        perror("No se pudo abrir el archivo.\n");
        return ERROR;
    }

    while (fgets(linea, LINEA_MAX, canciones) != NULL)
    {
        linea[strcspn(linea, "\n")] = '\0'; // eliminamos salto de linea si existiese.
        // tokenizamos la linea.
        token = strtok(linea, ",");
        snprintf(buffer, BUFFER_SIZE, "%d - %s - ", cont, token);
        for (i = 0; i < 3; i++)
        {
            token = strtok(NULL, ",");
            strcat(buffer, token);
            strcat(buffer, " - ");
        }
        token = strtok(NULL, ",");
        strcat(buffer, token);
        strcat(buffer, "\n");        
        if (send(client_sock, buffer, strlen(buffer), 0) < 0)
        {
            perror("Error al enviar linea.\n");
            fclose(canciones);
            return ERROR;
        }
        cont++;
        usleep(50);
    }
    // enviamos senial de fin.
    if (send(client_sock, FIN, strlen(FIN), 0) < 0)
    {
        perror("Error al enviar senial de fin.\n");
        fclose(canciones);
        return ERROR;
    }
    
    fclose(canciones);
    return OK;
}

/*!
 * @brief   Menu de filtrado de canciones en el servidor.
 *          Recibe del cliente la opcion de filtrado (por artista o genero) y llama a la funcion de filtrado correspondiente.
 * @param client_sock Descriptor del socket del cliente.
 * @param buffer      Buffer utilizado para recibir datos.
 * @return OK(0) si la operacion es exitosa, ERROR(-1) si ocurre un problema.
*/
int menu_filtrar_servidor(int client_sock, char *buffer)
{
    int opcion;
    ssize_t bytes_received;

    // recibir opcion elegida por el cliente.
    if ((bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0)) <= 0)
    {
        perror("Error al recibir opcion elegida.\n");
        return ERROR;
    }
    buffer[bytes_received] = '\0';
    opcion = atoi(buffer); // convertir opcion a entero.
    if (opcion == 1)
    {
        return filtrar_servidor(client_sock, buffer, ARTISTA);
    } else if (opcion == 2)
    {
        return filtrar_servidor(client_sock, buffer, GENERO);
    }

    printf("Opcion invalida.\n");
    return ERROR;
}

/*!
 * @brief   Filtra las canciones por un criterio especifico (artista o genero).
 *          Envia al cliente las canciones que cumplen con el criterio de filtrado ingresado.
 * @param client_sock Descriptor del socket del cliente.
 * @param buffer      Buffer utilizado para enviar datos.
 * @param sector      Indica el campo a filtrar (ARTISTA o GENERO).
 * @return OK(0) si el filtrado es exitoso, ERROR(-1) si ocurre un problema.
*/
int filtrar_servidor(int client_sock, char* buffer, int sector)
{
    ssize_t bytes_received;
    int cont, i;
    char linea[LINEA_MAX];
    char filtro[FILTRO_MAX];
    char* segmentos[5];
    char* token = NULL;
    FILE *canciones = fopen("media.csv", "r");
    if (canciones == NULL)
    {
        perror("No se pudo abrir archivo de registro de canciones.\n");
        return ERROR;
    }
    // recibir filtro.
    if ((bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0)) <= 0)
    {
        perror("Error al recibir filtro.\n");
        fclose(canciones);
        return ERROR;
    }
    buffer[bytes_received] = '\0';
    strcpy(filtro, buffer);
    cont = 1;
    while (fgets(linea, LINEA_MAX, canciones) != NULL) // leemos el registro linea por linea.
    {
        i = 0; 
        linea[strcspn(linea, "\n")] = '\0'; // eliminar salto de linea.
        // tokenizamos.
        token = strtok(linea, ",");
        while (token != NULL && i < 5)
        {
            segmentos[i++] = token;
            token = strtok(NULL, ",");
        }
        // validar que cumple el filtro.
        if (verificar(segmentos[sector], filtro) == OK)
        {
            // damos formato y enviamos.
            snprintf(buffer, BUFFER_SIZE, "%d - %s - %s - %s - %s - %s\n", 
                     cont, segmentos[0], segmentos[1], segmentos[2], segmentos[3], segmentos[4]);
            if (send(client_sock, buffer, strlen(buffer), 0) < 0)
            {
                perror("Error al enviar datos al cliente");
                fclose(canciones);
                return ERROR;
            }
        }
        cont++;
        usleep(50);
    }

    // enviamos senial de fin.
    if (send(client_sock, FIN, strlen(FIN), 0) < 0)
    {
        perror("Error al enviar senial de fin.\n");
        fclose(canciones);
        return ERROR;
    }
    fclose(canciones);
    return OK;
}

/*!
 * @brief   Verifica si un dato cumple con el filtro ingresado.
 *          Compara el dato con el filtro ingresado por el cliente, ignorando diferencias en mayusculas/minusculas.
 * @param dato   Dato a comparar (por ejemplo, el nombre del artista o genero).
 * @param filtro Filtro ingresado por el cliente.
 * @return OK(0) si el dato cumple con el filtro, ERROR(-1) en caso contrario.
*/
int verificar(char *dato, char *filtro)
{
    int posicion1 = 0, posicion2 = 0;

    while (dato[posicion1] != '\0' && filtro[posicion2] != '\0')
    {
        if (tolower(dato[posicion1]) != tolower(filtro[posicion2]))
        {
            return ERROR;
        }
        posicion1++;
        posicion2++;
    }
    // verificamos que finalicen al mismo tiempo.
    if (dato[posicion1] != '\0' || filtro[posicion2] != '\0')
    {
        return ERROR;
    }

    return OK;
}

/*!
 * @brief   Envia una cancion solicitada por el cliente.
 *          Busca el archivo correspondiente a la cancion solicitada, verifica su existencia,
 *          y lo envia al cliente en bloques.
 * @param client_sock Descriptor del socket del cliente.
 * @param buffer      Buffer utilizado para enviar datos.
 * @return OK(0) si la cancion es enviada exitosamente, ERROR(-1) si ocurre algun problema.
*/
int escuchar_cancion_servidor(int client_sock, char* buffer)
{
    ssize_t bytes_received;
    size_t bytes_read;
    FILE* cancion = NULL;

    // recibir nombre de la cancion.
    if ((bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0)) <= 0)
    {
        perror("Error al recibir nombre de la cancion.\n");
        return ERROR;
    }
    buffer[bytes_received] = '\0';
    if (strcmp(buffer, FIN) == 0) // validamos si usuario sigue conectado.
    {
        printf("\n");
        return OK;
    }
    // abrir el archivo.
    if (access(buffer, F_OK) != 0) // verificar si el archivo existe.
    {
        if (send(client_sock, FIN, strlen(FIN), 0) < 0)
        {
            perror("Error al enviar aviso de inexistencia.\n");
            return OK;
        }
        return OK;
    }
    if ((cancion = fopen(buffer, "rb")) == NULL)
    {
        perror("Error al abrir archivo de cancion.\n");
        if (send(client_sock, "ERROR", strlen("ERROR"), 0) < 0)
        {
            perror("Error al abrir archivo cancion.\n");
            return ERROR;
        }
        return ERROR;
    }
    else 
    {
        if (send(client_sock, "OK", strlen("OK"), 0) < 0)
        {
            perror("Error al enviar confirmacion.\n");
            return ERROR;
        }
    }
    printf("Enviando archivo: %s\n", buffer);
    // enviar el archivo en bloques.
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, cancion)) > 0)
    {
        if (send(client_sock, buffer, bytes_read, 0) < 0)
        {
            perror("Error al enviar datos del archivo.\n");
            fclose(cancion);
            return ERROR;
        }
        usleep(50);
    }
    // enviar indicador de fin de transmision.
    usleep(500000);
    if (send(client_sock, FIN, strlen(FIN), 0) < 0)
    {
        perror("Error al enviar indicador de fin de transmision.\n");
        fclose(cancion);
        return ERROR;
    }

    fclose(cancion);
    return OK;
}