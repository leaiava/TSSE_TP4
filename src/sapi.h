#include "stdint.h"

#ifndef _SAPI_H_
#define _SAPI_H_

typedef uint8_t bool_t;

typedef uint64_t tick_t;

typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;

typedef enum {
   ADC_ENABLE, 
   ADC_DISABLE,
} adcInit_t;

typedef enum {
	   CH1 = 0, // CH1 =   2 ADC0_1/ADC1_1
	   CH2 = 1, // CH2 = 143 ADC0_2/ADC1_2
	   CH3 = 2, // CH3 = 139 ADC0_3/ADC1_3
	   CH4 = 3, // CH4 =  ADC0_4/ADC1_4
} adcMap_t;


void adcConfig ( adcInit_t config );
bool_t delayRead( delay_t * delay );
void delayInit( delay_t * delay, tick_t duration );
uint16_t adcRead( adcMap_t analogInput );

#endif /* _SAPI_H_ */