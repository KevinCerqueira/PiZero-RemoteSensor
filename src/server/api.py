import json
from flask import Flask
from flask import jsonify
from flask import request
from database_control import DatabaseControl
from publisher import Publisher

db = DatabaseControl()
pub = Publisher()
app = Flask(__name__)

@app.route("/measures")
def measures():
    last10 = db.get_last_qntd(10)
    return jsonify(last10)

@app.route("/interval/<interval>", methods=['GET', 'POST'])
def interval(interval):
    response = pub.send("g02pb3EGK {'I':" + str(interval) + "}")
    return jsonify(str(response))

if __name__ == "__main__":
    app.run(host="0.0.0.0")