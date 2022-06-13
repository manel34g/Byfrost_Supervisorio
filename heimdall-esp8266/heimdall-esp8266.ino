#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Luiz";
const char* password = "31122201";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

//Esp8266
//sttArd(); Função que verifica as mensagens recebidas do Arduino UNO

#define MSG_BUFFER_SIZE  (50)

char msg[MSG_BUFFER_SIZE];
String Ord = "";

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.write(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; i < length; i++) {
    Ord.concat((char)payload[i]);
  }
  if (Ord.indexOf("add") == 0) {
    String str = "received_" + Ord.substring(4, 7);
    char msg[50];
    str.toCharArray(msg, 50);
    Serial.println(Ord.substring(0, 7));
    client.publish("byf/hem", msg);
  }
  Ord = "";
}

void reconnect() {

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);


    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("byf/hem");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");


      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    client.publish("byf/hem/stt", "on");
  }
  char c;
  String msg;
  if (Serial.available() != 0) {
    while (Serial.available() != 0) {
      c = Serial.read();
      msg.concat(c);
    }
    if (msg == "e") {
      Serial.println("foi");
      client.publish("byf/hem", "end");
      }
    if (msg == "p") {
      Serial.println("preparado");
      client.publish("byf/hem", "prepared");
      }
    Serial.println(msg);
    msg = "";
  }
}
String sttArd () {
  char c;
  String msg;
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      c = Serial.read();
      msg.concat(c);
    }
    return msg;
    Serial.println(msg);
    msg = "";
  }

}
