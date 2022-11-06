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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// configuracion USART que se usara = Serial_1, 28800, EVEN

//handler para ADC
ADC_Config_t handlerADCJoy = {0};

//Pines handler para comunicación Serial
GPIO_Handler_t handlerPinTx = {0};
GPIO_Handler_t handlerPinRx = {0};

GPIO_Handler_t handlerPinR = {0};
GPIO_Handler_t handlerPinG = {0};
GPIO_Handler_t handlerPinB = {0};

GPIO_Handler_t handlerI2cSCL = {0};
GPIO_Handler_t handlerI2cSDA = {0};

//Pin del blinky
GPIO_Handler_t handlerLEDPin = {0};

//handler para USART1   //RECORDAR CAMBIAR EL CODIGO POR USART1, PINES A9 EN TX Y A10 EN RX, Y CAMBIAR EL CALLBACK POR CALBACK DE USART 1
USART_Handler_t handlerUSART1 = {0};

//handler para PWM

PWM_Handler_t handlerPwmR   = {0};
PWM_Handler_t handlerPwmG   = {0};
PWM_Handler_t handlerPwmB   = {0};

//Handlers de Timers
BasicTimer_Handler_t handlerLEDTim = {0};
BasicTimer_Handler_t handlerADCTim = {0};

//handlers I2C
I2C_Handler_t handlerI2C1 = {0};





void inSystem (void);
void parseCommands(char *stringVector);




//Variables necesarias para el programa
uint32_t adcData[2];
uint8_t adcFlag = RESET;
uint8_t rxData = '\0';
uint8_t counterR = 0;
uint8_t counterG = 0;
uint8_t counterB = 0;
char cmd[32];
char userMsg[64];

unsigned int firstParameter;
unsigned int secondParameter;
unsigned int thirdParameter;

char bufferReception[64];
char bufferData1[64];
uint8_t counterReception = 0;
uint8_t doneTransaction = RESET;
uint8_t counterADC = 0;

uint8_t duttyUpR = 0;
uint8_t duttyUpG = 0;
uint8_t duttyUpB1 = 0;
uint8_t duttyUpB2 = 0;
uint8_t duttyDownR = 0;
uint8_t duttyDownG1 = 0;
uint8_t duttyDownG2 = 0;
uint8_t duttyDownB = 0;
uint16_t vectorArcUp = 0;
uint16_t vectorArcDown = 0;

int16_t x = 0;
int16_t y = 0;

uint8_t rmax = 125;

uint8_t com1 = 0;
uint8_t com2 = 0;
uint8_t com3 = 0;
uint8_t com4 = 0;
uint8_t com5 = 0;
uint8_t com6 = 0;
uint8_t com7 = 0;
uint8_t com8 = 0;
uint8_t com9 = 0;
uint8_t com10 = 0;


//Macros OLED

#define ADDRESS 0b0111100









