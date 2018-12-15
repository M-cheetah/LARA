#include <SoftwareSerial.h>
#include <stdlib.h>
int sensor1=6;
int sensor1State=0;
String apiKey = "***********";
// connect 10 to TX of Serial USB
// connect 11 to RX of serial USB
SoftwareSerial ser(10, 11); // TX, RsX
int trigPin = 6; // connect trigger pin of ultrasonic sensor to 6
int echoPin = 5; // connect echo pin of ultrasonic sensor to 5
long rate=0;
long preread=0;
long counter =0;
int object=0;
float halfval=0;

void setup() {     
  
  Serial.begin(115200);           

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  
   ser.begin(115200);  
  ser.println("AT+RST");
}


void loop() {
  long duration, distance;
  counter ++;
  
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration=pulseIn(echoPin, HIGH);
  distance =(duration/2)/29.1;
  Serial.print("Counter = ");
  Serial.println(counter);
  Serial.print(distance);
  Serial.println("CM");
  delay(500);  
 if(counter %2 !=0)
 {
  rate=distance-preread;
  preread=distance;
  halfval= preread/2;
  
 }
 Serial.print("halfval = ");
  Serial.println(halfval);
   if(distance<=halfval)
  {
    object=2;
    
    distance=50;
    rate=0;
  }
  String state1=String(distance); 
  String state2=String(rate);
  String state3=String(counter);
  String state4=String(object); 
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  Serial.println(cmd); 
  
  String getStr = "GET /update?api_key=";
  getStr += apiKey;  
  getStr +="&field1=";
  getStr += String(state1);  
  getStr +="&field2=";
  getStr += String(state2);
  getStr +="&field3=";
  getStr += String(state3);
  getStr +="&field4=";
  getStr += String(state4);
  getStr += "\r\n\r\n";
  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);
  ser.print(getStr);
  Serial.print(getStr); 
   object --;  
  delay(16000); 
}
