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


	/*2. Reiniciamos el periferico de forma que inicia en un estado conocido*/
	ptrHandlerI2C->ptrI2Cx->CR1 |= I2C_CR1_SWRST;
	__NOP();
	ptrHandlerI2C->ptrI2Cx->CR1 &= ~I2C_CR1_SWRST;

	/*
	 * 3. Indicamos cual es la velocidad del reloj principal; que es la señal
	 * ultilizada por el periferico para generar la señal de reloj para el bus I2C
	 */
	ptrHandlerI2C->ptrI2Cx->CR2 &= ~(0b111111 << I2C_CR2_FREQ_Pos);
	ptrHandlerI2C->ptrI2Cx->CR2 |= (MAIN_CLOCK_16_MHz_FOR_I2C << I2C_CR2_FREQ_Pos);
	/*
	 * 4. Configuramos el modo I2C en el que el sistema funciona
	 * en esta configuracion se incluye tambien la velocidad del reloj
	 * y el tiempo maximo para el cambio de la señal (T-rise).
	 * Todo comienza con los dos registros en 0
	 */
	ptrHandlerI2C->ptrI2Cx->CCR = 0;
	ptrHandlerI2C->ptrI2Cx->TRISE = 0;

	if (ptrHandlerI2C->modeI2C == I2C_MODE_SM){

		//estamos en modo "standar" (SM Mode)
		//Seleccionamos el modo estandar
		ptrHandlerI2C->ptrI2Cx->CCR &= ~I2C_CCR_FS;

		//Configuraos el registro ue se encarga de generar la señal del reloj
		ptrHandlerI2C->ptrI2Cx->CCR |= (I2C_MODE_SM_SPEED_100KHz << I2C_CCR_CCR_Pos);

		// Configuraos el registro que controla el tiempo T-Rise maximo.
		ptrHandlerI2C->ptrI2Cx->TRISE |= I2C_MAX_RISE_TIME_SM;
	}else{
		//Estamos en modo "Fast" (FM mode)
		//Seleccionamos el modo Fast
		ptrHandlerI2C->ptrI2Cx->CCR |= I2C_CCR_FS;

		//Configuramos el registro que se encarga de generar la señal del reloj
		ptrHandlerI2C->ptrI2Cx->CCR |= (I2C_MODE_FM_SPEED_400KHz << I2C_CCR_CCR_Pos);

		//configuramos el registro que controla el tiempo T-Rise maximo
		ptrHandlerI2C->ptrI2Cx->TRISE |= I2C_MAX_RISE_TIME_FM;
	}

	/*
	 * 5. Activamos el modulo I2C
	 */
	ptrHandlerI2C->ptrI2Cx->CR1 |= I2C_CR1_PE;


}
/*
 * 8. Generamos la conducion de stop
 */
void i2c_stopTransaction (I2C_Handler_t *ptrHandlerI2C){

	/*
	 * 7. Generamos la condicion de stop
	 */
	ptrHandlerI2C->ptrI2Cx->CR1 |= I2C_CR1_STOP;
}

void i2c_start_Transaction (I2C_Handler_t *ptrHandlerI2C){

	//  1. Verificamos que la linea no esta ocupada- bit "busy" en I2C_CR2
	 while(ptrHandlerI2C->ptrI2Cx->SR2 & I2C_SR2_BUSY){
		 __NOP();
	 }
	// 2. Generamos la señal "start"
	 ptrHandlerI2C->ptrI2Cx->CR1 |= I2C_CR1_START;

	/*
	 * 2a. Esperamos a que la bandera del evento "Start" se levante
	 * Mientras esperamos el valor de SB es 0, entonces la negacion
	 * (!) es 1.
	 */
	 while(!(ptrHandlerI2C->ptrI2Cx->SR1 & I2C_SR1_SB)){
		 __NOP();
	 }
}

void i2c_reStartTransaction (I2C_Handler_t *ptrHandlerI2C){
	// 2. Generamos la señal "start"
	ptrHandlerI2C->ptrI2Cx->CR1 |= I2C_CR1_START;

	/*
	 * 2a. Esperamos a que la bandera del evento "Start" se levante
	 * Mientras esperamos el valor de SB es 0, entonces la negacion
	 * (!) es 1.
	 */
	 while(!(ptrHandlerI2C->ptrI2Cx->SR1 & I2C_SR1_SB)){
		 __NOP();
	 }

}

//7a. Activamos la indicacion para no-ACK (Indicacion para el slave de terminar)

void i2c_sendNoAck (I2C_Handler_t *ptrHandlerI2C){
	//(Debemos escribir cero en la posicion ACK del registro de control 1)
	ptrHandlerI2C->ptrI2Cx->CR1 &= ~I2C_CR1_ACK;
}

//7b. Activamos la indicacion para ACK (indicacion para el slave de terminar)

void i2c_sendAck (I2C_Handler_t *ptrHandlerI2C){
	//(Debemos escribir 1 en la posicion ACK del registro de control 1)
	ptrHandlerI2C->ptrI2Cx->CR1 |= I2C_CR1_ACK;
}

void i2c_sendSlaveAddressRW (I2C_Handler_t *ptrHandlerI2C, uint8_t slaveAddress, uint8_t readOrWrite){
	// Definimos una variable auxiliar
	uint8_t auxByte = 0;
	(void) auxByte;
	/*
	 *  Enviamos la direccion del slave y el bit que indica que deseamos
	 *  escribir (0) en el siguiente paso se envia la direccion de memoria que se desea
	 *  escribir.
	 */
	ptrHandlerI2C->ptrI2Cx->DR = (slaveAddress <<1) | readOrWrite;

	/*
	 * Esperamos hasta que la bandera del evento"addr" se levante
	 * esto nos indica que la direccion fue enviada satisfactoriamente
	 */

	while ( !(ptrHandlerI2C->ptrI2Cx->SR1 & I2C_SR1_ADDR)){
		__NOP();
	}

	/*
	 * Debemos limpiar la bandera de la recepcion de ACK de la "addr",
	 * para lo cual debemos leer en secuencia primero el I2C_SR1 y luego I2C_SR2
	 */

	auxByte = ptrHandlerI2C->ptrI2Cx->SR1;
	auxByte = ptrHandlerI2C->ptrI2Cx->SR2;
}

void i2c_sendMemoryAddress (I2C_Handler_t *ptrHandlerI2C, uint8_t memAddr){
	// Enviamos la direccion de memoria que deseamos leer
	ptrHandlerI2C->ptrI2Cx->DR = memAddr;

	//Esperamos hasta que el byte sea transmitido//

	while(!(ptrHandlerI2C->ptrI2Cx->SR1 & I2C_SR1_BTF)){
		__NOP();
	}
}

void i2c_sendDataByte (I2C_Handler_t *ptrHandlerI2C, uint8_t dataToWrite){
	//Cargamos el valor que deseamos escribir
	ptrHandlerI2C->ptrI2Cx->DR = dataToWrite;

	// Esperamos hasta que el byte sea transmitido
	while ( !(ptrHandlerI2C->ptrI2Cx->SR1 & I2C_SR1_BTF)){
		__NOP();
	}
}


