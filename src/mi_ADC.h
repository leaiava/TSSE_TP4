/*
 * mi_ADC.h
 *
 *  Created on: Aug 5, 2021
 *      Author: lea
 */

#ifndef _MI_ADC_H_
#define _MI_ADC_H_
#include "sapi.h"
#include "mi_uart.h"
#include "stdbool.h"

/*
 * 65 Hz, 15384,6us -> 16mS -> 62,5Hz
 * 131 Hz, 7633,6us -> 8mS -> 125Hz
 * 262 Hz, 3816,8us -> 4mS -> 250Hz
 * 524 Hz, 1908,4us -> 2mS -> 500Hz
 * 1048 Hz, 954,2us -> 1mS -> 1KHz
 * 2096 Hz, 477,1us -> 0.5mS -> 2KHz
 * 4193 Hz, 238,5us -> 0.2mS -> 5KHz
 * El systick interrumpe cada 0,1mS
 * Los siguientes tiempos están expresados en mS*10
 */
#define	FRECUENCIA_MUESTREO_62_5	160	//!< 16mS entre muestras
#define	FRECUENCIA_MUESTREO_125		80  //!< 8mS entre muestras
#define	FRECUENCIA_MUESTREO_250		40	//!< 4mS entre muestras
#define	FRECUENCIA_MUESTREO_500		20	//!< 2mS entre muestras
#define	FRECUENCIA_MUESTREO_1000	10	//!< 1mS entre muestras
#define	FRECUENCIA_MUESTREO_2000	5	//!< 0.5mS entre muestras
#define	FRECUENCIA_MUESTREO_5000	2	//!< 0.2mS entre muestras


/*!
 * @Brief Lista de todos los estados posibles del dispositivo
 */
typedef enum {
	ESTADO_STANDBY,						//!< Estado inicial luego del reset
	ESTADO_CONECTADO,					//!< Cuando establece conexión con un dispositivo externo
	ESTADO_CONFIGURANDO,				//!< Estado para configurar cantidad de canales y frecuencia de sampling
	ESTADO_LISTO_PARA_ADQUIRIR,			//!< Si tiene configurado una frecuencia de muestreo y canales a usar queda en este estado
	ESTADO_ADQUIRIENDO					//!< Adquiriendo y enviando datos por la UART.
}mis_estados_t;

/*!
 * @Brief Estructura de datos para manejar la MEF que controla los ADCs
 */
typedef struct{
	mis_estados_t estado;				//!< Variable que contiene el estado del dispositivo
	bool_t	CH1;						//!< Indica si el canal 1 esta encendido (TRUE) o apagado (FALSE)
	bool_t	CH2;						//!< Indica si el canal 2 esta encendido (TRUE) o apagado (FALSE)
	bool_t	CH3;						//!< Indica si el canal 3 esta encendido (TRUE) o apagado (FALSE)
	uint32_t frecuencia;				//!< Indica la frecuencia de muestreo a utilizar
	packet_t paquete;					//!< Variable para guardar los datos de los ADCs
	delay_t delay;						//!< Guardo el delay necesario para obtener la frecuenia de muestreo configurada
	bool_t FlagComandoRecibido;			//!< Flag que indica en TRUE cuando llegó un comando válido
}miADC_t;

/*!
 * @Brief Inicializa la MEF que controlará el funcionamiento de los ADCs
 * @param ptrmiADCs Puntero a la estructura de datos de control
 */
void ADCinicializarMEF(miADC_t* ptrmiADCs);

/*!
 * @Brief Actualiza el estado de la MEF que controla el funcionamiento de los ADCs
 *
 * @param ptrmiADCs Puntero a la estructura de datos de control
 */
void ADCactualizarMEF(miADC_t* ptrmiADCs);

/*!
 * @Brief Verifica si el ADC esta listo para adquirir
 *
 * @param ptrmiADCs Estructura que tiene toda la información del ADC
 * @return TRUE si está listo para adquirir
 * @return FALSE si no está listo para adquirir
 */
static bool_t ADClisto(miADC_t* ptrmiADCs);
/*!
 * @Brief ejecuta accion de configuracion
 *
 * @param command Comando que llegó por la UART
 * @param ptrmiADCs  Estructura que tiene toda la información del ADC
 */
static void ADCguardarconfiguracion(miADC_t* ptrmiADCs, cmd_t command);

/*!
 * @Brief Adquiere de los ADCs que se hayan configurado y guarda los datos en la estructura
 *
 * @param ptrmiADCs  Estructura que tiene toda la información del ADC
 */
static void ADCadquirir(miADC_t* ptrmiADCs);

#endif /* _MI_ADC_H_ */
