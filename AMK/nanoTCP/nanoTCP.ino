#include <UIPEthernet.h>

EthernetClient client;
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x78, 0xEE  };
IPAddress ip(192, 168, 1, 115);
signed long next;
int btn = 3;
int led = 5;
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

  pinMode(btn, INPUT);
  pinMode(led, OUTPUT);
  Serial.println("Connecting.....");
}

void loop() {
  
  while (Serial.available()) {
    // get the new byte:
    //Serial.println("receiving");
    /////////////////////////////////////

    /////////////////////////////////////
    char inChar = (char)Serial.read();
    Serial.println(inChar);
    delay(100);
    // add it to the inputString:
    client.connect(IPAddress(192, 168, 0, 101), 5000);
    client.print(inChar);
  }








  
//  if(client.connect(IPAddress(192, 168, 1, 12), 5000))
//  {
//    Serial.println("Client connected");
//    count += 1;
//    client.println(count);
//
//    int size;
//    while ((size = client.available()) > 0)
//    {
//      uint8_t* msg = (uint8_t*)malloc(size);
//      Serial.println('receive data');
//      size = client.read(msg, size);
//      Serial.write(msg, size);
//      free(msg);
//      if ( size > 0) {
//
//        digitalWrite(led, HIGH);
//      }
//    }
//close:
//    //disconnect client
//    Serial.println("Client disconnect");
//    client.stop();
//  }
//  delay(5000);
}
