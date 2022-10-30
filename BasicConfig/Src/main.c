/**
 **************************************************************************************************
 * @file           : MainTaller7Parte1.c
 * @authors        : Alejandro Vélez Patrouilleau - alvelezp@unal.edu.co
 * 				   : Juan Pablo Toro Arenas		  - jutoroa@unal.edu.co
 * @brief          : Introduccion al sprintF y a la conversión ADC:
 *
 **************************************************************************************************
 */

#include <stm32f4xx.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "GPIOxDriver.h"
#include "BasicTimer.h"
#include "AdcDriver.h"
#include "USARTxDriver.h"
#include "PwmDriver.h"
#include "I2CDriver.h"
#include "EXTIDriver.h"


// Definicion de los handlers necesarios
GPIO_Handler_t handlerPWMPin		= {0};

// handler encargado del PWM
PWM_Handler_t handlerPWM = {0};




/* Definición de prototipos de funciones */
void iniSystem(void);

/**
 * Funcion principal del programa.
 * Esta funcion es el corazón del programa!!
 * */
int main(void) {

	// Inicializamos todos los elementos del sistema
	iniSystem();

	while(1){

	}

}

/*
 * Funcion encargada de la inicializacion de los elementos del sistema
 */
void iniSystem(void){


	/* Configurando el pin sobre el que funciona el PWM */
	handlerPWMPin.pGPIOx 							= GPIOA;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinNumber		= PIN_1;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinPuPdControl= GPIO_PUPDR_NOTHING;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEEDR_MEDIUM;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinAltFunMode	= AF1;
	GPIO_Config(&handlerPWMPin);


	handlerPWM.ptrTIMx = TIM2;
	handlerPWM.config.channel = PWM_CHANNEL_2;
	handlerPWM.config.duttyCicle= 20;
	handlerPWM.config.periodo = 2000;
	handlerPWM.config.prescaler = BTIMER_SPEED_100us;

	pwm_Config(&handlerPWM);
	startPwmSignal(&handlerPWM);






}

