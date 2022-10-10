/**
 ******************************************************************************
 * USART_TIMERS
 * Autor: Juan Esteban Rodriguez Ochoa
 * Correo: jrodriguezoc@unal.edu.co
 ******************************************************************************
 ******************************************************************************
 */
#include "BasicTimer.h"
#include "GPIOxDriver.h"
#include "USARTxDriver.h"
#include <stdint.h>
#include <stdbool.h>


void inSystem (void);

/*
 * Definimos todas las variables que almacenan la estructura de cada periferico a usar,
 * los GPIO para el LED  y el botton, y otro para usar el protocolo de comunicación USART.
 * Las variables respectivas para el Timer y también para el USART. Todas las inicializamos en 0
 */
GPIO_Handler_t handlerUserLed = {0};
GPIO_Handler_t handlerGPIOUSART1 = {0};
GPIO_Handler_t handlerUserButton = {0};
BasicTimer_Handler_t handlerTIM2 = {0};
USART_Handler_t handlerUSART1= {0};


/*
 * Creamos 2 variables, una bandera que nos ayudará a usar las interrupciones, y un contador
 * que almacenará los caracteres del codigo ASCII a partir del 32 en adelante, ya que los numeros
 * anteriores almacenan funciones.
 */
uint8_t flag1 = 0;
uint8_t countr = 32;

int main (void){

	while(1){

	}
}
