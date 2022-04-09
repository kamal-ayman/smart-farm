// send to db...
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "farm-app-9238a-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "HMFiyzyufFU0oncKCvAnHnBUiBxFMPGYkkpGYXD5"
#define WIFI_SSID "oliver"
#define WIFI_PASSWORD "123123123"
#include <user_interface.h>

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

String sensorName[4] = {"temperature", "airHumidity", "soilHumidity", "warningSystem"};
String sensorVal[4] = {"0", "0", "0", "0"};
String data;

// "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
DynamicJsonBuffer jsonBuffer;
String sensor = "";
void loop() {
  while (Serial.available())
  {
    char x = Serial.read();
    if (x == ':') {
      sensor = data;
      data = "";
    }
    else if (x == ',') {
      for (int i = 0; i < 4; i++) {
        if (sensor == sensorName[i]) {
          sensorVal[i] = data;
          sensor = "";
          data = "";
        }
      }
    }
    else if (x == '\n') {
      data = "{";
      for (int i = 0; i < 4; i++) {
        data += "\"" + sensorName[i] + "\":\"" + sensorVal[i] + "\"";
        if (i == 4 - 1) break;
        data += ",";
      }
      data += "}";
      JsonObject& root = jsonBuffer.parseObject(data);
      Firebase.set("/data", root);
      data = "";
    }
    else data += String(x);
  }
}
