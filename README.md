# Problema 03 – IoT: A Internet das Coisas

### Comando para compilar código na raspberry:
```sh
$ gcc file1.c file2.c -o executable -lwiringPi -lwiringPiDev
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
