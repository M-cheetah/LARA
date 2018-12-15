#include <PubSubClient.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <ArduinoJson.h>
#include "SoftwareSerial.h"

#define ORG "*********"
#define DEVICE_TYPE "*********"
#define DEVICE_ID "*********"
#define TOKEN "**********"
#define WIFI_AP "*********"
#define WIFI_PASSWORD "**********"

WiFiEspClient espClient;

SoftwareSerial soft(10, 11); // RX-Rx, TX-Tx
int status = WL_IDLE_STATUS;

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

const char publishTopic[] = "iot-2/evt/status/fmt/json";
const char responseTopic[] = "iotdm-1/response";
const char manageTopic[] = "iotdevice-1/mgmt/manage";
const char updateTopic[] = "iotdm-1/device/update";
const char rebootTopic[] = "iotdm-1/mgmt/initiate/device/reboot";

void callback(char* publishTopic, char* payload, unsigned int payloadLength);

PubSubClient client(server, 1883, callback, espClient);

int publishInterval = 30000; // 30 seconds
long lastPublishMillis;
int trigPin = 6;
int echoPin = 5;
long duration, distance;
int period = 5000;
unsigned long time_now = 0;
            
void setup() {
        Serial.begin(115200);
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
        InitWiFi();
        Serial.print(WiFi.localIP());
        if (!!!client.connected()) {
                Serial.print("Reconnecting client to ");
                Serial.println(server);
                while (!!!client.connect(clientId, authMethod, token)) {
                Serial.print(".");
        }
        Serial.println();
        }

}


void loop() {
                 
            digitalWrite(trigPin,HIGH);
            delayMicroseconds(1000);
            digitalWrite(trigPin, LOW);
            duration=pulseIn(echoPin, HIGH);
            distance =(duration/2)/29.1;
            
          String payload = "{\"d\":{ \"level\" :";
          payload += distance;
          payload += "} }";

          Serial.print("Sending payload: ");
          Serial.println(payload);

          while (!!!client.publish(publishTopic, (char *)payload.c_str())) {
                  Serial.println("Publish ok");
                  if (!!!client.connected()) {
                        Serial.print("Reconnecting client to ");
                        Serial.println(server);
                        while (!!!client.connect(clientId, authMethod, token)) {
                              Serial.print(".");
                              delay(1000);
                        }
                        Serial.println();
                  }
          
          } 
            time_now = millis();           
            Serial.println("5 sec Delay");           
            while(millis() < time_now + period){
                //wait approx. [period] ms
            }  
}

void InitWiFi()
{
      // initialize serial for ESP module
      soft.begin(112500);
      // initialize ESP module
      WiFi.init(&soft);
      
      Serial.println("Connecting to AP …");
      // attempt to connect to WiFi network
      while ( status != WL_CONNECTED) {
            Serial.print("Attempting to connect to WPA SSID: ");
            Serial.println(WIFI_AP);
            // Connect to WPA/WPA2 network
            status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
            delay(1000);
      }
      Serial.println("Connected to AP");
}

void callback(char* publishTopic, char* payload, unsigned int length) {
      Serial.println("callback invoked");
}
