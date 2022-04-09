// • hello there, if you are not "kamal" please do not edit this program ■#_#■...♦\1
// --------------------------------------------------------------------------------♦\2
// • let's start ☺♥                                                                ♦|0
//                                                                                 ♦/1
// -------------------------------------------------------------------------------♦/2


#include <Servo.h>
#include<SoftwareSerial.h>
SoftwareSerial myW_r(10, 11);
SoftwareSerial myW_s(2,  3);

// tercker...
Servo horizontal;
Servo vertical;

int servoh = 90;
int servov = 90;

int servohLimitHigh = 90 + 35;
int servohLimitLow = 35;

int servovLimitHigh = 90 + 35;
int servovLimitLow = 35;


#define ldrlt  A0
#define ldrrt  A2
#define ldrld  A3
#define ldrrd  A1

// Water level...
#define sensorPower 53
#define sensorPin A5

// DHT11..
#include "DHT.h"
#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LCD Display...
#include <LiquidCrystal.h>
const short int rs = 19, en = 18, d4 = 17, d5 = 16, d6 = 15, d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Display...
int DisplayTime = 0;
bool checkDisplay = true;
bool checkClear = true;
String last_distance;
bool firstClear = true;

// ultraSonic...
const short int echoPin = 6;
const short int trigPin = 7;
long int duration;
int durationVal;

int startALed = 35;
int endALed = 39;
bool switchALed = true;
int nled = 7;
int startNLed = 43;
int endNLed = 49;
int count = 0;
int minDistanceOfALed = 5;
int minDistanceOfNLed = 40;
float Distance = 0;
float last;
float sizeOfNLed = minDistanceOfNLed / nled;
const short int buzer = 8;

bool checkWarning = false;
//int leds = 5;
//int ledr = 30;
//int ledn;

// Water Humidity...
int percentHumidityWaterValue = 0;
const short int WaterHumiditySensor = A4;
int humidityWaterValue;
const short int waterPumpOut = 22;
const short int waterPumpIn = 23;
bool checkWater = true;

int i = 0;
int y = 0;
int ii = 0;
int yy = 0;
int yyy = 0;
int val = 0;

int readWaterLevel = 0;

String data;
String dataName;
String dataValue;
String powerName[3] = {"default", "pump", "ultraSonic"};
String powerVal[3] = {"off", "off", "off"};
String sensorName[4] = {"temperature", "airHumidity", "soilHumidity", "warningSystem"};
String sensorVal[4] = {"11", "22", "33", "44"};

