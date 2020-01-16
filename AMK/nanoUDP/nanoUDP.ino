/*
   UIPEthernet UdpClient example.

   UIPEthernet is a TCP/IP stack that can be used with a enc28j60 based
   Ethernet-shield.

   UIPEthernet uses the fine uIP stack by Adam Dunkels <adam@sics.se>

        -----------------

   This UdpClient example tries to send a packet via udp to 192.168.0.1
   on port 5000 every 5 seconds. After successfully sending the packet it
   waits for up to 5 seconds for a response on the local port that has been
   implicitly opened when sending the packet.

   Copyright (C) 2013 by Norbert Truchsess (norbert.truchsess@t-online.de)
*/

#include <UIPEthernet.h>

EthernetUDP udp;
int led = 2;
unsigned long next;
char a;
int c;
String val;
char rec;
String lane1,lane2;
void setup() {

  Serial.begin(9600);

  uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

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
  next = millis() + 5000;
  digitalWrite(led, LOW);
}

void loop() {

  int success;
  int len = 0;
  val = "";
  a = NULL;
 delay(500);
  while (!Serial.available()) {

   
    //a = (char)Serial.read(); // read the incoming data as string
    a = '1';
    Serial.println(a);
resend:
    do
    {
      success = udp.beginPacket(IPAddress(192, 168, 0, 101), 5000);
      Serial.print("beginPacket: ");
      Serial.println(success ? "success" : "failed");
      
    }
    while (!success < 0);
    if (!success )
      goto stop;

    success = udp.write(a);

    Serial.print("bytes written: ");
    Serial.println(success);

    success = udp.endPacket();

    Serial.print("endPacket: ");
    Serial.println(success ? "success" : "failed");

    do
    {
      //check for new udp-packet:
      success = udp.parsePacket();
    }
    while (!success < 0);
    if (!success )
      goto resend;

    Serial.print("received: '");
    do
    {
      c = udp.read();
      Serial.write(c);
      //Serial.print(c);
      rec = c;
      val += rec;
      len++;
    }
    while ((success = udp.available()) > 0);
    Serial.print("', ");
    Serial.print(len);
    Serial.println(" bytes");

    Serial.println(val);

    lane1 = getValue(val,':',0);
    lane2 = getValue(val,':',1);
    if ( lane2.toInt() == 10 ) {

      
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }


    //finish reading this packet:
    udp.flush();

stop:
    udp.stop();
    Serial.println("Stop");

  }










  //  if (((signed long)(millis() - next)) > 0)
  //  {
  //
  //    Serial.println();
  //    do
  //    {
  //      success = udp.beginPacket(IPAddress(192, 168, 0, 101), 5000);
  //      Serial.print("beginPacket: ");
  //      Serial.println(success ? "success" : "failed");
  //      //beginPacket fails if remote ethaddr is unknown. In this case an
  //      //arp-request is send out first and beginPacket succeeds as soon
  //      //the arp-response is received.
  //    }
  //    while (!success && ((signed long)(millis() - next)) < 0);
  //
  //    if (!success )
  //      goto stop;
  //
  //    success = udp.write("1");
  //
  //    Serial.print("bytes written: ");
  //    Serial.println(success);
  //
  //    success = udp.endPacket();
  //
  //    Serial.print("endPacket: ");
  //    Serial.println(success ? "success" : "failed");
  //
  //    do
  //    {
  //      //check for new udp-packet:
  //      success = udp.parsePacket();
  //    }
  //    while (!success && ((signed long)(millis() - next)) < 0);
  //    if (!success )
  //      goto stop;
  //
  //    Serial.print("received: '");
  //    do
  //    {
  //      int c = udp.read();
  //      Serial.write(c);
  //      Serial.print(c);
  //      len++;
  //    }
  //    while ((success = udp.available()) > 0);
  //    Serial.print("', ");
  //    Serial.print(len);
  //    Serial.println(" bytes");
  //    digitalWrite(led, HIGH);
  //
  //    //finish reading this packet:
  //    udp.flush();
  //
  //stop:
  //    udp.stop();
  //    Serial.println("Stop");
  //    next = millis() + 5000;
  //  }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

