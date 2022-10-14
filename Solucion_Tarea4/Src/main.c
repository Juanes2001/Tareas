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
#include "EXTIDriver.h"
#include <stdint.h>
#include <stdbool.h>

uint8_t clk = 0;
uint8_t data = 0;
char buffer[64];
uint8_t counter = 0;


void inSystem (void);


GPIO_Handler_t handlerBlinkyLed        = {0}; //. PA5
BasicTimer_Handler_t handlerTIM2       = {0}; //.

GPIO_Handler_t handlerExInButtonPin    = {0};
EXTI_Config_t handlerExtiConfig1       = {0}; //.

GPIO_Handler_t handlerExInClock        = {0}; //. PA0
GPIO_Handler_t handlerExInData         = {0}; //. PA1
EXTI_Config_t handlerExtiConfig0       = {0}; //.

GPIO_Handler_t handlerPinTx            = {0}; //. PA2
USART_Handler_t handlerUSART2          = {0}; //.

GPIO_Handler_t handlerPinTran1         = {0}; //.PA6
GPIO_Handler_t handlerPinTran2         = {0}; //.PA7
BasicTimer_Handler_t handlerTIM5       = {0};


GPIO_Handler_t handlerPinSeg1         = {0};
GPIO_Handler_t handlerPinSeg2         = {0};
GPIO_Handler_t handlerPinSeg3         = {0};
GPIO_Handler_t handlerPinSeg4         = {0};
GPIO_Handler_t handlerPinSeg5         = {0};
GPIO_Handler_t handlerPinSeg6         = {0};
GPIO_Handler_t handlerPinSeg7         = {0};

int main (void){

	inSystem();

	while(1){

		if (clk == 1 && data == 0){
			counter++;
			if (counter <= 50 && counter>=0){
				sprintf(buffer, "CW direction = %u \n", counter);
				writeMsg(&handlerUSART2, buffer);
			}else{
				counter = 0;
				sprintf(buffer, "CW direction = %u \n", counter);
				writeMsg(&handlerUSART2, buffer);
			}
			clk = 0;

		}else if (clk == 1 && data == 1){
			counter--;
			if (counter <= 50 && counter>=0){
				sprintf(buffer, "CCW direction = %u \n", counter);
				writeMsg(&handlerUSART2, buffer);
			}else{
				counter = 50;
				sprintf(buffer, "CCW direction = %u \n", counter);
				writeMsg(&handlerUSART2, buffer);
			}
			clk = 0;
			data = 0;
		}else{
			__NOP();
		}





	}
}


