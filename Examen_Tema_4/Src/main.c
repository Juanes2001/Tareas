/**
 ******************************************************************************
 * Examen_Tema_4
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

#include <stdint.h>
#include <stdbool.h>

// configuracion USART que se usara = Serial_1, 28800, EVEN

//handler para ADC
ADC_Config_t handlerADCJoy = {0};

//Pines handler para comunicación Serial
GPIO_Handler_t handlerPinTx = {0};
GPIO_Handler_t handlerPinRx = {0};

//Pin del blinky
GPIO_Handler_t handlerLEDPin = {0};

//handler para USART1
USART_Handler_t handlerUSART1 = {0};

//handler para PWM
PWM_Handler_t handlerPWMrgb = {0};

//Handlers de Timers
BasicTimer_Handler_t handlerTIM2 = {0};


void inSystem (void);

int main(void){

	inSystem();

	while(1){

	}
}

void inSystem (void){

	handlerTIM2.ptrTIMx = TIM2;
	handlerTIM2.TIMx_Config.TIMx_interruptEnable = 1;
	handlerTIM2.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerTIM2.TIMx_Config.TIMx_period = 2500;
	handlerTIM2.TIMx_Config.TIMx_speed = BTIMER_SPEED_100us;
	BasicTimer_Config(&handlerTIM2);

	startTimer(&handlerTIM2);

	handlerLEDPin.pGPIOx = GPIOA;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerLEDPin);

//	handlerADCJoy.channelVector = {ADC_CHANNEL_0, ADC_CHANNEL_1};


}


void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerLEDPin);
}


