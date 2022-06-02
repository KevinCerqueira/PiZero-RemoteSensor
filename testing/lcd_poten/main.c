// compilar: gcc main.c read_voltage.c -o test -lwiringPi -lwiringPiDev
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <lcd.h>
#include <stdint.h>

#include "constants.h"

//USE WIRINGPI PIN NUMBERS
#define LCD_RS 6    // Register select pin
#define LCD_E 31    // Enable Pin
#define LCD_D4 26   // Data pin 4
#define LCD_D5 27   // Data pin 5
#define LCD_D6 28   // Data pin 6
#define LCD_D7 29   // Data pin 7
#define MAXTIMINGS 85

int main(void) {
	
	int lcd;
	wiringPiSetup();
	lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
	
	if(openI2CBus("/dev/i2c-1") == -1)
	{
		return EXIT_FAILURE;
	}
	setI2CSlave(0x48);
	while(1)
	{
		lcdPosition(lcd, 0, 0);
		lcdPrintf(lcd, "LUMI = %.2f W", readVoltage(0));

		lcdPosition(lcd, 0, 1);
		lcdPrintf(lcd, "PRESS = %.2f Pa", readVoltage(1));

		delay(1000); 
	}

	return EXIT_SUCCESS;
}
