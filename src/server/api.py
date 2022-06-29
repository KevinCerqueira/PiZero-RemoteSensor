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

from flask import Flask, render_template
from flask import jsonify
from flask import request
from database_control import DatabaseControl
from publisher import Publisher
import paho.mqtt.client as mqtt


database = DatabaseControl()
pub = Publisher()

app = Flask(__name__, template_folder='./')

@app.route("/")
def home():
    return render_template('home.html')

# Retorna as ultimas 10 medidas
@app.route("/measures")
def measures():
    last10 = database.get_last_qntd(10)
    interval = database.get_last_interval()
    return jsonify({'interval': interval, 'measures': last10})

# Recebe o intervalo de tempo via POST
@app.route("/interval/<interval>", methods=['GET', 'POST'])
def interval(interval):
    response = pub.send("g02pb3EGK {'interval':" + str(interval) + "}")
    # response = publish("g02pb3EGK {'interval':" + str(interval) + "}")
    return jsonify(str(response))

# def publish(data):
#     mqtt_server = mqtt.Client("G02_THEBESTGROUP_PUB")
#     mqtt_server.username_pw_set("aluno", "aluno*123")
#     mqtt_server.on_publish = on_publish
#     mqtt_server.connect("10.0.0.101", 1883)
    
#     topic = 'G02_THEBESTGROUP/INTERVALO'
    
#     return  mqtt_server.publish(topic, data)

	
# # Quando houver uma publicação
# def on_publish(client, userdata, result):
#     print('Data published: {}'.format(result))

if __name__ == "__main__":
    app.run(host="0.0.0.0")