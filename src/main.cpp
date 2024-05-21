#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif 

#include <PubSubClient.h>

#define LedPin      33                           // ESP32 led pin
#define VibPin      14                           // ESP32 led pin

const char* ssid = "WIFI-SSID"; // anpassen auf WLAN
const char* password = "1234"; // anpassen auf WLAN

const char* mqtt_server = "test.mosquitto.org"; // Mosquitto Server URL

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi()
{ 
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED) 
    { 
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{ 
    char msg = 0;
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");

    for(int i = 0 ; i < length; i++){ msg = (char)payload[i]; }
    Serial.println(msg);
    
    if('1' == msg){ 
      digitalWrite(LedPin, HIGH); 
      digitalWrite(VibPin, HIGH); 
      }
    else if('2' == msg){ 
      digitalWrite(LedPin, LOW); 
      digitalWrite(VibPin, LOW); 
      }
}

void reconnect() 
{ 
  while(!client.connected()) 
  {
      Serial.println("Attempting MQTT connection...");

      if(client.connect("ESPClient")) 
      {
          Serial.println("Connected");
          client.subscribe("/LedControl");
      } 
      else 
      {
          Serial.print("Failed, rc=");
          Serial.print(client.state());
          Serial.println(" try again in 5 seconds");
          delay(5000);
      }
    }
}

void setup()
{    
    Serial.begin(115200);
    pinMode(LedPin, OUTPUT);
    pinMode(VibPin, OUTPUT);
    setup_wifi(); 
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}
void loop()
{
    if(!client.connected()) { reconnect(); }
    client.loop();
}
