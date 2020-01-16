int latchPin1 = 2; //pin 12 on the 595 
int dataPin1 = 3; //pin 14 on the 595 
int clockPin1 = 4; //pin 11 on the 595 


//leds pins assign
int y1 = 7;
int g1 = 6;
int r1 = 8;
int y2 = 32;
int g2 = 30;
int r2 = 34;

// 7 segments digital numbers array
int number[10] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};

void setup() { 
  pinMode(latchPin1, OUTPUT);
  pinMode(dataPin1, OUTPUT);
  pinMode(clockPin1, OUTPUT);
  pinMode(y1, OUTPUT);
  pinMode(y2, OUTPUT);
  pinMode(g1, OUTPUT);
  pinMode(g2, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);

  Serial.begin(9600);
   
}

void loop() { 
 lane2(latchPin1,dataPin1,clockPin1);
 lane1(latchPin1,dataPin1,clockPin1);

}

void writeTo595(int latchPin,int dataPin,int clockPin,int num1,int num2,int num3, int num4){
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, number[num1]);
    shiftOut(dataPin, clockPin, MSBFIRST, number[num2]);
    shiftOut(dataPin, clockPin, MSBFIRST, number[num3]);
    shiftOut(dataPin, clockPin, MSBFIRST, number[num4]);
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
      writeTo595(latchPin,dataPin,clockPin,n1,n2,n3,n4);
   }else if(fnum <=0 && check){
      digitalWrite(y1,HIGH);
      digitalWrite(r2,HIGH);
      digitalWrite(g1,LOW);
      writeTo595(latchPin,dataPin,clockPin,0,diff_num--,n3,n4);
      
   }else if(snum <= 0){
     digitalWrite(r1,HIGH);
     digitalWrite(y2,HIGH);
     digitalWrite(g2,LOW);
      writeTo595(latchPin,dataPin,clockPin,n1,n2,0,diff_num--);
   }
   
   fnum--;
   snum--;
 }
  ledoff();
}

void lane1(int latchPin,int dataPin,int clockPin){
  ForDelay(latchPin,dataPin,clockPin,number,7,10);  
}

void lane2(int latchPin,int dataPin,int clockPin){
   ForDelay(latchPin,dataPin,clockPin,number,10,7);
}

void ledoff(){
  digitalWrite(g2,LOW);
  digitalWrite(y2,LOW);
  digitalWrite(r2,LOW);
  digitalWrite(g1,LOW);
  digitalWrite(y1,LOW);
  digitalWrite(r1,LOW);
}
