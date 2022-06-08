// 21
// 24
// 25
// gcc -o led2 led.c -lwiringPi -lwiringPiDev 
#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>


#define BTN_1   5
#define LED             6

int main(){
        wiringPiSetupGpio();

        pinMode(LED, OUTPUT);
        pinMode(BTN_1, INPUT);

        printf("Setup.\n"); 
        int t = 1;
              while(t == 1){
                      int bt_in = digitalRead(BTN_1);
                      printf("%d", bt_in);
                      if(bt_in){
                              digitalWrite(LED, HIGH);
                      }else {
                              digitalWrite(LED, LOW);
                      }
                      delay(1000);
              }
}
