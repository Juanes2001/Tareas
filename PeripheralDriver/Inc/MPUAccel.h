/*
 * MPUAccel.h
 *
 *  Created on: 26/11/2022
 *      Author: ALEJANDRA MARIA
 */

#ifndef MPUACCEL_H_
#define MPUACCEL_H_

#include <stm32f4xx.h>
#include "I2CDriver.h"



/* Definiciones para funcionamiento del acelerometro */
#define ACCEL_ADDRESS 		0b1101001;
#define ACCEL_XOUT_H 		59
#define ACCEL_XOUT_L		60
#define ACCEL_YOUT_H		61
#define ACCEL_YOUT_L		62
#define ACCEL_ZOUT_H		63
#define ACCEL_ZOUT_L		64

#define PWR_MGMT_l			107
#define WHO_AM_I			117

void readAccelValues(I2C_Handler_t *ptrHandlerI2C);
int16_t X_Accel(I2C_Handler_t *ptrHandlerI2C);
int16_t Y_Accel(I2C_Handler_t *ptrHandlerI2C);



#endif /* MPUACCEL_H_ */
