/* WiFi_ArmCrawler */

#include <WiFi.h>

#define PIN_L1 25
#define PIN_L2 26

#define PIN_R1 32
#define PIN_R2 33

const char ssid[] = "*****";
const char pass[] = "*****";

WiFiUDP udp;
unsigned int localPort = 10000;
const int PACKET_SIZE = 256;
char packetBuffer[PACKET_SIZE];

void motor_stop(int pin1, int pin2) {  
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}

void motor_forward(int pin1, int pin2) {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
}

void motor_back(int pin1, int pin2) {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
}

void setup() {
  
  pinMode(PIN_L1, OUTPUT);
  pinMode(PIN_L2, OUTPUT);
  pinMode(PIN_R1, OUTPUT);
  pinMode(PIN_R2, OUTPUT);
  
  WiFi.softAP(ssid, pass);
  udp.begin(localPort);

}

void loop() {
  int rlen;
  int val = 0;

  while (1) {
    rlen = udp.parsePacket();
    
    if(rlen < 10) {
      delay(1);
      continue;
    }
    
    udp.read(packetBuffer, (rlen > PACKET_SIZE) ? PACKET_SIZE : rlen);
    val = packetBuffer[1];
    
    if(val) {
      switch (val) {
        case 1:// Up
          motor_forward(PIN_L1, PIN_L2);
          motor_forward(PIN_R1, PIN_R2);
          break;
        case 2: // Down
          motor_back(PIN_L1, PIN_L2);
          motor_back(PIN_R1, PIN_R2);
          break;
        case 4: // Right
          motor_forward(PIN_L1, PIN_L2);
          motor_back(PIN_R1, PIN_R2);
          break;
        case 8: // Left
          motor_back(PIN_L1, PIN_L2);
          motor_forward(PIN_R1, PIN_R2);
          break;
      }      
    } else {
      motor_stop(PIN_L1, PIN_L2);
      motor_stop(PIN_R1, PIN_R2);
    }
    
  }
  
}
