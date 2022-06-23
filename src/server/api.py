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

from flask import Flask
from flask import jsonify
from flask import request
from database_control import DatabaseControl
from publisher import Publisher

database = DatabaseControl()
publisher = Publisher()
app = Flask(__name__)

# Retorna as ultimas 10 medidas
@app.route("/measures")
def measures():
    last10 = database.get_last_qntd(10)
    return jsonify(last10)

# Recebe o intervalo de tempo via POST
@app.route("/interval/<interval>", methods=['GET', 'POST'])
def interval(interval):
    response = publisher.send("g02pb3EGK {'I':" + str(interval) + "}")
    return jsonify(str(response))

if __name__ == "__main__":
    app.run(host="0.0.0.0")