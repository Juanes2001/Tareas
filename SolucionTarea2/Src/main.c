/*
 * IntroGPIOxMain.c
 *
 *  Created on: Aug 27, 2022
 *      Author: JUAN ESTEBAN RODRIGUEZ OCHOA
 *      correo: jrodriguezoc@unal.edu.co
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
	handlerUserLedPin.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_NOTHING;
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
	// Esta función NO debería ser creaad dentro de otra función.

	for (uint8_t i=0; i<6; i++){

		//Vemos que al aplicar el ciclo for, y observando el estado del pin en el IDR dentro del handlerUserLedPin.
		//estara cambiando de estado cada vez que aplicamos la funcion , BIN_IDR almacenará el estado del pin.
		GPIOxTooglePin (&handlerUserLedPin);
		BIN_IDR=GPIO_ReadPin(&handlerUserLedPin);
	}

	/**
	 * Punto 3---> Configuración del user Button para que por pulsaciones se ejecute la funcion GPIOxTooglePin
	 * sobre el user LED.
	 */

	/*
	 * A continuacion vamos a definir la configuracion del USER Button para asi poder programar el estado del mismo
	 * y vincularlo con el estado del LED.
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
	//Aqui en BIN_IDR estaremos leyendo el estado del PIN sin pl
	for(uint8_t i=0; i<3;i++){
		BIN_IDR = GPIO_ReadPin(&handlerUserButton2); //Para cada iteracion analizar el estado del PIN C13 con el User Button
													 // presionado y otro con el mismo sin presionar, veremos que respectivamente
													 // la variable almacenara un 0 o un 1.
	}

	/*
	 * El siguiente loop infinito muestra la funcion de el GPIOxTooglePin() vinculado con el USER Button,
	 * Vemos que para que cada pulsacion corresponda exactamente con un estado especifico en el PIN del LED
	 * tendremos que agregar un "delay" en el cual permite que al microcontrolador le de tiempo de mantener el cambio de estado del LED
	 * gracias a la pulsación.
	 */