void setup() {
  //  horizontal.attach(31);
  //  vertical.attach(30);
  //    horizontal.write(180);
  //    vertical.write(45);
  //  horizontal.write(90);
  //  vertical.write(90);
  //  delay(1000);
  dht.begin();
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(sensorPower, OUTPUT);
  pinMode(52, OUTPUT);
  digitalWrite(sensorPower, LOW);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(waterPumpOut, OUTPUT);
  pinMode(waterPumpIn, OUTPUT);
  pinMode(buzer, OUTPUT);
  myW_r.begin(9600);
  myW_s.begin(9600);
  for (int i = startNLed; i <= endNLed; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  for (int i = startALed; i <= endALed; i++)
    pinMode(i, OUTPUT);
}

void loop() {
  //  tracker();
  SetData();
  db();
  //  db();
  SetPower();
}

void SetData() {
  //  tracker();
  LedAndAlarm(checkWarning);
  WaterLevel();
  airhumidityAndTemperature();
  humidityWater();
  WaterLevel();
  Display();
}

void SetPower() {
  if (powerVal[0] == "off") {
    if (powerVal[1] == "on")
      digitalWrite(waterPumpOut, HIGH);
    else if (powerVal[1] == "off")
      digitalWrite(waterPumpOut, LOW);
    if (powerVal[2] == "on" && sensorVal[3].toInt() <= minDistanceOfALed) {
      checkWarning = true;
    }
    else {
      checkWarning = false;
    }
  }
  else if (powerVal[0] == "on") {
    AutoRunWater();
    if (sensorVal[3].toInt() < minDistanceOfALed) {
      i++;
    }
    else {
      i = 0;
      checkWarning = false;
      //      digitalWrite(buzer, 0);
    }
    if (i == 2) {
      i = 0;
      checkWarning = true;
      //      digitalWrite(buzer, 1);
    }
  }
}

void Display() {
  lcd.setCursor(0, 0);
  if (checkClear == true) {
    lcd.clear();
    checkClear = false;
  }
  if (checkWarning == false) {
    if (DisplayTime <= 150 && checkDisplay == true) {
      lcd.print("  air_H:  ");
      lcd.print(sensorVal[1] + "% ");
      lcd.setCursor(0, 1);
      lcd.print("  soil_H: ");
      lcd.print(sensorVal[2] + "% ");
      DisplayTime++;
      if (DisplayTime == 150) {
        checkClear = true;
        checkDisplay = false;
      }
    }
    else {
      lcd.print("  Temp:   ");
      lcd.print(sensorVal[0] + "C ");
      lcd.setCursor(0, 1);
      lcd.print("  warSys: ");
      if (yy <= 30) {
        lcd.print(last_distance);
        yy++;
      } else {
        last_distance = sensorVal[3] + "cm  ";
        yy = 0;
      }
      DisplayTime--;
      if (DisplayTime == 0) {
        checkClear = true;
        checkDisplay = true;
      }
    }
    firstClear = true;
  }
  else {
    if (firstClear == true) {
      checkClear = true;
      firstClear = false;
    }
    lcd.print("  Warning !!!!");
    lcd.setCursor(0, 1);
    lcd.print( "      " + sensorVal[3] + "cm ");
  }
}

void humidityWater() {
  humidityWaterValue = analogRead(WaterHumiditySensor);
  percentHumidityWaterValue = map(humidityWaterValue, 1023, 0, 0, 100);
  sensorVal[2] = String(percentHumidityWaterValue);
}

void AutoRunWater() {
  if (percentHumidityWaterValue <= 30)
    digitalWrite(waterPumpOut, HIGH);
  else if (percentHumidityWaterValue > 60)
    digitalWrite(waterPumpOut, LOW);
}
//
//void AutoRunWater() {
//  if (percentHumidityWaterValue <= 30 && checkWater) {
//    digitalWrite(waterPumpOut, HIGH);
//    checkWater = false;
//  }
//  else if (percentHumidityWaterValue > 60) {
//    digitalWrite(waterPumpOut, LOW);
//    checkWater = true;
//  }
//}

int last_duration = 0;


int delayNLed = 0;
int delayALed = 0;

void LedAndAlarm(bool Alarm) {
  getDistance();
  count = 0;
  last = Distance;
  while (last < minDistanceOfNLed) {
    count++;
    last += sizeOfNLed;
  }
  for (int i = startNLed; i < startNLed + count; i++) {
    digitalWrite(i, HIGH);
  }
  if (delayNLed == 100) {
    for (int i = startNLed + count; i <= endNLed; i++)
      digitalWrite(i, LOW);
    delayNLed = 0;
  }
  else {
    delayNLed++;
  }
  if (Distance < minDistanceOfALed && Alarm) {
    //    Serial.println(String(delayALed));
    if (delayALed == 7 && switchALed) {
      for (int i = startALed; i <= endALed; i++)
        digitalWrite(i, HIGH);
      digitalWrite(buzer, HIGH);
      //      checkWarning = true;
      switchALed = false;
      delayALed = 0;
    } else if (delayALed == 7 && !switchALed) {
      for (int i = startALed; i <= endALed; i++)
        digitalWrite(i, LOW);
      digitalWrite(buzer, LOW);
      //      checkWarning = false;
      switchALed = true;
      delayALed = 0;
    }
    delayALed++;
  } else if (!switchALed) {
    if (delayALed == 7) {
      for (int i = startALed; i <= endALed; i++)
        digitalWrite(i, LOW);
      digitalWrite(buzer, LOW);
      switchALed = true;
      //      checkWarning = false;
      delayALed = 0;
    }
    delayALed++;
  }
}
void getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  durationVal = int(duration * 0.034 / 2);
  if (durationVal != last_duration) {
    last_duration = durationVal;
  } else {
    Distance = duration * 0.034 / 2;
    sensorVal[3] = String(durationVal);
  }
}

