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

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variables que se usaron para la demostracion, counter: variable contadora que se imprimira
// en la comunicacion serial, auxData, para recibir los caracteres de la comunicacion
// serial tipo comandos [start (s), stop(p)], bufferData, arreglo string que guardara
// el mensaje que se enviara por comunicacion serial, flag, para recibir las interrupciones del timper 2
// en el main.

uint32_t counter    = 0;
uint8_t auxData     = '\0';
char bufferData[64] = {0};
uint8_t flag        = 0;

//handlers GPIO para el led blinky, el pin Tx y Rx, y el pin de salida de señal de reloj

GPIO_Handler_t handlerPINLED           = {0};
GPIO_Handler_t handlerUSARTPinTx       = {0};
GPIO_Handler_t handlerUSARTPinRx       = {0};
GPIO_Handler_t handlerPINHighFreq      = {0};

//handler del timer para interrupciones
BasicTimer_Handler_t handlerTIMHighVel = {0};
//handler Usart 2
USART_Handler_t handlerUSARTHighVel    = {0};


void initSystem(void);
//Aqui se ejecuta todo el programa , el corazon del mismo
int main(void){

	//Se ejecuta ya la nueva opcion de aumentar la velocidad del MCU a su maximo potencial
	// ademas de la iniciacion de las configuraciones de los handlers
	RCC_enableMaxFrequencies();
	initSystem();

	while(1){

		if (auxData != '\0'){
			if (auxData == 's'){
				startTimer(&handlerTIMHighVel);
				writeMsg(&handlerUSARTHighVel, "Timer ON \n \r");
				auxData = '\0';
			}else if (auxData == 'p'){
				stopTimer(&handlerTIMHighVel);
				writeMsg(&handlerUSARTHighVel, "Timer OFF \n \r");
				auxData = '\0';
			}
		}

		if (flag){
			sprintf(bufferData, "counter = %u \n\r",(unsigned int) counter);
			writeMsg(&handlerUSARTHighVel, bufferData);
			flag = RESET;
		}

	}

}

void initSystem(void){

	//PIN de salida de señal de reloj A8, conectar a osciloscopio si se desa para ver la salida
	// de señal con un preescalamiento de 4 (25MHz aprox)

	handlerPINHighFreq.pGPIOx = GPIOA;
	handlerPINHighFreq.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINHighFreq.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerPINHighFreq.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINHighFreq.GPIO_PinConfig.GPIO_PinNumber = PIN_8;
	handlerPINHighFreq.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINHighFreq.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINHighFreq);




	//PIN LED para el blinky junto con su timer configurado con sus nuevas macros que corresponden
	//a la nueva velocidad de preescalamiento
	handlerPINLED.pGPIOx = GPIOA;
	handlerPINLED.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerPINLED.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerPINLED.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerPINLED.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerPINLED.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPINLED.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPINLED);

	handlerTIMHighVel.ptrTIMx = TIM2;
	handlerTIMHighVel.TIMx_Config.TIMx_interruptEnable = 1;
	handlerTIMHighVel.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerTIMHighVel.TIMx_Config.TIMx_period = 2500;
	handlerTIMHighVel.TIMx_Config.TIMx_speed = BTIMER_SPEED_100MHz_100us;
	BasicTimer_Config(&handlerTIMHighVel);

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


	handlerUSARTHighVel.ptrUSARTx                      = USART2;
	handlerUSARTHighVel.USART_Config.USART_baudrate    = USART_BAUDRATE_CUSTOM_USART2;
	handlerUSARTHighVel.USART_Config.USART_enableInRx  = USART_INTERRUPT_RX_ENABLE;
	handlerUSARTHighVel.USART_Config.USART_enableInTx  = USART_INTERRUPT_TX_DISABLE;
	handlerUSARTHighVel.USART_Config.USART_mode        = USART_MODE_RXTX;
	handlerUSARTHighVel.USART_Config.USART_parity      = USART_PARITY_NONE;
	handlerUSARTHighVel.USART_Config.USART_stopbits    = USART_STOPBIT_1;
	handlerUSARTHighVel.USART_Config.USART_datasize    = USART_DATASIZE_8BIT;
	handlerUSARTHighVel.USART_Config.USART_parityError = 0;
	USART_Config(&handlerUSARTHighVel);

}
//Interrupcion del timer, incluye el cambio de estado para el LED, el aumento de conteo del counter
// y el seteo del flag
void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerPINLED);
	counter++;
	flag = SET;
}
//interrupciones del USART2 por recepcion, almacenamos en auxData el comando enviado por Coolterm
void usart2Rx_Callback(void){
	auxData = getRxData();
}

