# IoT-Based Temperature and Humidity Monitoring System

A real-time IoT-based environmental monitoring system powered by an **ESP32 microcontroller** and **DHT11 sensor**, designed to track ambient temperature and humidity seamlessly via the **Blynk IoT platform**.

---

## 🚀 Features
* **Real-Time Environmental Tracking:** Continuous monitoring of ambient temperature and humidity values.
* **Cloud Integration:** Live data synchronization and visualization on the Blynk Console dashboard.
* **Remote Monitoring:** Access environmental metrics from anywhere using the Blynk mobile/web application.
* **Interactive Dashboard:** Visual gauge widgets displaying live values for precise tracking.

---

## 🛠️ Hardware Components
1. **ESP32 Microcontroller Module** (Wi-Fi enabled)
2. **DHT11 Sensor** (Temperature & Humidity Sensor)
3. **Connecting Wires & Breadboard**
4. **Micro-USB Cable** (for programming and power supply)

---

## 💻 Software & Cloud Platform
* **Arduino IDE** (for compiling and uploading firmware)
* **Blynk IoT Cloud Console** (for setting up the dashboard and gauges)
* **Required Libraries:**
  * `WiFi.h`
  * `BlynkSimpleEsp32.h`
  * `DHT.h`

---

## 📊 Blynk Dashboard Preview
* The Blynk Console displays real-time circular gauge metrics for both **Temperature (°C)** and **Humidity (%)**.
* Data updates dynamically as environmental conditions change.

---

## 📄 Repository Structure
* `/firmware/` - Contains the source code files (`.ino`) for the ESP32.
* `/assets/` - Contains Blynk dashboard screenshots and system visual references.

---

Project code

/************************************************
   IoT Smart Temperature & Humidity Monitoring
   ESP8266 + DHT11 + Servo + Relay + Blynk
************************************************/

//=============== BLYNK ===================
#define BLYNK_TEMPLATE_ID "TMPL3JsTIoa-g"
#define BLYNK_TEMPLATE_NAME "tempraturedetector"
#define BLYNK_AUTH_TOKEN "HSmeA-Lzh7URLOTCY00jmRCfjxY2b4Dc"

//=============== LIBRARIES ===============
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Servo.h>

//=============== WIFI ====================
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

//=============== DHT11 ===================
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//=============== SERVO ===================
Servo servo;
#define SERVO_PIN D5

//=============== RELAY ===================
#define RELAY_PIN D1

//=============== TIMER ===================
BlynkTimer timer;

//=========================================
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println("DHT Error");
    return;
  }

  // Serial Monitor
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print(" °C   ");

  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.println(" %");

  // Send data to Blynk
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  // Automatic Window Control
  if (t > 30)
  {
    servo.write(90);      // Open Window
  }
  else if (t < 28)
  {
    servo.write(0);       // Close Window
  }
}

//=========================================
// Relay Control from Blynk Switch (V2)
//=========================================
BLYNK_WRITE(V2)
{
  int relayState = param.asInt();

  if (relayState == 1)
  {
    digitalWrite(RELAY_PIN, LOW);   // Relay ON
    Serial.println("Relay ON");
  }
  else
  {
    digitalWrite(RELAY_PIN, HIGH);  // Relay OFF
    Serial.println("Relay OFF");
  }
}

//=========================================
void setup()
{
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  dht.begin();

  servo.attach(SERVO_PIN);
  servo.write(0);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, sendSensor);
}

//=========================================
void loop()
{
  Blynk.run();
  timer.run();
}
