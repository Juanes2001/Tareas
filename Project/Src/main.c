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
GPIO_Handler_t handlerPWMPinServo = {0};
GPIO_Handler_t handlerPWMPinMotor = {0};
GPIO_Handler_t handlerPinSwitch1 = {0};
GPIO_Handler_t handlerPinSwitch2 = {0};

ADC_Config_t handlerADCJoy = {0};


GPIO_Handler_t handlerUSARTPinTx       = {0};
GPIO_Handler_t handlerUSARTPinRx       = {0};

//Timer para controlador de velocidades
BasicTimer_Handler_t handlerBlinky = {0};

USART_Handler_t handlerUSART2    = {0};

PWM_Handler_t handlerPWMControlServo = {0};
PWM_Handler_t handlerPWMControlMotor = {0};


uint8_t rxData  = '\0';
char bufferData[64];
uint8_t flag = RESET;
uint8_t counterMotor = 0;
uint8_t duttyUp = 0;
uint8_t duttyDown = 0;
uint16_t freqUp = 0;
uint16_t freqDown = 0;
char bufferReception[64] = {0};
uint8_t counterReception = 0;
uint8_t doneTransaction = RESET;

char cmd[32];
char userMsg[64];



unsigned int firstParameter;
unsigned int secondParameter;
unsigned int thirdParameter;




void initSystem(void);
void parseCommands(char *command);

