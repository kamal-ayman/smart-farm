// send to db...
#include <SoftwareSerial.h>
SoftwareSerial myW(0, 2);
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "farm-app-9238a-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "HMFiyzyufFU0oncKCvAnHnBUiBxFMPGYkkpGYXD5"
#define WIFI_SSID "oliver"
#define WIFI_PASSWORD "123123123"
#include <user_interface.h>

String data;
String dataName;
String dataValue;
int i = 0;

uint8_t mac[] = {0x06, 0x07, 0x08, 0x09, 0x10, 0x11};

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.hostname("send_wifi_by_oliver");
  wifi_set_macaddr(STATION_IF, &mac[0]);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting\n");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".\n");
    delay(100);
  }
  Serial.print('\n');
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Serial.print('\n');
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}

void loop() {
  if (Serial.available())
  {
    char x = Serial.read();
    if (x == '\n') {
      if (i == 0 && data == "temperature" || data == "airHumidity" || data == "soilHumidity" || data == "warningSystem") {
        dataName = data;
        i = 1;
      }
      else if (i == 1) {
        dataValue = data;
        Firebase.setString("data/" + dataName, dataValue);
        i = 0;
      }
      data = "";
    }
    else data += String(x);
  }
}