//	while(1){ //Descomentar para observar la función de encendido y apagado por el user button
//
//
//			if (GPIO_ReadPin(&handlerUserButton2)==0){
//				GPIOxTooglePin(&handlerUserLedPin);
//				for(uint32_t i=0; i<160000;i++){
//					NOP();
//				}
//
//			}
//
//
//	}


	/**
	 * Punto 4---> Configuración de LEDs controladas por el microcontrolador.
	 */

	/*
	 * Primero definimos los pines que vamos a usar, en este caso el PIN C6 que traera un boton externo en configuración
	 * PULLDOWN, para ello configuramos este ultimo en PULLDOWN y agregamos una fuente de voltaje externa al boton, también los
	 * pines C10 (LED rojo), C11(LED naranja), C12 (LED verde).
	 */

	GPIO_Handler_t handlerUserButton3 = {0};

	handlerUserButton3.pGPIOx                                = GPIOC;
	handlerUserButton3.GPIO_PinConfig.GPIO_PinNumber         = PIN_6;
	handlerUserButton3.GPIO_PinConfig.GPIO_PinMode           = GPIO_MODE_IN;
	handlerUserButton3.GPIO_PinConfig.GPIO_PinOPType         = GPIO_OTYPE_PUSHPULL;
	handlerUserButton3.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_PULLDOWN;
	handlerUserButton3.GPIO_PinConfig.GPIO_PinSpeed          = GPIO_OSPEEDR_MEDIUM;
	handlerUserButton3.GPIO_PinConfig.GPIO_PinAltFunMode     = AF0;

	GPIO_Config(&handlerUserButton3);

	BIN_IDR = GPIO_ReadPin(&handlerUserButton3);

	GPIO_Handler_t handlerUserLedPin1 = {0};

	handlerUserLedPin1.pGPIOx                                = GPIOC;
	handlerUserLedPin1.GPIO_PinConfig.GPIO_PinNumber         = PIN_10;
	handlerUserLedPin1.GPIO_PinConfig.GPIO_PinMode           = GPIO_MODE_OUT;
	handlerUserLedPin1.GPIO_PinConfig.GPIO_PinOPType         = GPIO_OTYPE_PUSHPULL;
	handlerUserLedPin1.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_NOTHING;
	handlerUserLedPin1.GPIO_PinConfig.GPIO_PinSpeed          = GPIO_OSPEEDR_MEDIUM;
	handlerUserLedPin1.GPIO_PinConfig.GPIO_PinAltFunMode     = AF0;

	GPIO_Config(&handlerUserLedPin1);

	GPIO_Handler_t handlerUserLedPin2 = {0};

	handlerUserLedPin2.pGPIOx                                = GPIOC;
	handlerUserLedPin2.GPIO_PinConfig.GPIO_PinNumber         = PIN_11;
	handlerUserLedPin2.GPIO_PinConfig.GPIO_PinMode           = GPIO_MODE_OUT;
	handlerUserLedPin2.GPIO_PinConfig.GPIO_PinOPType         = GPIO_OTYPE_PUSHPULL;
	handlerUserLedPin2.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_NOTHING;
	handlerUserLedPin2.GPIO_PinConfig.GPIO_PinSpeed          = GPIO_OSPEEDR_MEDIUM;
	handlerUserLedPin2.GPIO_PinConfig.GPIO_PinAltFunMode     = AF0;

	GPIO_Config(&handlerUserLedPin2);

	GPIO_Handler_t handlerUserLedPin3 = {0};

	handlerUserLedPin3.pGPIOx                                = GPIOC;
	handlerUserLedPin3.GPIO_PinConfig.GPIO_PinNumber         = PIN_12;
	handlerUserLedPin3.GPIO_PinConfig.GPIO_PinMode           = GPIO_MODE_OUT;
	handlerUserLedPin3.GPIO_PinConfig.GPIO_PinOPType         = GPIO_OTYPE_PUSHPULL;
	handlerUserLedPin3.GPIO_PinConfig.GPIO_PinPuPdControl    = GPIO_PUPDR_NOTHING;
	handlerUserLedPin3.GPIO_PinConfig.GPIO_PinSpeed          = GPIO_OSPEEDR_MEDIUM;
	handlerUserLedPin3.GPIO_PinConfig.GPIO_PinAltFunMode     = AF0;

	GPIO_Config(&handlerUserLedPin3);

	/*
	 * Luego de configurar los pines, conectamos los tres ultimos PINES a resistencias de 220 OHMS correspondiente a cada LED, y conectamos los ultimos a GND del MCU de tal forma que los pines sean configurados
	 * con un cambio de estado en funcion de las pulsaciones que se le de al boton. El siguiente Loop infinito condiciona la pulsacion de boton, con el encendido instantaneo de los tres LEDs, posteriormente, vamos
	 * apagando los LEDs como indica la tarea usando ciclos for como "delays".
	 */


	while(1){



		if (GPIO_ReadPin(&handlerUserButton3)==1){
			GPIO_WritePin(&handlerUserLedPin1, SET);
			GPIO_WritePin(&handlerUserLedPin2, SET);
			GPIO_WritePin(&handlerUserLedPin3, SET);
			for(uint32_t i=0; i<=5000000;i++){
				NOP();
			}
			GPIO_WritePin(&handlerUserLedPin3, RESET);
			for(uint32_t i=0; i<=2000000;i++){
				NOP();
			}
			GPIO_WritePin(&handlerUserLedPin2, RESET);
			for(uint32_t i=0; i<=1000000;i++){
				NOP();
			}
			GPIO_WritePin(&handlerUserLedPin1, RESET);

		}
		else{
			NOP();
		}

	}







	while(1){
		NOP();
	}

}
