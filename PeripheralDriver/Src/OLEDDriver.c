

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
#include "SysTickDriver.h"

char letterArray[5] = {0};
//OLED Driver

void sendDataBytes(I2C_Handler_t *ptrHandlerI2C, char *dataBytes, uint8_t sizeArray){

	i2c_startTransaction(ptrHandlerI2C);

	i2c_sendSlaveAddressRW(ptrHandlerI2C, ptrHandlerI2C->slaveAddress, I2C_WRITE_DATA);

	i2c_sendDataByte (ptrHandlerI2C, OLED_CONTROLBYTE_DISPLAY);


	if (sizeArray < 128){
		for (uint8_t k=0; k < sizeArray; k++){
				i2c_sendDataByte (ptrHandlerI2C, *(dataBytes+k));
		}
	}else{

		for (uint8_t j=0; j < 128; j++){
			i2c_sendDataByte (ptrHandlerI2C, *(dataBytes+j));
		}
	}

	i2c_stopTransaction(ptrHandlerI2C);

}

void sendCommandByte ( I2C_Handler_t *ptrHandlerI2C,  char command){

	i2c_startTransaction(ptrHandlerI2C);

	i2c_sendSlaveAddressRW(ptrHandlerI2C, ptrHandlerI2C->slaveAddress, I2C_WRITE_DATA);

	i2c_sendDataByte (ptrHandlerI2C, OLED_CONTROLBYTE_CONFIG);

	i2c_sendDataByte (ptrHandlerI2C, command);

	i2c_stopTransaction(ptrHandlerI2C);
}




//Funciones especiales

void startOLED (I2C_Handler_t *ptrHandlerI2Ctr){
	//Apagamos la pantalla OLED
	sendCommandByte(ptrHandlerI2Ctr, 0b10101110);
	// Seteamos la divicion de frecuencia del oscilador interno y la frecuencia propia de oscilacion
	sendCommandByte(ptrHandlerI2Ctr, 0b01010000);
	// Seteamos el Multiplex Ratio
	sendCommandByte(ptrHandlerI2Ctr, 0b10101000);
	sendCommandByte(ptrHandlerI2Ctr, 0b00111111);
	//Seteamos que el Dysplay Offset comienze en 00H
	sendCommandByte(ptrHandlerI2Ctr, 0b11010011);
	sendCommandByte(ptrHandlerI2Ctr, 0b00000000);
	// Comenzamos a imprimir valores desde la fila 0
	sendCommandByte(ptrHandlerI2Ctr, 0b01000000);
	//Seteamos la fuente de poder
	sendCommandByte(ptrHandlerI2Ctr, 0b10101101);
	sendCommandByte(ptrHandlerI2Ctr, 0b10001011);
	//Seteamos el segment Re-map
	sendCommandByte(ptrHandlerI2Ctr, 0b10100001);
	//Seteamos la direccion de escaneo de las filas
	sendCommandByte(ptrHandlerI2Ctr, 0b11001000);
	//Configuramos los pines del hardware
	sendCommandByte(ptrHandlerI2Ctr, 0b11011010);
	sendCommandByte(ptrHandlerI2Ctr, 0b00010010);
	//Configuracion del contraste
	sendCommandByte(ptrHandlerI2Ctr, 0b10000001);
	sendCommandByte(ptrHandlerI2Ctr, 0xFF);
	//Seteo de el periodo de pre-carga
	sendCommandByte(ptrHandlerI2Ctr, 0b11011001);
	sendCommandByte(ptrHandlerI2Ctr, 0x1F);
	//VCOMH Deselect Level
	sendCommandByte(ptrHandlerI2Ctr, 0xDB);
	sendCommandByte(ptrHandlerI2Ctr, 0x40);
	//SET Vpp
	sendCommandByte(ptrHandlerI2Ctr, 0x33);
	//Set normal/inverse Display
	sendCommandByte(ptrHandlerI2Ctr, 0xA6);
	//Configuracion para limpiar pantalla
	clearDisplay(ptrHandlerI2Ctr);
	sendCommandByte(ptrHandlerI2Ctr, 0b10101111);
	delay_Ms(200);


}

void stopOLED (I2C_Handler_t *ptrHandlerI2Ctr){
	sendCommandByte(ptrHandlerI2Ctr, 0b10101110);
	//Set Charge Pump
	sendCommandByte(ptrHandlerI2Ctr, 0x8D);
	sendCommandByte(ptrHandlerI2Ctr, 0x10);
	delay_Ms(200);
}

