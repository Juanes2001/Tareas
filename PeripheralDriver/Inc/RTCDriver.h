/*
 * RTCDriver.h
 *
 *  Created on: 4/11/2022
 *      Author: ALEJANDRA MARIA
 */

#ifndef RTCDRIVER_H_
#define RTCDRIVER_H_

#include "stm32f4xx.h"

#define RTC_DAY_FORBIDDEN	0
#define RTC_DAY_MONDAY		1
#define RTC_DAY_THURSDAY	2
#define RTC_DAY_WEDNESDAY	3
#define RTC_DAY_TUESDAY		4
#define RTC_DAY_FRIDAY		5
#define RTC_DAY_SATURDAY	6
#define RTC_DAY_SUNDAY		7

typedef struct
{
	uint8_t  year;
	uint8_t  month;
	uint8_t  weekDay;
	uint16_t day;
	uint8_t	 hour;
	uint8_t  minutes;
	uint8_t seconds;

}Rtc_Config_t;


void rtcConfig(Rtc_Config_t *rtcConfig);
//void Date(uint8_t year, uint8_t month, uint16_t day);
//void Time(uint8_t hour, uint8_t minutes, uint8_t seconds);
uint8_t* getDate(void);


#endif /* RTCDRIVER_H_ */
