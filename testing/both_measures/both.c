#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <lcd.h>
#include <stdint.h>
#include <time.h>

#include "constants.h"

#define MAXTIMINGS      85
#define DHTPIN          5

int dht11_dat[5] = { 0, 0, 0, 0, 0 };
int lcd; 

//USE WIRINGPI PIN NUMBERS
#define BTN_1 25
#define LCD_RS 6    // Register select pin
#define LCD_E 31    // Enable Pin
#define LCD_D4 26   // Data pin 4
#define LCD_D5 27   // Data pin 5
#define LCD_D6 28   // Data pin 6
#define LCD_D7 29   // Data pin 7
#define MAXTIMINGS 85

void m_delay(int ms){
    // Storing start time
    clock_t start_time = clock();
  
    // looping till required time is not achieved
    while (clock() < start_time + ms)
        ;
}

void read_dht11_dat(){
       // wiringPiSetup();
        //lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D$

        uint8_t laststate       = HIGH;
        uint8_t counter         = 0;
        uint8_t j               = 0, i;
        float   f; 
 
        dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
        
        pinMode( DHTPIN, OUTPUT );
        digitalWrite( DHTPIN, LOW );
        delay(18);
        digitalWrite( DHTPIN, HIGH );
        delayMicroseconds(40);
        pinMode( DHTPIN, INPUT );
          
        for ( i = 0; i < MAXTIMINGS; i++ ){
                counter = 0;
                while ( digitalRead( DHTPIN ) == laststate ){
                        counter++;
                        delayMicroseconds( 1 );
                        if ( counter == 255 ){
                                break;
                        }
                }
          
                laststate = digitalRead( DHTPIN );
                if ( counter == 255 )
                        break;
 
                if ( (i >= 4) && (i % 2 == 0) ){
                        dht11_dat[j / 8] <<= 1;
                        if ( counter > 16 )
                                dht11_dat[j / 8] |= 1;
                        j++;
                }
                if ( (j >= 40) &&
                   (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF))){
                      lcdPosition(lcd, 0, 0);
                      lcdPrintf(lcd, "H %d.%d       ", dht11_dat[0], dht11_dat[1]);
                      lcdPosition(lcd, 0, 1);
                      lcdPrintf(lcd, "T %d.%d C        ", dht11_dat[2], dht11_dat[3]);
                }
        }
}

int main(){
        printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
 
        if ( wiringPiSetup() == -1 )
                exit( 1 );

        if(openI2CBus("/dev/i2c-1") == -1){
                return EXIT_FAILURE;
        }
        setI2CSlave(0x48);
        
        lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7,$
        pinMode(BTN_1, INPUT);

        int measureMode = 1;
        int selectBtnState = 0;
        
        while (1){
                int bt_in = digitalRead(BTN_1);
                switch(selectBtnState){
                        case 0:
                                if(bt_in == 0){
                                        selectBtnState = 1;
                                }
                        break;
                        case 1:
                                if(bt_in == 1){
                                        selectBtnState = 2;
                                }
                        break;
                        case 2:
                                measureMode = !measureMode;
                                selectBtnState = 0;
                        break;  
                }

                if(measureMode == 0){
                        lcdPosition(lcd, 0, 0);
                        lcdPrintf(lcd, "LUMI = %.2f W", readVoltage(0));

                        lcdPosition(lcd, 0, 1);
                        lcdPrintf(lcd, "PRESS = %.2f Pa", readVoltage(1));

                }
                else{
                        read_dht11_dat();
                }
                m_delay(1000); 
        }
        return 0;
}                       





