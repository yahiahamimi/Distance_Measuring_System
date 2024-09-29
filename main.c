/******************************************************************************
 *
 * File Name: main.c
 *
 * Description: source file for main
 *
 * Author: Yahia Hamimi
 ******************************************************************************/

#include "lcd.h"
#include "ultrasonic.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	uint16 distance = 0;
	SREG |= (1 << 7);
	LCD_init();
	Ultrasonic_init();
	LCD_displayString("Distance =    cm");

	while (1)
	{
		distance = Ultrasonic_readDistance();
		LCD_moveCursor(0, 11);
		if (distance > 100)
		{
			LCD_intgerToString(distance);
		}
		else
		{
			LCD_intgerToString(distance);
			LCD_displayCharacter(' ');
		}
		_delay_ms(100);
	}
}