int main(void){

	inSystem();



	while(1){


		x = adcData[0]-2000;
		y = adcData[1]-2000;
		vectorArcUp = (180*atan2(y,x))/M_PI;
		vectorArcDown =180+(180*atan2(-y,-x))/M_PI;


		duttyUpR = (100*(vectorArcDown + 120))/120;
		duttyUpG =  (100*vectorArcUp)/120;
		duttyUpB1 = (100*(vectorArcUp-120))/120;
		duttyUpB2 = (100*(vectorArcDown-120))/120;
		duttyDownR = 100-(100*vectorArcUp)/120;
		duttyDownG1 = 100-(100*(vectorArcUp-120))/120;
		duttyDownG2 = 100-(100*(vectorArcDown-120))/120;
		duttyDownB = 100-(100*(vectorArcDown+120))/120;





		if (((x>-50) & (x<50)) & ((y>-50) & (y<50)) ){
			updateDuttyCycle(&handlerPwmR, 100);
			updateDuttyCycle(&handlerPwmG, 100);
			updateDuttyCycle(&handlerPwmB, 100);


		}else{
			if ((vectorArcUp > 0) & (vectorArcUp < 120) ){
				updateDuttyCycle(&handlerPwmR, duttyDownR);
				updateDuttyCycle(&handlerPwmG, duttyUpG);
				updateDuttyCycle(&handlerPwmB, 0);
			}else if ((vectorArcUp > 120) & (vectorArcUp < 180) ){
				updateDuttyCycle(&handlerPwmR, 0);
				updateDuttyCycle(&handlerPwmG, duttyDownG1);
				updateDuttyCycle(&handlerPwmB, duttyUpB1);
			}else if ((vectorArcDown > 180) & (vectorArcDown < 240) ){
				updateDuttyCycle(&handlerPwmR, 0);
				updateDuttyCycle(&handlerPwmG, duttyDownG2);
				updateDuttyCycle(&handlerPwmB, duttyUpB2);
			}else if ((vectorArcDown > 240) & (vectorArcDown < 380) ){
				updateDuttyCycle(&handlerPwmR, duttyUpR);
				updateDuttyCycle(&handlerPwmG, 0);
				updateDuttyCycle(&handlerPwmB, duttyDownB);
			}
		}

//		if (adcFlag){
//			sprintf(bufferData1, "vx = %u   vy = %u \n \r", adcData[0],adcData[1]);
//			writeMsg(&handlerUSART1, bufferData1);
//			adcFlag = RESET;
//		}



		//COMANDOS


		if (rxData != '\0'){
			bufferReception[counterReception] = rxData;
			counterReception++;

			if (rxData == '@'){
				doneTransaction = SET;

				//bufferReception[counterReception] = '\0';
				writeMsg(&handlerUSART1, bufferReception);

				counterReception = 0;

			}

			rxData = '\0';

		}

		if (doneTransaction){
			parseCommands(bufferReception);
			doneTransaction = RESET;
		}

		/*
		 * OLED Code
		 * Para Escribir sobre los registros de la OLED
		 * primero debemos enviar un Byte de control para que los bytes siguientes sean bytes dirigidos hacia la memoria del
		 * display (memoria RAM) o como bytes de comandos
		 * Esto solo se hace para escribir datos sobre los registros
		 *
		 *  CASOS:
		 *  	c0 = 0 el ultimo byte enviado es el ultimo byte de control, los demas seran
		 *  	bytes de datos.
		 *
		 *  	c0 = 1 Los dos bytes siguientes seran uno de dato y otro byte de control
		 *  			 *
		 *  	D/C = 0 indica que los bytes de datos serán enviados como comandos para la pantalla
		 *
		 *  	D/C = 1 indica que los bytes de datos serán enviados a la memoria RAM
		 */






	}
}

