#include <UIPEthernet.h>

EthernetClient client;
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x78, 0xEE  };
IPAddress ip(192, 168, 1, 115);
signed long next;
int led = 5;

int latchPin1 = 3; //pin 12 on the 595
int dataPin1 = 4; //pin 14 on the 595
int clockPin1 = 2; //pin 11 on the 595

int y1 = 7;
int g1 = 6;
int r1 = 8;

int number[10] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};
int readPin;
int count;
void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac);
  Serial.print("localIP: ");
  Serial.println(Ethernet.localIP());
  Serial.print("subnetMask: ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("gatewayIP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("dnsServerIP: ");
  Serial.println(Ethernet.dnsServerIP());
  next = 0;


  pinMode(led, OUTPUT);
  Serial.println("Connecting.....");
}

void loop() {

  //  while (Serial.available()) {
  // get the new byte:
  //Serial.println("receiving");
  /////////////////////////////////////

  /////////////////////////////////////
  //    char inChar = (char)Serial.read();
  //    Serial.println(inChar);
  //    delay(100);
  //    // add it to the inputString:
  //    client.connect(IPAddress(192, 168, 0, 101), 5000);
  //    client.print(inChar);


  for (int i=0;i<35;i++) {
    if (client.connect(IPAddress(192, 168, 0, 101), 5000))
    {

      if(i < 10){
         client.print("red");
      }else if( i >= 10 && i <=15){
         client.print("yellow");
      }else if(i > 15 && i <= 35){
        client.print("green");
      }
      //Serial.println("Client connected");

      int size;
      while ((size = client.available()) > 0)
      {
        uint8_t* msg = (uint8_t*)malloc(size);
        size = client.read(msg, size);
        Serial.write(msg, size);
        free(msg);
      }
close:
      //disconnect client
      Serial.println("Client disconnect");
      client.stop();
    }
    else
      Serial.println("Client connect failed");
  }
}