void inSystem (void){

	handlerBlinkyLed.pGPIOx = GPIOA;
	handlerBlinkyLed.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerBlinkyLed.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerBlinkyLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerBlinkyLed.GPIO_PinConfig.GPIO_PinPuPdControl =  GPIO_PUPDR_NOTHING;
	handlerBlinkyLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerBlinkyLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerBlinkyLed);

	handlerTIM2.ptrTIMx = TIM2;
	handlerTIM2.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerTIM2.TIMx_Config.TIMx_speed = BTIMER_SPEED_100us;
	handlerTIM2.TIMx_Config.TIMx_period = 2500;
	handlerTIM2.TIMx_Config.TIMx_interruptEnable = 1;
	BasicTimer_Config(&handlerTIM2);

	startTimer (&handlerTIM2);


	handlerExInClock.pGPIOx 							= GPIOA;
	handlerExInClock.GPIO_PinConfig.GPIO_PinNumber      = PIN_0;
	handlerExInClock.GPIO_PinConfig.GPIO_PinAltFunMode  = AF0;
	handlerExInClock.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IN;
	handlerExInClock.GPIO_PinConfig.GPIO_PinPuPdControl =  GPIO_PUPDR_NOTHING;
	handlerExInClock.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerExInClock.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	handlerExtiConfig0.pGPIOHandler 		= &handlerExInClock;
	handlerExtiConfig0.edgeType 			= EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiConfig0);


	handlerExInData.pGPIOx 							= GPIOA;
	handlerExInData.GPIO_PinConfig.GPIO_PinNumber      = PIN_1;
	handlerExInData.GPIO_PinConfig.GPIO_PinAltFunMode  = AF0;
	handlerExInData.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IN;
	handlerExInData.GPIO_PinConfig.GPIO_PinPuPdControl =  GPIO_PUPDR_NOTHING;
	handlerExInData.GPIO_PinConfig.GPIO_PinOPType      = GPIO_OTYPE_PUSHPULL;
	handlerExInData.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerExInData);

	handlerPinTx.pGPIOx 							= GPIOA;
	handlerPinTx.GPIO_PinConfig.GPIO_PinNumber		= PIN_2;
	handlerPinTx.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinTx.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinTx.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEEDR_FAST;
	handlerPinTx.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerPinTx.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinTx);

	handlerUSART2.ptrUSARTx = USART2;
	handlerUSART2.USART_Config.USART_baudrate = USART_BAUDRATE_115200;
	handlerUSART2.USART_Config.USART_datasize = USART_DATASIZE_8BIT;
	handlerUSART2.USART_Config.USART_mode = USART_MODE_TX;
	handlerUSART2.USART_Config.USART_stopbits = USART_STOPBIT_1;
	handlerUSART2.USART_Config.USART_parity = USART_PARITY_NONE;
	handlerUSART2.USART_Config.USART_enableInRx = USART_INTERRUPT_RX_DISABLE;
	USART_Config(&handlerUSART2);


	handlerPinTran1.pGPIOx                              = GPIOA;
	handlerPinTran1.GPIO_PinConfig.GPIO_PinNumber       = PIN_6;
	handlerPinTran1.GPIO_PinConfig.GPIO_PinAltFunMode   = AF0;
	handlerPinTran1.GPIO_PinConfig.GPIO_PinMode         = GPIO_MODE_OUT;
	handlerPinTran1.GPIO_PinConfig.GPIO_PinOPType       =  GPIO_OTYPE_PUSHPULL;
	handlerPinTran1.GPIO_PinConfig.GPIO_PinPuPdControl  = GPIO_PUPDR_NOTHING;
	handlerPinTran1.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinTran1);
	GPIO_WritePin(&handlerPinTran1, RESET);

	handlerPinTran1.pGPIOx                              = GPIOA;
	handlerPinTran1.GPIO_PinConfig.GPIO_PinNumber       = PIN_7;
	handlerPinTran1.GPIO_PinConfig.GPIO_PinAltFunMode   = AF0;
	handlerPinTran1.GPIO_PinConfig.GPIO_PinMode         = GPIO_MODE_OUT;
	handlerPinTran1.GPIO_PinConfig.GPIO_PinOPType       =  GPIO_OTYPE_PUSHPULL;
	handlerPinTran1.GPIO_PinConfig.GPIO_PinPuPdControl  = GPIO_PUPDR_NOTHING;
	handlerPinTran1.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinTran2);
	GPIO_WritePin(&handlerPinTran2, SET);

	handlerTIM5.ptrTIMx = TIM5;
	handlerTIM5.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerTIM5.TIMx_Config.TIMx_speed = BTIMER_SPEED_100us;
	handlerTIM5.TIMx_Config.TIMx_period = 2500;
	handlerTIM5.TIMx_Config.TIMx_interruptEnable = 1;
	BasicTimer_Config(&handlerTIM5);

	startTimer (&handlerTIM5);

	handlerPinSeg1.pGPIOx                              = GPIOC;
	handlerPinSeg1.GPIO_PinConfig.GPIO_PinNumber       = PIN_1;
	handlerPinSeg1.GPIO_PinConfig.GPIO_PinAltFunMode   = AF0;
	handlerPinSeg1.GPIO_PinConfig.GPIO_PinMode         = GPIO_MODE_OUT;
	handlerPinSeg1.GPIO_PinConfig.GPIO_PinOPType       =  GPIO_OTYPE_PUSHPULL;
	handlerPinSeg1.GPIO_PinConfig.GPIO_PinPuPdControl  = GPIO_PUPDR_NOTHING;
	handlerPinSeg1.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinSeg1);
	GPIO_WritePin(&handlerPinSeg1, RESET);

	handlerPinSeg2.pGPIOx                              = GPIOC;
	handlerPinSeg2.GPIO_PinConfig.GPIO_PinNumber       = PIN_2;
	handlerPinSeg2.GPIO_PinConfig.GPIO_PinAltFunMode   = AF0;
	handlerPinSeg2.GPIO_PinConfig.GPIO_PinMode         = GPIO_MODE_OUT;
	handlerPinSeg2.GPIO_PinConfig.GPIO_PinOPType       =  GPIO_OTYPE_PUSHPULL;
	handlerPinSeg2.GPIO_PinConfig.GPIO_PinPuPdControl  = GPIO_PUPDR_NOTHING;
	handlerPinSeg2.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinSeg2);
	GPIO_WritePin(&handlerPinSeg2, RESET);

	handlerPinSeg3.pGPIOx                              = GPIOC;
	handlerPinSeg3.GPIO_PinConfig.GPIO_PinNumber       = PIN_4;
	handlerPinSeg3.GPIO_PinConfig.GPIO_PinAltFunMode   = AF0;
	handlerPinSeg3.GPIO_PinConfig.GPIO_PinMode         = GPIO_MODE_OUT;
	handlerPinSeg3.GPIO_PinConfig.GPIO_PinOPType       =  GPIO_OTYPE_PUSHPULL;
	handlerPinSeg3.GPIO_PinConfig.GPIO_PinPuPdControl  = GPIO_PUPDR_NOTHING;
	handlerPinSeg3.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinSeg3);
	GPIO_WritePin(&handlerPinSeg3, RESET);

	handlerPinSeg4.pGPIOx                              = GPIOC;
	handlerPinSeg4.GPIO_PinConfig.GPIO_PinNumber       = PIN_4;
	handlerPinSeg4.GPIO_PinConfig.GPIO_PinAltFunMode   = AF0;
	handlerPinSeg4.GPIO_PinConfig.GPIO_PinMode         = GPIO_MODE_OUT;
	handlerPinSeg4.GPIO_PinConfig.GPIO_PinOPType       =  GPIO_OTYPE_PUSHPULL;
	handlerPinSeg4.GPIO_PinConfig.GPIO_PinPuPdControl  = GPIO_PUPDR_NOTHING;
	handlerPinSeg4.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinSeg4);
	GPIO_WritePin(&handlerPinSeg4, RESET);

	handlerPinSeg5.pGPIOx                              = GPIOC;
	handlerPinSeg5.GPIO_PinConfig.GPIO_PinNumber       = PIN_5;
	handlerPinSeg5.GPIO_PinConfig.GPIO_PinAltFunMode   = AF0;
	handlerPinSeg5.GPIO_PinConfig.GPIO_PinMode         = GPIO_MODE_OUT;
	handlerPinSeg5.GPIO_PinConfig.GPIO_PinOPType       =  GPIO_OTYPE_PUSHPULL;
	handlerPinSeg5.GPIO_PinConfig.GPIO_PinPuPdControl  = GPIO_PUPDR_NOTHING;
	handlerPinSeg5.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinSeg5);
	GPIO_WritePin(&handlerPinSeg5, RESET);

	handlerPinSeg6.pGPIOx                              = GPIOC;
	handlerPinSeg6.GPIO_PinConfig.GPIO_PinNumber       = PIN_6;
	handlerPinSeg6.GPIO_PinConfig.GPIO_PinAltFunMode   = AF0;
	handlerPinSeg6.GPIO_PinConfig.GPIO_PinMode         = GPIO_MODE_OUT;
	handlerPinSeg6.GPIO_PinConfig.GPIO_PinOPType       =  GPIO_OTYPE_PUSHPULL;
	handlerPinSeg6.GPIO_PinConfig.GPIO_PinPuPdControl  = GPIO_PUPDR_NOTHING;
	handlerPinSeg6.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinSeg6);
	GPIO_WritePin(&handlerPinSeg6, RESET);

	handlerPinSeg7.pGPIOx                              = GPIOC;
	handlerPinSeg7.GPIO_PinConfig.GPIO_PinNumber       = PIN_7;
	handlerPinSeg7.GPIO_PinConfig.GPIO_PinAltFunMode   = AF0;
	handlerPinSeg7.GPIO_PinConfig.GPIO_PinMode         = GPIO_MODE_OUT;
	handlerPinSeg7.GPIO_PinConfig.GPIO_PinOPType       =  GPIO_OTYPE_PUSHPULL;
	handlerPinSeg7.GPIO_PinConfig.GPIO_PinPuPdControl  = GPIO_PUPDR_NOTHING;
	handlerPinSeg7.GPIO_PinConfig.GPIO_PinSpeed 		= GPIO_OSPEEDR_FAST;
	GPIO_Config(&handlerPinSeg7);
	GPIO_WritePin(&handlerPinSeg7, RESET);
}

void callback_extInt0(void){
		clk = 1;
		data = GPIO_ReadPin(&handlerExInData);

}


void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerBlinkyLed);
}

void BasicTimer5_Callback(void){
	GPIOxTooglePin(&handlerPinTran1);
	GPIOxTooglePin(&handlerPinTran2);
}