int main (void){

	initSystem();

	while(1){

		if (rxData != '\0'){
			if (rxData == 'm'){
				if (handlerPWMControlMotor.ptrTIMx->CR1 & TIM_CR1_CEN){
					stopPwmSignal(&handlerPWMControlMotor);
					writeChar(&handlerUSART2, rxData);
					rxData = '\0';

				}else {
					startPwmSignal(&handlerPWMControlMotor);
					writeChar(&handlerUSART2, rxData);
					rxData = '\0';
				}

			}else if (rxData == 'd'){
				duttyUp = handlerPWMControlMotor.config.duttyCicle;
				duttyUp++;
				if (duttyUp >= 100){
					updateDuttyCycle(&handlerPWMControlMotor, 100);
				}else{
					updateDuttyCycle(&handlerPWMControlMotor, duttyUp);
					rxData = '\0';
				}

			}else if (rxData == 'u'){
				duttyDown = handlerPWMControlMotor.config.duttyCicle;
				duttyDown--;
				if (duttyDown == 0){
					updateDuttyCycle(&handlerPWMControlMotor, 1);
				}else{
					updateDuttyCycle(&handlerPWMControlMotor, duttyDown);
					rxData = '\0';
				}

			}

//			if (auxData == 'm'){
//				if (handlerPWMControlServo.ptrTIMx->CR1 & TIM_CR1_CEN){
//					stopPwmSignal(&handlerPWMControlServo);
//					writeChar(&handlerUSART2, auxData);
//					auxData = '\0';
//
//				}else {
//					startPwmSignal(&handlerPWMControlServo);
//					writeChar(&handlerUSART2, auxData);
//					auxData = '\0';
//				}
//
//			}else if (auxData == 'd'){
//				duttyUp = handlerPWMControlServo.config.duttyCicle;
//				duttyUp++;
//				if (duttyUp >= 100){
//					updateDuttyCycle(&handlerPWMControlServo, 100);
//				}else{
//					updateDuttyCycle(&handlerPWMControlServo, duttyUp);
//					auxData = '\0';
//				}
//
//			}else if (auxData == 'u'){
//				duttyDown = handlerPWMControlServo.config.duttyCicle;
//				duttyDown--;
//				if (duttyDown == 0){
//					updateDuttyCycle(&handlerPWMControlServo, 1);
//				}else{
//					updateDuttyCycle(&handlerPWMControlServo, duttyDown);
//					auxData = '\0';
//				}
//
//			}

//				else if (auxData == '-'){
//					freqDown = (handlerPWMControlServo.config.periodo) + 1;
//
//				if (freqDown == 0){
//					updateFrequency(&handlerPWMControlServo, 1);
//				}else{
//					updateFrequency(&handlerPWMControlServo, freqDown);
//					auxData = '\0';
//				}
//
//			}else if (auxData == '+'){
//					freqUp = (handlerPWMControlServo.config.periodo)/10 - 1;
//
//				if (freqUp == 0){
//					updateFrequency(&handlerPWMControlServo, 1);
//				}else{
//					updateFrequency(&handlerPWMControlServo, freqUp);
//					auxData = '\0';
//				}
//
//			}


		}

		if (rxData != '\0'){
			bufferReception[counterReception] = rxData;
			counterReception++;

			if (rxData == '@'){
				doneTransaction = SET;
				writeMsg(&handlerUSART2, bufferReception);

				bufferReception[counterReception] = '\0';

				counterReception = 0;

			}

			rxData = '\0';

		}

		if (doneTransaction){
			parseCommands(bufferReception);
			doneTransaction = RESET;
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

	handlerPWMPinServo.pGPIOx = GPIOB;
	handlerPWMPinServo.GPIO_PinConfig.GPIO_PinAltFunMode = AF2;
	handlerPWMPinServo.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerPWMPinServo.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPWMPinServo.GPIO_PinConfig.GPIO_PinNumber = PIN_7;
	handlerPWMPinServo.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPWMPinServo.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPWMPinServo);

	handlerPWMPinMotor.pGPIOx = GPIOB;
	handlerPWMPinMotor.GPIO_PinConfig.GPIO_PinAltFunMode = AF2;
	handlerPWMPinMotor.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerPWMPinMotor.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPWMPinMotor.GPIO_PinConfig.GPIO_PinNumber = PIN_6;
	handlerPWMPinMotor.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPWMPinMotor.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPWMPinMotor);

	handlerPWMControlServo.ptrTIMx           = TIM4;
	handlerPWMControlServo.config.channel    = PWM_CHANNEL_2;
	handlerPWMControlServo.config.duttyCicle = 95;
	handlerPWMControlServo.config.periodo    = 200;
	handlerPWMControlServo.config.prescaler  = PWM_SPEED_100us;
	pwm_Config(&handlerPWMControlServo);

	handlerPWMControlMotor.ptrTIMx           = TIM4;
	handlerPWMControlMotor.config.channel    = PWM_CHANNEL_1;
	handlerPWMControlMotor.config.duttyCicle = 50;
	handlerPWMControlMotor.config.periodo    = 200;
	handlerPWMControlMotor.config.prescaler  = PWM_SPEED_100us;
	pwm_Config(&handlerPWMControlMotor);

	handlerPinSwitch1.pGPIOx = GPIOB;
	handlerPinSwitch1.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPinSwitch1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPinSwitch1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPinSwitch1.GPIO_PinConfig.GPIO_PinNumber = PIN_3;
	handlerPinSwitch1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPinSwitch1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinSwitch1);
	GPIO_WritePin(&handlerPinSwitch1, RESET);

	handlerPinSwitch2.pGPIOx = GPIOB;
	handlerPinSwitch2.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPinSwitch2.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPinSwitch2.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPinSwitch2.GPIO_PinConfig.GPIO_PinNumber = PIN_4;
	handlerPinSwitch2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPinSwitch2.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinSwitch2);
	GPIO_WritePin(&handlerPinSwitch2, SET);

	handlerADCJoy.channelVector[0] = 0;
	handlerADCJoy.channelVector[1] = 1;
	handlerADCJoy.dataAlignment = ADC_ALIGNMENT_RIGHT;
	handlerADCJoy.resolution = ADC_RESOLUTION_12_BIT;
	handlerADCJoy.samplingPeriod = ADC_SAMPLING_PERIOD_28_CYCLES;
	ADC_ConfigMultichannel(&handlerADCJoy, 2);




}


void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerLEDPin);
}


