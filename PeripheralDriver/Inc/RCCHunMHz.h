/*
 * RCCHunMHz.h
 *
 *  Created on: 13 nov. 2022
 *      Author: JUAN ESTEBAN
 */

#ifndef RCCHUNMHZ_H_
#define RCCHUNMHZ_H_

#include "stm32f4xx.h"


// Aqui configuraremos el RCC de manera que podamos usar el modulo PLL para incrementar la señal interna de reloj
// HSI del MCU, para asi darle una señal de reloj de alta velocidad a AHB, APB2 de 100 MHz y a APB1 de 50MHz.

void RCC_enableMaxFrequencies(void);


#endif /* RCCHUNMHZ_H_ */
