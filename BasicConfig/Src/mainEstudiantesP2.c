/**
 **************************************************************************************************
 * @file           : MainTaller7Parte2.c
 * @authors        : Alejandro Vélez Patrouilleau - alvelezp@unal.edu.co
 * 				   : Juan Pablo Toro Arenas		  - jutoroa@unal.edu.co
 * @brief          : Introduccion al sprintF y a la conversión ADC:
 *
 **************************************************************************************************
 */

#include <stm32f4xx.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "stm32f4xx.h"
#include "GPIOxDriver.h"
#include "BasicTimer.h"
#include "USARTxDriver.h"
#include "AdcDriver.h"
#include "I2CDriver.h"
#include "EXTIDriver.h"
#include "PwmDriver.h"


// Definicion de los handlers necesarios
GPIO_Handler_t handlerStateLed = {0};
GPIO_Handler_t handlerPinTx		= {0};
GPIO_Handler_t handlerPinRx		= {0};

// Timer encargado del Blinky
BasicTimer_Handler_t handlerStateTimer 	= {0};

// Utiliza la conexion USB
USART_Handler_t handlerUsart2		= {0};

// Variables USART
uint8_t rxData = 0;
char bufferData[64] = "esto es una pequeña prueba";

uint32_t systemTicks = 0;
uint32_t systemTicksStart = 0;
uint32_t systemTicksEnd = 0;

/* Configuracion para el I2C*/
GPIO_Handler_t handlerI2cSDA = {0};
GPIO_Handler_t handlerI2cSCL = {0};
I2C_Handler_t handlerAcceletometer = {0};

uint8_t i2cBuffer = 0;


#define ACCEL_ADDRESS 0b11100101
#define ACCEL_XOUT_H  59
#define ACCEL_XOUT_L  60
#define ACCEL_YOUT_H  61
#define ACCEL_YOUT_L  62
#define ACCEL_ZOUT_H  63
#define ACCEL_ZOUT_L  64

#define PWR_MGMT_1    107
#define WHO_AM_I      175


/* Definición de prototipos de funciones */
void InitSystem(void);

/**
 * Funcion principal del programa.
 * Esta funcion es el corazón del programa!!
 *
 * */
int main(void) {

	// Inicializamos todos los elementos del sistema
	InitSystem();

	while(1){

		if(rxData != '\0'){
			writeChar(&handlerUsart2, rxData);

			if(rxData == 'd'){
				i2cBuffer = i2c_readSingleRegister(&handlerAcceletometer, WHO_AM_I);
				sprintf (bufferData, "dataRead = 0x%2x \n", (unsigned int) i2cBuffer);
				writeMsg(&handlerUsart2, bufferData);
			}else if (rxData == 'p'){
				i2cBuffer =i2c_readSingleRegister(&handlerAcceletometer, PWR_MGMT_1);
				sprintf (bufferData, "dataRead = 0x%2x \n", (unsigned int) i2cBuffer);
				writeMsg(&handlerUsart2, bufferData);
				rxData = '\0';
			}else if (rxData == 'r'){
				i2c_writeSingleRegister(&handlerAcceletometer, PWR_MGMT_1, 0x00);
				rxData = '\0';

			}else if (rxData == 'x'){
				uint8_t AccelX_low = i2c_readSingleRegister(&handlerAcceletometer, ACCEL_XOUT_L);
				uint8_t AccelX_high = i2c_readSingleRegister(&handlerAcceletometer, ACCEL_XOUT_H);
				int16_t AccelX = AccelX_high << 8 | AccelX_low;
				sprintf(bufferData, "AccelX = %d \n", (int) AccelX);
				writeMsg(&handlerUsart2, bufferData);
				rxData = '\0';
			}else if (rxData == 'y'){
				uint8_t AccelY_low = i2c_readSingleRegister(&handlerAcceletometer, ACCEL_YOUT_L);
				uint8_t AccelY_high = i2c_readSingleRegister(&handlerAcceletometer, ACCEL_YOUT_H);
				int16_t AccelY = AccelY_high << 8 | AccelY_low;
				sprintf(bufferData, "AccelY = %d \n", (int) AccelY);
				writeMsg(&handlerUsart2, bufferData);
				rxData = '\0';
			}else if (rxData == 'z'){
				uint8_t AccelZ_low = i2c_readSingleRegister(&handlerAcceletometer, ACCEL_ZOUT_L);
				uint8_t AccelZ_high = i2c_readSingleRegister(&handlerAcceletometer, ACCEL_ZOUT_H);
				int16_t AccelZ = AccelZ_high << 8 | AccelZ_low;
				sprintf(bufferData, "AccelZ = %d \n", (int) AccelZ);
				writeMsg(&handlerUsart2, bufferData);
				rxData = '\0';
			}
			else{
				rxData = '\0';
			}
		}

	}
}

