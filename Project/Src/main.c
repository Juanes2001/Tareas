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
GPIO_Handler_t handlerPINT1 = {0};
GPIO_Handler_t handlerPINT2 = {0};
GPIO_Handler_t handlerPINT3 = {0};
GPIO_Handler_t handlerPINT4 = {0};
GPIO_Handler_t handlerPINT5 = {0};
GPIO_Handler_t handlerPINT6 = {0};


GPIO_Handler_t handlerUSARTPinTx       = {0};
GPIO_Handler_t handlerUSARTPinRx       = {0};

//Timer para controlador de velocidades
BasicTimer_Handler_t handlerTIMMotor = {0};

USART_Handler_t handlerUSART2    = {0};


uint8_t auxData  = '\0';
char bufferData[64];
uint8_t flag = RESET;
uint8_t counterMotor = 0;





void initSystem(void);
void startMotor(void);
void updateRPMMotor(uint8_t period);

int main (void){

	initSystem();

	while(1){

		// Aqui se almacenara la secuencia de giro la cual se ejecutara repetidas veces para hacer girar el motor
		if (flag){
			switch (counterMotor) {
				case 1:

					GPIO_WritePin(&handlerPINT1, SET);
					GPIO_WritePin(&handlerPINT2, RESET);
					GPIO_WritePin(&handlerPINT3, RESET);
					GPIO_WritePin(&handlerPINT4, RESET);
					GPIO_WritePin(&handlerPINT5, RESET);
					GPIO_WritePin(&handlerPINT6, SET);


					break;
				case 2:

					GPIO_WritePin(&handlerPINT1, RESET);
					GPIO_WritePin(&handlerPINT2, SET);
					GPIO_WritePin(&handlerPINT3, RESET);
					GPIO_WritePin(&handlerPINT4, RESET);
					GPIO_WritePin(&handlerPINT5, RESET);
					GPIO_WritePin(&handlerPINT6, SET);

					break;
				case 3:

					GPIO_WritePin(&handlerPINT1, RESET);
					GPIO_WritePin(&handlerPINT2, SET);
					GPIO_WritePin(&handlerPINT3, RESET);
					GPIO_WritePin(&handlerPINT4, SET);
					GPIO_WritePin(&handlerPINT5, RESET);
					GPIO_WritePin(&handlerPINT6, RESET);
					break;
				case 4:

					GPIO_WritePin(&handlerPINT1, RESET);
					GPIO_WritePin(&handlerPINT2, RESET);
					GPIO_WritePin(&handlerPINT3, SET);
					GPIO_WritePin(&handlerPINT4, SET);
					GPIO_WritePin(&handlerPINT5, RESET);
					GPIO_WritePin(&handlerPINT6, RESET);

					break;
				case 5:

					GPIO_WritePin(&handlerPINT1, RESET);
					GPIO_WritePin(&handlerPINT2, RESET);
					GPIO_WritePin(&handlerPINT3, SET);
					GPIO_WritePin(&handlerPINT4, RESET);
					GPIO_WritePin(&handlerPINT5, SET);
					GPIO_WritePin(&handlerPINT6, RESET);
					break;
				case 6:

					GPIO_WritePin(&handlerPINT1, SET);
					GPIO_WritePin(&handlerPINT2, RESET);
					GPIO_WritePin(&handlerPINT3, RESET);
					GPIO_WritePin(&handlerPINT4, RESET);
					GPIO_WritePin(&handlerPINT5, SET);
					GPIO_WritePin(&handlerPINT6, RESET);

					break;
				default:
					__NOP();
					break;
			}//Fin del switch case
			flag = RESET;
			if (counterMotor >= 6){
				counterMotor = SET;
			}
		}



//		if (auxData != '\0'){
//			if (auxData == '1'){
//				GPIOxTooglePin(&handlerPINT1);
//				sprintf(bufferData,"1 = %u \n\r",(unsigned int) GPIO_ReadPin(&handlerPINF1));
//				writeMsg(&handlerUSART2, bufferData);
//				auxData = '\0';
//
//			}else if (auxData == '2'){
//				GPIOxTooglePin(&handlerPINF2);
//				sprintf(bufferData,"2 = %u \n\r",(unsigned int) GPIO_ReadPin(&handlerPINF2));
//				writeMsg(&handlerUSART2, bufferData);
//				auxData = '\0';
//			}else if (auxData == '3'){
//				GPIOxTooglePin(&handlerPINF3);
//				sprintf(bufferData,"3 = %u \n\r",(unsigned int) GPIO_ReadPin(&handlerPINF3));
//				writeMsg(&handlerUSART2, bufferData);
//				auxData = '\0';
//			}else if (auxData == '4'){
//				GPIOxTooglePin(&handlerPINF4);
//				sprintf(bufferData,"4 = %u \n\r",(unsigned int) GPIO_ReadPin(&handlerPINF4));
//				writeMsg(&handlerUSART2, bufferData);
//				auxData = '\0';
//			}else if (auxData == '5'){
//				GPIOxTooglePin(&handlerPINF5);
//				sprintf(bufferData,"5 = %u \n\r",(unsigned int) GPIO_ReadPin(&handlerPINF5));
//				writeMsg(&handlerUSART2, bufferData);
//				auxData = '\0';
//			}else if (auxData == '6'){
//				GPIOxTooglePin(&handlerPINF6);
//				sprintf(bufferData,"6 = %u \n\r",(unsigned int) GPIO_ReadPin(&handlerPINF6));
//				writeMsg(&handlerUSART2, bufferData);
//				auxData = '\0';
//			}
//		}

	}

}

