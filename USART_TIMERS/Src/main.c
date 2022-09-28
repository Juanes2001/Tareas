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

int main(void)
{
/*
 * La función inSystem() entrega todas las configuraciones de todos los periféricos que
 * usaremos para la tarea.  (Dentro de la funcion se explicará la configuracion detallada
 * de cada periférico.)
 *
 */
	inSystem();

	/*
	 * Configurados los perifericos, procedemos a dejar el programa en un while infinito,
	 * donde el programa evaluara constantemente si el MCU entra en una interrupcion o no,
	 * para hacer la evaluacion usamos el flag1 definido anteriormente, donde 1 representa que
	 * el programa entro en una interrupcion, y 0 indica que no lo esta.
	 */
	while(1){
		if (flag1 == 1){
			/*
			 * Entrada en la interrupcion, querremos el programa haga 3 cosas, nos cambie el
			 * estado del LED (blinky) nos mande un valor fijo por el Pin específico configurado
			 * como TX del protocolo, y cuando presionamos el User Button, ya los valores a
			 * mandar no sean fijos, sino que sean distintos sucedida cada interrupción.
			 */

			if (GPIO_ReadPin(&handlerUserButton) == 0){
				/*
				 * Presionado el botón, la interrupción mandará uno de los caracteres del
				 * ASCII comenzando por el caracter 33.
				 *
				 */
				countr += 1;
				/*
				 * Hacemos un condicional que permita resetear los digitos que representan
				 * los caracteres.
				 */
				if (countr >= 255){
					countr = 32;
				}
				/*
				 * Mandamos el valor respectivo por el PIN A9 al analizador cuando el User
				 * Button está presionado.
				 */
				writeChar(&handlerUSART1, countr);

			}else{
				/*
				 * Mandamos el valor fijo por el PIN A9 al analizador cuando el User Button
				 * no esta presionado.
				 */
				writeChar(&handlerUSART1, 45);
			}
			/*
			 * Bajamos la bandera para que se cumpla la condicion de interrupción.
			 */
			flag1 = 0;
		}else{
			/*
			 * Mientras no se ejecute la interrupción, nuestro deseo es que e programa no
			 * haga absolutamente nada.
			 */
			__NOP();
		}
	}

}

void inSystem (void){


	/*
	 * Configuramos el pin que mandara la señal al LED , simplemente un pin de estado
	 * en modo salida.
	 */
	handlerUserLed.pGPIOx =GPIOA;
	handlerUserLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerUserLed.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerUserLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerUserLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerUserLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	handlerUserLed.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;

	GPIO_Config(&handlerUserLed);
	/*
	 * Configuramos el pin de estado que sera controlado por el USER Button.
	 */
	handlerUserButton.pGPIOx =GPIOC;
	handlerUserButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	handlerUserButton.GPIO_PinConfig.GPIO_PinNumber = PIN_13;
	handlerUserButton.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerUserButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerUserButton.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	handlerUserButton.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;

	GPIO_Config(&handlerUserButton);

	handlerGPIOUSART1.pGPIOx =GPIOA;
	handlerGPIOUSART1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerGPIOUSART1.GPIO_PinConfig.GPIO_PinNumber = PIN_9;
	handlerGPIOUSART1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerGPIOUSART1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerGPIOUSART1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	handlerGPIOUSART1.GPIO_PinConfig.GPIO_PinAltFunMode = AF7;

	GPIO_Config(&handlerGPIOUSART1);

	handlerTIM2.ptrTIMx = TIM2;
	handlerTIM2.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerTIM2.TIMx_Config.TIMx_speed = BTIMER_SPEED_100us;
	handlerTIM2.TIMx_Config.TIMx_period = 2500;
	handlerTIM2.TIMx_Config.TIMx_interruptEnable=1;

	BasicTimer_Config(&handlerTIM2);

	handlerUSART1.ptrUSARTx = USART1;
	handlerUSART1.USART_Config.USART_baudrate = USART_BAUDRATE_115200;
	handlerUSART1.USART_Config.USART_datasize = USART_DATASIZE_9BIT;
	handlerUSART1.USART_Config.USART_mode = USART_MODE_TX;
	handlerUSART1.USART_Config.USART_stopbits = USART_STOPBIT_1;

	USART_Config(&handlerUSART1);


}

void BasicTimer2_Callback(void){
		GPIOxTooglePin(&handlerUserLed);
		flag1 = 1;
}