/*
 * Funcion encargada de la inicializacion de los elementos del sistema
 */
void InitSystem(void){

	//Configuracion del pin que controla el led de la board
	handlerStateLed.pGPIOx                             = GPIOA;
	handlerStateLed.GPIO_PinConfig.GPIO_PinNumber      = PIN_8;
	handlerStateLed.GPIO_PinConfig.GPIO_PinAltFunMode  = AF0;
	handlerStateLed.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	handlerStateLed.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerStateLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerStateLed.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerStateLed);

	GPIO_WritePin(&handlerStateLed, SET);

	handlerPinTx.pGPIOx                             = GPIOA;
	handlerPinTx.GPIO_PinConfig.GPIO_PinAltFunMode  = AF7;
	handlerPinTx.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerPinTx.GPIO_PinConfig.GPIO_PinNumber      = PIN_2;
	handlerPinTx.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPinTx.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPinTx.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinTx);

	handlerPinRx.pGPIOx                             = GPIOA;
	handlerPinRx.GPIO_PinConfig.GPIO_PinAltFunMode  = AF7;
	handlerPinRx.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerPinRx.GPIO_PinConfig.GPIO_PinNumber      = PIN_3;
	handlerPinRx.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPinRx.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPinRx.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinRx);


	handlerUsart2.ptrUSARTx = USART2;
	handlerUsart2.USART_Config.USART_baudrate = USART_BAUDRATE_115200;
	handlerUsart2.USART_Config.USART_datasize = USART_DATASIZE_8BIT;
	handlerUsart2.USART_Config.USART_enableInRx = USART_INTERRUPT_RX_ENABLE;
	handlerUsart2.USART_Config.USART_enableInTx = USART_INTERRUPT_TX_DISABLE;
	handlerUsart2.USART_Config.USART_mode       = USART_MODE_RXTX;
	handlerUsart2.USART_Config.USART_parity   = USART_PARITY_ODD;
	handlerUsart2.USART_Config.USART_stopbits = USART_STOPBIT_1;
	USART_Config(&handlerUsart2);

	handlerStateTimer.ptrTIMx = TIM2;
	handlerStateTimer.TIMx_Config.TIMx_interruptEnable = 1;
	handlerStateTimer.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerStateTimer.TIMx_Config.TIMx_period = 2500;
	handlerStateTimer.TIMx_Config.TIMx_speed = BTIMER_SPEED_100us;
	BasicTimer_Config(&handlerStateTimer);

	config_SysTickMs();

	handlerI2cSCL.pGPIOx                             = GPIOB;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinAltFunMode  = AF4;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinNumber      = PIN_6;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_OPENDRAIN;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLUP;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerI2cSCL);

	handlerI2cSDA.pGPIOx                             = GPIOB;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinAltFunMode  = AF4;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinNumber      = PIN_7;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_OPENDRAIN;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLUP;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerI2cSDA);

	handlerAcceletometer.ptrI2Cx = I2C1;
	handlerAcceletometer.modeI2C = I2C_MODE_FM;
	handlerAcceletometer.slaveAddress = ACCEL_ADDRESS;
	i2c_config(&handlerAcceletometer);


}

void usart2Rx_Callback(void){
	rxData = getRxData();
}


void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerStateLed);
}