void initSystem(void){
	//Pines digitales para
	handlerPINT1.pGPIOx = GPIOC;
	handlerPINT1.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINT1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINT1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINT1.GPIO_PinConfig.GPIO_PinNumber = PIN_0;
	handlerPINT1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINT1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINT1);
	GPIO_WritePin(&handlerPINT1, RESET);

	handlerPINT2.pGPIOx = GPIOC;
	handlerPINT2.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINT2.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINT2.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINT2.GPIO_PinConfig.GPIO_PinNumber = PIN_1;
	handlerPINT2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINT2.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINT2);
	GPIO_WritePin(&handlerPINT2, RESET);

	handlerPINT3.pGPIOx = GPIOC;
	handlerPINT3.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINT3.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINT3.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINT3.GPIO_PinConfig.GPIO_PinNumber = PIN_2;
	handlerPINT3.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINT3.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINT3);
	GPIO_WritePin(&handlerPINT3, RESET);

	handlerPINT4.pGPIOx = GPIOC;
	handlerPINT4.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINT4.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINT4.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINT4.GPIO_PinConfig.GPIO_PinNumber = PIN_3;
	handlerPINT4.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINT4.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINT4);
	GPIO_WritePin(&handlerPINT4, RESET);

	handlerPINT5.pGPIOx = GPIOC;
	handlerPINT5.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINT5.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINT5.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINT5.GPIO_PinConfig.GPIO_PinNumber = PIN_4;
	handlerPINT5.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINT5.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINT5);
	GPIO_WritePin(&handlerPINT5, RESET);

	handlerPINT6.pGPIOx = GPIOC;
	handlerPINT6.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINT6.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINT6.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINT6.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerPINT6.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINT6.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINT6);
	GPIO_WritePin(&handlerPINT6, RESET);


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

void BasicTimer2_Callback(void){
	flag = SET;
	counterMotor++;
}

void usart2Rx_Callback(void){
	auxData = getRxData();
}

void startMotor(void){
	startTimer(&handlerTIMMotor);
}

void updateRPMMotor(uint8_t period){
	stopTimer(&handlerTIMMotor);
	TIM2->ARR = period - 1;
	startTimer(&handlerTIMMotor);
}

