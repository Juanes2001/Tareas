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
GPIO_Handler_t handlerPinRx     = {0};
GPIO_Handler_t handlerPinTx  = {0};

// handler encargado del PWM
PWM_Handler_t handlerPWM = {0};

USART_Handler_t handlerUSART2 = {0};

uint8_t rxData = '\0';
char bufferchar[64];
uint8_t counter = 0;








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

		if (rxData == 'u'){

			updateDuttyCycle(&handlerPWM, handlerPWM.config.duttyCicle + 1);
			 rxData = '\0';
		}else if (rxData == 'd'){

			updateDuttyCycle(&handlerPWM,handlerPWM.config.duttyCicle - 1);
			 rxData = '\0';
		}else{
			rxData = '\0';
		}


	}

}

/*
 * Funcion encargada de la inicializacion de los elementos del sistema
 */
void iniSystem(void){


	handlerPinRx.pGPIOx 							= GPIOA;
	handlerPinRx.GPIO_PinConfig.GPIO_PinNumber		= PIN_3;
	handlerPinRx.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinRx.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinRx.GPIO_PinConfig.GPIO_PinPuPdControl= GPIO_PUPDR_NOTHING;
	handlerPinRx.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEEDR_FAST;
	handlerPinRx.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinRx);


	handlerPinTx.pGPIOx 							= GPIOA;
	handlerPinTx.GPIO_PinConfig.GPIO_PinNumber		= PIN_2;
	handlerPinTx.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinTx.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinTx.GPIO_PinConfig.GPIO_PinPuPdControl= GPIO_PUPDR_NOTHING;
	handlerPinTx.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEEDR_FAST;
	handlerPinTx.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinTx);


	handlerUSART2.ptrUSARTx = USART2;
	handlerUSART2.USART_Config.USART_baudrate = USART_BAUDRATE_115200;
	handlerUSART2.USART_Config.USART_datasize = USART_DATASIZE_8BIT;
	handlerUSART2.USART_Config.USART_enableInRx = USART_INTERRUPT_RX_ENABLE;
	handlerUSART2.USART_Config.USART_enableInTx = USART_INTERRUPT_TX_DISABLE;
	handlerUSART2.USART_Config.USART_mode = USART_MODE_RXTX;
	handlerUSART2.USART_Config.USART_parity = USART_PARITY_NONE;
	handlerUSART2.USART_Config.USART_stopbits = USART_STOPBIT_1;

	USART_Config(&handlerUSART2);


	/* Configurando el pin sobre el que funciona el PWM */
	handlerPWMPin.pGPIOx 							= GPIOA;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinNumber		= PIN_6;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinPuPdControl= GPIO_PUPDR_NOTHING;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEEDR_FAST;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinAltFunMode	= AF2;
	GPIO_Config(&handlerPWMPin);


	handlerPWM.ptrTIMx = TIM3;
	handlerPWM.config.channel = PWM_CHANNEL_1;
	handlerPWM.config.duttyCicle= 50;
	handlerPWM.config.periodo = 200;
	handlerPWM.config.prescaler = BTIMER_SPEED_1ms;

	pwm_Config(&handlerPWM);
	startPwmSignal(&handlerPWM);






}

void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerPWMPin);
}

void usart2Rx_Callback(void){
	rxData= getRxData();
}

