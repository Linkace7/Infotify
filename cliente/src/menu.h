/*!
 * @file    menu.h
 * @brief   Declaraciones de funciones para la conexion y gestion del menu del cliente.
 * @author  Grupo 3
 * @date    18/12/2024
 * @details Contiene declaraciones de las funciones necesarias para:
 *          - Establecer una conexion TCP con el servidor.
 *          - Mostrar y gestionar el menu principal del cliente.
*/

/*!
 * @brief   Establece una conexion TCP con el servidor.
 *          Esta funcion crea un socket, configura la direccion del servidor y establece una conexion.
 * @return  Retorna el descriptor del socket si la conexion es exitosa.
 *          Retorna ERROR (-1) si ocurre un error.
*/
int conexion(void);

/*!
 * @brief   Muestra el menu principal del cliente y gestiona sus opciones.
 *          Presenta opciones disponibles al cliente, como iniciar sesion, registrarse o salir.
 *          Gestiona la comunicacion con el servidor para validar credenciales y realizar operaciones.
 * @param sock Descriptor del socket de conexion con el servidor.
*/
void menu_cliente(int sock);

/*!
 * @brief   Muestra las opciones del menu al cliente.
 *          Presenta un menu de opciones y valida que el usuario ingrese un valor valido.
 * @return  Retorna la opcion seleccionada por el cliente (1, 2 o 3).
*/
int op_menu(void);