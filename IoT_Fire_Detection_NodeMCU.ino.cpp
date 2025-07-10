#define BLYNK_TEMPLATE_ID "Your_Template_ID"
#define BLYNK_DEVICE_NAME "Fire Alert System"
#define BLYNK_AUTH_TOKEN "Your_Auth_Token"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Your_SSID";    // Your Wi-Fi name
char pass[] = "Your_PASSWORD"; // Your Wi-Fi password

#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define FLAME_SENSOR D1
#define BUZZER D6
#define LED D7

int flameState = HIGH;

BlynkTimer timer;

void sendSensorData() {
  float temp = dht.readTemperature();
  Blynk.virtualWrite(V0, temp);

  flameState = digitalRead(FLAME_SENSOR);
  Blynk.virtualWrite(V1, flameState == LOW ? 1 : 0);

  if (flameState == LOW || temp > 50) {
    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED, HIGH);
    Blynk.logEvent("fire_alert", "🔥 Fire or high temperature detected!");
  } else {
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, LOW);
  }
}

void setup() {
  pinMode(FLAME_SENSOR, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED, LOW);

  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}
