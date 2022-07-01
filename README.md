# Problema 03 – IoT: A Internet das Coisas

<p>Projeto que captura medidas de um sensor DHT11 e dois potenciômetros em uma Raspberry Pi Zero, mostrando medidas num display LCD. As medidas também são enviadas para um servidor python para serem exibidas em uma view remota, implementando o protocolo de comunicação MQTT. Usuários na view podem alterar a frequência na qual as medições são feitas.
</p>

## Instalação

<ol>
  <li>Baixe o projeto.</li>
  <li>Transfira os seguintes arquivos da pasta "src/raspberry" para uma Raspberry Pi Zero: "main_final.c", "constants.h", "read_voltage.c". Certifique-se que todos os arquivos estão na mesma pasta.</li>
  
</ol>

### Comando para transferir arquivos para raspberry:
```sh
$ scp arquivo.c pi@[endereço da raspberry]:[filepath desejado]
```

### Comando para compilar código na raspberry:
```sh
$ gcc file1.c read_voltage.c -o executable  -lwiringPi -lwiringPiDev -lm -lpthread -lmosquitto
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
