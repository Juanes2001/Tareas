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
PWM_Handler_t handlerPwmR = {0};
PWM_Handler_t handlerPwmG = {0};
PWM_Handler_t handlerPwmB = {0};
PWM_Handler_t handlerADCPwm = {0};

//Handlers de Timers
BasicTimer_Handler_t handlerLEDTim = {0};




void inSystem (void);




//Variables necesarias para el programa
uint8_t adcData = 0;
uint8_t adcConvertion = RESET;
uint8_t rxData = '\0';
uint8_t counter1 = 0;
uint8_t counter2 = 0;
uint8_t counter3 = 0;
uint8_t dutty = 0;



int main(void){

	inSystem();

	while(1){

		if (rxData == 's'){
			startPwmSignal(&handlerADCPwm);
			startPwmSignal(&handlerPwmR);
			startPwmSignal(&handlerPwmB);
			startPwmSignal(&handlerPwmG);
			rxData = '\0';
		}else if(rxData == 'p'){
			stopPwmSignal(&handlerADCPwm);
			stopPwmSignal(&handlerPwmR);
			stopPwmSignal(&handlerPwmG);
			stopPwmSignal(&handlerPwmB);
			rxData = '\0';
		}

		if (rxData == 'r'){

			if (handlerPwmR.ptrTIMx->CCR2 >=100){

				updateDuttyCycle(&handlerPwmR, 100 );
			}else{
				updateDuttyCycle(&handlerPwmR, handlerPwmR.config.duttyCicle + 1 );
				updateDuttyCycle(&handlerPwmG, handlerPwmG.config.duttyCicle - 1 );
				updateDuttyCycle(&handlerPwmB, handlerPwmB.config.duttyCicle - 1 );
			}

		}
	}
}

void inSystem (void){

	handlerLEDTim.ptrTIMx = TIM2;
	handlerLEDTim.TIMx_Config.TIMx_interruptEnable = 1;
	handlerLEDTim.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerLEDTim.TIMx_Config.TIMx_period = 2500;
	handlerLEDTim.TIMx_Config.TIMx_speed = BTIMER_SPEED_100us;
	BasicTimer_Config(&handlerLEDTim);

	startTimer(&handlerLEDTim);

	handlerLEDPin.pGPIOx = GPIOA;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerLEDPin);

	//Conversion del JOYSTICK
    for (uint8_t i = 0 ; i < 2 ; i++){
    	handlerADCJoy.channelVector[i] = i;
    }
    handlerADCJoy.dataAlignment = ADC_ALIGNMENT_RIGHT;
    handlerADCJoy.resolution = ADC_RESOLUTION_8_BIT;
    handlerADCJoy.samplingPeriod = ADC_SAMPLING_PERIOD_28_CYCLES;
    ADC_ConfigMultichannel(&handlerADCJoy, 2);

    handlerADCPwm.ptrTIMx            = TIM3;
    handlerADCPwm.config.channel     = PWM_CHANNEL_1;
    handlerADCPwm.config.periodo     = 10;
    handlerADCPwm.config.duttyCicle  = 50;
    handlerADCPwm.config.prescaler   = BTIMER_SPEED_100us;
    pwm_Config(&handlerADCPwm);
    adcTimerEventConfig();

    //Comunicacion Serial

    handlerPinTx.pGPIOx = GPIOA;
    handlerPinTx.GPIO_PinConfig.GPIO_PinAltFunMode  = AF7;
    handlerPinTx.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
    handlerPinTx.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
    handlerPinTx.GPIO_PinConfig.GPIO_PinNumber      = PIN_2;
    handlerPinTx.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
    handlerPinTx.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinTx);


	handlerPinRx.pGPIOx                             = GPIOA;
	handlerPinRx.GPIO_PinConfig.GPIO_PinAltFunMode  = AF7;
	handlerPinRx.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerPinRx.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPinRx.GPIO_PinConfig.GPIO_PinNumber      = PIN_3;
	handlerPinRx.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPinRx.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinRx);

	handlerUSART1.ptrUSARTx = USART2;
	handlerUSART1.USART_Config.USART_baudrate = USART_BAUDRATE_28800;//37,7222 de Mantiza
	handlerUSART1.USART_Config.USART_enableInRx = USART_INTERRUPT_RX_ENABLE;
	handlerUSART1.USART_Config.USART_enableInTx = USART_INTERRUPT_TX_ENABLE;
	handlerUSART1.USART_Config.USART_mode = USART_MODE_RXTX;
	handlerUSART1.USART_Config.USART_parity = USART_PARITY_ODD;
	handlerUSART1.USART_Config.USART_stopbits = USART_STOPBIT_1;
	handlerUSART1.USART_Config.USART_datasize = USART_DATASIZE_9BIT;
	USART_Config(&handlerUSART1);


	handlerPwmR.ptrTIMx           = TIM3;
	handlerPwmR.config.channel    = PWM_CHANNEL_2;
	handlerPwmR.config.duttyCicle = 100;
	handlerPwmR.config.periodo    = 100;
	handlerPwmR.config.prescaler  = BTIMER_SPEED_100us;
	pwm_Config(&handlerPwmR);

	handlerPwmG.ptrTIMx           = TIM3;
	handlerPwmG.config.channel    = PWM_CHANNEL_3;
	handlerPwmG.config.duttyCicle = 100;
	handlerPwmG.config.periodo    = 100;
	handlerPwmG.config.prescaler  = BTIMER_SPEED_100us;
	pwm_Config(&handlerPwmG);

	handlerPwmB.ptrTIMx           = TIM3;
	handlerPwmB.config.channel    = PWM_CHANNEL_4;
	handlerPwmB.config.duttyCicle = 100;
	handlerPwmB.config.periodo    = 100;
	handlerPwmB.config.prescaler  = BTIMER_SPEED_100us;
	pwm_Config(&handlerPwmB);

}





void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerLEDPin);
}

void adcComplete_Callback(void){
	adcData = getADC();

	adcConvertion = SET;
}

void usart2Rx_Callback(void){
	rxData = getRxData();
}

