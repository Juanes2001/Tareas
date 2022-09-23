/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
#include "BasicTimer.h"
#include "GPIOxDriver.h"
#include "USARTxDriver.h"
#include <stdint.h>
#include <stdbool.h>
void TIM2_IRQHandler(void);

GPIO_Handler_t handlerUserLed = {0};
GPIO_Handler_t handlerGPIOUSART1 = {0};
GPIO_Handler_t handlerUserButton = {0};
BasicTimer_Handler_t handlerTIM2 = {0};
USART_Handler_t handlerUSART1= {0};


int main(void)
{
	handlerUserLed.pGPIOx =GPIOA;
	handlerUserLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerUserLed.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerUserLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerUserLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerUserLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	handlerUserLed.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;

	GPIO_Config(&handlerUserLed);

	handlerUserButton.pGPIOx =GPIOC;
	handlerUserButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	handlerUserButton.GPIO_PinConfig.GPIO_PinNumber = PIN_13;
	handlerUserButton.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerUserButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerUserButton.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	handlerUserButton.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;

	GPIO_Config(&handlerUserButton);

	handlerGPIOUSART1.pGPIOx =GPIOA;
	handlerGPIOUSART1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerGPIOUSART1.GPIO_PinConfig.GPIO_PinNumber = PIN_9;
	handlerGPIOUSART1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerGPIOUSART1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerGPIOUSART1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	handlerGPIOUSART1.GPIO_PinConfig.GPIO_PinAltFunMode = AF7;

	GPIO_Config(&handlerGPIOUSART1);

	handlerTIM2.ptrTIMx = TIM2;
	handlerTIM2.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerTIM2.TIMx_Config.TIMx_speed = BTIMER_SPEED_100us;
	handlerTIM2.TIMx_Config.TIMx_period = 2500;
	handlerTIM2.TIMx_Config.TIMx_interruptEnable=1;

	BasicTimer_Config(&handlerTIM2);

	handlerUSART1.ptrUSARTx = USART1;
	handlerUSART1.USART_Config.USART_baudrate = USART_BAUDRATE_115200;
	handlerUSART1.USART_Config.USART_datasize = USART_DATASIZE_8BIT;
	handlerUSART1.USART_Config.USART_mode = USART_MODE_TX;
	handlerUSART1.USART_Config.USART_stopbits = USART_STOPBIT_1;

	USART_Config(&handlerUSART1);

	TIM2_IRQHandler();

	while(1){

	}

}

void BasicTimer2_Callback(void){
		GPIOxTooglePin(&handlerUserLed);
		writeChar(&handlerUSART1, 45);
		if (GPIO_ReadPin(&handlerUserButton) == 0){
			writeChar(&handlerUSART1, 46);
		}
}

