#include <ESP8266WiFi.h>

#include <PubSubClient.h>

#include <OneWire.h>

#include <DallasTemperature.h>



OneWire barramento(D4);
DallasTemperature sensor(&barramento);

#include <ESP8266WiFi.h>
extern "C" {
#include <espnow.h>
#include <user_interface.h>

//
const char* ssid = "";
const char* password = "";

//const char* ssid = "mandark";
//const char* password = "#lasa2010";
const char* mqtt_server = "mqtt.mqtzao.com";

//char switchState = 0;
//char lastSwitchState = 1;

int switchState = 0;
int lastSwitchState = 1;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
float temp = 0;

String totalmsg_temp;
String totalmsg_door;

//int inputPin = 13; // porta infrared (D7)

//char inputPin = 13;
int inputPin = 13;
int val = 0; // pin status door

void setup_wifi() {
  delay(10);
  // Inciando a conexao wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT ...");
    // Attempt to connect
    if (client.connect("arduinoClient_temperature_sensor")) {
      Serial.println("conectado");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" Reconectar em 5 segundos");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  sensor.begin();
  pinMode(inputPin, INPUT);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
   
  sensor.requestTemperatures();
  //Serial.println(sensor.getTempCByIndex(0));
  temp = sensor.getTempCByIndex(0);
  // client.publish("test/temperature", String(temp).c_str(),1);
  val = digitalRead(inputPin); //

  //Serial.println(WiFi.softAPmacAddress());
  totalmsg_temp = "{" + String ("\"") + "Temperatura" + String ("\"") + ": " + String(sensor.getTempCByIndex(0)) + " , " + String ("\"") + "status_door" + String ("\"") + ": " + String(digitalRead(inputPin)) + " , " + String ("\"") + "id_board" + String ("\"") + ": " + String(ESP.getChipId()) + "}" ;
  client.publish("test/temperature", String(totalmsg_temp).c_str(),1);

  switchState = digitalRead(inputPin);
 
   if (switchState != lastSwitchState) {  
    if (switchState == 1) {
        delay(250);
        Serial.print("Porta aberta");
        totalmsg_door = "{" + String ("\"") + "status_door" + String ("\"") + ": " + String(switchState) + " , " + String ("\"") + "id_board" + String ("\"") + ": " + String(ESP.getChipId()) + "}" ;
        client.publish("test/status_door", String(totalmsg_door).c_str(),1);
        lastSwitchState = switchState;
       
    } else if (switchState == 0) {
        delay(250);
        Serial.print("Porta fechada");
        totalmsg_door = "{" + String ("\"") + "status_door" + String ("\"") + ": " + String(switchState) + " , " + String ("\"") + "id_board" + String ("\"") + ": " + String(ESP.getChipId()) + "}" ;
        client.publish("test/status_door", String(totalmsg_door).c_str(),1);
        lastSwitchState = switchState;
       
    }
  } else {
     
  }


 
 // Serial.println(val);
  //Serial.println(temp);
 // Serial.println(totalmsg_temp);
  //Serial.println(totalmsg_door);
  }
} }
