// rec from db..
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "farm-app-9238a-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "HMFiyzyufFU0oncKCvAnHnBUiBxFMPGYkkpGYXD5"
#define WIFI_SSID "oliver"
#define WIFI_PASSWORD "123123123"
#include <user_interface.h>

String powerName[3] = {"default", "pump", "ultraSonic"};

uint8_t mac[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.hostname("Rec_Wifi_by_oliver");
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

String data;

void loop() {
  data = "";
  FirebaseObject ob = Firebase.get("power");
  for (int i = 0; i < 3; i++) {
    data += powerName[i] + ":" + ob.getString(powerName[i]) + ",";
  }
  Serial.print(data);
}
