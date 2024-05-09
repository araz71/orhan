#!/usr/bin/python3
import time
import sys

import paho.mqtt.client as mqtt

sys.path.insert(1, "../../cli/")
from db import Database
from cli import perror, psucc

db = Database()

# Set up the MQTT broker connection
broker = 'localhost'
port = 1883
topic = "registers"
client_id = "RegisterReader"

def mypsucc(msg: str):
    psucc(msg, False)

def myperror(msg: str):
    perror(msg, False)

def on_connect(client, user_data, flag, rc):
    if rc == 0:
        mypsucc("Connected.")
    else:
        myperror("Can not connect to broker.")
        exit(-1);

def on_message(client, userdata, msg):
    if msg.topic == "registers":
        parts = msg.payload.decode().split(",")
        if len(parts) == 3:
            sn = parts[0]
            regID = parts[1]
            data = parts[2]

            if len(sn) != 10:
                myperror("Serial number is incorrect.")
            else:
                # Search device in database
                device = db.list_devices(sn)
                if len(device) == 0:
                    myperror(f"Device[{sn}] not found.")
                else:
                    if db.get_register(sn, int(regID)) != None:
                        db.set_register(sn, int(regID), data)
                        mypsucc(f"Reg[{regID}] on Device[{sn}] Updated.",)
                    else:
                        myperror(f"Reg[{regID}] on Device[{sn}] NOT EXISTS.")
        else:
            myperror("Unknown message")

print("Starting Mqtt Client v0.1")
client = mqtt.Client(client_id=client_id)

client.on_connect = on_connect
client.on_message = on_message

client.connect(broker, port)
client.subscribe(topic)

client.loop_start()

while True:
    time.sleep(1)
    pass

client.loop_stop()