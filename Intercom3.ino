#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <TEA5767Radio.h>

#define PICKUP 13
#define BUZZ 12

const char* ssid = "";
const char* password = "";
const char* ok_response = "OK";
const unsigned int localUdpPort = 18515;  // local port to listen on
char frameBuffer[2];

WiFiUDP Udp;
TEA5767Radio radio = TEA5767Radio();

void setup()
{
  Serial.begin(115200);
  pinMode(PICKUP, OUTPUT);
  pinMode(BUZZ, OUTPUT);
//  Wire.begin();
//  radio.setFrequency(100.4, true); // pick your own frequency
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize == 0) {
    delay(100);
  } else if (packetSize == 2) {
    Udp.read(frameBuffer, 2);


    Serial.print("Packet from "); 
    Serial.print(Udp.remoteIP());
    Serial.print(":"); 
    Serial.print(Udp.remotePort());
    Serial.print(" - "); 
    Serial.print((int) frameBuffer[0]); 
    Serial.print(" "); 
    Serial.println((int) frameBuffer[1]); 


    switch(frameBuffer[0]) {
      case 0:
//        radio.setFrequency(80, frameBuffer[1] == 0);
        break;
      case 1:
        digitalWrite(PICKUP, frameBuffer[1] != 0);
        Serial.print("Setting PICKUP to ");
        Serial.print(frameBuffer[1] != 0);
        break;
      case 2:
        digitalWrite(BUZZ, frameBuffer[1] != 0);        
        Serial.print("Setting BUZZ to ");
        Serial.print(frameBuffer[1] != 0);
        break;
    }
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ok_response, 2);
    Udp.endPacket();
  }
}
