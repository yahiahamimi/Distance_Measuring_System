/*
 * ultrasonic.c
 *
 *  Created on: Mar 2, 2024
 *      Author: Yahia Hamimi
 */
#include"icu.h"
#include"gpio.h"
#include"ultrasonic.h"
#include"avr/io.h"
#include<util/delay.h>
/*******************************************************************************
 *                          Global Variables                                   *
 *******************************************************************************/
 uint8 g_edgeCount;
 uint16 g_timeHigh;
 uint16 g_timePeriod;
 uint16 g_timePeriodPlusHigh;

ICU_ConfigType ICU_Configurations = {F_CPU_8,RAISING};
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void Ultrasonic_init(void){
	ICU_init(&ICU_Configurations);
	ICU_setCallBack(Ultrasonic_edgeProcessing);
	GPIO_setupPinDirection(ULTRASONIC_TRIGGER_PORT_ID,ULTRASONIC_TRIGGER_PIN_ID,PIN_OUTPUT);

}
void Ultrasonic_Trigger(void){
		GPIO_writePin(ULTRASONIC_TRIGGER_PORT_ID,ULTRASONIC_TRIGGER_PIN_ID,LOGIC_HIGH);
		_delay_us(10);
		GPIO_writePin(ULTRASONIC_TRIGGER_PORT_ID,ULTRASONIC_TRIGGER_PIN_ID,LOGIC_LOW);
}
uint16 Ultrasonic_readDistance(void){
	Ultrasonic_Trigger();
/*Now, here we have selected an internal 8MHz oscillator frequency for ATmega32
 * with Prescaler F_CPU/8 for timer frequency.
 * Then time to execute 1 instruction is 1 us.
 * So, the timer gets incremented after 1 us time elapse. = 17000 x (TIMER value) x 1 x 10^-6 cm
 * = 0.017 x (TIMER value) cm = (TIMER value) / 58.8 cm */

	uint16 distance=((float)(g_timePeriodPlusHigh-g_timePeriod)/58.8);
	return distance;
	}

void Ultrasonic_edgeProcessing(void){
		g_edgeCount++;
		if(g_edgeCount == 1)
		{
			/*
			 * Clear the timer counter register to start measurements from the
			 * first detected rising edge
			 */
			ICU_clearTimerValue();
			/* Detect falling edge */
			ICU_setEdgeDetectionType(FALLING);
		}
		else if(g_edgeCount == 2)
		{
			/* Store the High time value */
			g_timeHigh = ICU_getInputCaptureValue();
			/* Detect rising edge */
			ICU_setEdgeDetectionType(RAISING);
		}
		else if(g_edgeCount == 3)
			{
				/* Store the Period time value */
				g_timePeriod = ICU_getInputCaptureValue();
				/* Detect falling edge */
				ICU_setEdgeDetectionType(FALLING);
			}
			else if(g_edgeCount == 4)
			{
				/* Store the Period time value + High time value */
				g_timePeriodPlusHigh = ICU_getInputCaptureValue();
				/* Clear the timer counter register to start measurements again */
				ICU_clearTimerValue();
				/* Detect rising edge */
				ICU_setEdgeDetectionType(RAISING);
				g_edgeCount=0;
			}


}
