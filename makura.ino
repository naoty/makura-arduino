#include <SPI.h>
#include <Ethernet.h>

int sensorPin = 0;

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xC2, 0x45 };
IPAddress server(50,19,86,241); // http://makura.herokuapp.com
EthernetClient client;

void setup() {
  Serial.begin(9600);
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure using DHCP");
  }
  
  connectServer();
}

void loop() {  
  if (client.connected()) {
    //sendPostRequest();
    
    int pressure = analogRead(sensorPin);
    sendPressure(pressure);
    delay(10000);
  } else {
    Serial.println();
    Serial.println("disconnected.");
    client.stop();
    
    delay(5000);
    connectServer();
  }
}

void connectServer() {
  Serial.println("connecting...");
  
  if (client.connect(server, 80)) {
    Serial.println("connected");
  } else {
    Serial.println("connection failed");
  }
}

void sendPressure(int pressure) {
  String pressureData = String("pressure=") + pressure;
  
  // Debug
  Serial.println(pressureData);
  
  client.println("POST / HTTP/1.1");
  client.println("Host: makura.herokuapp.com");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(pressureData.length(), DEC);
  client.println();
  client.println(pressureData);
}
