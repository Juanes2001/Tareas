/*
 * RCCHunMHz.c
 *
 *  Created on: 13 nov. 2022
 *      Author: JUAN ESTEBAN
 */


#include "RCCHunMHz.h"


void RCC_enableMaxFrequencies(void){

	//Antes de configurar el PLL del RCC, debemos cambiar la velocidad de lectura de la memoria flash
	FLASH->ACR &= ~(0xF << FLASH_ACR_LATENCY_Pos);
	FLASH->ACR |= (0b0011 << FLASH_ACR_LATENCY_Pos);

	//Nos aseguramos que el PLL esta apagado para asi hacer la configuracion del mismo, ademas selecc
	RCC->CR &= ~(RCC_CR_PLLON);
	//Configuramos la salida MCO1 para verificar el estado real del MCU y su velocidad
	RCC->CFGR |= RCC_CFGR_MCO1;
	//Le damos una division a la señal que pasa por MCO1 de 5 para poder leerla en el osciloscopio
	RCC->CFGR |= RCC_CFGR_MCO1PRE;

	RCC->CFGR |= RCC_CFGR_SW_PLL;

	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL)){
		__NOP();
	}

	//Antes de configurar el PLL referenciamos cual sera la fuente para el PLL, en nuestro caso sera el HSI sobre el mismo registro
	RCC->PLLCFGR &= 0x00;

	//Montamos sobre el RCC_PLL config las subdivisiones necesarias para obtener la salida de frecuencia de reloj que deseamos
	RCC->PLLCFGR |= (8 << RCC_PLLCFGR_PLLM_Pos);
	RCC->PLLCFGR |= (100 << RCC_PLLCFGR_PLLN_Pos);
	RCC->PLLCFGR |= (2 << RCC_PLLCFGR_PLLP_Pos);

	//Seleccionamos los preescalers adecuados para los AHB y APBx en el registro RCC_CFGR
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	//Configurado el PLL para salida de 100MHz entonces ya podemos activar el PLL
	RCC->CR |= RCC_CR_PLLON;
	while (RCC->CR & RCC_CR_PLLRDY){
		__NOP();
	}
}
