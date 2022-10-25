/*
 * I2CDriver.c
 *
 *  Created on: 23 oct. 2022
 *      Author: JUAN ESTEBAN
 */

#include <stdint.h>
#include "I2CDriver.h"

/*
 * Recordar que se debe configurar los pines para el I2C (SDA y SCL)
 * para lo cual se necesita el modilo GPIO y los pines configurados en el modo
 * alternate function.
 * Ademas, estos pines deben ser configurados como salidas Open-Dran
 * y con la resistencias en modo pull-up
 */

void i2c_config(I2C_Handler_t *ptrHandlerI2C){
	/* 1Activamos la señal de reloj para el modulo I2C seleccionado*/
	if(ptrHandlerI2C->ptrI2Cx == I2C1){
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	}else if(ptrHandlerI2C->ptrI2Cx == I2C2){
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	}else if(ptrHandlerI2C->ptrI2Cx == I2C3){
		RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
	}
}
