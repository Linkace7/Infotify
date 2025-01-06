/*!
 * @file    canciones.h
 * @brief   Definiciones y declaraciones de funciones para la gestion de canciones en el servidor del sistema cliente-servidor.
 * @author  Grupo 3
 * @date    18/12/2024
 * @details Este archivo contiene:
 *          - Definiciones de macros y constantes utilizadas en la gestion de canciones.
 *          - Declaraciones de funciones para listar, filtrar y enviar canciones solicitadas por los clientes.
*/

/*!
 * @def OK
 * @brief Codigo de retorno para indicar exito.
*/
#define OK 0

/*!
 * @def ERROR
 * @brief Codigo de retorno para indicar un error generico.
*/
#define ERROR -1

/*!
 * @def ERROR_USUARIO
 * @brief Codigo de retorno para indicar un error relacionado con el usuario.
*/
#define ERROR_USUARIO -2

/*!
 * @def ERROR_DE_MEMORIA
 * @brief Codigo de retorno para indicar un error de memoria.
*/
#define ERROR_DE_MEMORIA -3

/*!
 * @def SALIR
 * @brief Codigo de retorno para indicar que el cliente desea salir.
*/
#define SALIR -4

/*!
 * @def BUFFER_SIZE
 * @brief Tamanio maximo del buffer para enviar y recibir datos.
*/
#define BUFFER_SIZE 1024

/*!
 * @def ARTISTA
 * @brief Identificador utilizado para filtrar canciones por artista.
*/
#define ARTISTA 1

/*!
 * @def GENERO
 * @brief Identificador utilizado para filtrar canciones por genero.
*/
#define GENERO 3

/*!
 * @def LINEA_MAX
 * @brief Tamanio maximo de una linea de texto procesada.
*/
#define LINEA_MAX 256

/*!
 * @def FILTRO_MAX
 * @brief Tamanio maximo de un filtro de texto.
*/
#define FILTRO_MAX 128

/*!
 * @def FIN
 * @brief Senial utilizada para indicar el fin de la transmision de datos.
*/
#define FIN "FIN"

/*!
 * @brief   Envia una cancion solicitada por el cliente.
 *          Busca el archivo correspondiente a la cancion solicitada, verifica su existencia,
 *          y lo envia al cliente en bloques.
 * @param client_sock Descriptor del socket del cliente.
 * @param buffer      Buffer utilizado para enviar datos.
 * @return OK(0) si la cancion es enviada exitosamente, ERROR(-1) si ocurre algun problema.
*/
int escuchar_cancion_servidor(int client_sock, char* buffer);

/*!
 * @brief   Verifica si un dato cumple con el filtro ingresado.
 *          Compara el dato con el filtro ingresado por el cliente, ignorando diferencias en mayusculas/minusculas.
 * @param dato   Dato a comparar (por ejemplo, el nombre del artista o genero).
 * @param filtro Filtro ingresado por el cliente.
 * @return OK(0) si el dato cumple con el filtro, ERROR(-1) en caso contrario.
*/
int verificar(char *dato, char *filtro);

/*!
 * @brief   Filtra las canciones por un criterio especifico (artista o genero).
 *          Envia al cliente las canciones que cumplen con el criterio de filtrado ingresado.
 * @param client_sock Descriptor del socket del cliente.
 * @param buffer      Buffer utilizado para enviar datos.
 * @param sector      Indica el campo a filtrar (ARTISTA o GENERO).
 * @return OK(0) si el filtrado es exitoso, ERROR(-1) si ocurre un problema.
*/
int filtrar_servidor(int client_sock, char* buffer, int sector);

/*!
 * @brief   Menu de filtrado de canciones en el servidor.
 *          Recibe del cliente la opcion de filtrado y llama a la funcion de filtrado correspondiente.
 * @param client_sock Descriptor del socket del cliente.
 * @param buffer      Buffer utilizado para recibir datos.
 * @return OK(0) si la operacion es exitosa, ERROR(-1) si ocurre un problema.
*/
int menu_filtrar_servidor(int client_sock, char *buffer);

/*!
 * @brief   Lista las canciones disponibles en el servidor.
 *          Envia el listado de canciones al cliente conectado. Lee las canciones desde un archivo CSV
 *          y las envia linea por linea.
 * @param client_sock Descriptor del socket del cliente.
 * @param buffer      Buffer utilizado para enviar datos.
 * @return OK(0) si el listado es exitoso, ERROR(-1) si ocurre algun problema.
*/
int listar_servidor(int client_sock, char* buffer);

/*!
 * @brief   Menu principal del servidor para gestionar opciones de canciones.
 *          Recibe las opciones seleccionadas por el cliente y llama a las funciones correspondientes
 *          para listar, filtrar o enviar canciones.
 * @param client_sock Descriptor del socket del cliente conectado.
 * @param buffer      Buffer utilizado para enviar y recibir datos.
*/
void menu_canciones_servidor(int client_sock, char* buffer);