void clearDisplay (I2C_Handler_t *ptrHandlerI2Ctr){

	char clearedBytes[128] = {0};


	for (uint8_t j = 0; j < 8 ; j++){
		setPage(ptrHandlerI2Ctr, j);
		setColumnAddress(ptrHandlerI2Ctr, j);
		sendDataBytes(ptrHandlerI2Ctr, clearedBytes, sizeof(clearedBytes));
	}

}

void setPage (I2C_Handler_t *ptrHandlerI2Ctr, uint8_t page){

	sendCommandByte(ptrHandlerI2Ctr, (0b10110000 | page) );
}

void setColumnAddress (I2C_Handler_t *ptrHandlerI2Ctr, uint8_t page){

	setPage(ptrHandlerI2Ctr, page);
	sendCommandByte(ptrHandlerI2Ctr, 0b00000010);
	sendCommandByte(ptrHandlerI2Ctr, 0b00010000);

}

void setLineAddress (I2C_Handler_t *ptrHandlerI2Ctr, uint8_t moveNum){

	sendCommandByte(ptrHandlerI2Ctr, (0b01000000 | (moveNum-1)));

}

char *letterTochar (uint8_t character){
	switch(character){
		case 'A':{

			letterArray[0] = 0b01111000;
			letterArray[1] = 0b00010100;
			letterArray[2] = 0b00010010;
			letterArray[3] = 0b00010100;
			letterArray[4] = 0b01111000;

			break;

		}case 'B':{
			letterArray[0] = 0b01111110;
			letterArray[1] = 0b01001010;
			letterArray[2] = 0b01001010;
			letterArray[3] = 0b01001010;
			letterArray[4] = 0b00110100;

			break;
		}case 'C':{
			letterArray[0] = 0b00111100;
			letterArray[1] = 0b01000010;
			letterArray[2] = 0b01000010;
			letterArray[3] = 0b01000010;
			letterArray[4] = 0b00100100;

			break;
		}case 'D':{
			letterArray[0] = 0b01111110;
			letterArray[1] = 0b01000010;
			letterArray[2] = 0b01000010;
			letterArray[3] = 0b01000010;
			letterArray[4] = 0b00111100;

			break;
		}case 'E':{
			letterArray[0] = 0b01111110;
			letterArray[1] = 0b01001010;
			letterArray[2] = 0b01001010;
			letterArray[3] = 0b01001010;
			letterArray[4] = 0b01000010;

			break;
		}case 'F':{
			letterArray[0] = 0b01111110;
			letterArray[1] = 0b00001010;
			letterArray[2] = 0b00001010;
			letterArray[3] = 0b00001010;
			letterArray[4] = 0b00000010;

			break;
		}case 'G':{
			letterArray[0] = 0b00111100;
			letterArray[1] = 0b01000010;
			letterArray[2] = 0b01010010;
			letterArray[3] = 0b01010010;
			letterArray[4] = 0b00110100;

			break;
		}case 'H':{
			letterArray[0] = 0b01111110;
			letterArray[1] = 0b00001000;
			letterArray[2] = 0b00001000;
			letterArray[3] = 0b00001000;
			letterArray[4] = 0b01111110;

			break;
		}case 'I':{
			letterArray[0] = 0b01000010;
			letterArray[1] = 0b01000010;
			letterArray[2] = 0b01111110;
			letterArray[3] = 0b01000010;
			letterArray[4] = 0b01000010;

			break;
		}case 'J':{
			letterArray[0] = 0b00000010;
			letterArray[1] = 0b00000010;
			letterArray[2] = 0b01111110;
			letterArray[3] = 0b01000010;
			letterArray[4] = 0b00110010;

			break;
		}case 'K':{
			letterArray[0] = 0b01111110;
			letterArray[1] = 0b00010000;
			letterArray[2] = 0b00011000;
			letterArray[3] = 0b00100100;
			letterArray[4] = 0b01000010;

			break;
		}case 'L':{
			letterArray[0] = 0b01111110;
			letterArray[1] = 0b01000000;
			letterArray[2] = 0b01000000;
			letterArray[3] = 0b01000000;
			letterArray[4] = 0b01000000;

			break;
		}case 'M':{
			letterArray[0] = 0b01111110;
			letterArray[1] = 0b00000100;
			letterArray[2] = 0b00001000;
			letterArray[3] = 0b00000100;
			letterArray[4] = 0b01111110;

			break;
		}case 'N':{
			letterArray[0] = 0b01111110;
			letterArray[1] = 0b00000100;
			letterArray[2] = 0b00011000;
			letterArray[3] = 0b00100000;
			letterArray[4] = 0b01111110;

			break;
		}case 'O':{
			letterArray[0] = 0b00111100;
			letterArray[1] = 0b01000010;
			letterArray[2] = 0b01000010;
			letterArray[3] = 0b01000010;
			letterArray[4] = 0b00111100;

			break;
		}case 'P':{
			letterArray[0] = 0b01111110;
			letterArray[1] = 0b00010010;
			letterArray[2] = 0b00010010;
			letterArray[3] = 0b00010010;
			letterArray[4] = 0b00001100;

			break;
		}case 'Q':{
			letterArray[0] = 0b00011100;
			letterArray[1] = 0b00100010;
			letterArray[2] = 0b00110010;
			letterArray[3] = 0b01110010;
			letterArray[4] = 0b01011100;

			break;
		}case 'R':{
			letterArray[0] = 0b01111110;
			letterArray[1] = 0b00010010;
			letterArray[2] = 0b00010010;
			letterArray[3] = 0b00110010;
			letterArray[4] = 0b01001100;

			break;
		}case 'S':{
			letterArray[0] = 0b00100100;
			letterArray[1] = 0b01001010;
			letterArray[2] = 0b01001010;
			letterArray[3] = 0b01001010;
			letterArray[4] = 0b00110100;

			break;
		}case 'T':{
			letterArray[0] = 0b00000010;
			letterArray[1] = 0b00000010;
			letterArray[2] = 0b01111110;
			letterArray[3] = 0b00000010;
			letterArray[4] = 0b00000010;

			break;
		}case 'U':{
			letterArray[0] = 0b00111110;
			letterArray[1] = 0b01000000;
			letterArray[2] = 0b01000000;
			letterArray[3] = 0b01000000;
			letterArray[4] = 0b00111110;

			break;
		}case 'V':{
			letterArray[0] = 0b00011110;
			letterArray[1] = 0b00100000;
			letterArray[2] = 0b01000000;
			letterArray[3] = 0b00100000;
			letterArray[4] = 0b00011110;

			break;
		}case 'W':{
			letterArray[0] = 0b00111110;
			letterArray[1] = 0b01000000;
			letterArray[2] = 0b00111000;
			letterArray[3] = 0b01000000;
			letterArray[4] = 0b00111111;

			break;
		}case 'X':{
			letterArray[0] = 0b01100010;
			letterArray[1] = 0b00010110;
			letterArray[2] = 0b00001000;
			letterArray[3] = 0b00010110;
			letterArray[4] = 0b01100010;

			break;
		}case 'Y':{
			letterArray[0] = 0b00000110;
			letterArray[1] = 0b00001000;
			letterArray[2] = 0b01110000;
			letterArray[3] = 0b00001000;
			letterArray[4] = 0b00000110;

			break;
		}case 'Z':{
			letterArray[0] = 0b01100010;
			letterArray[1] = 0b01010010;
			letterArray[2] = 0b01001010;
			letterArray[3] = 0b01000110;
			letterArray[4] = 0b01000010;

			break;
		}
		case '!':{
			letterArray[0] = 0b01011110;
			letterArray[1] = 0b00000000;
			letterArray[2] = 0b00000000;
			letterArray[3] = 0b00000000;
			letterArray[4] = 0b00000000;

			break;
		}
		case '0':{
			letterArray[0] = 0b00111100;
			letterArray[1] = 0b01000110;
			letterArray[2] = 0b01011010;
			letterArray[3] = 0b01100010;
			letterArray[4] = 0b00111100;

			break;
		}
		case '1':{
			letterArray[0] = 0b00001000;
			letterArray[1] = 0b00000100;
			letterArray[2] = 0b01111110;
			letterArray[3] = 0b00000000;
			letterArray[4] = 0b00000000;

			break;
		}
		case '2':{
			letterArray[0] = 0b01001100;
			letterArray[1] = 0b01100010;
			letterArray[2] = 0b01010010;
			letterArray[3] = 0b01001010;
			letterArray[4] = 0b01000100;

			break;
		}
		case '3':{
			letterArray[0] = 0b00100100;
			letterArray[1] = 0b01000010;
			letterArray[2] = 0b01001010;
			letterArray[3] = 0b01001010;
			letterArray[4] = 0b00110100;

			break;
		}
		case '4':{
			letterArray[0] = 0b00010000;
			letterArray[1] = 0b00011000;
			letterArray[2] = 0b00010100;
			letterArray[3] = 0b01111110;
			letterArray[4] = 0b00010000;

			break;
		}
		case '5':{
			letterArray[0] = 0b01001110;
			letterArray[1] = 0b01001010;
			letterArray[2] = 0b01001010;
			letterArray[3] = 0b01001010;
			letterArray[4] = 0b00110010;

			break;
		}
		case '6':{
			letterArray[0] = 0b00111100;
			letterArray[1] = 0b01010010;
			letterArray[2] = 0b01010010;
			letterArray[3] = 0b01010010;
			letterArray[4] = 0b00100100;

			break;
		}
		case '7':{
			letterArray[0] = 0b01000010;
			letterArray[1] = 0b00100010;
			letterArray[2] = 0b00010010;
			letterArray[3] = 0b00001010;
			letterArray[4] = 0b00000110;

			break;
		}
		case '8':{
			letterArray[0] = 0b00110100;
			letterArray[1] = 0b01001010;
			letterArray[2] = 0b01001010;
			letterArray[3] = 0b01001010;
			letterArray[4] = 0b00110100;

			break;
		}
		case '9':{
			letterArray[0] = 0b00100100;
			letterArray[1] = 0b01001010;
			letterArray[2] = 0b01001010;
			letterArray[3] = 0b01001010;
			letterArray[4] = 0b00111100;

			break;
		}
		case '/':{
			letterArray[0] = 0b01000000;
			letterArray[1] = 0b00100000;
			letterArray[2] = 0b00011000;
			letterArray[3] = 0b00000100;
			letterArray[4] = 0b00000010;

			break;
		}
		case ':':{
			letterArray[0] = 0b00000000;
			letterArray[1] = 0b00000000;
			letterArray[2] = 0b00101000;
			letterArray[3] = 0b00000000;
			letterArray[4] = 0b00000000;

			break;
		}
		case '_':{
			letterArray[0] = 0b01000000;
			letterArray[1] = 0b01000000;
			letterArray[2] = 0b01000000;
			letterArray[3] = 0b01000000;
			letterArray[4] = 0b01000000;

			break;
		}

		default:{
			__NOP();

			break;
		}


	}

	return letterArray;
}




