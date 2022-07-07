# Problema 03 – IoT: A Internet das Coisas

Projeto que captura medidas de um sensor DHT11 e dois potenciômetros em uma Raspberry Pi Zero, mostrando medidas num display LCD. As medidas também são enviadas para um servidor python, implementando o protocolo de comunicação MQTT, para serem exibidas em uma view remota. Usuários na view podem alterar a frequência na qual as medições são feitas, assim como também podem alterar a mesma frequência nos próprios butões/dipswitch da Raspberry.
## Sumário
- [Requisitos](#requisitos)
- [Sobre MQTT](#sobre-mqtt)
- [Diagrama do Sistema](#diagrama-do-sistema)
  - [Parte Raspberry](#parte-raspberry)
  - [Parte Broker](#parte-broker)
  - [Parte Remota](#parte-remota)
    - [Subscriber](#subscriber)
    - [Banco de Dados](#banco-de-dados)
    - [Servidor](#servidor)
    - [Back-End (View)](#back-end-view)
    - [Front-End (View)](#front-end-view)
- [Pré-requisitos](#pré-requisitos)
  - [Na Raspberry PI Zero](#na-raspberry-pi)
    - [Utilizando a IHM da Raspberry Pi](#utilizando-a-ihm-da-raspberry-pi)
  - [No Remoto](#no-remoto)
- [Startando o projeto](#startando-o-projeto)
  - [Parte Raspberry](#parte-raspberry)
  - [Parte Remota](#parte-remota-1)
- [Principais telas do sistema](#principais-telas-do-sistema)

## Requisitos
- Códigos e protocolos:
  - Código deverá ser escrito em linguagem C; :white_check_mark: 
  - Usar protocolo MQTT. :white_check_mark: 
- O sistema deverá possuir uma IHM local com interface baseada em display LCD, botões e chaves; :white_check_mark: 
- O sistema deverá implementar uma interface em forma de aplicativo para Desktop ou Smartphone. Esta interface deve ser capaz de apresentar as medições coletadas e as configurações para funcionamento da estação de medição; :white_check_mark: 
- O sistema deve apresentar informações sobre a conexão com a estação de
medição na IHM local e remotamente. :white_check_mark: 

## Sobre MQTT
O padrão de troca de mensagens no MQTT é o publish/subscriber (publicador/subscritor). Neste padrão, quando um elemento da rede deseja receber uma determinada informação, ele a subscreve, fazendo uma requisição para um outro elemento da rede capaz de gerir as publicações e subscrições. 
Na rede MQTT este elemento é conhecido como broker, o intermediário no processo de comunicação. Elementos que desejam publicar informações o fazem também através do broker, enviando-lhe as informações que possuem.
Apesar de o broker representar um elo de fragilidade na rede ao centralizar as comunicações, ele permite um desacoplamento entre as partes comunicantes, algo não possível em modelos de comunicação do tipo cliente/servidor.
A identificação das mensagens no MQTT se dá através de tópicos (topics). O tópico lembra o conceito de URI, com níveis separados por barras (“/”). Elementos da rede podem enviar diversos tópicos para o broker e subscritores podem escolher os tópicos que desejam subscrever.
Os nossos tópicos configurados são G02THEBESTGROUP/MEDICOES que efetuam a transição de medições da raspberry pra view e  G02THEBESTGROUP/INTERVALO que faz a comunicação da mudança de intervalo pra raspberry.

## Diagrama do sistema
![diagrama](/telas/diagrama.png)

### Parte Raspberry
![diagrama](/telas/Diagrama_medicoesLCD.png)
Responsável por coletar medidas, mostrar medidas no lcd, enviar medidas para o broker, e receber mudanças de intervalo da Parte Remota e pelos botões acoplados a raspberry.

Para ler do DHT11, o programa utiliza os métodos digitalWrite() e digitalRead() da biblioteca wiringPi. Para ler medidas dos potenciômetros, são utilizados os métodos read() e write() da biblioteca i2c-dev.

Os valores medidos são guardados em 2 matrizes. Uma para o DHT11 e outra para os potenciômetros. A matriz do DHT11 guarda 4 linhas: 2 linhas para humidade e 2 para temperatura. Cada medida precisa de 2 linhas pois o DHT11 envia uma parte inteira e outra de ponto flutuante em suas medidas. Na matriz dos potenciômetros só existem 2 linhas.

Todas essas medidas e inserções ocorrem em uma thread separada do laço principal do programa, chamada de measure_thread.

Para mostrar mensagens no lcd, foi utilizado o método lcdPrintf() da wiringPi. As mensagens mostradas são controladas belos botões e dip switches na raspberry, utilizando digitalRead() para coletar as entradas do usuário no laço principal do programa e alterar as informações no lcd de acordo.

Para receber e mandar mensagens no broker, antes da inicialização do programa, são feitas duas conexões com o broker, para o subscriber e publisher da raspberry. Quando a conexão subscriber é feita, o mesmo é conectado a um método on_message() utilizando o mosquitto_message_callback_set() da biblioteca mosquitto. Assim, toda vez que o subscriber recebe uma mensagem do broker, esta mensagem é processada no método on_message().

Para o publisher, depois que uma conexão é feita, são utilizados os métodos mosquitto_reconnect(),  mosquitto_publish(), e mosquitto_disconnect() para conectar ao broker, mandar as medidas, e desconectar, respectivamente. Estas publicações ocorrem dentro de measure_thread.

### Parte Broker
O Broker (intermediário) faz jus a sua tradução, é o intermedário entre os publishers (publicadores) e subscribers (inscritos) por meio de canais (ou tópicos). Utilizamos o do próprio laboratório (10.0.0.101:1883), mas é possível utilizar qualquer um que seja disponível. No nosso caso, utilizamos dois canais:
 - **Canal 1 (Medições):** Por onde comunicamos as medições, onde o publisher será o que está contido na Raspberry e o subscriber será o que está contido no servidor. Fazemos a comunicação seguindo o template abaixo:
```sh
key {"H": "0.0", "T": "0.0", "P": "0.0", "L": "0.0", "datetime": "YYYY-mm-dd H:i:s", "interval": "2"}
```
    - key: nossa chave pessoal, para sabermos que se trata de uma requisição enviada pelo nosso sistema: g02pb3EGK (Grupo02_problema3_EsdrasGuilhermeKevin);
    - H: Humidade;
    - T: Temperatura;
    - P: Pressão;
    - L: Luminosidade;
    - datetime: Data e hora da medição;
    - interval: frequência do intervalo atual quando a medição foi tirada.
  - **Canal 2 (Intervalo):** Por onde comunicamos a frequência do intervalo, onde o publisher será o servidor e o subscriber será a raspberry. Seguindo o seguinte template:
```sh
key {"interval":2}
```  
    - interval: Valor do intervalo para ser alterado na raspberry.
    
### Parte Remota
#### Subscriber
Responsável por receber todos os dados que são publicados no CANAL 1 (Medições), trata-los e armazena-los no Banco de Dados.
#### Banco de Dados
Faz a gestão do banco de dados (src/server/database_controller.py), insere, deleta e retorna as medições do sistema no banco de dados (src/server/database/measures.json). Recebe as medições do Subscriber, da parte remota, insere no banco e retorna as medições para o Servidor quando requisitado.
#### Servidor
Responsável por tratar as requisições vindas da view e envia-las para o publisher (src/server/publisher.py) para realizar a publicação, e também responsável por consultar o banco de dados para retornar as medições para view quando a mesma pedir.
#### Back-End (View)
Responsável por configurar as requisições para as chamadas ao Servidor (src/web/controllers/Controller.php). Trata os dados que são recebidos do Servidor e envia para o Front-End (src/web/controllers/measures.php), e recebe os dados do Front-End e trata para enviar para o Servidor (src/web/controllers/interval.php).
#### Front-End (View)
Exibe todos os dados das medições em uma lista com as 10 últimas medições e recebe via Modal as frequências dos intervalos do usuário e envia para o Back-End.

# Pré-requisitos
## Parte Raspberry

O programa precisa das bibliotecas mosquitto, wiringPi, e i2c-dev (já contidas na Raspberry PI Zero do LEDs).

### Utilizando a IHM da Raspberry Pi

Foram utilizados 2 push buttons e uma dip switch como fontes de entrada nesse projeto. O interruptor 3 da dip switch muda as medidas que são mostradas no LCD entre medidas de Temperatura/Humidade e Luminosidade/Pressão. O interruptor 4 troca o modo de mostragem da IHM para mostrar o intervalo de medida atual, ou mostrar as medidas.

Enquanto no modo de medida, os botões podem ser utilizados para mostrar os resultados de medidas mais recentes ou antigas. No modo de intervalo, os botões incrementam ou diminuem o intervalo de medida em 1 segundo.

## Parte Remota
As instalações dos pré-requisitos diferem de sistema para sistema, então disponibilizamos os requisitos juntamente com seus respectivos sites com instruções para instalar/utilizar.
- **Python 3**:
  - Instalação no Windows: https://python.org.br/instalacao-windows/
  - Instalação no Linux: https://python.org.br/instalacao-linux/
  - Instalação no MAC: https://python.org.br/instalacao-mac/
- **Docker**:
  - Instalação: https://docs.docker.com/get-docker/
    - A instalação no windowns é um pouco mais 'chata', então é sempre bom rever se está tudo OK com a instalação, principlamente se tratando do WSL2 (sempre bom revisar com algum tutorial em video!).
- **PHP 7**
  - Utilizamos o Docker, pois é mais fácil do que instalar o próprio PHP (seja qual for o sistema) então basta ter o Docker instalado.

# Startando o projeto  
## Parte da Raspberry
### Comandos para serem executados na raspberry
Necessário rodar os seguintes comandos na raspberry do LEDs, laboratório de Hardware da UEFS, ou em qualquer Raspberry PI Zero com a mesma configuração e sensores. Lembre-se de transferir os seguintes arquivos da pasta "src/raspberry" para a Raspberry Pi Zero: "main_final.c", "constants.h" e "read_voltage.c". Certifique-se que todos os arquivos estão na mesma pasta.
#### Transferir arquivos para raspberry:
```sh
scp arquivo.c pi@[endereço da raspberry]:[filepath desejado]
```
#### Compilar código na raspberry:
```sh
gcc main_final.c read_voltage.c -o executable  -lwiringPi -lwiringPiDev -lm -lpthread -lmosquitto
```
#### Executar código na raspberry:
```sh
sudo ./executable
```
## Parte Remota
### Comandos para serem executadas no computador de quem iniciará a parte remota
É importante lembrar que **é necessário deixar os comandos abaixo rodando em terminais separados** para que o sistema funcione completamente. Ou seja, um terminal rodando a view, um a API e outro o Subscriber. Assim, **executar todos os comandos a seguir na pasta raiz do repositório**:

#### Rodar o container da parte em PHP:
```sh
docker-compose up
```
### Instalar as dependências da parte do python:
```sh
pip install -r src/server/requirements.txt
```
#### Rodar a API (Servidor) que faz a intercomunicação entre o Banco de Dados e o Back-End:

- Se tiver no Linux
```sh
python3 src/server/api.py
```
- Se tiver no Windows
```sh
python src/server/api.py
```
#### Rodar o Subscriber que recebe as informações enviadas da Raspberry pelo Broker
- Se tiver no Linux
```sh
python3 src/server/subscriber.py
```
- Se tiver no Windows
```sh
python src/server/subscriber.py
```
## ATENÇÃO! Talvez, a depender do seu sistema, será necessário alterar a linha 25 do arquivo src/web/controllers/Controller.php, como mostra na figura abaixo, altere o trecho de código "$_SERVER['REMOTE_ADDR']" para o seu IPv4, por exemplo "10.0.0.109", como é mostrado na linha 24:
![ipv4](/telas/ipv4.png)

## E pronto! Após isso tudo, basta acessar em qualquer navegar o link http://localhost/
## Principais telas do sistema

### - Tela de Inicio (Dashboard)
![inicio](/telas/inicio.png)

  - Descrição
    ![inicio_descrito](/telas/incio_descrito.png)
    1. Label Refresh #0: Número de vezes que o sistema fez requisição a API para verificar se houve atualização. E assim atualizar os dados com as últimas medições.
    2. Botão Intervalo 0s: Botão que abre modal para edição do intervalo.
    3. Lista com as medições dos senções.

### - Modal de edição de intervalo.
![intervalo](/telas/intervalo.png)

### - Intervalo editado com sucesso.
![intervalo_sucesso](/telas/intervalo_sucesso.png)

### - Intervalo não editado (retornado erro). 
![intervalo_erro](/telas/intervalo_erro.png)

### - LOG do sistema de Subscriber e Publisher do servidor.
![log](/telas/log.png)
