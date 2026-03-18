#include <Arduino.h>

#define BLYNK_TEMPLATE_ID "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME "Device"
#define BLYNK_AUTH_TOKEN "zSzaOeEbANBJVQek2o8y4cmxIRhpBEpU"


#define BLYNK_PRINT Serial

// Khai báo thư viện

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>


#include "WiFiManager.h"  

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Khai báo biến
#define Button1 4
#define Button2 16
#define Button3 17
#define Button4 18
#define gas 34
#define gas1 35
#define R1 27
#define R2 26
#define R3 25
#define R4 33
#define Buzzer 32
uint8_t dissplay = 0;

bool Lamp = 0;
bool Lamp1 = 0;
bool Fan = 0;
bool Fan1 = 0;
WiFiManager wifiManager;
char ssid[] = "";
char pass[] = "";

#define DHTPIN 13
#define DHTPIN1 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
DHT dht1(DHTPIN1, DHTTYPE);
BlynkTimer timer;

unsigned long previousMillis = 0; 
const long interval = 2000;  
// Khai báo hàm


void configModeCallback(WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void sendSensor() {
  // Đọc dữ liệu từ cảm biến
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();  
 
  // Gửi dữ liệu lên Blynk
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, t);
  Blynk.virtualWrite(V3, h1);
  Blynk.virtualWrite(V4, t1);
  uint16_t value = analogRead(gas);
  uint16_t value1 = analogRead(gas1);
  Blynk.virtualWrite(V5, value);
  Blynk.virtualWrite(V6, value1);
  if (value >= 800 || value1 >= 800) {
    digitalWrite(Buzzer, HIGH);
  } else {
    digitalWrite(Buzzer, LOW);
  }

unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  // Hiển thị lên màn hình LCD
    char buffer[16];
    char buffer1[16];
    if (dissplay < 4) {
      if (value >= 800) {
        dissplay = 4;
      } else if (value1 >= 800) {
        dissplay = 5;
      } else if (dissplay > 3) {
        dissplay = 0;
      }
    }

    switch (dissplay) {
      case 0:

        lcd.setCursor(0, 0);
        sprintf(buffer, "Room1: GAS %d", value);
        lcd.print(buffer);
        lcd.setCursor(0, 1);

        sprintf(buffer1, "Lamp %d  Fan %d  ", Lamp, Fan);
        lcd.print(buffer1);
        
        dissplay = 1;
        break;
      case 1:
        lcd.setCursor(0, 0);
        sprintf(buffer, "  Temp1  Humi1 ", value);
        lcd.print(buffer);
        lcd.setCursor(0, 1);

        sprintf(buffer1, "   %d C   %d", (uint8_t)t, (uint8_t)h);
        lcd.print(buffer1);
        lcd.print(" %");
        
        dissplay = 2;
        break;
      case 2:
        lcd.setCursor(0, 0);

        sprintf(buffer, "Room2: GAS %d", value1);
        lcd.print(buffer);
        lcd.setCursor(0, 1);

        sprintf(buffer1, "Lamp %d  Fan %d  ", Lamp1, Fan1);
        lcd.print(buffer1);
        
        dissplay = 3;
        break;
      case 3:
        lcd.setCursor(0, 0);
        sprintf(buffer, "  Temp2  Humi2 ", value);
        lcd.print(buffer);
        lcd.setCursor(0, 1);
        sprintf(buffer1, "   %d C   %d", (uint8_t)t1, (uint8_t)h1);
        lcd.print(buffer1);
        lcd.print(" %");
        
        dissplay = 0;
        break;
      case 4:
        lcd.setCursor(0, 0);

        sprintf(buffer, "Room1: GAS %d", value);
        lcd.print(buffer);
        lcd.setCursor(0, 1);


        lcd.print("Vuot qua nguong");
        if (value1 >= 800) {
          dissplay = 5;
        } else dissplay = 0;
        break;
      case 5:
        lcd.setCursor(0, 0);

        sprintf(buffer, "Room2: GAS %d", value1);
        lcd.print(buffer);
        lcd.setCursor(0, 1);


        lcd.print("Vuot qua nguong");
        if (value >= 800) {
          dissplay = 4;
        } else dissplay = 0;
        break;
    }
  }
}
// Hàm được gọi khi giá trị của nút trên Blynk thay đổi
BLYNK_WRITE(V0) {
  Lamp = param.asInt();  
}
BLYNK_WRITE(V7) 
{
  Fan = param.asInt();  
}
BLYNK_WRITE(V8)  
{
  Lamp1 = param.asInt();  
}
BLYNK_WRITE(V9)  
{
  Fan1 = param.asInt(); 
}

void setup() {
  // Khởi tạo màn hình LCD
  lcd.init();  
  lcd.backlight();
  // Khởi tạo cổng Serial
  Serial.begin(115200);
  lcd.setCursor(0, 0);
  // Khởi tạo cổng Serial cho thư viện WiFiManager
  wifiManager.setAPCallback(configModeCallback);
  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    lcd.print("failed to connect");
    ESP.restart();

    delay(1000);
  }
  // Thanh cong thi bao ra man hinh
  Serial.println("connected...");
  lcd.print("Connected...");


  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.iot-cm.com", 8080);

  dht.begin();
  dht1.begin();
  // Khởi tạo cổng I/O
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);
  pinMode(Button3, INPUT_PULLUP);
  pinMode(Button4, INPUT_PULLUP);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(gas, INPUT);
  pinMode(gas1, INPUT);


  timer.setInterval(1000L, sendSensor);
  // Gửi dữ liệu từ Blynk về thiết bị
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V7);
  Blynk.syncVirtual(V8);
  Blynk.syncVirtual(V9);
}

void loop() {
  // Chạy Blynk
  Blynk.run();
  timer.run();
  // Điều khiển thiết bị từ nút nhấn trên blynk
  if (Lamp == 1) {
    digitalWrite(R1, HIGH);
  } else {
    digitalWrite(R1, LOW);
  }
  if (Lamp1 == 1) {
    digitalWrite(R2, HIGH);
  } else {
    digitalWrite(R2, LOW);
  }
  if (Fan == 1) {
    digitalWrite(R3, HIGH);
  } else {
    digitalWrite(R3, LOW);
  }
  if (Fan1 == 1) {
    digitalWrite(R4, HIGH);
  } else {
    digitalWrite(R4, LOW);
  }

  // Đọc trạng thái của nút nhấn và điều khiển thiết bị
  if (digitalRead(Button1) == LOW) {
    while (digitalRead(Button1) == LOW)
      delay(100);
    Lamp = 1 - Lamp;
    digitalWrite(R1, Lamp);
    Blynk.virtualWrite(V0, Lamp);
  }
  if (digitalRead(Button2) == LOW) {
    while (digitalRead(Button2) == LOW)
      delay(100);
    Fan = 1 - Fan;
    digitalWrite(R3, Fan);
    Blynk.virtualWrite(V7, Fan);
  }
  if (digitalRead(Button3) == LOW) {
    while (digitalRead(Button3) == LOW)
      delay(100);
    Lamp1 = 1 - Lamp1;
    digitalWrite(R2, Lamp1);
    Blynk.virtualWrite(V8, Lamp1);
  }
  if (digitalRead(Button4) == LOW) {
    while (digitalRead(Button4) == LOW)
      delay(100);
    Fan1 = 1 - Fan1;
    digitalWrite(R4, Fan1);
    Blynk.virtualWrite(V9, Fan1);
  }
}