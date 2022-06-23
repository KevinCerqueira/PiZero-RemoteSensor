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
import util
import paho.mqtt.client as mqtt

class Publisher:
	
	HOST = ''
	PORT = 0
	
	mqtt_server = None
	
	topic = ''
	
	# Controlador da base de dados
	database = None
	
	queue_request = None
	thread_request = None
	
	controllevels = None
	
	close = False
	
	app = None
	
	def __init__(self):
		# Iniciando o Server
		self.HOST = 'broker.emqx.io' # util.env('BROKER_HOST')
		self.PORT = int(util.env('BROKER_PORT'))
		
		self.mqtt_server = mqtt.Client(util.env('CLIENTID_PUB'))
		self.mqtt_server.username_pw_set(util.env('BROKER_USER'), util.env('BROKER_PASS'))
		self.mqtt_server.on_publish = self.on_publish
		self.mqtt_server.connect(self.HOST, self.PORT)
		
		self.topic = "G02_THEBESTGROUP/INTERVALO" # util.env('TOPICO_INTERVALO')
		
		self.log('SERVER READY')

	
	# Quando houver uma publicação
	def on_publish(self, client, userdata, result):
		self.log('Data published: {}'.format(result))
	
	# Função principal, onde o servidor irá receber as conexões
	def send(self, data):
		self.log('Sending data to broker: {}'.format(data))
		return self.mqtt_server.publish(self.topic, data)
	
	# Log do sistema
	def log(self, msg):
		util.log('PUB', msg)

if __name__ == '__main__':
	server = Publisher()
	server.send('g02p3EGK {"H": "555", "T": "100", "P": "100", "L": "100"}')
			