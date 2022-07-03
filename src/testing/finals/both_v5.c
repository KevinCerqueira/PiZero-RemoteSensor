/**
 * Componente Curricular: MI Concorrência e Conectividade
 * Autor: Esdras Abreu, Guilherme Nobre e Kevin Cerqueira
 *
 * Declaro que este código foi elaborado por mim de forma individual e
 * não contém nenhum trecho de código de outro colega ou de outro autor,
 * tais como provindos de livros e apostilas, e páginas ou documentos
 * eletrônicos da Internet. Qualquer trecho de código de outra autoria que
 * uma citação para o  não a minha está destacado com  autor e a fonte do
 * código, e estou ciente que estes trechos não serão considerados para fins
 * de avaliação. Alguns trechos do código podem coincidir com de outros
 * colegas pois estes foram discutidos em sessões tutorias.
 */

// compilar: gcc both_v4.c read_voltage.c -o both4 -lwiringPi -lwiringPiDev -lm -lpthread -lmosquitto

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <mosquitto.h>
#include <wiringPi.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <lcd.h>
#include <string.h>

#include "constants.h"

// matriz com medidas do dht

int dht_matrix[6][10] = {{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
                        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
                        };

// matriz com medidas dos potenciometros

float poten_matrix [2][10] =   {{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
                                };


int dht11_dat[5] = {0, 0, 0, 0, 0}; //medida do dht11
float lum_dat = 0; // medida do potenciometro 1
float pressure_dat = 0; // medida do potenciometro 2
int lcd; // display lcd
int delay_time = 5000; // intervalo de medida
int max_hist = 0; // valor que determina o histórico mais antigo

struct mosquitto *mosq_pub;
struct mosquitto *mosq_sub;

//USE WIRINGPI PIN NUMBERS
#define MAXTIMINGS      85
#define DHTPIN          4 // GPIO 23

#define BTN_1 21 // GPIO 5
#define BTN_2 24
#define BTN_3 25 // GPIO 26

#define DP_ON_1 7 // GPIO 4

#define DP3 2 // GPIO 27
#define DP4 3 // GPIO 22

#define LCD_RS 6    // Register select pin
#define LCD_E 31    // Enable Pin
#define LCD_D4 26   // Data pin 4
#define LCD_D5 27   // Data pin 5
#define LCD_D6 28   // Data pin 6
#define LCD_D7 29   // Data pin 7
#define MAXTIMINGS 85

// funções recursivas quee inserem medidas nas matrizes de medidas
/* inserções sao feitas no inicio de cada lista de medida, "empurrando" as outras medidas para um indice maior através
de outras inserções em indices maiores
*/

void dht_insert(int value, int list_index, int element_index){
    int temp = dht_matrix[list_index][element_index];
    dht_matrix[list_index][element_index] = value;
    if(element_index == 9){ // máximo de 10 medidas então a função para no indice 9
        return;
    }
    else{
        dht_insert(temp, list_index, element_index+1);
    }
}

void poten_insert(float value, int list_index, int element_index){
    float temp = poten_matrix[list_index][element_index];
    poten_matrix[list_index][element_index] = value;
    if(element_index == 9){ // máximo de 10 medidas então a função para no indice 9
        return;
    }
    else{
        poten_insert(temp, list_index, element_index+1);
    }
}


// função que lê dados do dht11 e guarda-os na matriz

void read_dht11_dat(){
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

// função que mostra medida do dht11 no LCD

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

// função que lê medida do potenciometro

void read_poten_data(){
        lum_dat = readVoltage(0);
        pressure_dat = readVoltage(1);
        poten_insert(lum_dat, 0, 0);
        poten_insert(pressure_dat, 1, 0);
}


// função que mostra medidas dos potenciometros no LCD

void print_poten_data(int hist){
        lcdPosition(lcd, 0, 0);
        lcdPrintf(lcd, "LUMI #%d: %.2f W", hist+1, poten_matrix[0][hist]);

        lcdPosition(lcd, 0, 1);
        lcdPrintf(lcd, "PRESS #%d: %.2f Pa", hist+1, poten_matrix[1][hist]);
}

// função que lê ambos os sensores e envia medidas para o broker

void read_sensors(){
    read_dht11_dat();
    read_poten_data();
	time_t measure_time = time(&measure_time);

	int hum_i = dht11_dat[0];
    int hum_f = dht11_dat[1];
    int temp_i = dht11_dat[2];
    int temp_f = dht11_dat[3];

	float lum = lum_dat;
	float press = pressure_dat;
	
	struct tm *ptm = gmtime(&measure_time);	

	char fim[16] = "}";
	char inicio[512] = "g02pb3EGK {";
	char hum_string[64];
	char temp_string[64];
	char lum_string[64];
	char press_string[64];

	char time_string[64];
	char time_stamp[128];	
	
	char interval_string[64];

	strftime(time_string, 64, "%Y-%m-%d %H:%M:%S", ptm);
	sprintf(time_stamp, "\"datetime\": \"%s\", \0", time_string);

	sprintf(hum_string, "\"H\": \"%d.%d\", \0", hum_i, hum_f);
	sprintf(temp_string, "\"T\": \"%d.%d\", \0", temp_i, temp_f);
	sprintf(press_string, "\"P\": \"%.2f\", \0", press);	
	sprintf(lum_string, "\"L\": \"%.2f\", \0", lum);
	sprintf(interval_string, "\"interval\": \"%d\"", delay_time/1000); 
	
	strcat(inicio, hum_string);
	strcat(inicio, temp_string);
	strcat(inicio, press_string);
	strcat(inicio, lum_string);
	strcat(inicio, time_stamp);
	strcat(inicio, interval_string);
	strcat(inicio, fim);
	
	printf("%s", asctime(ptm));
	printf("SENDING THIS TO MQTT: \n%s\n", inicio);
	mosquitto_publish(mosq_pub, NULL, "G02_THEBESTGROUP/MEDICOES", 256, inicio, 0, false);
}

// função que mostra o intervalo de medida no LCD

void print_delay(){
    lcdPosition(lcd, 0, 0);
    lcdPrintf(lcd, "INTERVALO:         ");
    lcdPosition(lcd, 0, 1);
    lcdPrintf(lcd, "%d ms             ", delay_time);
}

/* função chamada pela thread responsável pelas medidas
A thread tem o seguinte comportamento:
1. Espera o intervalo de medida
2. Lê os sensores
3. Incrementa a váriavel max_hist, permitindo que o usuário veja um histórico mais profundo dependendo de quantas medidas
já foram feitas.
4. envia as medidas mais recentes através do publisher
*/

void* measure_thread(void* arg){
    while(1){
        clock_t start = clock();
        delay(delay_time);
        printf("%d Miliseconds have passed.\n", delay_time);
        read_sensors();
        max_hist = fmin(9, max_hist+1);
        printf("Measures taken. \n");
        time_t datetime = time(NULL);
        start = clock();
    }
    return NULL;
}


// classe que simula o comportamento de um botão

struct PushButton{
    int state;
};

void init_button(struct PushButton *bt){
    bt->state = 0;
}

// função que controla a máquina de estados dos botões PushButton
/*
Estados: 
0 - Ocioso
1 - Botão pressionado
2 - Botão solto
*/

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

void on_connect(struct mosquitto *mosq, void *obj, int connect) {
	if(connect!=0){ 
		printf("Erro no resultado do codigo: %d\n", connect);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, "G02_THEBESTGROUP/INTERVALO", 0);
}

void on_message(struct mosquito *mosq, void *obj, const struct mosquitto_message *msg){
	printf("Nova mensagem com o tópico %s: %s\n", msg->topic, (char *) msg->payload);
}

// função principal

int main(){
	
	int user_sub, connect_sub, id;
	int user_pub, connect_pub;

    printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );

    if ( wiringPiSetup() == -1 )
            exit( 1 );

    printf("wPI setup\n");

    if(openI2CBus("/dev/i2c-1") == -1){
            return EXIT_FAILURE;
    }

    setI2CSlave(0x48);

	printf("I2C setup\n");

	mosquitto_lib_init();
	id = 12;
	mosq_pub = mosquitto_new("G02_THEBESTGROUP_PUB_RASP", true, NULL);
	mosq_sub = mosquitto_new("G02_THEBESTGROUP_SUB_RASP", true, &id);
	
	user_sub = mosquitto_username_pw_set(mosq_sub, "aluno", "aluno*123");
	user_pub = mosquitto_username_pw_set(mosq_pub, "aluno", "aluno*123");
	
	mosquitto_connect_callback_set(mosq_sub, on_connect);
	mosquitto_message_callback_set(mosq_sub, on_message);

	connect_sub = mosquitto_connect(mosq_sub, "10.0.0.101", 1883, 5);
	connect_pub = mosquitto_connect(mosq_pub, "10.0.0.101", 1883, 5);

	if(user_pub != 0 || connect_pub != 0 || user_sub != 0 || connect_sub != 0){
		printf("Cliente não foi conectado ao Broker!");
		mosquitto_destroy(mosq_pub);
		mosquitto_destroy(mosq_sub);
		return -1;
	}

	printf("Broker setup.\n");

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

	//mosquitto_loop_start(mosq_sub);	
	//getchar();

    while (loop == 1){
		
        measureMode = digitalRead(DP3);
        configMode = digitalRead(DP4);

        int bt_in1 = btn_is_released(digitalRead(BTN_1), &bt1);
        int bt_in3 = btn_is_released(digitalRead(BTN_3), &bt3);
		
        if(configMode == 0){
            if(bt_in3){
                delay_time = delay_time + 1000;
            }
            else if(bt_in1){
                delay_time = fmax(2000, delay_time - 1000);
            }
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
