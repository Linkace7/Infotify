/*!
 * @file    canciones.h
 * @brief   Definiciones y declaraciones de funciones para la gestion de canciones en el cliente.
 * @author  Grupo 3
 * @date    18/12/2024
 * @details Este archivo contiene:
 *          - Macros y constantes utilizadas en la gestion de canciones.
 *          - Declaraciones de funciones para listar, filtrar y pasar canciones.
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
 * @def SERVER_PORT
 * @brief Puerto del servidor al que se conecta el cliente.
*/
#define SERVER_PORT 9090

/*!
 * @def SERVER_IP
 * @brief Direccion IP del servidor al que se conecta el cliente.
*/
#define SERVER_IP "127.0.0.1"

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
 * @brief Senial utilizada para indicar el fin de transmision de datos.
*/
#define FIN "FIN"

/*!
 * @brief   Muestra menu de opciones para gestionar canciones.
 *          Presenta opciones disponibles (listar, filtrar, escuchar o salir)
 *          y valida que la entrada sea correcta.
 * @return  Opcion seleccionada por el cliente.
*/
int op_menu_canciones(void);

/*!
 * @brief   Muestra menu de canciones y gestiona las opciones seleccionadas.
 *          Este menu permite listar canciones, filtrarlas o reproducirlas. Envia las opciones seleccionadas al servidor
 *          y maneja las respuestas correspondientes.
 * @param sock   Descriptor del socket de conexion con el servidor.
 * @param buffer Buffer utilizado para enviar y recibir datos.
*/
void menu_canciones_cliente(int sock, char* buffer);

/*!
 * @brief   Lista las canciones disponibles en el servidor.
 *          Envia una solicitud al servidor para obtener el listado de canciones y muestra los resultados en pantalla.
 * @param sock   Descriptor del socket de conexion con el servidor.
 * @param buffer Buffer utilizado para recibir datos.
 * @return OK(0) si el listado es exitoso, ERROR(-1) si ocurre un error.
*/
int listar_cliente(int sock, char* buffer);

/*!
 * @brief   Muestra opciones de filtrado al cliente.
 *          Presenta un menu con opciones disponibles para filtrar canciones (por artista o genero)
 *          y valida que la entrada sea correcta.
 * @return  Opcion seleccionada por el cliente (1 para artista, 2 para genero).
*/
int op_filtrar(void);

/*!
 * @brief   Envia una opcion de filtro al servidor y solicita canciones filtradas.
 *          Permite al cliente seleccionar un criterio de filtrado (por artista o por genero)
 *          y envia esta opcion al servidor. Luego llama a filtrar_cliente() para manejar los resultados
 *          recibidos desde el servidor.
 * @param sock   Descriptor del socket de conexion con el servidor.
 * @param buffer Buffer utilizado para enviar y recibir datos.
 * @return OK(0) si la operacion es exitosa, ERROR(-1) si ocurre un error.
*/
int menu_filtrar_cliente(int sock, char* buffer);

/*!
 * @brief   Solicita y envia un filtro para listar canciones especificas.
 *          Envia un filtro al servidor y recibe las canciones que cumplen con ese criterio.
 * @param sock   Descriptor del socket de conexion con el servidor.
 * @param buffer Buffer utilizado para enviar y recibir datos.
 * @return OK(0) si la operacion es exitosa, ERROR(-1) si ocurre un error.
*/
int filtrar_cliente(int sock, char *buffer);

/*!
 * @brief   Solicita y descarga una cancion desde el servidor.
 *          Envia al servidor el numero de la cancion solicitada, recibe los datos de la cancion y los guarda en un archivo local.
 * @param sock   Descriptor del socket de conexion con el servidor.
 * @param buffer Buffer utilizado para enviar y recibir datos.
 * @return OK(0) si la operacion es exitosa, ERROR(-1) si ocurre un error.
*/
int escuchar_cancion_cliente(int sock, char* buffer);