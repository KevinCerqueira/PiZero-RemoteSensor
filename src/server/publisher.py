"""
 * Componente Curricular: MI Concorrência e Conectividade
 * Autor: 
 *
 * Declaro que este código foi elaborado por mim de forma individual e
 * não contém nenhum trecho de código de outro colega ou de outro autor,
 * tais como provindos de livros e apostilas, e páginas ou documentos
 * eletrônicos da Internet. Qualquer trecho de código de outra autoria que
 * uma citação para o  não a minha está destacado com  autor e a fonte do
 * código, e estou ciente que estes trechos não serão considerados para fins
 * de avaliação. Alguns trechos do código podem coincidir com de outros
 * colegas pois estes foram discutidos em sessões tutorias.
"""
from cmath import log
import paho.mqtt.client as mqtt
from datetime import datetime
from datetime import date
import os

class Publisher:

	HOST = '10.0.0.101'
	PORT = 1883
	
	# Servidor
	server_socket = None
	
	mqtt_server = None
	
	topic = ''
	
	# Controlador da base de dados
	database = None
	
	queue_request = None
	thread_request = None
	
	controllevels = None
	
	close = False
	
	log_directory = ''
	
	def __init__(self):
		# Iniciando o Server
		self.mqtt_server = mqtt.Client('G02_THEBESTGROUP_PUB')
		self.mqtt_server.username_pw_set("aluno", "aluno*123")
		self.mqtt_server.on_publish = self.on_publish
		self.mqtt_server.connect(self.HOST, self.PORT)
		self.topic = 'G02_THEBESTGROUP/INTERVALO'
		self.log_directory = os.path.dirname(os.path.realpath(__file__)) + '/logs/log_' + str(date.today()) + '.log'
		
		print('SERVER ON\n')

	def on_publish(self, client, userdata, result):
		self.log('Data published: {}'.format(result))
	
	# Função principal, onde o servidor irá receber as conexões
	def send(self, data):
		self.log('Sendind data to broker: {}'.format(data))
		return self.mqtt_server.publish(self.topic, data)
		
	def log(self, msg):
		with open(self.log_directory, 'a', encoding='utf-8') as log_file:
			log_file.write("PUBLISHER [" + str(datetime.now()) +"] "+ msg + '\n')
		return True

if __name__ == '__main__':
	server = Publisher()
	server.send('g02p3EGK {"H": "555", "T": "100", "P": "100", "L": "100"}')
			