void inSystem (void){

	handlerLEDTim.ptrTIMx = TIM2;
	handlerLEDTim.TIMx_Config.TIMx_interruptEnable = 1;
	handlerLEDTim.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerLEDTim.TIMx_Config.TIMx_period = 2500;
	handlerLEDTim.TIMx_Config.TIMx_speed = BTIMER_SPEED_100us;
	BasicTimer_Config(&handlerLEDTim);

	startTimer(&handlerLEDTim);

	handlerLEDPin.pGPIOx = GPIOA;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerLEDPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerLEDPin);

	//Conversion del JOYSTICK
	handlerADCTim.ptrTIMx = TIM4;
	handlerADCTim.TIMx_Config.TIMx_interruptEnable = 1;
	handlerADCTim.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerADCTim.TIMx_Config.TIMx_period = 100;
	handlerADCTim.TIMx_Config.TIMx_speed = BTIMER_SPEED_100us;
	BasicTimer_Config(&handlerADCTim);


	handlerADCTim.ptrTIMx = TIM4;
	handlerADCTim.TIMx_Config.TIMx_interruptEnable = 1;
	handlerADCTim.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerADCTim.TIMx_Config.TIMx_period = 100;
	handlerADCTim.TIMx_Config.TIMx_speed = BTIMER_SPEED_100us;



    handlerADCJoy.channelVector[0] = 0;
    handlerADCJoy.channelVector[1] = 1;
    handlerADCJoy.dataAlignment = ADC_ALIGNMENT_RIGHT;
    handlerADCJoy.resolution = ADC_RESOLUTION_12_BIT;
    handlerADCJoy.samplingPeriod = ADC_SAMPLING_PERIOD_28_CYCLES;
    ADC_ConfigMultichannel(&handlerADCJoy, 2);

    //Comunicacion Serial

    handlerPinTx.pGPIOx                             = GPIOA;
    handlerPinTx.GPIO_PinConfig.GPIO_PinAltFunMode  = AF7;
    handlerPinTx.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
    handlerPinTx.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
    handlerPinTx.GPIO_PinConfig.GPIO_PinNumber      = PIN_2;
    handlerPinTx.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
    handlerPinTx.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinTx);


	handlerPinRx.pGPIOx                             = GPIOA;
	handlerPinRx.GPIO_PinConfig.GPIO_PinAltFunMode  = AF7;
	handlerPinRx.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerPinRx.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPinRx.GPIO_PinConfig.GPIO_PinNumber      = PIN_3;
	handlerPinRx.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPinRx.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinRx);

	handlerUSART1.ptrUSARTx                     = USART2;
	handlerUSART1.USART_Config.USART_baudrate   = USART_BAUDRATE_28800;//37,7222 de Mantiza
	handlerUSART1.USART_Config.USART_enableInRx = USART_INTERRUPT_RX_ENABLE;
	handlerUSART1.USART_Config.USART_enableInTx = USART_INTERRUPT_TX_DISABLE;
	handlerUSART1.USART_Config.USART_mode       = USART_MODE_RXTX;
	handlerUSART1.USART_Config.USART_parity     = USART_PARITY_ODD;
	handlerUSART1.USART_Config.USART_stopbits   = USART_STOPBIT_1;
	handlerUSART1.USART_Config.USART_datasize   = USART_DATASIZE_9BIT;
	USART_Config(&handlerUSART1);


	handlerPinR.pGPIOx                             = GPIOC;
	handlerPinR.GPIO_PinConfig.GPIO_PinAltFunMode  = AF2;
	handlerPinR.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerPinR.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPinR.GPIO_PinConfig.GPIO_PinNumber      = PIN_7;
	handlerPinR.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPinR.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinR);


	handlerPinG.pGPIOx                             = GPIOC;
	handlerPinG.GPIO_PinConfig.GPIO_PinAltFunMode  = AF2;
	handlerPinG.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerPinG.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPinG.GPIO_PinConfig.GPIO_PinNumber      = PIN_8;
	handlerPinG.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPinG.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinG);


	handlerPinB.pGPIOx                             = GPIOC;
	handlerPinB.GPIO_PinConfig.GPIO_PinAltFunMode  = AF2;
	handlerPinB.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerPinB.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerPinB.GPIO_PinConfig.GPIO_PinNumber      = PIN_9;
	handlerPinB.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPinB.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinB);



	handlerPwmR.ptrTIMx           = TIM3;
	handlerPwmR.config.channel    = PWM_CHANNEL_2;
	handlerPwmR.config.duttyCicle = 100;
	handlerPwmR.config.periodo    = 100;
	handlerPwmR.config.prescaler  = BTIMER_SPEED_100us;
	pwm_Config(&handlerPwmR);


	handlerPwmG.ptrTIMx           = TIM3;
	handlerPwmG.config.channel    = PWM_CHANNEL_3;
	handlerPwmG.config.duttyCicle = 100;
	handlerPwmG.config.periodo    = 100;
	handlerPwmG.config.prescaler  = BTIMER_SPEED_100us;
	pwm_Config(&handlerPwmG);

	handlerPwmB.ptrTIMx           = TIM3;
	handlerPwmB.config.channel    = PWM_CHANNEL_4;
	handlerPwmB.config.duttyCicle = 100;
	handlerPwmB.config.periodo    = 100;
	handlerPwmB.config.prescaler  = BTIMER_SPEED_100us;
	pwm_Config(&handlerPwmB);




	//OLED display

	handlerI2cSCL.pGPIOx                             = GPIOB;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinAltFunMode  = AF4;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinNumber      = PIN_6;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_OPENDRAIN;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLUP;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerI2cSCL);

	handlerI2cSDA.pGPIOx                             = GPIOB;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinAltFunMode  = AF4;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_ALTFN;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinNumber      = PIN_7;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_OPENDRAIN;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLUP;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerI2cSDA);

	handlerI2C1.ptrI2Cx = I2C1;
	handlerI2C1.modeI2C = I2C_MODE_FM;
	handlerI2C1.slaveAddress = ADDRESS;
	i2c_config(&handlerI2C1);


}



