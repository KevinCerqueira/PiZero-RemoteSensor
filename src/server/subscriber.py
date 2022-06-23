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
import paho.mqtt.client as mqtt
import json
import sys
import os
import threading
import base64
import re
import util
from collections import deque
from database_control import DatabaseControl
from datetime import datetime
from datetime import date

class Subscriber:
	
	HOST = ''
	PORT = 0
	mqtt_server = None
	
	topic = ''
	
	# Controlador da base de dados
	database = None
	
	queue_request = None
	thread_request = None
	
	close = False
	
	return_codes = ["Connection successful", "Connection refused – incorrect protocol version", "Connection refused – invalid client identifier", "Connection refused – server unavailable", "Connection refused – bad username or password", "Connection refused – not authorised"]
	
	def __init__(self):
		
		# Iniciando o Server
		self.HOST = 'broker.emqx.io' # util.env('BROKER_HOST')
		self.PORT = int(util.env('BROKER_PORT'))
		
		self.mqtt_server = mqtt.Client(util.env('CLIENTID_SUB'))
		self.mqtt_server.username_pw_set(util.env('BROKER_USER'), util.env('BROKER_PASS'))
		self.mqtt_server.connect(self.HOST, self.PORT)
		
		self.topic = "G02_THEBESTGROUP/MEDICOES" # util.env('TOPICO_MEDICOES')
		
		self.queue_request = deque()
		self.thread_request = threading.Thread(target=self.queue)
		self.thread_request.start()
		
		self.db_control = DatabaseControl()
		
		self.log('SERVER READY')
		
		self.work()
	
	def on_connect(self, client, userdata, flags, rc):
		if(rc == 0):
			self.mqtt_server.subscribe(self.topic)
			self.log("Connected to a broker!")
		else:
			if(rc < 6):
				self.log("Bad Connection! Returned code: " + str(self.return_codes[int(rc)]))
			else:
				self.log("Bad Connection! Returned undefined code: " + str(rc))
				
		
	def on_disconnect(self, client, userdata, flags):
		self.mqtt_server.subscribe(self.topic)
		self.log("Disconnected to a broker!")

	def on_message(self, client, userdata, message):
		self.log("[FROM "+self.HOST+":"+str(self.PORT)+"]: " + str(message.payload.decode()))
		self.receptor(message.payload.decode())
		
	def log(self, msg):
		util.log('SUB', msg)

	# Função principal, onde o servidor irá receber as conexões
	def work(self):
		# while not self.close:
		self.mqtt_server.on_connect = self.on_connect
		self.mqtt_server.on_disconnect = self.on_disconnect
		self.mqtt_server.on_message = self.on_message
		self.mqtt_server.loop_forever()
		
		if(self.close and len(self.queue_request) == 0):
			self.log('SERVER OFF')
			self.mqtt_server.disconnect()
			return sys.exit()
	
	# Trata os dados recebidos
	def receptor(self, request):
		data = None
		
		# Verificando se é realmente o cliente
		if(util.env('MQTT_PERSONAL_KEY') not in request):
			self.log('Conexão externa.')
			return
		
		# Recebendo requisição para fechar o servidor
		if('close' in request):
			self.log('Fechando o servidor.')
			self.close = True
			sys.exit()
		
		# Buscando pelo json contendo os dados na requisição
		for index in request:
			if(index == '{'):
				data = json.loads(request[request.find('{') : request.find('}') + 1])
		
		# Adicionando a requisição a fila de requisições
		if(index != None):
			self.queue_request.append({'data': data})
		else:
			self.data_not_found()
	
	# Consome a fila de requisições
	def queue(self):
		while True:
			if(len(self.queue_request) > 0):
				self.log('Connections: ' + str(len(self.queue_request)))
				request = self.queue_request.popleft()
				self.routing(request['data'])
	
	# Função responsável pelo roteamente, identifica os metodos e as rotas requisitadas
	def routing(self, data):
		if('H' not in data or 'T' not in data or 'P' not in data or 'L' not in data or 'datetime' not in data):
			return self.data_not_found()
		return self.insert_measure(data)
	
	# Caso a rota informada não esteja dentre as disponiveis
	def data_not_found(self):
		self.log('Dados da requisicao incorretos.')

	# Atualiza todos os atributos de um paciente
	def insert_measure(self, data):
		success = self.db_control.insert_measure(data)
		if(success == False):
			self.log('Nao foi possivel inserir a medicao.')
		return True

if __name__ == '__main__':
	server = Subscriber()
	server.work()
			