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
#include <math.h>

// configuracion USART que se usara = Serial_1, 28800, EVEN

//handler para ADC
ADC_Config_t handlerADCJoy = {0};

//Pines handler para comunicación Serial
GPIO_Handler_t handlerPinTx = {0};
GPIO_Handler_t handlerPinRx = {0};

GPIO_Handler_t handlerPinR = {0};
GPIO_Handler_t handlerPinG = {0};
GPIO_Handler_t handlerPinB = {0};

//Pin del blinky
GPIO_Handler_t handlerLEDPin = {0};

//handler para USART1
USART_Handler_t handlerUSART1 = {0};

//handler para PWM
//PWM_Handler_t handlerADCPwm = {0};

PWM_Handler_t handlerPwmR   = {0};
PWM_Handler_t handlerPwmG   = {0};
PWM_Handler_t handlerPwmB   = {0};

//Handlers de Timers
BasicTimer_Handler_t handlerLEDTim = {0};
BasicTimer_Handler_t handlerADCTim = {0};




void inSystem (void);




//Variables necesarias para el programa
uint8_t adcData[2];
uint8_t adcConvertion = RESET;
uint8_t rxData = '\0';
uint8_t counterR = 0;
uint8_t counterG = 0;
uint8_t counterB = 0;
char bufferData1 [64];
char bufferData2 [64];
uint8_t counterADC = 0;

uint8_t duttyR = 0;
uint8_t duttyG = 0;
uint8_t duttyB = 0;



int main(void){

	inSystem();



	while(1){
		counterR = handlerPwmR.ptrTIMx->CCR2;
		counterG = handlerPwmG.ptrTIMx->CCR3;
		counterB = handlerPwmB.ptrTIMx->CCR4;



		if (rxData == 's'){
			startTimer(&handlerADCTim);
			startPwmSignal(&handlerPwmR);
			startPwmSignal(&handlerPwmG);
			startPwmSignal(&handlerPwmB);
			rxData = '\0';
		}else if (rxData == 'p'){
			stopTimer(&handlerADCTim);
			stopPwmSignal(&handlerPwmR);
			stopPwmSignal(&handlerPwmG);
			stopPwmSignal(&handlerPwmB);
			rxData = '\0';
		}

		if (adcConvertion){
			sprintf(bufferData1, "Vx = %u  ", adcData[0]);
			sprintf(bufferData2, "Vy = %u \n \r", adcData[1]);
			writeMsg(&handlerUSART1, bufferData1);
			writeMsg(&handlerUSART1, bufferData2);
			adcConvertion = RESET;
		}
		else{
			__NOP();
		}










//		if (rxData == 'r'){
//			if (counterR > 100){
//				updateDuttyCycle(&handlerPwmR, 0);
//			}else if(counterR <= 100) {
//				updateDuttyCycle(&handlerPwmR, counterR + 1 );
//			}
//			rxData = '\0';
//		}
//		if (rxData == 'g'){
//			if (counterG > 100){
//				updateDuttyCycle(&handlerPwmG, 0);
//			}else if(counterG <= 100) {
//				updateDuttyCycle(&handlerPwmG, counterG + 1 );
//			}
//			rxData = '\0';
//		}
//		if (rxData == 'b'){
//			if (counterB > 100){
//				updateDuttyCycle(&handlerPwmB, 0);
//			}else if(counterB <= 100) {
//				updateDuttyCycle(&handlerPwmB, counterB + 1 );
//			}
//			rxData = '\0';
//		}
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
	handlerADCTim.ptrTIMx = TIM4;
	handlerADCTim.TIMx_Config.TIMx_interruptEnable = 1;
	handlerADCTim.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerADCTim.TIMx_Config.TIMx_period = 100;
	handlerADCTim.TIMx_Config.TIMx_speed = BTIMER_SPEED_100us;
	BasicTimer_Config(&handlerADCTim);


//	handlerADCPwm.ptrTIMx = TIM3;
//	handlerADCPwm.config.channel = PWM_CHANNEL_1;
//	handlerADCPwm.config.duttyCicle = 50;
//	handlerADCPwm.config.periodo = 100;
//	handlerADCPwm.config.prescaler = BTIMER_SPEED_100us;
//	pwm_Config(&handlerADCPwm);
	handlerADCTim.ptrTIMx = TIM4;
	handlerADCTim.TIMx_Config.TIMx_interruptEnable = 1;
	handlerADCTim.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerADCTim.TIMx_Config.TIMx_period = 100;
	handlerADCTim.TIMx_Config.TIMx_speed = BTIMER_SPEED_100us;


    for (uint8_t i = 0 ; i < 2 ; i++){
    	handlerADCJoy.channelVector[i] = i;
    }
    handlerADCJoy.dataAlignment = ADC_ALIGNMENT_RIGHT;
    handlerADCJoy.resolution = ADC_RESOLUTION_8_BIT;
    handlerADCJoy.samplingPeriod = ADC_SAMPLING_PERIOD_28_CYCLES;
    ADC_ConfigMultichannel(&handlerADCJoy, 2);

    //Comunicacion Serial

    handlerPinTx.pGPIOx                             = GPIOA;
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

	handlerUSART1.ptrUSARTx                     = USART2;
	handlerUSART1.USART_Config.USART_baudrate   = USART_BAUDRATE_28800;//37,7222 de Mantiza
	handlerUSART1.USART_Config.USART_enableInRx = USART_INTERRUPT_RX_ENABLE;
	handlerUSART1.USART_Config.USART_enableInTx = USART_INTERRUPT_TX_DISABLE;
	handlerUSART1.USART_Config.USART_mode       = USART_MODE_RXTX;
	handlerUSART1.USART_Config.USART_parity     = USART_PARITY_ODD;
	handlerUSART1.USART_Config.USART_stopbits   = USART_STOPBIT_1;
	handlerUSART1.USART_Config.USART_datasize   = USART_DATASIZE_9BIT;
	USART_Config(&handlerUSART1);


	handlerPinR.pGPIOx                             = GPIOC;
	handlerPinR.GPIO_PinConfig.GPIO_PinAltFunMode  = AF2;
	handlerPinR.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerPinR.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPinR.GPIO_PinConfig.GPIO_PinNumber      = PIN_7;
	handlerPinR.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPinR.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinR);


	handlerPinG.pGPIOx                             = GPIOC;
	handlerPinG.GPIO_PinConfig.GPIO_PinAltFunMode  = AF2;
	handlerPinG.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerPinG.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPinG.GPIO_PinConfig.GPIO_PinNumber      = PIN_8;
	handlerPinG.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPinG.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinG);


	handlerPinB.pGPIOx                             = GPIOC;
	handlerPinB.GPIO_PinConfig.GPIO_PinAltFunMode  = AF2;
	handlerPinB.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerPinB.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPinB.GPIO_PinConfig.GPIO_PinNumber      = PIN_9;
	handlerPinB.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPinB.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinB);



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
	counterADC++;
	if ((counterADC % 2) != 0){
		adcData[0] = getADC();
	}else if ((counterADC % 2) == 0){
		adcData[1] = getADC();
	}

	if ((counterADC % 2) == 0){
		adcConvertion = SET;
	}


}

void usart2Rx_Callback(void){
	rxData = getRxData();
}

void BasicTimer4_Callback(void){
	startSingleADC();
}

