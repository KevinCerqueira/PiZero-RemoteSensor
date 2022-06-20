import paho.mqtt.client as paho
broker='10.0.0.101'
port=1883

def on_message(self, client, userdata, message):
	print(message.payload.decode())
client1= paho.Client("G02_THEBESTGROUP")                           #create client object
client1.on_message = on_message               
client1.username_pw_set("aluno", "aluno*123") 
client1.subscribe('G02_THEBESTGROUP/MEDICOES')
client1.connect(broker,port)                                 #establish connection              #assign function to callbac           #assign function to callback
client1.loop_forever()                                 #establish connection
# ret= client1.publish("G02_THEBESTGROUP/MEDICOES","onnnnnnnnnnn")                   #publish