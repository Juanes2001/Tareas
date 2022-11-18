/**
 ******************************************************************************
 * Solucion Tarea 5
 * Autor: Juan Esteban Rodriguez Ochoa
 * Correo: jrodriguezoc@unal.edu.co
 ******************************************************************************
 ******************************************************************************
 */
#include "BasicTimer.h"
#include "GPIOxDriver.h"
#include "USARTxDriver.h"
#include "EXTIDriver.h"
#include "PwmDriver.h"
#include "I2CDriver.h"
#include "AdcDriver.h"
#include "OLEDDriver.h"
#include "RTCDriver.h"

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Pines controladores para el motor
GPIO_Handler_t handlerPINF1 = {0};
GPIO_Handler_t handlerPINF2 = {0};
GPIO_Handler_t handlerPINF3 = {0};
GPIO_Handler_t handlerPINF4 = {0};
GPIO_Handler_t handlerPINF5 = {0};
GPIO_Handler_t handlerPINF6 = {0};


GPIO_Handler_t handlerUSARTPinTx       = {0};
GPIO_Handler_t handlerUSARTPinRx       = {0};

//Timer para controlador de velocidades
BasicTimer_Handler_t handlerTIMMotor = {0};

USART_Handler_t handlerUSART2    = {0};


uint8_t auxData  = '\0';
char bufferData[64];





void initSystem(void);

int main (void){

	initSystem();

	while(1){

		if (auxData != '\0'){
			if (auxData == '1'){
				GPIOxTooglePin(&handlerPINF1);
				sprintf(bufferData,"1 = %u \n\r",(unsigned int) GPIO_ReadPin(&handlerPINF1));
				writeMsg(&handlerUSART2, bufferData);
				auxData = '\0';

			}else if (auxData == '2'){
				GPIOxTooglePin(&handlerPINF2);
				sprintf(bufferData,"2 = %u \n\r",(unsigned int) GPIO_ReadPin(&handlerPINF2));
				writeMsg(&handlerUSART2, bufferData);
				auxData = '\0';
			}else if (auxData == '3'){
				GPIOxTooglePin(&handlerPINF3);
				sprintf(bufferData,"3 = %u \n\r",(unsigned int) GPIO_ReadPin(&handlerPINF3));
				writeMsg(&handlerUSART2, bufferData);
				auxData = '\0';
			}else if (auxData == '4'){
				GPIOxTooglePin(&handlerPINF4);
				sprintf(bufferData,"4 = %u \n\r",(unsigned int) GPIO_ReadPin(&handlerPINF4));
				writeMsg(&handlerUSART2, bufferData);
				auxData = '\0';
			}else if (auxData == '5'){
				GPIOxTooglePin(&handlerPINF5);
				sprintf(bufferData,"5 = %u \n\r",(unsigned int) GPIO_ReadPin(&handlerPINF5));
				writeMsg(&handlerUSART2, bufferData);
				auxData = '\0';
			}else if (auxData == '6'){
				GPIOxTooglePin(&handlerPINF6);
				sprintf(bufferData,"6 = %u \n\r",(unsigned int) GPIO_ReadPin(&handlerPINF6));
				writeMsg(&handlerUSART2, bufferData);
				auxData = '\0';
			}
		}

	}

}

void initSystem(void){

	handlerPINF1.pGPIOx = GPIOA;
	handlerPINF1.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINF1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINF1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINF1.GPIO_PinConfig.GPIO_PinNumber = PIN_0;
	handlerPINF1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINF1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINF1);
	GPIO_WritePin(&handlerPINF1, RESET);

	handlerPINF2.pGPIOx = GPIOA;
	handlerPINF2.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINF2.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINF2.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINF2.GPIO_PinConfig.GPIO_PinNumber = PIN_1;
	handlerPINF2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINF2.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINF2);
	GPIO_WritePin(&handlerPINF2, RESET);

	handlerPINF3.pGPIOx = GPIOB;
	handlerPINF3.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINF3.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINF3.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINF3.GPIO_PinConfig.GPIO_PinNumber = PIN_2;
	handlerPINF3.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINF3.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINF3);
	GPIO_WritePin(&handlerPINF3, RESET);

	handlerPINF4.pGPIOx = GPIOB;
	handlerPINF4.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINF4.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINF4.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINF4.GPIO_PinConfig.GPIO_PinNumber = PIN_3;
	handlerPINF4.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINF4.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINF4);
	GPIO_WritePin(&handlerPINF4, RESET);

	handlerPINF5.pGPIOx = GPIOA;
	handlerPINF5.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINF5.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINF5.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINF5.GPIO_PinConfig.GPIO_PinNumber = PIN_4;
	handlerPINF5.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINF5.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINF5);
	GPIO_WritePin(&handlerPINF5, RESET);

	handlerPINF6.pGPIOx = GPIOA;
	handlerPINF6.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINF6.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINF6.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINF6.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerPINF6.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINF6.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINF6);
	GPIO_WritePin(&handlerPINF6, RESET);


	handlerTIMMotor.ptrTIMx = TIM2;
	handlerTIMMotor.TIMx_Config.TIMx_interruptEnable = 1;
	handlerTIMMotor.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerTIMMotor.TIMx_Config.TIMx_period = 2500;
	handlerTIMMotor.TIMx_Config.TIMx_speed = BTIMER_SPEED_10us;
	BasicTimer_Config(&handlerTIMMotor);

	handlerUSARTPinTx.pGPIOx = GPIOA;
	handlerUSARTPinTx.GPIO_PinConfig.GPIO_PinAltFunMode = AF7;
	handlerUSARTPinTx.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerUSARTPinTx.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerUSARTPinTx.GPIO_PinConfig.GPIO_PinNumber = PIN_2;
	handlerUSARTPinTx.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerUSARTPinTx.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerUSARTPinTx);

	handlerUSARTPinRx.pGPIOx = GPIOA;
	handlerUSARTPinRx.GPIO_PinConfig.GPIO_PinAltFunMode = AF7;
	handlerUSARTPinRx.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerUSARTPinRx.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerUSARTPinRx.GPIO_PinConfig.GPIO_PinNumber = PIN_3;
	handlerUSARTPinRx.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerUSARTPinRx.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerUSARTPinRx);

	handlerUSART2.ptrUSARTx                      = USART2;
	handlerUSART2.USART_Config.USART_baudrate    = USART_BAUDRATE_115200;
	handlerUSART2.USART_Config.USART_enableInRx  = USART_INTERRUPT_RX_ENABLE;
	handlerUSART2.USART_Config.USART_enableInTx  = USART_INTERRUPT_TX_DISABLE;
	handlerUSART2.USART_Config.USART_mode        = USART_MODE_RXTX;
	handlerUSART2.USART_Config.USART_parity      = USART_PARITY_NONE;
	handlerUSART2.USART_Config.USART_stopbits    = USART_STOPBIT_1;
	handlerUSART2.USART_Config.USART_datasize    = USART_DATASIZE_8BIT;
	handlerUSART2.USART_Config.USART_parityError = 0;
	USART_Config(&handlerUSART2);

}

//void BasicTimer2_Callback(void){
//
//}

void usart2Rx_Callback(void){
	auxData = getRxData();
}

