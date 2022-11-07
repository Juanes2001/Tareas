/*
 * RTCDriver.c
 *
 *  Created on: 5/11/2022
 *      Author: Juan Esteban Rodriguez Ochoa
 */


#include "RTCDriver.h"

void Rtc_Congif (RTC_Handler_t *ptrHandlerRtc){
	//Activamos el PWR parapoder desactivar su sistema de bloqueo en el RTC
	RCC->APB1ENR = RCC_APB1ENR_PWREN;
	//Desactivamos la proteccion de PWR para modificar registros
	PWR->CR |= PWR_CR_DBP;

	/* 1. Debemos activar el LSE */

	//Encendemos el LSE clock
	RCC->BDCR |= RCC_BDCR_LSEON;
	// Esperamos hasta que el LSE este listo para ser usado
	while(!(RCC->BDCR & RCC_BDCR_LSERDY)){
		__NOP();
	}
	// Seleccionamos el LSE para RTC
	RCC->BDCR |= (0b01 << RCC_BDCR_RTCSEL_Pos);
	// Activamos el RTC clock
	RCC->BDCR |= RCC_BDCR_RTCEN;


	//Activamos el modo escritura de los registros RTC
	ptrHandlerRtc->ptrRTC->WPR =  0xCA;
	ptrHandlerRtc->ptrRTC->WPR =  0x53;

	//CONFIGURACION DEL CALENDARIO :
	//Seteamos el bit INIT para entrar en el initialization mode
	ptrHandlerRtc->ptrRTC->ISR |= RTC_ISR_INIT;
	// Y permitimos las actualizaciones del calendario
	ptrHandlerRtc->ptrRTC->ISR |= RTC_ISR_INITF;

	//Configuramos para que tengamos un reloj de un segundo de precision
	// Para ello ponemos PREDIV_A y PREDIV_S en RTC_PRER
	ptrHandlerRtc->ptrRTC->PRER |= (0x0FF << RTC_PRER_PREDIV_S_Pos);
	ptrHandlerRtc->ptrRTC->PRER |= (0x07F << RTC_PRER_PREDIV_A_Pos);

	//Seteamos el horario actual en formato 24H

	Rtc_BDC_Convertion(ptrHandlerRtc);




}

 void Rtc_BDC_Convertion (RTC_Handler_t *ptrHandlerRtc){
	// convertimos el formato de horas en BDC
	 uint8_t HU = (ptrHandlerRtc->RTC_config.rtcHours%10);
	 uint8_t HD = (ptrHandlerRtc->RTC_config.rtcHours-ptrHandlerRtc->RTC_config.rtcHours%10)/10;
	 // convertimos el formato de Minutos en BDC
	 uint8_t MU = (ptrHandlerRtc->RTC_config.rtcMinutes%10);
	 uint8_t MD = (ptrHandlerRtc->RTC_config.rtcMinutes-ptrHandlerRtc->RTC_config.rtcMinutes%10)/10;
	 // convertimos el formato de segundos en BDC
	 uint8_t SU = (ptrHandlerRtc->RTC_config.rtcSeconds%10);
	 uint8_t SD = (ptrHandlerRtc->RTC_config.rtcSeconds-ptrHandlerRtc->RTC_config.rtcSeconds%10)/10;
	 // convertimos el formato de meses en BDC
	 uint8_t MoU = (ptrHandlerRtc->RTC_config.rtcMonth%10);
	 uint8_t MoD = (ptrHandlerRtc->RTC_config.rtcMonth-ptrHandlerRtc->RTC_config.rtcMonth%10)/10;
	 // convertimos el formato de dias en BDC
	 uint8_t DU = (ptrHandlerRtc->RTC_config.rtcDay%10);
	 uint8_t DD = (ptrHandlerRtc->RTC_config.rtcDay-ptrHandlerRtc->RTC_config.rtcDay%10)/10;
}
