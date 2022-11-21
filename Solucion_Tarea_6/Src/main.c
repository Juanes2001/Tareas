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

//handlers GPIO para el led blinky, el pin Tx y Rx, y el pin de salida de señal de reloj

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

uint32_t timestamp1 = 0;
uint32_t timestamp2 = 0;
uint32_t deltaTimestamp = 0;
uint8_t counterCapture = 0;
uint8_t auxData = '\0';
uint8_t flag = RESET;
char bufferData[64];

void initSystem(void);

//Aqui se ejecuta todo el programa , el corazon del mismo
int main(void){

	initSystem();

	while(1){

		if (flag){
			writeMsg(&handlerUSART2Capture, "si");
			flag = RESET;
		}

		if (auxData != '\0'){
			if (auxData == 's'){
				startPwmSignal(&handlerPWMFreq);
				startCapture(&handlerCaptureFreq);
				writeChar(&handlerUSART2Capture, auxData);
				writeMsg(&handlerUSART2Capture, "Timer5 ON \n \r");
				auxData = '\0';
			}else if (auxData == 'p'){
				stopCapture(&handlerCaptureFreq);
				writeChar(&handlerUSART2Capture, auxData);
				writeMsg(&handlerUSART2Capture, "Timer5 OFF \n \r");
				auxData = '\0';
			}
		}
		if (auxData == 'l'){
			if (flag){
				sprintf(bufferData, "period = %ums\n\r",(unsigned int) timestamp2 - (unsigned int) timestamp1);
				writeMsg(&handlerUSART2Capture, bufferData);
				flag = RESET;
				counterCapture = 0;
				timestamp1 = 0;
				timestamp2 = 0;
			}
		}
	}
}


void initSystem(void){

	handlerPINCapture.pGPIOx = GPIOA;
	handlerPINCapture.GPIO_PinConfig.GPIO_PinAltFunMode = AF2;
	handlerPINCapture.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerPINCapture.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINCapture.GPIO_PinConfig.GPIO_PinNumber = PIN_0;
	handlerPINCapture.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINCapture.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINCapture);

	handlerCaptureFreq.ptrTIMx                 = TIM5;
	handlerCaptureFreq.config.channel          = CAPTURE_CHANNEL_1;
	handlerCaptureFreq.config.edgeSignal       = CAPTURE_RISING_EDGE;
	handlerCaptureFreq.config.prescalerCapture = CAPTURE_PREESCALER_1_1;
	handlerCaptureFreq.config.timerSpeed       = CAPTURE_TIMER_SPEED_1us;
	capture_Config(&handlerCaptureFreq);

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

	handlerPINPwm.pGPIOx = GPIOB;
	handlerPINPwm.GPIO_PinConfig.GPIO_PinAltFunMode = AF2;
	handlerPINPwm.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerPINPwm.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINPwm.GPIO_PinConfig.GPIO_PinNumber = PIN_4;
	handlerPINPwm.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINPwm.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINPwm);

	handlerPWMFreq.ptrTIMx                = TIM3;
	handlerPWMFreq.config.channel         = PWM_CHANNEL_1;
	handlerPWMFreq.config.duttyCicle      = 50;
	handlerPWMFreq.config.periodo         = 1000;
	handlerPWMFreq.config.prescaler       = PWM_SPEED_100us;
	pwm_Config(&handlerPWMFreq);


}

void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerPINLED);
}

void Capture_TIM5_Ch1_Callback(void){
	flag = SET;
	if (counterCapture == 0){
		timestamp1 = TIM5->CCR1;
		counterCapture++;
	}else if (counterCapture == 1){
		timestamp2 = TIM5->CCR1;
		counterCapture++;
	}else{
		__NOP();
	}

}

//Interrupciones del USART2 por recepcion, almacenamos en auxData el comando enviado por Coolterm
void usart2Rx_Callback(void){
	auxData = getRxData();
}

