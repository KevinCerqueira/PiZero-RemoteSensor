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

class Subscriber:

	HOST = 'broker.emqx.io'
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
	
	def __init__(self):
		# Iniciando o Server
		self.mqtt_server = mqtt.Client('G02_THEBESTGROUP')
		self.mqtt_server.on_publish = self.on_publish
		self.mqtt_server.connect(self.HOST, self.PORT)
		self.topic = 'G02_THEBESTGROUP/INTERVALO'
		print('SERVER ON\n')

	def on_publish(self, client, userdata, result):
		print('Data published: {}'.format(result))
	
	# Função principal, onde o servidor irá receber as conexões
	def send(self, data):
		return self.mqtt_server.publish(data)

if __name__ == '__main__':
	server = Subscriber()
	server.send('teste123')
			