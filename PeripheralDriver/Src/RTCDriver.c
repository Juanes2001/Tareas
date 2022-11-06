/*
 * RTCDriver.c
 *
 *  Created on: 5/11/2022
 *      Author: ALEJANDRA MARIA
 */


#include "RTCDriver.h"

void Rtc_Congif (RTC_Handler_t *ptrHandlerRtc){
	//Activamos el PWR parapoder desactivar su sistema de bloqueo en el RTC
	RCC->APB1ENR = RCC_APB1ENR_PWREN;
	//Desactivamos la proteccion de PWR para modificar registros
	PWR->CR |= PWR_CR_DBP;

	/* 1. Debemos activar el LSE (Cristal de 32kHz de la board)*/
	RCC->BDCR |= RCC_BDCR_LSEON;				// Encendemos el LSE clock
	while(!(RCC->BDCR & RCC_BDCR_LSERDY)){
		__NOP();
	}
	RCC->BDCR |= (0b01 << RCC_BDCR_RTCSEL_Pos);	// Seleccionamos el LSE para RTC
	RCC->BDCR |= RCC_BDCR_RTCEN;				// Activamos el RTC clock


	//Activamos el modo escritura de los registros RTC
	ptrHandlerRtc->ptrRTC->WPR =  0xCA;
	ptrHandlerRtc->ptrRTC->WPR =  0x53;

	//CONFIGURACION DEL CALENDARIO :




}
