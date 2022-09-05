/*
 * IntroGPIOxMain.c
 *
 *  Created on: Aug 27, 2022
 *      Author: JUAN ESTEBAN
 */


#include <stdint.h>

#include "stm32f411xx_hal.h"
#include "GPIOxDriver.h"

/*Funcion principal del programa. Es aca donde se ejecuta todo
 */

int main(void){

	/**
	 * Punto 1---> Reconocimiento de error de la función GPIO_ReadPin()
	 */

	/*
	 * Este punto se resuelve teniendo en cuenta que lo que queremos que nos entregue la función es el estado (ON/OFF) del pin configuirado dentro del main.
	 * Para ello, una solucion posible es con un puntero, crear una ruta que me ubique el registro de todos los pines GPIO, los IDR,
	 * (El cual corresponde a un número binario de 16 bits, el mismo número de pines en cada puerto). Cada bit almacenado corresponde al estado propio del pin
	 * correspondiente. Asi, si queremos saber el estado de un pin en una posicion x, debemos hacer shift a la derecha x veces para ubicar asi al estado del IDR
	 * en la posicion 0, para asi obtener un 1 o un 0 lógico. Ésta solucion funciona solo para un pin encendido, pero si varios pines estan encendidos, tendremos
	 * que al hacer shift derecho, estaremos omitiendo los bitos posiblemente en estado ON a la izquierda de el bit en la posicion 0 posterior al shift.
	 *
	 */

	//Comprobaremos lo deducido. encenderemos 2 pines distintos en un mismo puerto.
	GPIO_Handler_t handlerUserLedPin ={0};


	handlerUserLedPin.pGPIOx                                = GPIOA;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinNumber         = PIN_5;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinMode           = GPIO_MODE_OUT;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinOPType         = GPIO_OTYPE_PUSHPULL;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_PULLDOWN;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinSpeed          = GPIO_OSPEEDR_MEDIUM;
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinAltFunMode     = AF0; //Ninguna función


	GPIO_Config(&handlerUserLedPin);

	GPIO_WritePin(&handlerUserLedPin, SET);

	GPIO_Handler_t handlerUserButton ={0};


	handlerUserButton.pGPIOx                                = GPIOA;
	handlerUserButton.GPIO_PinConfig.GPIO_PinNumber         = PIN_12;
	handlerUserButton.GPIO_PinConfig.GPIO_PinMode           = GPIO_MODE_OUT;
	handlerUserButton.GPIO_PinConfig.GPIO_PinOPType         = GPIO_OTYPE_PUSHPULL;
	handlerUserButton.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_PULLUP;
	handlerUserButton.GPIO_PinConfig.GPIO_PinSpeed          = GPIO_OSPEEDR_MEDIUM;
	handlerUserButton.GPIO_PinConfig.GPIO_PinAltFunMode     = AF0; //Ninguna función

	GPIO_Config(&handlerUserButton);

	GPIO_WritePin(&handlerUserButton, RESET);



	uint16_t BITS_IDR = GPIOA->IDR;
	/*
	 * En la variable BITS_IDR almacenaremos el binario del registro IDR, el cual corresponde a
	 * los estados de los pines 0 al 15 de cada puerto, en el ejemplo anterior vemos que modificamos
	 * independientemente el Pin 5 y el Pin 12 (Encendemos el pin 5 y apagamos el pin 12 respectivamente).
	 * Podemos ver que efectivamente tales posiciones estan con su correspondiente bit de encendido o apagado.
	 * Pero también existen algunos pines que siguen activados. Esto es un problema porque si queremos ver el
	 * estado de un bit en especifico, al hacer shift derecho, algunos bits a la izquierda estaran aun en ON.
	 */

	/*
	 * Para resolver este problema una solución sencilla es aplicar una mascara que elimine todos los bits que
	 * no queremos del registro IDR fuera de la posicion deseada dentro de la función GPIO_ReadPin().De la siguiente manera:
	 * uint16_t mask= 0b1 << pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber;
	 * pinValue &= mask;
	 * return pinValue
	 */

	uint8_t BIN_IDR = GPIO_ReadPin(&handlerUserLedPin);
	//Al ejecutar paso a paso la funcion vemos que ahora si cumple con lo que queremos.

	/**
	 * Punto 2--> Creación de la función GPIOxTooglePin para cambiar el estado del PINx.
	 */

	while(1){


		GPIOxTooglePin (&handlerUserLedPin);
		BIN_IDR=GPIO_ReadPin(&handlerUserLedPin);
	}


	while(1){
		NOP();
	}

}
