

/*
 * OLEDDriver.c
 *
 *  Created on: 20/10/2022
 *      Author: Juan Esteban Rodriguez Ochoa
 *      Email: jrodriguezoc@unal
 */


#include <stdint.h>
#include "I2CDriver.h"
#include "OLEDDriver.h"

//OLED Driver

void sendBytes(I2C_Handler_t *ptrHandlerI2C, uint8_t command, char *dataToSent){

	i2c_startTransaction(ptrHandlerI2C);

	i2c_sendSlaveAddressRW(ptrHandlerI2C, ptrHandlerI2C->slaveAddress, I2C_WRITE_DATA);

	i2c_sendDataByte (ptrHandlerI2C, command);

	i2c_sendDataByte (ptrHandlerI2C, *(dataToSent+1));

	i2c_sendDataByte (ptrHandlerI2C, *(dataToSent+2));

	i2c_stopTransaction(ptrHandlerI2C);

}

