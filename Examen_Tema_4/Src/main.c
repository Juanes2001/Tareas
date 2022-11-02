/**
 ******************************************************************************
 * Examen_Tema_4
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

#include <stdint.h>
#include <stdbool.h>

// configuracion USART que se usara = Serial_1, 28800, EVEN

//handler para ADC
ADC_Config_t handlerADCJoy = {0};

//Pines handler para comunicación Serial
GPIO_Handler_t handlerPinTx = {0};
GPIO_Handler_t handlerPinRx = {0};

//handler para USART1
USART_Handler_t handlerUSART1 = {0};

//handler para PWM
PWM_Handler_t handlerPWMrgb = {0};

//Handlers de Timers
BasicTimer_Handler_t handlerTIM2 = {0};


void inSystem (void);

int main(void){

	inSystem();

	while(1){

	}
}

void inSystem (void){




}


