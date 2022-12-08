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
#include "RCCHunMHz.h"
#include "CaptureFreqDriver.h"

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//handlers GPIO para el led blinky, el pin Tx y Rx, y el pin de salida de señal PWM y el pin de captura

GPIO_Handler_t handlerPINLED       = {0};
GPIO_Handler_t handlerUSARTPinTx   = {0};
GPIO_Handler_t handlerUSARTPinRx   = {0};
GPIO_Handler_t handlerPINCapture   = {0};
GPIO_Handler_t handlerPINPwm       = {0};

//handler del timer para interrupciones
BasicTimer_Handler_t handlerTIMLED = {0};

//handler PWM para lectura de frecuencia
PWM_Handler_t handlerPWMFreq = {0};

//handler Usart 2
USART_Handler_t handlerUSART2Capture    = {0};

//Capture handler
Capture_Handler_t handlerCaptureFreq = {0};

//Variables usadas en la tarea, timestamp1 el tiempo inferior del periodo y timestamp2 es el tiempo superior
//deltaTimestamp es la diferencia de tiempo, es decir, el periodo de la señal dependiendo del
//prescaler, counterCapture es un contador bandera para almacenar de forma logica los tiempos
//inferior y superior, auxData son los caracteres comandos que se envian desde el coolterm
//flag, es la bandera correspondiente a la interrupcion por letura de flanco. bufferData como
//siempre almacenara el mensaje con el dato calculado del periodo.
uint32_t timestamp1 = 0;
uint32_t timestamp2 = 0;
int32_t deltaTimestamp = 0;
uint8_t counterCapture = 0;
uint8_t auxData = '\0';
uint8_t flag = RESET;
char bufferData[64];

void initSystem(void);

//Este es el corazon del programa
int main(void){

		initSystem();

		while(1){
			//Con el comando l desde el coolterm podemos visualizar el valor del periodo de la señal PWM
			if (auxData != '\0'){
				if (auxData == 'l'){
					sprintf(bufferData, "period = %ums \n\r",(unsigned int) deltaTimestamp);
					writeMsg(&handlerUSART2Capture, bufferData);
					auxData = '\0';

				}
			}

			// Este condicional calculara el periodo de la señal mediante interrupciones
			if (flag){
				if (counterCapture == 0){
					clean(&handlerCaptureFreq);
					timestamp1 = timeStamp(&handlerCaptureFreq);
					counterCapture++;
				}else if (counterCapture == 1){
					timestamp2 =timeStamp(&handlerCaptureFreq);
					deltaTimestamp = getFreq(&handlerCaptureFreq, timestamp1, timestamp2);
					counterCapture = 0;
				}
				flag = RESET;
			}

	}
}

void initSystem(void){
	//Pin de recepcion de señal B4 correspondiente a el canal 1 del TIM3 junto con el handler
	//de captura con un prescaler de 1 a 1 y un tiempo entre cuentas de 100 micro segundos
	handlerPINCapture.pGPIOx                             = GPIOB;
	handlerPINCapture.GPIO_PinConfig.GPIO_PinAltFunMode  = AF2;
	handlerPINCapture.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerPINCapture.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPINCapture.GPIO_PinConfig.GPIO_PinNumber      = PIN_4;
	handlerPINCapture.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINCapture.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINCapture);

	handlerCaptureFreq.ptrTIMx                 = TIM3;
	handlerCaptureFreq.config.channel          = CAPTURE_CHANNEL_1;
	handlerCaptureFreq.config.edgeSignal       = CAPTURE_FALLING_EDGE;
	handlerCaptureFreq.config.prescalerCapture = CAPTURE_PREESCALER_1_1;
	handlerCaptureFreq.config.timerSpeed       = CAPTURE_TIMER_SPEED_100us;
	capture_Config(&handlerCaptureFreq);
	startCapture(&handlerCaptureFreq);

	//PIN LED para el blinky junto con su timer configurado con sus nuevas macros que corresponden
	//a la nueva velocidad de preescalamiento
	handlerPINLED.pGPIOx                             = GPIOA;
	handlerPINLED.GPIO_PinConfig.GPIO_PinAltFunMode  = AF0;
	handlerPINLED.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	handlerPINLED.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPINLED.GPIO_PinConfig.GPIO_PinNumber      = PIN_5;
	handlerPINLED.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINLED.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINLED);
	GPIO_WritePin(&handlerPINLED, SET);

	handlerTIMLED.ptrTIMx = TIM2;
	handlerTIMLED.TIMx_Config.TIMx_interruptEnable = 1;
	handlerTIMLED.TIMx_Config.TIMx_mode            = BTIMER_MODE_UP;
	handlerTIMLED.TIMx_Config.TIMx_period          = 2500;
	handlerTIMLED.TIMx_Config.TIMx_speed           = BTIMER_SPEED_100us;
	BasicTimer_Config(&handlerTIMLED);
	startTimer(&handlerTIMLED);

	//USART 2 para comunicacion serial con el nuevo macro (baudrate) correspondiente
	// a la nueva velocidad de procesamiento, tambien se incluyen los pines Tx y Rx correspondientes

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

	handlerUSART2Capture.ptrUSARTx                      = USART2;
	handlerUSART2Capture.USART_Config.USART_baudrate    = USART_BAUDRATE_115200;
	handlerUSART2Capture.USART_Config.USART_enableInRx  = USART_INTERRUPT_RX_ENABLE;
	handlerUSART2Capture.USART_Config.USART_enableInTx  = USART_INTERRUPT_TX_DISABLE;
	handlerUSART2Capture.USART_Config.USART_mode        = USART_MODE_RXTX;
	handlerUSART2Capture.USART_Config.USART_parity      = USART_PARITY_NONE;
	handlerUSART2Capture.USART_Config.USART_stopbits    = USART_STOPBIT_1;
	handlerUSART2Capture.USART_Config.USART_datasize    = USART_DATASIZE_8BIT;
	handlerUSART2Capture.USART_Config.USART_parityError = 0;
	USART_Config(&handlerUSART2Capture);

	//PIN A0 como salida de PWM en el canal 1 del TIM5.

	handlerPINPwm.pGPIOx = GPIOA;
	handlerPINPwm.GPIO_PinConfig.GPIO_PinAltFunMode = AF2;
	handlerPINPwm.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerPINPwm.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINPwm.GPIO_PinConfig.GPIO_PinNumber = PIN_0;
	handlerPINPwm.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINPwm.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINPwm);

	handlerPWMFreq.ptrTIMx                = TIM5;
	handlerPWMFreq.config.channel         = PWM_CHANNEL_1;
	handlerPWMFreq.config.duttyCicle      = 50;
	handlerPWMFreq.config.periodo         = 600;
	handlerPWMFreq.config.prescaler       = PWM_SPEED_100us;
	pwm_Config(&handlerPWMFreq);
	startPwmSignal(&handlerPWMFreq);


}
//Callback para el blinky
void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerPINLED);
}
//Callback para captura de frecuencia de TIM3 canal 1
void Capture_TIM3_Ch1_Callback(void){
	flag = SET;
}

//Interrupciones del USART2 por recepcion, almacenamos en auxData el comando enviado por Coolterm
void usart2Rx_Callback(void){
	auxData = getRxData();
}

/*
 * Hola Juan Esteban,
 * En general es la misma solución que todos aparentemente decidieron hacer.
 * No es ideal tener algo ahí recibiendo siempre sin necesidad, es mejor tener algo que se pueda iniciar y detener.
 *
 * Calificación = 4.5
 *
 * */