void drawMSG (I2C_Handler_t *ptrHandlerI2Ctr, char *msg , uint8_t sizeMsg){

	clearDisplay(ptrHandlerI2Ctr);
	uint8_t i = 0;
//	uint8_t renglones = 0;
	char mensaje[sizeMsg][8];


	if (((sizeMsg-sizeMsg%16)/16) <= 1 ){
		setColumnAddress(ptrHandlerI2Ctr, 0);
		while (*(msg+i)!='\0'){
			for (uint8_t j = 0 ; j<8 ; j++){
				if (*(msg+i) == ' '){
					for (uint8_t k = 0; k<8 ; k++){
						mensaje[i][k]= 0;
					}
					i++;
					j--;

				}else if (j<=4){
					mensaje[i][j]= *(letterTochar (*(msg+i))+j);
				}else{
					mensaje[i][j]= 0;
				}
			}
			i++;
		}

		for (uint8_t s = 0; s < sizeMsg ; s++){
			for (uint8_t l = 0; l < 8 ; l++){
				sendDataOneByte(ptrHandlerI2Ctr, mensaje[s][l]);
			}
		}

	}
//		else {
//		renglones = (sizeMsg-sizeMsg%16)/16;
//
//		for (uint8_t t=0; t<renglones ; t++){
//			setColumnAddress(ptrHandlerI2Ctr, t);
//			while (*(msg+i)!='\0'){
//				for (uint8_t j = 0 ; j<8 ; j++){
//					if (*(msg+i) == ' '){
//						for (uint8_t k = 0; k<8 ; k++){
//							mensaje[i][k]= 0;
//						}
//
//					}else if (j<=4){
//						mensaje[i][j]= *(letterTochar (*(msg+i))+j);
//					}else{
//						mensaje[i][j]= 0;
//					}
//				}
//				i++;
//			}
//
//			for (uint8_t s = 0; s < sizeMsg ; s++){
//				for (uint8_t l = 0; l < 8 ; l++){
//					sendDataOneByte(ptrHandlerI2Ctr, mensaje[s][l]);
//				}
//			}
//
//		}
//	}



}


void sendDataOneByte (I2C_Handler_t *ptrHandlerI2C, char oneByte){

		i2c_startTransaction(ptrHandlerI2C);

		i2c_sendSlaveAddressRW(ptrHandlerI2C, ptrHandlerI2C->slaveAddress, I2C_WRITE_DATA);

		i2c_sendDataByte (ptrHandlerI2C, OLED_CONTROLBYTE_DISPLAY);

		i2c_sendDataByte(ptrHandlerI2C, oneByte);

		i2c_stopTransaction(ptrHandlerI2C);
}