void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerLEDPin);
}

void adcComplete_Callback(void){
	counterADC++;
	if ((counterADC % 2) != 0){
		adcData[0] = getADC();
	}else if ((counterADC % 2) == 0){
		adcData[1] = getADC();
	}

	if ((counterADC % 2) == 0){
		adcFlag = SET;
	}


}

void usart2Rx_Callback(void){
	rxData = getRxData();
}

void BasicTimer4_Callback(void){
	startSingleADC();
}

void parseCommands(char *stringVector){

	sscanf(stringVector, "%s %u %u %u %s", cmd ,&firstParameter, &secondParameter, &thirdParameter, userMsg);
	com1 = strcmp(cmd, "startled") ;
	com2 = strcmp(cmd, "p");
	com3 = strcmp(cmd, "setled_period");
	com4 = strcmp(cmd, "startdisplay");
	com5 = strcmp(cmd, "stopdisplay");
	com6 = strcmp(cmd, "print_msg");
	com7 = strcmp(cmd, "printdate");
	com8 = strcmp(cmd, "set_cronometer");
	com9 = strcmp(cmd, "game");
	com10 = strcmp(cmd, "movie");



	if (strcmp(cmd, "help") == 0){
		writeMsg(&handlerUSART1, "HELP MENU CMD : \n");                                        //0
		writeMsg(&handlerUSART1, "1)  startled \n");                                           //1
		writeMsg(&handlerUSART1, "2)  p \n");                                            //2
		writeMsg(&handlerUSART1, "3)  setled_period #RnewPeriod #GnewPeriod #BnewPeriod \n");  //3
		writeMsg(&handlerUSART1, "4)  startdisplay \n");                                       //4
		writeMsg(&handlerUSART1, "5)  stopdisplay \n");                                        //5
		writeMsg(&handlerUSART1, "6)  print_msg \n");                                          //6
		writeMsg(&handlerUSART1, "7)  printdate \n");                                          //7
		writeMsg(&handlerUSART1, "8)  set_cronometer #hours #minutes #seconds \n");            //8
		writeMsg(&handlerUSART1, "9)  game \n");                                               //9
		writeMsg(&handlerUSART1, "10) movie \n");                                              //10

	}else if (com1 == 0){
		startTimer(&handlerADCTim);
		startPwmSignal(&handlerPwmR);
		startPwmSignal(&handlerPwmG);
		startPwmSignal(&handlerPwmB);


	}else if (com2  == 0){
		stopTimer(&handlerADCTim);
		stopPwmSignal(&handlerPwmR);
		stopPwmSignal(&handlerPwmG);
		stopPwmSignal(&handlerPwmB);
	}
	else if (com3 == 0){
		updateFrequency(&handlerPwmR, firstParameter);
		updateFrequency(&handlerPwmG, secondParameter);
		updateFrequency(&handlerPwmB, thirdParameter);
	}

	else{
		writeMsg(&handlerUSART1, "WRONG CMD, WRITE IT AGAING \n");
	}

}

