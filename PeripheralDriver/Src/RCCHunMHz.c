/*
 * RCCHunMHz.c
 *
 *  Created on: 13 nov. 2022
 *      Author: JUAN ESTEBAN
 */


#include "RCCHunMHz.h"


void RCC_enableMaxFrequencies(void){

	//Nos aseguramos que el PLL esta apagado para asi hacer la configuracion del mismo, ademas selecc
	RCC->CR &= ~(RCC_CR_PLLON);
	RCC->CFGR |= RCC_CFGR_SW_HSI;

	//Antes de configurar el PLL referenciamos cual sera la fuente para el PLL, en nuestro caso sera el HSI sobre el mismo registro
	RCC->PLLCFGR &= 0x00;

	//Montamos sobre el RCC_PLL config las subdivisiones necesarias para obtener la salida de frecuencia de reloj que deseamos
	RCC->PLLCFGR |= (8 << RCC_PLLCFGR_PLLM_Pos);
	RCC->PLLCFGR |= (100 << RCC_PLLCFGR_PLLN_Pos);
	RCC->PLLCFGR |= (2 << RCC_PLLCFGR_PLLP_Pos);

	//Seleccionamos los preescalers adecuados para los AHB y APBx en el registro RCC_CFGR
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	//Configurado el PLL para salida de 100MHz entonces ya podemos activar el PLL
	RCC->CR |= RCC_CR_PLLON;
	while (RCC->CR & RCC_CR_PLLRDY){
		__NOP();
	}
}
