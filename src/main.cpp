#include <Arduino.h>
#include <GyverOS.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

GyverOS<5> OS;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const String textConnect = "Connect";

const char* ssid = "WIFI_SSID";       // your WIFI name
const char* password = "WIFI_PASS";   // your WIFI password

const char* host = "api.coindesk.com";
const char* url = "/v1/bpi/currentprice.json";
const int httpsPort = 443;

int second = 1;

DynamicJsonDocument doc(1024);
WiFiClientSecure client;

void printBitcoinRate() {
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  String res = client.readString();
  Serial.println(res);
  String json = res.substring(res.indexOf('{'), res.indexOf("}}}"));
  // add removed symbols
  json += "}}}";
  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  JsonObject btc = doc.as<JsonObject>();
  String rate = btc[String("bpi")][String("USD")][String("rate_float")];
  lcd.clear();
  lcd.home();
  lcd.print(rate + " BTC");
  Serial.println("closing connection");
}

void clearingDots() {
  for (unsigned int i = textConnect.length(); i <= textConnect.length() + 3; i++) {
    lcd.setCursor(i, 0);
    lcd.print(" ");
  }
  lcd.setCursor(textConnect.length(), 0);
}

void runnigDots() {
  static int countOfDots = 1;
  if (countOfDots == 4) {
    clearingDots();
    countOfDots = 1;
    return;
  }
  lcd.setCursor(textConnect.length() + countOfDots - 1, 0);
  lcd.print(".");
  countOfDots++;
}

void connect() {
  if (WiFi.status() == WL_CONNECTED) {
    OS.stop(0);
    OS.stop(1);
    OS.stop(2);
    lcd.clear();
    lcd.home();
    OS.start(3);
    Serial.println("Connected to wifi");
  }
}

void countSeconds() {
  lcd.setCursor(0, 1);
  lcd.print(second++);
  lcd.print("s");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  lcd.init();
  lcd.backlight();
  lcd.print(textConnect);
  lcd.setCursor(0, 1);
  lcd.print(String(second++) + "s");
  client.setInsecure();
  client.setTimeout(15000);

  OS.attach(0, runnigDots, 300);
  OS.attach(1, connect, 1000);
  OS.attach(2, countSeconds, 1000);
  OS.attach(3, printBitcoinRate, 1000);
  OS.stop(3);
}

void loop() {
  OS.tick();
}
