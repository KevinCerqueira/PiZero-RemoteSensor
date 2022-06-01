#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//USE WIRINGPI PIN NUMBERS
#define LCD_RS 6    // Register select pin
#define LCD_E 31    // Enable Pin
#define LCD_D4 26   // Data pin 4
#define LCD_D5 27   // Data pin 5
#define LCD_D6 28   // Data pin 6
#define LCD_D7 29   // Data pin 7
#define MAXTIMINGS 85

int lcd;

int main(void)
{
        int lcd;
        wiringPiSetup();
        lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
        
        while (1)
        {
                lcdPosition(lcd, 0, 0);
                lcdPrintf(lcd, "Hello World!");

                lcdPosition(lcd, 0, 1);
                lcdPrintf(lcd, "THE BEST GROUP");

                delay(1000); 
        }

        return(0);
}