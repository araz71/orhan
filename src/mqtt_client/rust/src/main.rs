use rumqttc::{MqttOptions, AsyncClient, QoS, Event, Incoming, Publish};
use std::time::Duration;
use std::string::String;
use tokio::io::Result;
use bytes::Bytes;
use diesel::prelude::*;
use diesel::mysql::MysqlConnection;
use mysql::Row;

mod schema {
    diesel::table! {
        registers(rid) {
            rid -> Integer,
            sn -> Text,
            type_id -> Text,
            value -> Text,
            label -> Text,
            last_updated -> Text,
            history -> Text,
        }
    }
}

use schema::registers;

#[derive(Queryable, Selectable)]
#[diesel(table_name = registers)]
pub struct Register<'a> {
    rid: i32,
    sn: &'a String,
    type_id: &'a String,
    value: &'a String,
    label: &'a String,
    last_updated: &'a String,
    history: &'a String,
}

impl Register<'_>
{
    fn from_row<'a>(row: &'a Row) -> Register<'a> {
        Register{
            rid: row.get::<i32, &str>("rid").unwrap(),
            sn: &String::from(row.get::<String, &str>("sn").unwrap()),
            type_id: todo!(),
            value: todo!(),
            label: todo!(),
            last_updated: todo!(),
            history: todo!(),
        }
    }
}

fn on_registers(data: Bytes) {
    // Message should be like SN,RegID,DATA
    if data.is_ascii() {
        let command : String = String::from_utf8(data.to_vec()).unwrap();
        let parts: Vec<&str> = command.split(",").collect();
        if parts.len() == 3 {
            let sn = parts[0];
            let regID = parts[1];
            let data = parts[2];

            if sn.len() == 10 {
                // Store register value if device found

            } else {
                println!("Incorrect serial number!");
            }
        } else {
            println!("Unknown message\r\n");
        }
    }
}

fn on_signup(data: Bytes) {

}

fn handle_packet(msg: Publish) {
    let data = msg.payload;
    match msg.topic.as_str() {
        "registers" => on_registers(data),
        "signup" => on_signup(data),
        _ => (),
    }
}

#[tokio::main(flavor = "current_thread")]
async fn main() -> Result<()> {
    println!("MQTT Client v1.0\r\n");

    let mut mqtt_options = MqttOptions::new("collector", "localhost", 1883);
    mqtt_options.set_keep_alive(Duration::from_secs(10));
    let (client, mut eventloop) = AsyncClient::new(mqtt_options, 1000);
    client.subscribe("registers", QoS::AtMostOnce).await.unwrap();

    let connection = &mut MysqlConnection::establish("mysql://root:12345@localhost/orhandb")
        .unwrap_or_else(|_| panic!("Could not connect to server."));

    let dev_registers: Vec<Register> = registers::table.load(connection).unwrap();
    for register in dev_registers {
        println!("RegID {}", register.rid);
    }

    while let Ok(notif) = eventloop.poll().await {
        // This means if Event is Incomming and command is Publish, then handle message
        match notif {
            Event::Incoming(p) => match p {
                Incoming::Publish(msg) => handle_packet(msg),
                _ => (),
            },
            _ => (),
        };
    };

    Ok(())
}
