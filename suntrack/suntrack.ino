#include <Servo.h>

Servo myservo1, myservo2;
int LDR1 = A0, LDR2 = A1, LDR3 = A2, LDR4 = A3;
int rRDL1 = 0, rRDL2 = 0, rRDL3 = 0, rRDL4 = 0;
int max1=0, max2=0, max3=0;
int ser1 = 80, ser2=90;

void setup() {
  myservo1.attach(31);
  myservo2.attach(30);

  Serial.begin(9600);
  myservo1.write(90);
  myservo2.write(90);
  delay(4000);
}

void loop() {

  rRDL1 = analogRead(LDR4) / 100; // ld
  rRDL2 = analogRead(LDR3) / 100; //rt
  rRDL3 = analogRead(LDR1) / 100; // lt
  rRDL4 = analogRead(LDR2) / 100; // rd


//  rRDL1 = analogRead(LDR1) / 100; // lt
//  rRDL2 = analogRead(LDR2) / 100; //rd
//  rRDL3 = analogRead(LDR3) / 100; // rt
//  rRDL4 = analogRead(LDR4) / 100; // ld


  max1 = max(rRDL1, rRDL2);
  max2 = max(rRDL3, rRDL4);
  max3 = max(max1, max2);
  
//  Serial.println(String(max3));
  Serial.println(String(rRDL4));
//  +", "+String(rRDL2) +", "+String(rRDL3) +", "+String(rRDL4));


  if(rRDL1<max3 && rRDL2<max3)
  {
    if(ser1<180)
      ser1+=1;
    myservo1.write(ser1);
  }
  if(rRDL3<max3 && rRDL4<max3)
  {
    if(ser1>0)
      ser1-=1;
    myservo1.write(ser1);
  }

  if(rRDL2<max3 && rRDL3<max3)
  {
    Serial.println("servo2 +" + String(ser2));
    if(ser2<90+35)
      ser2+=1;
    myservo2.write(ser2);
  }
  if(rRDL1<max3 && rRDL4<max3)
  {
    Serial.println("servo2 -" + String(ser2));
    if(ser2>35)
      ser2-=1;
    myservo2.write(ser2);
    
  }
  delay(15);

 
}
