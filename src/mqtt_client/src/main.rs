use rumqttc::{MqttOptions, AsyncClient, QoS, Event, Incoming};
use std::time::Duration;
use tokio::io::Result;
use std::io::Write;

#[tokio::main(flavor = "current_thread")]
async fn main() -> Result<()> {
    println!("MQTT Client v1.0\r\n");

    let mut mqtt_options = MqttOptions::new("collector", "localhost", 1883);
    mqtt_options.set_keep_alive(Duration::from_secs(10));
    let (mut client, mut eventloop) = AsyncClient::new(mqtt_options, 1000);
    client.subscribe("registers", QoS::AtMostOnce).await.unwrap();

    loop {
        while let Ok(notif) = eventloop.poll().await {
            let msg = match notif {
                Event::Incoming(msg) => Some(msg),
                _ => None,
            };

            let packet = match msg {
                Some(v) => {
                    match v {
                        Incoming::Publish(v) => Some(v),
                        _ => None
                    }
                },
                None => None
            };

            if packet.is_some() {
                println!("{:?}", packet.unwrap().payload);
            }
        }
    }
}
