/*
 * mi_uart.h
 *
 *  Created on: Aug 5, 2021
 *      Author: lea
 */

#ifndef _MI_UART_H_
#define _MI_UART_H_

#include "sapi.h"
//! Se define la velocidad de la UART
#define	UART_VELOCIDAD	1152000

/*!
 * @Brief Lista de todos los comandos que se reciben y envían por la UART
 */
typedef enum {
	CMD_HOLA = '1', //!< Iniciar la comunicación. - 1
	CMD_OK, //!< Se interpreto bien el comando recibido - 2
	CMD_NO_OK, //!< No se interpreto el comando recibido - 3
	CMD_CHAU, //!< Terminar comunicacion - 4
	CMD_CONFIG_INICIAR, //!< Inicia configuracion - 5
	CMD_CONFIG_TERMINAR, //!< Termina configuracion - 6
	CMD_CONFIG_CH1_ON, //!< Canal 1 habilitdo - 7
	CMD_CONFIG_CH1_OFF, //!< Canal 1 deshabilitado - 8
	CMD_CONFIG_CH2_ON, //!< Canal 2 habilitdo - 9
	CMD_CONFIG_CH2_OFF, //!< Canal 2 deshabilitado - :
	CMD_CONFIG_CH3_ON, //!< Canal 3 habilitdo - ;
	CMD_CONFIG_CH3_OFF, //!< Canal 3 deshabilitado - <
	CMD_CONFIG_FREC_1, //!< Configura ADC con frecuencia 1 - =
	CMD_CONFIG_FREC_2, //!< Configura ADC con frecuencia 2 - >
	CMD_CONFIG_FREC_3, //!< Configura ADC con frecuencia 3 - ?
	CMD_CONFIG_FREC_4, //!< Configura ADC con frecuencia 4 - @
	CMD_CONFIG_FREC_5, //!< Configura ADC con frecuencia 5 - A
	CMD_CONFIG_FREC_6, //!< Configura ADC con frecuencia 6 - B
	CMD_CONFIG_FREC_7, //!< Configura ADC con frecuencia 7 - C
	CMD_ADQUIRIR, //!< Inicia la adquisición - D
	CMD_PARAR, //!< Para la adquisición - E
	CMD_OCUPADO, //!< Respuesta si no puede atender un comando - F
	CMD_LEER_ESTADO, //!< Pregunta en que estado se encuentra - G
}cmd_t;

/*!
 * En este tipo de variable se guardan los datos convertidos por los ADCs
 */
typedef struct {
	uint16_t data_CH1; //!< Data del canal 1
	uint16_t data_CH2; //!< Data del canal 2
	uint16_t data_CH3; //!< Data del canal 3
}packet_t;

/*!
 * @Brief	Inicializa la UART
 * Selecciona por default la UART_USB y la velocidad según un define
 */
void UART_inicializar(void);

/*!
 * @brief	Lee comandos de la UART inicializada con uartInit
 * Depende de sapi.h
 * @param command Puntero a la direccion de memoria donde guardar el comando que llega.
 * @return TRUE Si había un comando para leer.
 * @return FALSE Si no había nada para leer.
 */
bool_t UART_getCmd(cmd_t* command);

/*!
 * @brief Envía un comando para la UART configurada con uartInit
 * @param command Comando a ser enviado de la lista cmd_t
 */
void UART_sendCmd(cmd_t command);

/*!
 * @Brief Envía un packete de datos por la UART configurada con uartInit
 * @param paquete Paquete del tipo packet_t a ser enviado
 */
void UART_sendPacket(packet_t paquete);

#endif /* _MI_UART_H_ */
