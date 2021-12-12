/*
 * mi_ADC.c
 *
 *  Created on: Aug 5, 2021
 *      Author: lea
 */

#include "mi_ADC.h"

void ADCinicializarMEF(miADC_t* ptrmiADCs){
	adcConfig( ADC_ENABLE );
	ptrmiADCs->estado = ESTADO_STANDBY;
	ptrmiADCs->CH1 = false;
	ptrmiADCs->CH2 = false;
	ptrmiADCs->CH3 = false;
	ptrmiADCs->frecuencia = 0;
	ptrmiADCs->FlagComandoRecibido = false;
}

void ADCactualizarMEF(miADC_t* ptrmiADCs){
	cmd_t command;

	switch (ptrmiADCs->estado){

	case ESTADO_STANDBY:{

		if (UART_getCmd(&command) ){

			if (command == CMD_HOLA){
				UART_sendCmd(CMD_OK);
				ptrmiADCs->FlagComandoRecibido = true;
				ptrmiADCs->estado = ESTADO_CONECTADO;
			}
			else {
				UART_sendCmd(CMD_NO_OK);
			}
		}
	}
	break;

	case ESTADO_CONECTADO:{

		if (ADClisto(ptrmiADCs))
			ptrmiADCs->estado = ESTADO_LISTO_PARA_ADQUIRIR;

		if (UART_getCmd(&command) ){
			switch (command) {
			case CMD_CONFIG_INICIAR:{
				UART_sendCmd(CMD_OK);
				ptrmiADCs->FlagComandoRecibido = true;
				ptrmiADCs->estado = ESTADO_CONFIGURANDO;
			}
			break;
			case CMD_CHAU:{
				UART_sendCmd(CMD_OK);
				ADCinicializarMEF( ptrmiADCs );
				ptrmiADCs->FlagComandoRecibido = true;
				ptrmiADCs->estado = ESTADO_STANDBY;
			break;
			}
			default:{
				UART_sendCmd(CMD_NO_OK);
			}
			}
		}
	}
	break;
	case ESTADO_CONFIGURANDO:{

		if (UART_getCmd(&command) ){
			if (command == CMD_CONFIG_TERMINAR){
				ptrmiADCs->FlagComandoRecibido = true;
				ptrmiADCs->estado = ESTADO_CONECTADO;
			}
			else if (command >= CMD_CONFIG_CH1_ON && command <= CMD_CONFIG_FREC_7 ){ //verifico que sea un comando de configuracion
				ADCguardarconfiguracion(ptrmiADCs,command);
				ptrmiADCs->FlagComandoRecibido = true;
				UART_sendCmd(CMD_OK);
			}
			else  {
				UART_sendCmd(CMD_NO_OK);
			}
		}
	}
	break;
	case ESTADO_LISTO_PARA_ADQUIRIR:{

		if (UART_getCmd(&command) ){
			switch (command) {
			case CMD_CONFIG_INICIAR:{
				UART_sendCmd(CMD_OK);
				ptrmiADCs->FlagComandoRecibido = true;
				ptrmiADCs->estado = ESTADO_CONFIGURANDO;
			}
			break;
			case CMD_ADQUIRIR:{
				UART_sendCmd(CMD_OK);
				ptrmiADCs->FlagComandoRecibido = true;
				ptrmiADCs->estado = ESTADO_ADQUIRIENDO;
			}
			break;
			case CMD_CHAU:{
				UART_sendCmd(CMD_OK);
				ADCinicializarMEF( ptrmiADCs );
				ptrmiADCs->FlagComandoRecibido = true;
				ptrmiADCs->estado = ESTADO_STANDBY;
			}
			break;
			default: {
				UART_sendCmd(CMD_NO_OK);

			}
			break;
			}
		}
	}
	break;

	case ESTADO_ADQUIRIENDO:{

		if ( delayRead( &(ptrmiADCs->delay)) ){
			ADCadquirir(ptrmiADCs);
		UART_sendPacket(ptrmiADCs->paquete);
		}

		if (UART_getCmd(&command) ){
			if (command == CMD_PARAR){
				ptrmiADCs->estado = ESTADO_LISTO_PARA_ADQUIRIR;
			}
			else {
				UART_sendCmd(CMD_OCUPADO);
			}
		}
	}
	break;
	default:;
	}
}

static void ADCguardarconfiguracion(miADC_t* ptrmiADCs, cmd_t command){
	switch (command) {

	case CMD_CONFIG_CH1_OFF:{
		ptrmiADCs->CH1 = false;
	}
	break;
	case CMD_CONFIG_CH1_ON:{
		ptrmiADCs->CH1 = true;
	}
	break;
	case CMD_CONFIG_CH2_OFF:{
		ptrmiADCs->CH2 = false;
	}
	break;
	case CMD_CONFIG_CH2_ON:{
		ptrmiADCs->CH2 = true;
	}
	break;
	case CMD_CONFIG_CH3_OFF:{
		ptrmiADCs->CH3 = false;
	}
	break;
	case CMD_CONFIG_CH3_ON:{
		ptrmiADCs->CH3 = true;
		UART_sendCmd(CMD_OK);
	}
	break;
	case CMD_CONFIG_FREC_1:{
		ptrmiADCs->frecuencia = 1;
		delayInit( &(ptrmiADCs->delay), FRECUENCIA_MUESTREO_62_5 );
	}
	break;
	case CMD_CONFIG_FREC_2:{
		ptrmiADCs->frecuencia = 2;
		delayInit( &(ptrmiADCs->delay), FRECUENCIA_MUESTREO_125 );
	}
	break;
	case CMD_CONFIG_FREC_3:{
		ptrmiADCs->frecuencia = 3;
		delayInit( &(ptrmiADCs->delay), FRECUENCIA_MUESTREO_250 );
	}
	break;
	case CMD_CONFIG_FREC_4:{
		ptrmiADCs->frecuencia = 4;
		delayInit( &(ptrmiADCs->delay), FRECUENCIA_MUESTREO_500 );
	}
	break;
	case CMD_CONFIG_FREC_5:{
		ptrmiADCs->frecuencia = 5;
		delayInit( &(ptrmiADCs->delay), FRECUENCIA_MUESTREO_1000 );
	}
	break;
	case CMD_CONFIG_FREC_6:{
		ptrmiADCs->frecuencia = 6;
		delayInit( &(ptrmiADCs->delay), FRECUENCIA_MUESTREO_2000 );
	}
	break;
	case CMD_CONFIG_FREC_7:{
		ptrmiADCs->frecuencia = 7;
		delayInit( &(ptrmiADCs->delay), FRECUENCIA_MUESTREO_5000 );
	}
	break;
	default:
		UART_sendCmd(CMD_NO_OK);
	break;
	}
}

static void ADCadquirir(miADC_t* ptrmiADCs){
	if(ptrmiADCs->CH1){
		ptrmiADCs->paquete.data_CH1 = adcRead(CH1);
	}
	if(ptrmiADCs->CH2){
		ptrmiADCs->paquete.data_CH2 = adcRead(CH2);
	}
	if(ptrmiADCs->CH3){
		ptrmiADCs->paquete.data_CH3 = adcRead(CH3);
	}

}

static bool_t ADClisto(miADC_t* ptrmiADCs){
	if ( ptrmiADCs->frecuencia == 0 )
		return false;
	if ( ptrmiADCs->CH1 || ptrmiADCs->CH2 || ptrmiADCs->CH3 )
		return true;
	else
		return false;
}
