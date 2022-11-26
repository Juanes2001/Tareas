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

GPIO_Handler_t handlerLEDPin = {0};
GPIO_Handler_t handlerPWMPin = {0};
ADC_Config_t handlerADCJoy = {0};


GPIO_Handler_t handlerUSARTPinTx       = {0};
GPIO_Handler_t handlerUSARTPinRx       = {0};

//Timer para controlador de velocidades
BasicTimer_Handler_t handlerBlinky = {0};

USART_Handler_t handlerUSART2    = {0};

PWM_Handler_t handlerPWMControl = {0};


uint8_t auxData  = '\0';
char bufferData[64];
uint8_t flag = RESET;
uint8_t counterMotor = 0;
uint8_t duttyUp = 0;
uint8_t duttyDown = 0;
uint16_t freqUp = 0;
uint16_t freqDown = 0;





void initSystem(void);

int main (void){

	initSystem();

	while(1){

		if (auxData != '\0'){
			if (auxData == 'm'){
				if (handlerPWMControl.ptrTIMx->CR1 & TIM_CR1_CEN){
					stopPwmSignal(&handlerPWMControl);
					writeChar(&handlerUSART2, auxData);
					auxData = '\0';

				}else {
					startPwmSignal(&handlerPWMControl);
					writeChar(&handlerUSART2, auxData);
					auxData = '\0';
				}

			}else if (auxData == 'd'){
				duttyUp = handlerPWMControl.config.duttyCicle;
				duttyUp++;
				if (duttyUp >= 100){
					updateDuttyCycle(&handlerPWMControl, 100);
				}else{
					updateDuttyCycle(&handlerPWMControl, duttyUp);
					auxData = '\0';
				}

			}else if (auxData == 'u'){
				duttyDown = handlerPWMControl.config.duttyCicle;
				duttyDown--;
				if (duttyDown == 0){
					updateDuttyCycle(&handlerPWMControl, 1);
				}else{
					updateDuttyCycle(&handlerPWMControl, duttyDown);
					auxData = '\0';
				}

			}
				else if (auxData == '-'){
				switch (handlerPWMControl.config.prescaler) {
					case PWM_SPEED_100us:
						freqDown = (handlerPWMControl.config.periodo) + 1;
						break;

					case PWM_SPEED_10us:
						freqDown = (handlerPWMControl.config.periodo) + 1;
						break;
					case PWM_SPEED_1us:
						freqDown = (handlerPWMControl.config.periodo) + 1;
						break;
					case PWM_SPEED_1ms:
						freqDown = handlerPWMControl.config.periodo + 1;
						break;
					default:
						__NOP();
						break;
				}
				if (freqDown == 0){
					updateFrequency(&handlerPWMControl, 1);
				}else{
					updateFrequency(&handlerPWMControl, freqDown);
					auxData = '\0';
				}

			}else if (auxData == '+'){
				switch (handlerPWMControl.config.prescaler) {
					case PWM_SPEED_100us:
						freqUp = (handlerPWMControl.config.periodo)/10 - 1;
						break;

					case PWM_SPEED_10us:
						freqUp = (handlerPWMControl.config.periodo)/100 - 1;
						break;
					case PWM_SPEED_1ms:
						freqUp= handlerPWMControl.config.periodo - 1;
						break;
					default:
						__NOP();
						break;
				}

				if (freqUp == 0){
					updateFrequency(&handlerPWMControl, 1);
				}else{
					updateFrequency(&handlerPWMControl, freqUp);
					auxData = '\0';
				}
			}


		}

	}

}

void initSystem(void){


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


	handlerLEDPin.pGPIOx = GPIOA;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinAltFunMode = AF7;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerLEDPin);
	GPIO_WritePin(&handlerLEDPin, SET);

	handlerBlinky.ptrTIMx = TIM2;
	handlerBlinky.TIMx_Config.TIMx_interruptEnable = 1;
	handlerBlinky.TIMx_Config.TIMx_mode            = BTIMER_MODE_UP;
	handlerBlinky.TIMx_Config.TIMx_period          = 2500;
	handlerBlinky.TIMx_Config.TIMx_speed           = BTIMER_SPEED_100us;
	BasicTimer_Config(&handlerBlinky);
	startTimer(&handlerBlinky);

	handlerPWMPin.pGPIOx = GPIOB;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinAltFunMode = AF2;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinNumber = PIN_6;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPWMPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPWMPin);


	handlerPWMControl.ptrTIMx           = TIM4;
	handlerPWMControl.config.channel    = PWM_CHANNEL_1;
	handlerPWMControl.config.duttyCicle = 95;
	handlerPWMControl.config.periodo    = 200;
	handlerPWMControl.config.prescaler  = PWM_SPEED_100us;
	pwm_Config(&handlerPWMControl);


	handlerADCJoy.channelVector[0] = 0;
	handlerADCJoy.channelVector[1] = 1;
	handlerADCJoy.dataAlignment = ADC_ALIGNMENT_RIGHT;
	handlerADCJoy.resolution = ADC_RESOLUTION_12_BIT;
	handlerADCJoy.samplingPeriod = ADC_SAMPLING_PERIOD_28_CYCLES;
	ADC_ConfigMultichannel(&handlerADCJoy, 2);




}


void BasicTimer3_Callback(void){
	GPIOxTooglePin(&handlerLEDPin);
}


void usart2Rx_Callback(void){
	auxData = getRxData();
}



