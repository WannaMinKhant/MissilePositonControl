#include <UIPEthernet.h>

EthernetClient client;
int latchPin1 = 2; //pin 12 on the 595
int dataPin1 = 3; //pin 14 on the 595
int clockPin1 = 4; //pin 11 on the 595

int number[10] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};

String val;
String lane1, lane2;
char k;
bool state;

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
  state = false;

}

void loop() {

  val = "";
  Serial.println("Client connect");

  if (client.connect(IPAddress(192, 168, 0, 101), 5000))
  {
    Serial.println("Client connected");
    if (!state) {
      client.println("Ready");
      state = true;
    }
    client.println("Ready");

    int size;
    while ((size = client.available()) > 0)
    {
      uint8_t* msg = (uint8_t*)malloc(size);
      // k = client.read();
      size = client.read(msg, size);
      Serial.write(msg, size);
      //val = (char*)msg;
      //val += k;
      Serial.println("recieve");
      free(msg);
    }
    Serial.println(size);
    // lane1 = getValue(val, ':', 0);
    // lane2 = getValue(val, ':', 1);
    //    Serial.println(lane1);
    //    Serial.println(lane2);
    // ForDelay(latchPin1, dataPin1, clockPin1, number, lane1.toInt(), lane2.toInt());

close:
    //disconnect client
    Serial.println("Client disconnect");
    client.stop();
  }
  else
    Serial.println("Client connect failed");

    delay(3000);
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

void writeTo595(int latchPin, int dataPin, int clockPin, int num1, int num2) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, number[num1]);
  shiftOut(dataPin, clockPin, MSBFIRST, number[num2]);
  digitalWrite(latchPin, HIGH);
  delay(1000);
}

void ForDelay(int latchPin, int dataPin, int clockPin, int numb[], int fnum, int snum) {

  bool check = false;
  int upper = 0;
  int lower = 0;

  if (fnum > snum) {
    upper = fnum;
    lower = snum;
  } else {
    upper = snum;
    lower = fnum;
    check = true;
  }

  int diff_num = abs(fnum - snum) - 1;
  for (int k = upper; k >= 0; k--) {
    int n1 = fnum / 10;
    int n2 = fnum % 10;

    if (snum >= 0 && fnum >= 0) {
      writeTo595(latchPin, dataPin, clockPin, n1, n2);
    } else if (fnum <= 0 && check) {
      writeTo595(latchPin, dataPin, clockPin, 0, diff_num--);
    }
    Serial.println(k);
    if (k == 1) {
      client.println("Ready");
    } else if(k == 0) {
      client.println("Busy");
    }
    fnum--;
    snum--;
  }
}
