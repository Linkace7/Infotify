/*!
 * @file    usuarios.h
 * @brief   Definiciones y declaraciones de funciones para la gestion de usuarios en el servidor del sistema cliente-servidor.
 * @author  Grupo 3
 * @date    18/12/2024
 * @details Este archivo contiene:
 *          - Estructuras y constantes relacionadas con la gestion de usuarios.
 *          - Declaraciones de funciones para validar, registrar y almacenar usuarios.
*/

/*!
 * @struct Cuenta
 * @brief Representa la informacion de una cuenta de usuario.
 *        Esta estructura almacena:
 *        - El nombre de usuario (maximo 25 caracteres mas terminador \0).
 *        - La contrasenia asociada (maximo 25 caracteres mas terminador \0).
*/
typedef struct Cuenta
{
    char usuario[26];     /**< Nombre de usuario. */
    char contrasenia[26]; /**< Contrasenia del usuario. */
} Cuenta;

/*!
 * @def EXITO
 * @brief Mensaje de exito al registrar un usuario.
*/
#define EXITO "Usuario registrado exitosamente."

/*!
 * @def ERROR_MEMORIA_USUARIOS
 * @brief Mensaje de error al abrir el archivo de datos.
*/
#define ERROR_MEMORIA_USUARIOS "No se pudo abrir archivo de datos."

/*!
 * @def ERROR_USUARIO_USUARIOS
 * @brief Mensaje de error al ingresar datos incorrectos.
*/
#define ERROR_USUARIO_USUARIOS "Error: Datos ingresados erroneos."

/*!
 * @def ERROR_VACIO_USUARIOS
 * @brief Mensaje de error cuando registro de usuarios esta vacio.
*/
#define ERROR_VACIO_USUARIOS "Base de datos sin usuario alguno. Registre alguno primero."

/*!
 * @def ERROR_REGISTRO_USUARIOS
 * @brief Mensaje de error al intentar registrar un usuario ya existente.
*/
#define ERROR_REGISTRO_USUARIOS "Error: Usuario ya registrado."

/*!
 * @def ERROR_GUARDAR
 * @brief Mensaje de error al guardar un nuevo usuario.
*/
#define ERROR_GUARDAR "Error al guardar nuevo usuario."

/*!
 * @brief   Establece conexion con el cliente mediante un socket.
 *          Crea un socket, configura la direccion del servidor y lo enlaza a un puerto.
 * @param server_sock Puntero al descriptor del socket del servidor.
 * @param server_ip   Direccion IP del servidor.
 * @param server_port Puerto del servidor.
 * @return OK(0) si la conexion se establece correctamente, ERROR(-1) si ocurre un problema.
*/
int conexion(int* server_sock, const char* server_ip, int server_port);

/*!
 * @brief   Bucle principal para gestionar las solicitudes del cliente.
 *          Acepta conexiones entrantes y procesa las solicitudes de los clientes, como inicio de sesion y registro.
 * @param server_sock Descriptor del socket del servidor.
*/
void menu_bucle_servidor(int server_sock);

/*!
 * @brief   Valida las credenciales de inicio de sesion del cliente.
 *          Compara el nombre de usuario y la contrasenia ingresados con los almacenados en la base de datos.
 * @param usuario     Nombre de usuario ingresado.
 * @param contrasenia Contrasenia ingresada.
 * @return Mensaje de exito o error segun el resultado de la validacion.
*/
char* validar_inicio(const char *usuario, const char *contrasenia);

/*!
 * @brief   Valida si un usuario puede registrarse.
 *          Verifica que el nombre de usuario no exista en la base de datos antes de registrarlo.
 * @param usuario Nombre de usuario ingresado.
 * @return Mensaje de exito o error segun el resultado de la validacion.
*/
char* validar_registro(const char *usuario);

/*!
 * @brief   Guarda un nuevo usuario en la base de datos.
 *          Escribe la informacion de un nuevo usuario en el archivo db.
 * @param usuario Estructura con los datos del usuario a registrar.
 * @return OK(0) si la operacion es exitosa, ERROR(-1) si ocurre un problema.
*/
int guardar_cuenta(Cuenta usuario);

/*!
 * @brief   Procesa las opciones seleccionadas por el cliente.
 *          Gestiona el inicio de sesion y registro de usuarios segun la opcion seleccionada por el cliente.
 * @param client_sock Descriptor del socket del cliente.
 * @param opcion      Opcion seleccionada por el cliente.
 * @param buffer      Buffer utilizado para enviar y recibir datos.
 * @param usuario     Datos del usuario (nombre de usuario y contrasenia).
 * @return OK(0) si la operacion se realiza correctamente, SALIR(-4) si el cliente finaliza la conexion.
*/
int procesar_opcion(int client_sock, int opcion, char* buffer, Cuenta usuario);