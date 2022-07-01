# Problema 03 – IoT: A Internet das Coisas

<p>Projeto que captura medidas de um sensor DHT11 e dois potenciômetros em uma Raspberry Pi Zero, mostrando medidas num display LCD. As medidas também são enviadas para um servidor python para serem exibidas em uma view remota, implementando o protocolo de comunicação MQTT. Usuários na view podem alterar a frequência na qual as medições são feitas.
</p>

## Pré-requisitos na Raspberry Pi

O programa precisa das bibliotecas mosquitto, wiringPi, e i2c-dev.

## Instalação na Raspberry Pi

<ol>
  <li>Baixe o projeto.</li>
  <li>Transfira os seguintes arquivos da pasta "src/raspberry" para uma Raspberry Pi Zero: "main_final.c", "constants.h", "read_voltage.c". Certifique-se que todos os arquivos estão na mesma pasta.</li>
  <li>Compile o código.</li>
  <li>Execute o código.</li>
</ol>

## Utilizando a IHM da Raspberry Pi

Foram utilizados 2 push buttons e uma dip switch como fontes de entrada nesse projeto. O interruptor 3 da dip switch muda as medidas que são mostradas no LCD entre medidas de Temperatura/Humidade e Luminosidade/Pressão. O interruptor 4 troca o modo de mostragem da IHM para mostrar o intervalo de medida atual, ou mostrar as medidas.

Enquanto no modo de medida, os botões podem ser utilizados para mostrar os resultados de medidas mais recentes ou antigas. No modo de intervalo, os botões incrementam ou diminuem o intervalo de medida em 1 segundo.

### Comando para transferir arquivos para raspberry:
```sh
$ scp arquivo.c pi@[endereço da raspberry]:[filepath desejado]
```

### Comando para compilar código na raspberry:
```sh
$ gcc main_final.c read_voltage.c -o executable  -lwiringPi -lwiringPiDev -lm -lpthread -lmosquitto
```
### Comando para executar código na raspberry:
```sh
$ sudo ./executable
```

### Comando para startar o container da view em PHP e a API em Python:
```sh
$ docker-compose up -d
```
### Comando para startar o subscriber que recebe as informações enviadas da Raspberry
- Se tiver no linux
```sh
$ python3 src/server/subscriber.py
```
- Se tiver no Windows
```sh
$ python src/server/subscriber.py
```
