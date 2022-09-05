/*
 * IntroGPIOxMain.c
 *
 *  Created on: Aug 27, 2022
 *      Author: JUAN ESTEBAN
 */


#include <stdint.h>

#include "stm32f411xx_hal.h"
#include "GPIOxDriver.h"

uint8_t BIN_IDR = 0;

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



	//uint16_t BITS_IDR = GPIOA->IDR; //Descomentar para observar en la evolucion de las variables como se ve modificada la variable

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

	//uint8_t BIN_IDR = GPIO_ReadPin(&handlerUserLedPin);
	//Al ejecutar paso a paso la funcion vemos que ahora si cumple con lo que queremos.

	/**
	 * Punto 2--> Creación de la función GPIOxTooglePin para cambiar el estado del PINx.
	 */

	void GPIOxTooglePin (GPIO_Handler_t *pPinState){
		uint8_t state = GPIO_ReadPin (pPinState);
		if (state== SET){
			GPIO_WritePin(pPinState, RESET);
		}
		else{
			GPIO_WritePin(pPinState, SET);
		}

		/*
		 * Vemos que la función que cambia de estado el pin deseado es simple, se usan las
		 * funciones de leer y escribir sobre la posición de memoria correspondiente  al pin
		 * que se desea modificar. Hacemos una lectura de el estado actual del pin con la funcion
		 * GPIO_ReadPin() usando el puntero *pPinState de tipo GPIO_Handler que apunta a los elementos
		 * de la estructura asignandole la posicion de memoria en la que se guardan las configuraciones
		 * del pin respectivo. Posterior a la lectura, la respuesta sera un valor logico 0 o 1, (OFF/ON respectivamente)
		 * por lo que hacemos un condicional simple , evaluando que si el estado es encendido (1) entonces aplicamos la
		 * función GPIO_WritePin() para cambiar el estado a apagado, de lo contrario, cambia el estado a encendido con
		 * la misma funcion write.
		 */

	}

	for (uint8_t i=0; i<6; i++){

		//Vemos que al aplicar el ciclo for, y observando el estado del pin en el IDR dentro del handlerUserLedPin.
		//estara cambiando de estado cada vez que aplicamos la funcion , BIN_IDR almacenará el estado del pin.
		GPIOxTooglePin (&handlerUserLedPin);
		BIN_IDR=GPIO_ReadPin(&handlerUserLedPin);
	}

	/**
	 * Punto 3---> Configuracion del user Button para que por pulsaciones se ejecute la funcion GPIOxTooglePin
	 * sobre el user LED.
	 */


	GPIO_Handler_t handlerUserButton2 = {0};

	handlerUserButton2.pGPIOx                                = GPIOC;
	handlerUserButton2.GPIO_PinConfig.GPIO_PinNumber         = PIN_13;
	handlerUserButton2.GPIO_PinConfig.GPIO_PinMode           = GPIO_MODE_IN;
	handlerUserButton2.GPIO_PinConfig.GPIO_PinOPType         = GPIO_OTYPE_PUSHPULL;
	handlerUserButton2.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_NOTHING;
	handlerUserButton2.GPIO_PinConfig.GPIO_PinSpeed          = GPIO_OSPEEDR_MEDIUM;
	handlerUserButton2.GPIO_PinConfig.GPIO_PinAltFunMode     = AF0;

	GPIO_Config(&handlerUserButton2);

	BIN_IDR = GPIO_ReadPin(&handlerUserButton2);

	while(1){


		if (GPIO_ReadPin(&handlerUserLedPin) == 1 || GPIO_ReadPin(&handlerUserLedPin) == 0){
			if (GPIO_ReadPin(&handlerUserButton2)==0){
				GPIOxTooglePin(&handlerUserLedPin);
			}
			else{
				NOP();
			}
		}
		else{
			NOP();
		}

	}
	while(1){
		NOP();
	}

}
