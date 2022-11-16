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

//Timer para controlador de velocidades
BasicTimer_Handler_t handlerTIMMotor = {0};

void initSystem(void);

int main (void){

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

	handlerPINF2.pGPIOx = GPIOA;
	handlerPINF2.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINF2.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINF2.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINF2.GPIO_PinConfig.GPIO_PinNumber = PIN_1;
	handlerPINF2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINF2.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINF2);

	handlerPINF3.pGPIOx = GPIOA;
	handlerPINF3.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINF3.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINF3.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINF3.GPIO_PinConfig.GPIO_PinNumber = PIN_0;
	handlerPINF3.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINF3.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINF3);


	handlerTIMMotor.ptrTIMx = TIM2;
	handlerTIMMotor.TIMx_Config.TIMx_interruptEnable = 1;
	handlerTIMMotor.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerTIMMotor.TIMx_Config.TIMx_period = 2500;
	handlerTIMMotor.TIMx_Config.TIMx_speed = BTIMER_SPEED_10us;
	BasicTimer_Config(&handlerTIMMotor);

}
