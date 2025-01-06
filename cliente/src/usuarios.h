/*!
 * @file    usuarios.h
 * @brief   Definiciones y estructuras relacionadas con la gestion de usuarios en el sistema cliente-servidor.
 * @author  Grupo 3
 * @date    18/12/2024
 * @details Este archivo contiene definiciones de constantes, estructuras y declaraciones de funciones
 *          relacionadas con el manejo de datos de usuarios, como nombres de usuario y contrasenias.
*/

/*!
 * @def EXITO
 * @brief Mensaje de exito al registrar un usuario.
*/
#define EXITO "Usuario registrado exitosamente."

/*!
 * @def ERROR_MEMORIA_USUARIOS
 * @brief Mensaje de error al abrir el archivo csv.
*/
#define ERROR_MEMORIA_USUARIOS "No se pudo abrir archivo de datos."

/*!
 * @def ERROR_USUARIO_USUARIOS
 * @brief Mensaje de error al ingresar datos incorrectos.
*/
#define ERROR_USUARIO_USUARIOS "Error: Datos ingresados erroneos."

/*!
 * @def ERROR_VACIO_USUARIOS
 * @brief Mensaje de error cuando el archivo de registro esta vacio.
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
 * @struct Cuenta
 * @brief Representa la informacion de una cuenta de usuario.
 *        Esta estructura almacena:
 *        - Nombre del usuario.
 *        - Contrasenia asociada a ese usuario.
*/
typedef struct Cuenta
{
    char usuario[26];        /**< @brief Nombre de usuario (maximo 25 caracteres mas terminador \0). */
    char contrasenia[26];    /**< @brief Contrasenia del usuario (maximo 25 caracteres mas terminador \0). */
} Cuenta;

/*!
 * @brief   Solicita al usuario ingresar su nombre de usuario y contrasenia.
 *          Se utiliza para ingresar los datos del usuario con una longitud limitada.
 *          Los valores ingresados se almacenan en las cadenas proporcionadas como parametros.
 * @param usuario     Puntero a la cadena donde se almacenara el nombre de usuario ingresado.
 * @param contrasenia Puntero a la cadena donde se almacenara la contrasenia ingresada.
*/
void ingresar_datos(char *usuario, char *contrasenia);