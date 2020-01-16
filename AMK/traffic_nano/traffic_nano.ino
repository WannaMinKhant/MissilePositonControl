#include <UIPEthernet.h>

EthernetUDP udp;
int led = 5;

int latchPin1 = 3; //pin 12 on the 595
int dataPin1 = 4; //pin 14 on the 595
int clockPin1 = 2; //pin 11 on the 595

int y1 = 7;
int g1 = 6;
int r1 = 8;

int number[10] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};

unsigned long next;
char a;
int c;
String val;
char rec;
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

  pinMode(latchPin1, OUTPUT);
  pinMode(dataPin1, OUTPUT);
  pinMode(clockPin1, OUTPUT);
  pinMode(y1, OUTPUT);
  pinMode(y2, OUTPUT);
  pinMode(g1, OUTPUT)
}

void loop() {

  int success;
  int len = 0;
  val = "";

  while (Serial.available()) {

    a = (char)Serial.read(); // read the incoming data as string
    Serial.println(a);

    do
    {
      success = udp.beginPacket(IPAddress(192, 168, 0, 101), 5000);
      Serial.print("beginPacket: ");
      Serial.println(success ? "success" : "failed");
      //beginPacket fails if remote ethaddr is unknown. In this case an
      //arp-request is send out first and beginPacket succeeds as soon
      //the arp-response is received.
    }
    while (!success && ((signed long)(millis() - next)) < 0);
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
    while (!success && ((signed long)(millis() - next)) < 0);
    if (!success )
      goto stop;

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
    if ( val.toInt() == 25 ) {


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
}

void writeTo595(int latchPin, int dataPin, int clockPin, int num1, int num2) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, number[num1]);
  shiftOut(dataPin, clockPin, MSBFIRST, number[num2]);
  //    shiftOut(dataPin, clockPin, MSBFIRST, number[num3]);
  //    shiftOut(dataPin, clockPin, MSBFIRST, number[num4]);
  digitalWrite(latchPin, HIGH);
  delay(1000);
}

void ForDelay(int latchPin,int dataPin,int clockPin,int numb[],int fnum,int snum){
  bool check = false;

  int upper = 0;
  int lower = 0;
  
  if(fnum > snum){
    ledoff();
    digitalWrite(g2,HIGH);
    digitalWrite(r1,HIGH);
    upper = fnum;
    lower = snum;
  }else{
    ledoff();
    digitalWrite(g1,HIGH);
    digitalWrite(r2,HIGH);
    upper = snum;
    lower = fnum;
    check = true;
  }
 
  int diff_num = abs(fnum-snum)-1;

 for(int k = upper;k >= 0;k--){
 
   int n1 = fnum / 10;
   int n2 = fnum % 10;
   int n3 = snum /10;
   int n4 = snum % 10;

   if(snum >= 0 && fnum >=0){
      writeTo595(latchPin,dataPin,clockPin,n1,n2);
   }else if(fnum <=0 && check){
      digitalWrite(y1,HIGH);
      digitalWrite(r2,HIGH);
      digitalWrite(g1,LOW);
      writeTo595(latchPin,dataPin,clockPin,0,diff_num--);
      
   }
   fnum--;
   snum--;
 }
  ledoff();
}

void ledoff() {
  digitalWrite(g1, LOW);
  digitalWrite(y1, LOW);
  digitalWrite(r1, LOW);
}
