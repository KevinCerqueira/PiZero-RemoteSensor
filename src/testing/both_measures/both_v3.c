#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>
#include <math.h>

#include <wiringPi.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <lcd.h>

#include "constants.h"

int dht_matrix[6][10] = {{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
                        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
                        };

double poten_matrix [2][10] =   {{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
                                };

int dht11_dat[5] = {0, 0, 0, 0, 0};
double lum_dat = 0;
double pressure_dat = 0;
int lcd; 
int delay_time = 5000;
int max_hist = 0;

//USE WIRINGPI PIN NUMBERS
#define MAXTIMINGS      85
#define DHTPIN          4

#define BTN_1 21
#define BTN_2 24
#define BTN_3 25

#define DP_ON_1 7

#define DP3 2
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

void dht_insert(int value, int list_index, int element_index){
    int temp = dht_matrix[list_index][element_index];
    dht_matrix[list_index][element_index] = value;
    if(element_index == 9){
        return;
    }
    else{
        dht_insert(temp, list_index, element_index+1);
    }
}

void poten_insert(int value, int list_index, int element_index){
    double temp = poten_matrix[list_index][element_index];
    poten_matrix[list_index][element_index] = value;
    if(element_index == 9){
        return;
    }
    else{
        poten_insert(temp, list_index, element_index+1);
    }
}


void read_dht11_dat(){
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
                        dht_insert(dht11_dat[0], 0, 0);
                        dht_insert(dht11_dat[1], 1, 0);
                        dht_insert(dht11_dat[2], 2, 0);
                        dht_insert(dht11_dat[3], 3, 0);
                }
        }
}

void print_dht_data(int hist){
        int hum_i = dht_matrix[0][hist];
        int hum_f = dht_matrix[1][hist];
        int temp_i = dht_matrix[2][hist];
        int temp_f = dht_matrix[3][hist];

        lcdPosition(lcd, 0, 0);
        lcdPrintf(lcd, "H #%d: %d.%d          ", hist+1, hum_i, hum_f);
        lcdPosition(lcd, 0, 1);
        lcdPrintf(lcd, "T #%d: %d.%d C        ", hist+1, temp_i, temp_f);
}

void read_poten_data(){
        //lum_dat = readVoltage(0);
        //pressure_dat = readVoltage(1);
        poten_insert(readVoltage(0), 4, 0);
        poten_insert(readVoltage(1), 5, 0);
}

void print_poten_data(int hist){
        lcdPosition(lcd, 0, 0);
        lcdPrintf(lcd, "LUMI #%d: %.2f W", hist+1, poten_matrix[0][hist]);

        lcdPosition(lcd, 0, 1);
        lcdPrintf(lcd, "PRESS #%d: %.2f Pa", hist+1, poten_matrix[1][hist]);
}

void read_sensors(){
    read_dht11_dat();
    read_poten_data();
}

void print_delay(){
    lcdPosition(lcd, 0, 0);
    lcdPrintf(lcd, "INTERVALO:    ");
    lcdPosition(lcd, 0, 1);
    lcdPrintf(lcd, "%d ms       ", delay_time);
}

void* measure_thread(void* arg){
    while(1){
        clock_t start = clock();
        delay(delay_time);
        printf("%d Miliseconds have passed.\n", delay_time);
        read_sensors();
        max_hist = fmin(9, max_hist+1);
        printf("Measures taken. \n");
        start = clock();
    }
    return NULL;
}


struct PushButton{
    int state;
};

void init_button(struct PushButton *bt){
    bt->state = 0;
}

int btn_is_released(int bt_in, struct PushButton *btn){
    switch(btn->state){
        case 0:
            if(bt_in == 0){
                    btn->state = 1;
            }
            break;
        case 1:
            if(bt_in == 1){
                    btn->state = 2;
            }
            break;
        case 2:
            btn->state = 0;
            return 1;
            break;  
    }
    return 0;
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

    pinMode(DP4, INPUT);
    pinMode(DP3, INPUT);
    pinMode(BTN_1, INPUT);
    pinMode(BTN_3, INPUT);

    int configMode = 0;
    int measureMode = 0;
    int hist_index = 0;

    struct PushButton bt1;
    struct PushButton bt2;
    struct PushButton bt3;

    init_button(&bt1);
    init_button(&bt2);
    init_button(&bt3);

    pthread_t timer;
    pthread_create(&timer, NULL, measure_thread, NULL);
	int loop = 1;
    read_sensors();

    while (loop == 1){
        measureMode = digitalRead(DP3);
        configMode = digitalRead(DP4);

        int bt_in1 = btn_is_released(digitalRead(BTN_1), &bt1);
        int bt_in3 = btn_is_released(digitalRead(BTN_3), &bt3);
        
        if(configMode == 0){
            print_delay();
        }
        else {
            if(bt_in3){
                // later
                hist_index = fmin(max_hist, hist_index+1); 
            }
            else if(bt_in1){
                // earlier
                hist_index = fmax(0, hist_index-1); 
            }
            if(measureMode == 0){
                print_poten_data(hist_index);
                // printf("poten measure\n");
            }
            else{
                print_dht_data(hist_index);
                // printf("sneed\n");
            }
        }
        
    }

    return 0;
}                       
