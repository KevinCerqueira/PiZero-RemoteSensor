import paho.mqtt.client as paho
broker='10.0.0.101'
port=1883
def on_connect(self, client, userdata, flags, rc):
		if(rc == 0):
			print("Connected to a broker!")
		else:
			print("Bad Connection! Returned code: " + str(rc))
def on_publish(client,userdata,result):    
    print(result)         #create function for callback
    print("data published \n")
    
client1= paho.Client("G02_THEBESTGROUP")                           #create client object
client1.on_connect = on_connect           
client1.on_publish = on_publish           
client1.username_pw_set("aluno", "aluno*123")               #assign function to callback
client1.connect(broker,port)                                 #establish connection
ret= client1.publish("G02_THEBESTGROUP/MEDICOES","onnnnnnnnnnn")                   #publish