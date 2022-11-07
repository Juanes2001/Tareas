/*
 * OLEDDriver.h
 *
 *  Created on: 6/11/2022
 *      Author: ALEJANDRA MARIA
 */

#ifndef OLEDDRIVER_H_
#define OLEDDRIVER_H_

#include <stm32f4xx.h>
#include "I2CDriver.h"

#define OLED_CONTROLBYTE_CONFIG  0x00
#define OLED_CONTROLBYTE_DISPLAY 0x40

#endif /* OLEDDRIVER_H_ */


void sendBytes(I2C_Handler_t *ptrHandlerI2C, uint8_t command, char *dataToSent);




