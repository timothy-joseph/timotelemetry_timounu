from flask import Flask, render_template, Response, stream_with_context
from flask_mqtt import Mqtt
from flask_socketio import SocketIO, emit
from datetime import datetime
import json
import time
import config

from mqtt_packet_to_json import decoded_mqtt_packet

app = Flask(__name__);
app.config['MQTT_BROKER_URL'] = config.mqtt_broker;
app.config['MQTT_BROKER_PORT'] = config.mqtt_port;
app.config['MQTT_USERNAME'] = config.mqtt_username;
app.config['MQTT_PASSWORD'] = config.mqtt_password;
app.config['MQTT_REFRESH_TIME'] = 1.0; # refresh time in seconds
mqtt = Mqtt(app)
socketio = SocketIO(app);

# mqtt callbacks
@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    print("subscribing");
    mqtt.subscribe(config.mqtt_topic)

@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
    data = {
        "topic": message.topic,
        "payload": message.payload.decode("ascii")
    };
    print(data);

    decoded_packet = decoded_mqtt_packet(data["payload"]);
    print(decoded_packet.to_json());

    socketio.emit("mqtt_data", decoded_packet.to_json());

# http callbacks
@app.route("/")
def index():
    return render_template('index.html');

if __name__ == "__main__":
    socketio.run(app, host = "0.0.0.0", port = config.app_port, debug = True,
             use_reloader = False);