String power = "";
void db() {
  while (myW_r.available()) {
    char x = myW_r.read();
    if (x == ':') {
      power = data;
      data = "";
    }
    else if (x == ',') {
      for (int i = 0; i < 3; i++) {
        if (power == powerName[i]) {
          powerVal[i] = data;
          power = "";
          data = "";
          Serial.println(powerName[y] + " --> " + powerVal[y]);
          y++;
          if (y == 3) {
            y = 0;
          }
        }
      }
    }
    else data += String(x);
  }
  data = "";
  for (int i = 0; i < 4; i++) {
    data = sensorName[i] + ":" + sensorVal[i] + ",";
    myW_s.print(data);    
  }
  myW_s.print("\n");
  data = "";
}

void airhumidityAndTemperature() {
  int airHumidityVal = dht.readHumidity();
  int temperatureVal = dht.readTemperature();
  int f = dht.readTemperature(true);
  if (String(airHumidityVal) == " NAN" || String(temperatureVal) == " NAN") {
    return;
  }
  sensorVal[0] = String(temperatureVal);
  sensorVal[1] = String(airHumidityVal);
}

int WaterLevel() {
  digitalWrite(sensorPower, HIGH);
  digitalWrite(52, HIGH);
  readWaterLevel = analogRead(sensorPin);
  readWaterLevel = 100 - map(readWaterLevel, 1023, 0, 0, 100);
  digitalWrite(sensorPower, LOW);
  digitalWrite(52, LOW);
  if (readWaterLevel < 30)
    digitalWrite(waterPumpIn, HIGH);
  else
    digitalWrite(waterPumpIn, LOW);
}

void tracker() {
  int lt = analogRead(ldrlt);
  int rt = analogRead(ldrrt);
  int ld = analogRead(ldrld);
  int rd = analogRead(ldrrd);
  int tol = 50;
  int avt = (lt + rt) / 2;
  int avd = (ld + rd) / 2;
  int avl = (lt + ld) / 2;
  int avr = (rt + rd) / 2;
  int dvert = avt - avd;
  int dhoriz = avl - avr;
  //  Serial.println("lt : " + String(lt) + " || rt : " +  String(rt));
  //  Serial.println("ld : " + String(ldrld) + " || rd : " +  String(ldrrd));
  if (-1 * tol > dvert || dvert > tol)
  {
    if (avt > avd)
    {
      servov = ++servov;
      if (servov > servovLimitHigh)
      {
        servov = servovLimitHigh;
      }
    }
    else if (avt < avd)
    {
      servov = --servov;
      if (servov < servovLimitLow)
      {
        servov = servovLimitLow;
      }
    }
    vertical.write(servov);
  }

  if (-1 * tol > dhoriz || dhoriz > tol) // check if the diffirence is in the tolerance else change horizontal angle
  {
    if (avl > avr)
    {
      servoh = --servoh;
      if (servoh < servohLimitLow)
      {
        servoh = servohLimitLow;
      }
    }
    else if (avl < avr)
    {
      servoh = ++servoh;
      if (servoh > servohLimitHigh)
      {
        servoh = servohLimitHigh;
      }
    }
    else if (avl = avr)
    {
      // nothing
    }
    horizontal.write(servoh);
  }
  delay(15);
}
