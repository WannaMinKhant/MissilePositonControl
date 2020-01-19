int latchPin1 = 2; //pin 12 on the 595
int dataPin1 = 3; //pin 14 on the 595
int clockPin1 = 4; //pin 11 on the 595

int number[10] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};

void setup() {
  Serial.begin(9600);
  pinMode(latchPin1, OUTPUT);
  pinMode(dataPin1, OUTPUT);
  pinMode(clockPin1, OUTPUT);
}

void loop() {
  ForDelay(latchPin1,dataPin1,clockPin1,number,10,15);
  ForDelay(latchPin1,dataPin1,clockPin1,number,15,10);
  
//  for (int i = 9; i >= 0; i--) {
//    writeTo595(latchPin1, dataPin1, clockPin1, i, i, 1, 1);
//    Serial.println(i);
//    delay(500);
//  }

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
      //Serial.println("decade");
    } else if (fnum <= 0 && check) {
      writeTo595(latchPin, dataPin, clockPin, 0, diff_num--);
      //Serial.println("ku");
    }
    fnum--;
    snum--;
  }
}
