/*
 * BasicTimer.h
 *
 *  Created on: Apr 18, 2022
 *      Author: Juan Esteban Rodriguez Ochoa
 */

#ifndef INC_BASICTIMER_H_
#define INC_BASICTIMER_H_

#include "stm32f4xx.h"

#define BTIMER_MODE_UP		0
#define BTIMER_MODE_DOWN	1

#define BTIMER_SPEED_10us	160
#define BTIMER_SPEED_100us	1600
#define BTIMER_SPEED_1ms	16000

#define BTIMER_SPEED_100MHz_10us	1000
#define BTIMER_SPEED_100MHz_100us	10000
#define BTIMER_SPEED_100MHz_1ms	    100000



/* Estructura que contiene la configuración mínima necesaria para el manejo del Timer.*/
typedef struct
{
	uint8_t		TIMx_mode; 		// Up or dowm
	uint32_t	TIMx_speed;		// A qué velocidad se incrementa el Timer
	uint32_t	TIMx_period;	// Valor en ms del periodo del Timer
	uint8_t		TIMx_interruptEnable;	// Activa o desactiva el modo interrupción del timer.
}BasicTimer_Config_t;

/* Handler para el Timer*/
typedef struct
{
	TIM_TypeDef			*ptrTIMx;
	BasicTimer_Config_t	TIMx_Config;
}BasicTimer_Handler_t;

void BasicTimer_Config(BasicTimer_Handler_t *ptrBTimerHandler);
void BasicTimer2_Callback(void); /* Esta función debe ser sobre-escrita en el main para que el sistema funcione*/
void BasicTimer3_Callback(void);
void BasicTimer4_Callback(void);
void BasicTimer5_Callback(void);
void Capture_TIM2_Ch1_Callback(void);
void Capture_TIM2_Ch2_Callback(void);
void Capture_TIM2_Ch3_Callback(void);
void Capture_TIM2_Ch4_Callback(void);
void Capture_TIM3_Ch1_Callback(void);
void Capture_TIM3_Ch2_Callback(void);
void Capture_TIM3_Ch3_Callback(void);
void Capture_TIM3_Ch4_Callback(void);
void Capture_TIM4_Ch1_Callback(void);
void Capture_TIM4_Ch2_Callback(void);
void Capture_TIM4_Ch3_Callback(void);
void Capture_TIM4_Ch4_Callback(void);
void Capture_TIM5_Ch1_Callback(void);
void Capture_TIM5_Ch2_Callback(void);
void Capture_TIM5_Ch3_Callback(void);
void Capture_TIM5_Ch4_Callback(void);

void startTimer (BasicTimer_Handler_t *ptrTimerConfig);
void stopTimer (BasicTimer_Handler_t *ptrTimerConfig);
#endif /* INC_BASICTIMER_H_ */
