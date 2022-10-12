/**
 ******************************************************************************
 * USART_TIMERS
 * Autor: Juan Esteban Rodriguez Ochoa
 * Correo: jrodriguezoc@unal.edu.co
 ******************************************************************************
 ******************************************************************************
 */
#include "BasicTimer.h"
#include "GPIOxDriver.h"
#include "USARTxDriver.h"
#include"EXTIDriver.h"
#include <stdint.h>
#include <stdbool.h>

uint8_t flag1 = 0;
uint8_t flag2 = 0;
uint8_t buffer[8];
uint8_t counter = 0;
uint8_t string1[];

void inSystem (void);


GPIO_Handler_t handlerBlinkyLed = {0};
//GPIO_Handler_t handlerUserPinUsart = {0};
//GPIO_Handler_t handlerExIn1 = {0};
//GPIO_Handler_t handlerExIn2 ={0};
EXTI_Config_t handlerExtiConfig0 = {0};
EXTI_Config_t handlerExtiConfig1 = {0};
EXTI_Config_t handlerExtiConfig2 = {0};
BasicTimer_Handler_t handlerTIM2 = {0};
USART_Handler_t handlerUSART2 = {0};




int main (void){

	inSystem();

	while(1){

		if (flag1){

			uint8_t string1 = "CW direction = ";
			writeMsg(&handlerUSART2, string1);
			counter++;
			sprintf(buffer, '%u \n', counter);
			writeMsg(&handlerUSART2, buffer);


		}else if



	}
}


void inSystem (void){

	handlerBlinkyLed.pGPIOx = GPIOA;
	handlerBlinkyLed.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerBlinkyLed.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerBlinkyLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerBlinkyLed.GPIO_PinConfig.GPIO_PinPuPdControl =  GPIO_PUPDR_NOTHING;
	handlerBlinkyLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerBlinkyLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;

	GPIO_Config(&handlerBlinkyLed);


//	handlerExIn0.pGPIOx = GPIOA;
//	handlerExIn0.GPIO_PinConfig.GPIO_PinNumber = PIN_0;
//	handlerExIn0.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
//	handlerExIn0.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
//	handlerExIn0.GPIO_PinConfig.GPIO_PinPuPdControl =  GPIO_PUPDR_NOTHING;
//	handlerExIn0.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
//	handlerExIn0.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;

//	GPIO_Config(&handlerExIn0);

	handlerExtiConfig0.pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber = PIN_0;
	handlerExtiConfig0.pGPIOHandler->pGPIOx = GPIOA;
	handlerExtiConfig0.pGPIOHandler->GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	handlerExtiConfig0.pGPIOHandler->GPIO_PinConfig.GPIO_PinAltFunMode =AF0;
	handlerExtiConfig0.pGPIOHandler->GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerExtiConfig0.pGPIOHandler->GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerExtiConfig0.pGPIOHandler->GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	handlerExtiConfig0.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE;

	extInt_Config(&handlerExtiConfig0);

	handlerExtiConfig1.pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber       = PIN_1;
	handlerExtiConfig1.pGPIOHandler->pGPIOx                              = GPIOA;
	handlerExtiConfig1.pGPIOHandler->GPIO_PinConfig.GPIO_PinMode         = GPIO_MODE_IN;
	handlerExtiConfig1.pGPIOHandler->GPIO_PinConfig.GPIO_PinAltFunMode   = AF0;
	handlerExtiConfig1.pGPIOHandler->GPIO_PinConfig.GPIO_PinOPType       = GPIO_OTYPE_PUSHPULL;
	handlerExtiConfig1.pGPIOHandler->GPIO_PinConfig.GPIO_PinPuPdControl  = GPIO_PUPDR_NOTHING;
	handlerExtiConfig1.pGPIOHandler->GPIO_PinConfig.GPIO_PinSpeed        = GPIO_OSPEEDR_FAST;
	handlerExtiConfig1.edgeType                                          = EXTERNAL_INTERRUPT_RISING_EDGE;

	extInt_Config(&handlerExtiConfig0);

	handlerTIM2.ptrTIMx = TIM2;
	handlerTIM2.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerTIM2.TIMx_Config.TIMx_speed = BTIMER_SPEED_100us;
	handlerTIM2.TIMx_Config.TIMx_period = 2500;
	handlerTIM2.TIMx_Config.TIMx_interruptEnable = 1;

	BasicTimer_Config(&handlerTIM2);

	startTimer (&handlerTIM2);

	handlerUSART2.ptrUSARTx = USART2;
	handlerUSART2.USART_Config.USART_baudrate = USART_BAUDRATE_115200;
	handlerUSART2.USART_Config.USART_datasize = USART_DATASIZE_8BIT;
	handlerUSART2.USART_Config.USART_mode = USART_MODE_RXTX;
	handlerUSART2.USART_Config.USART_stopbits = USART_STOPBIT_1;

	USART_Config(&handlerUSART2);





}

void callback_extInt0(void){
	flag1 = 1;
}

void callback_extInt1(void){
	flag2 = 1;
}

void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerBlinkyLed);
}
