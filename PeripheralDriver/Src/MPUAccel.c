/*
 * MPUAccel.c
 *
 *  Created on: 26/11/2022
 *      Author: ALEJANDRA MARIA
 */

#include "MPUAccel.h"
#include "I2CDriver.h"

uint8_t i2cBuffer   = 0;  //almacena valores leidos en los registros del acelerometro
uint8_t AccelX_low  = 0;
uint8_t AccelX_high = 0;
int16_t AccelX      = 0;
uint8_t AccelY_low  = 0;
uint8_t AccelY_high = 0;
int16_t AccelY      = 0;

void readAccelValues(I2C_Handler_t *ptrHandlerI2C){

	i2cBuffer = i2c_readSingleRegister(ptrHandlerI2C, WHO_AM_I);

	i2cBuffer = i2c_readSingleRegister(ptrHandlerI2C, PWR_MGMT_l);

	i2c_writeSingleRegister(ptrHandlerI2C, PWR_MGMT_l, 0x00);
}

void writeAccelValues(I2C_Handler_t *ptrHandlerI2C,uint8_t address, uint8_t newValue){

	i2c_startTransaction (ptrHandlerI2C);

	i2c_sendSlaveAddressRW (ptrHandlerI2C,ACCEL_ADDRESS,0);

	i2c_sendMemoryAddress (ptrHandlerI2C, address);

	i2c_sendDataByte (ptrHandlerI2C, newValue);

	i2c_stopTransaction (ptrHandlerI2C);

}

int16_t X_Accel(I2C_Handler_t *ptrHandlerI2C){
	AccelX_low  = i2c_readSingleRegister(ptrHandlerI2C,ACCEL_XOUT_L);
	AccelX_high = i2c_readSingleRegister(ptrHandlerI2C,ACCEL_XOUT_H);
	AccelX = AccelX_high << 8 | AccelX_low;
	return AccelX;
}
int16_t Y_Accel(I2C_Handler_t *ptrHandlerI2C){

	AccelY_low = i2c_readSingleRegister(ptrHandlerI2C,ACCEL_YOUT_L);
	AccelY_high = i2c_readSingleRegister(ptrHandlerI2C,ACCEL_YOUT_H);
	AccelY = AccelY_high << 8 | AccelY_low;
	return AccelY;
}
