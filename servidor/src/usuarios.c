/*!
 * @file    usuarios.c
 * @brief   Gestion de usuarios en el servidor: conexion, validacion de inicio de sesion, registro y almacenamiento de usuarios.
 * @author  Grupo 3
 * @date    18/12/2024
 * @details Este archivo contiene funciones para:
 *          - Establecer conexion con el cliente mediante sockets.
 *          - Validar credenciales de usuario para inicio de sesion.
 *          - Registrar nuevos usuarios y almacenarlos en un archivo db.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "usuarios.h"
#include "canciones.h"

/*!
 * @brief   Establece conexion con el cliente mediante un socket.
 *          Crea un socket, configura la direccion del servidor y lo enlaza a un puerto.
 *          Luego pone el servidor en modo escucha para aceptar conexiones entrantes.
 * @param server_sock Puntero al descriptor del socket del servidor.
 * @param server_ip   Direccion IP del servidor.
 * @param server_port Puerto del servidor.
 * @return OK(0) si la conexion se establece correctamente, ERROR(-1) si ocurre un problema.
*/

int conexion(int* server_sock, const char* server_ip, int server_port)
{
    struct sockaddr_in server_addr;
    
    // crear el socket del servidor.
    if (((*server_sock) = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error al crear el socket del servidor.\n");
        return ERROR;
    }
    // configurar direccion del servidor.
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("Error al convertir direccion IP.\n");
        close(*server_sock);
        return ERROR;
    }
    // enlazar el socket a la direccion y puerto especificados.
    if (bind((*server_sock), (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error al enlazar el socket.\n");
        close(*server_sock);
        return ERROR;
    }
    // escuchar conexiones entrantes.
    if (listen((*server_sock), 1) < 0)
    {
        perror("Error al escuchar en el socket.\n");
        close(*server_sock);
        return ERROR;
    }
    printf("Servidor en espera de conexiones en el puerto %d...\n", server_port);
    
    return OK;
}

/*!
 * @brief   Bucle principal para gestionar las solicitudes del cliente.
 *          Acepta conexiones entrantes y procesa las solicitudes de los clientes, como inicio de sesion y registro.
 * @param server_sock Descriptor del socket del servidor.
*/
void menu_bucle_servidor(int server_sock)
{
    int client_sock, opcion;
    ssize_t bytes_received;
    char buffer[BUFFER_SIZE];
    char* opcion_aux = NULL;
    struct sockaddr_in client_addr;
    Cuenta usuario;
    socklen_t addr_len = sizeof(client_addr);

    while (1) // bucle para recibir y responder al cliente.
    {   
        printf("Esperando cliente.\n");
        // aceptar una conexion entrante.
        if ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len)) < 0)
        {
            perror("Error al aceptar la conexion.\n");
            continue;
        }
        printf("Nuevo cliente conectado.\n");
        while(1)
        {
            // recibir usuario, contrasenia y opcion.
            if ((bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0)) < 0)
            {
                perror("Error al recibir datos de usuario.\n");
                break;
            }
            else if (bytes_received == 0)
            {
                printf("Conexion finalizada por el cliente.\n");
                break;
            }
            buffer[bytes_received] = '\0';
            // separar usuario, contrasenia y opcion.
            opcion_aux = strtok(buffer, ":");
            strcpy(usuario.usuario, strtok(NULL, ":"));
            strcpy(usuario.contrasenia, strtok(NULL, ":"));
            opcion = atoi(opcion_aux);
            // procesar y responder segun la opcion.
            if (procesar_opcion(client_sock, opcion, buffer, usuario) == SALIR)
            {
                break;
            }
        }
        // cerrar socket cliente.
        close(client_sock);
    }
    // cerrar socket servidor.
    close(server_sock);
}

/*!
 * @brief   Procesa opciones seleccionadas por el cliente.
 *          Gestiona el inicio de sesion y registro de usuarios segun la opcion seleccionada por el cliente.
 * @param client_sock Descriptor del socket del cliente.
 * @param opcion      Opcion seleccionada por el cliente.
 * @param buffer      Buffer utilizado para enviar y recibir datos.
 * @param usuario     Estructura de datos del usuario (nombre de usuario y contrasenia).
 * @return OK(0) si la operacion se realiza correctamente, SALIR(-4) si el cliente finaliza la conexion.
*/
int procesar_opcion(int client_sock, int opcion, char* buffer, Cuenta usuario)
{
    if (opcion == 1) // iniciar sesion.
    {
        printf("Ingreso a iniciar sesion.\n");
        // enviar respuesta.
        snprintf(buffer, BUFFER_SIZE, "%s", validar_inicio(usuario.usuario, usuario.contrasenia));
        if (send(client_sock, buffer, strlen(buffer), 0) < 0)
        {
            perror("Error al enviar respuesta inicio de sesion.\n");
            return SALIR;
        }
        if (strcmp(buffer, EXITO) == 0)
        {
            menu_canciones_servidor(client_sock, buffer);
            return SALIR;
        }
    } else if (opcion == 2) // registrar usuario.
    {
        printf("Ingreso a registrar usuario.\n");
        snprintf(buffer, BUFFER_SIZE, "%s", (validar_registro(usuario.usuario)));
        if (strcmp(buffer, EXITO) == 0)
        {
            if (guardar_cuenta(usuario) == ERROR) // si hay error, terminamos todo.
            {
                snprintf(buffer, BUFFER_SIZE, "%s", ERROR_GUARDAR); 
                if (send(client_sock, buffer, strlen(buffer), 0) < 0)
                {
                    perror("Error al enviar respuesta validar registro.\n");
                    return SALIR;
                }
                return SALIR;
            }
            if (send(client_sock, buffer, strlen(buffer), 0) < 0)
            {
                perror("Error al enviar respuesta validar registro.\n");
                return SALIR;
            }
            menu_canciones_servidor(client_sock, buffer);
            return SALIR;
        }
        if (send(client_sock, buffer, strlen(buffer), 0) < 0)
        {
            perror("Error al enviar respuesta validar registro.\n");
            return SALIR;
        }
    }
    return OK;    
}

