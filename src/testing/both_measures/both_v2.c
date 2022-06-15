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
#define DHTPIN          1

int dht11_dat[5] = { 0, 0, 0, 0, 0 };
int lum_dat = 0;
int pressure_dat = 0;
int lcd; 

//USE WIRINGPI PIN NUMBERS
#define BTN_1 4
#define BTN_3 25

#define DP_ON_1 7
#define DP4 3

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

int read_dht11_dat(){
       // wiringPiSetup();
        //lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

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
                        return 1;
                }
                else {
                        return 0;
                }
        }
}

void print_dht_data(){
        lcdPosition(lcd, 0, 0);
        lcdPrintf(lcd, "H %d.%d          ", dht11_dat[0], dht11_dat[1]);
        lcdPosition(lcd, 0, 1);
        lcdPrintf(lcd, "T %d.%d C        ", dht11_dat[2], dht11_dat[3]);
}

void read_poten_data(){
        lum_dat = readVoltage(0);
        pressure_dat = readVoltage(1);
}

void print_poten_data(){
        lcdPosition(lcd, 0, 0);
        lcdPrintf(lcd, "LUMI = %.2f W", lum_dat);

        lcdPosition(lcd, 0, 1);
        lcdPrintf(lcd, "PRESS = %.2f Pa", pressure_dat);
}

int main(){
        printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
 
        if ( wiringPiSetup() == -1 )
                exit( 1 );
	
        printf("wPI setup\n");

        if(openI2CBus("/dev/i2c-1") == -1){
                return EXIT_FAILURE;
        }

        setI2CSlave(0x48);
	printf("I2C setup\n");

	lcd = lcdInit(2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
        pinMode(BTN_3, INPUT);

        clock_t start_time = clock();
        int measure_delay = 2000;

        int measureMode = 0;
        int selectBtnState = 0;
	int loop = 1;
        
        while (loop == 1){
                int bt_in = digitalRead(BTN_3);
				//	printf("%d \n", bt_in);
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

                if(start_time + measure_delay <= clock()){
						printf("measure taken!\n");
                        read_dht11_dat();
                        read_poten_data();
                        start_time = clock();
                }else {
					printf("%f", start_time + measure_delay - clock());
				}

                if(measureMode == 0){
                        print_poten_data();
                        // printf("poten measure\n");
                }
                else{
                        print_dht_data();
                        // printf("sneed\n");
                }

        }
        return 0;
}                       