void usart2Rx_Callback(void){
	rxData = getRxData();
}

void parseCommands(char *command){

	sscanf(command, "%s %u %u %u %s", cmd ,&firstParameter, &secondParameter, &thirdParameter, userMsg);



	if (strcmp(cmd, "help") == 0){
		writeMsg(&handlerUSART2, "HELP MENU CMD : \n");
		writeMsg(&handlerUSART2, "1)  startMotor  #Direction  #RPM   clock Wise ---> 1 counter Clock Wise --> 0\n");
		writeMsg(&handlerUSART2, "2)  reverseMotor \n");
		writeMsg(&handlerUSART2, "3)  stopMotor \n");
 		writeMsg(&handlerUSART2, "4)   \n");
		writeMsg(&handlerUSART2, "5)   \n");
		writeMsg(&handlerUSART2, "6)   \n");
		writeMsg(&handlerUSART2, "7)   \n");
		writeMsg(&handlerUSART2, "8)   \n");
		writeMsg(&handlerUSART2, "9)   \n");
		writeMsg(&handlerUSART2, "10)  \n");
		writeMsg(&handlerUSART2, "11)  \n");

	}
	else if (strcmp(cmd, "startMotor") == 0){
		startPwmSignal(&handlerPWMControlMotor);
		updateDuttyCycle(&handlerPWMControlMotor, secondParameter);
		if (firstParameter){
			GPIO_WritePin(&handlerPinSwitch1, RESET);
			GPIO_WritePin(&handlerPinSwitch2, SET);
		}else{
			GPIO_WritePin(&handlerPinSwitch1, SET);
			GPIO_WritePin(&handlerPinSwitch2, RESET);
		}


	}
//	else if (com2  == 0){
//		stopTimer(&handlerADCTim);
//		stopPwmSignal(&handlerPwmR);
//		stopPwmSignal(&handlerPwmG);
//		stopPwmSignal(&handlerPwmB);
//		setPWM = RESET;
//	}
//	else if (com3 == 0){
//		if (setPWM){
//			updateFrequency(&handlerPwmR, firstParameter);
//			updateFrequency(&handlerPwmG, secondParameter);
//			updateFrequency(&handlerPwmB, thirdParameter);
//		}else{
//			writeMsg(&handlerUSART1, "Turn on the LED first \n");
//		}
//	}
//	else if (com4 == 0){
//			updateFrequency(&handlerPwmR, 10);
//			updateFrequency(&handlerPwmG, 10);
//			updateFrequency(&handlerPwmB, 10);
//
//	}
//	else if (com5 == 0){
//		startOLED(&handlerI2C1);
//		drawMSG(&handlerI2C1, "BIENVENIDO!", 11);
//
//	}
//	else if (com6 == 0){
//		stopOLED(&handlerI2C1);
//
//	}
//	else if (com7 == 0){
//		uint8_t i =0;
//		uint8_t flag = RESET;
//		while (*(userMsg+i)!='\0'){
//			if ((*(userMsg+i) <= 122) & (*(userMsg+i) >= 97)){
//				flag = SET;
//				break;
//			}else{
//				__NOP();
//			}
//		}
//
//		if (flag){
//			writeMsg(&handlerUSART1, "REWRITE MESSAGE IN CAPITAL LETTER \n");
//		}else{
//			drawMSG(&handlerI2C1, userMsg, sizeof(userMsg));
//			flag = RESET;
//		}
//
//
//
//	}
//	else if (com10 == 0){
//		setScrollOUT = RESET;
//		if (firstParameter == SET){
//			setScrollIN =SET;
//
//		}else{
//			setScrollIN =RESET;
//		}
//
//
//	}
//	else if (com11 == 0){
//		clearDisplay(&handlerI2C1);
//		setScrollOUT = SET;
//
//
//	}
	else{
		writeMsg(&handlerUSART2, "WRONG CMD, WRITE IT AGAING \n");
	}

}