/*!
 * @brief   Valida las credenciales de inicio de sesion del cliente.
 *          Compara nombre de usuario y la contrasenia ingresados con los almacenados en la base de datos.
 * @param usuario     Nombre de usuario ingresado.
 * @param contrasenia Contrasenia ingresada.
 * @return Mensaje de exito o error segun el resultado de la validacion.
*/
char* validar_inicio(const char *usuario, const char *contrasenia)
{
    Cuenta validar;
    FILE* baseDatos = fopen("usuarios.db", "rb");
    if (baseDatos == NULL)
    {
        return ERROR_MEMORIA_USUARIOS;
    }

    // revisamos que este cargado algun usuario.
    if (fread(&validar, sizeof(Cuenta), 1, baseDatos) != 1)
    {
        if (ferror(baseDatos) != 0)
        {
            perror("Error al leer archivo de usuarios.\n");
            fclose(baseDatos);
            return ERROR_MEMORIA_USUARIOS;
        }
        fclose(baseDatos);
        return ERROR_VACIO_USUARIOS;
    }
    fseek(baseDatos, 0, SEEK_SET);
    // validamos usuario comparando.
    while (fread(&validar, sizeof(Cuenta), 1, baseDatos) == 1)
    {
        if (strcmp(validar.usuario, usuario) == 0 && strcmp(validar.contrasenia, contrasenia) == 0)
        {
            fclose(baseDatos);
            return EXITO;
        }
    }
    if (ferror(baseDatos) != 0)
    {
        perror("Error al leer archivo de usuarios.\n");
        fclose(baseDatos);
        return ERROR_MEMORIA_USUARIOS;
    }

    fclose(baseDatos);
    return ERROR_USUARIO_USUARIOS;
}

/*!
 * @brief   Valida si un usuario puede registrarse.
 *          Verifica que el nombre de usuario no exista en la base de datos antes de registrarlo.
 * @param usuario Nombre de usuario ingresado.
 * @return Mensaje de exito o error segun el resultado de la validacion.
*/
char* validar_registro(const char *usuario)
{
    Cuenta validar;
    FILE* baseDatos = NULL;

    if (access("usuarios.db", F_OK) == 0) // verificar si el archivo de registros ya existe.
    {
        if ((baseDatos = fopen("usuarios.db", "rb")) == NULL)
        {
            perror("Error al leer archivo de registro.\n");
            return ERROR_MEMORIA_USUARIOS;
        }
    }
    else // caso contrario, creamos uno.
    {
        if ((baseDatos = fopen("usuarios.db", "wb")) == NULL)
        {
            perror("Error al crear archivo de registro.\n");
            return ERROR_MEMORIA_USUARIOS;
        }
        fclose(baseDatos);
        return EXITO; // dado que esta vacio, sera exitosa la validacion.
    }
    // validamos que usuario a guardar no este registrado.
    while (fread(&validar, sizeof(Cuenta), 1, baseDatos) == 1)
    {
        if (strcmp(validar.usuario, usuario) == 0)
        {
            fclose(baseDatos);
            return ERROR_REGISTRO_USUARIOS;
        }
    }
    if (ferror(baseDatos) != 0)
    {
        perror("Error al leer archivo de usuarios.\n");
        fclose(baseDatos);
        return ERROR_MEMORIA_USUARIOS;
    }

    fclose(baseDatos);
    return EXITO;
}

/*!
 * @brief   Guarda un nuevo usuario en la base de datos.
 *          Escribe informacion de un nuevo usuario en el archivo db.
 * @param usuario Estructura con datos del usuario a registrar.
 * @return OK(0) si la operacion es exitosa, ERROR(-1) si ocurre un problema.
*/
int guardar_cuenta(Cuenta usuario)
{
    FILE *baseDatos = fopen("usuarios.db", "ab");
    if (baseDatos == NULL)
    {
        perror("No se pudo abrir o crear archivo de datos.\n");
        return ERROR;
    }

    if (fwrite(&usuario, sizeof(Cuenta), 1, baseDatos) != 1)
    {
        perror("Error al escribir nuevo usuario.\n");
        fclose(baseDatos);
        return ERROR;
    }
    printf("Usuario registrado exitosamente.\n");
    
    fclose(baseDatos);
    return OK;
}