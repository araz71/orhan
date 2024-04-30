import paho.mqtt.client as mqtt
import sys

sys.path.insert(1, "../../cli/")
from db import Database

db = Database()

# Set up the MQTT broker connection
broker = 'localhost'
port = 1883
topic = "registers"
client_id = "RegisterReader"

def on_connect(client, user_data, flag, rc):
    if rc == 0:
        print("Connected sucessfully.")
    else:
        print("Can not connect to broker.")
        exit(-1);

def on_message(client, userdata, msg):
    if msg.topic == "registers":
        parts = msg.payload.decode().split(",")
        if len(parts) == 3:
            sn = parts[0]
            regID = parts[1]
            data = parts[2]

            if len(sn) != 10:
                print("Serial number is incorrect.")
            else:
                # Search device in database
                print("Search device...")
                device = db.list_devices(sn)
                if len(device) == 0:
                    print("Device not found")
                else:
                    db.set_register(sn, int(regID), data)
                    print(f"Reg[{regID}] on Device[{sn}] Updated successfully.")

        else:
            print("Unknown message")

client = mqtt.Client(client_id=client_id)

client.on_connect = on_connect
client.on_message = on_message

client.connect(broker, port)
client.subscribe(topic)

client.loop_start()

while True:
    pass

client.loop_stop()