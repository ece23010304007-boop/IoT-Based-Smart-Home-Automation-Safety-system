# IoT-Based Smart Home Environmental Monitoring and Gas Safety System

An advanced IoT-based safety and environmental monitoring system powered by an **ESP32 microcontroller**, designed to track ambient temperature/humidity and detect hazardous gas or smoke leaks in real-time via the **Blynk IoT platform**.

---

##  Features
* **Hazard & Gas Detection:** Real-time monitoring of smoke, LPG, and flammable gases using the MQ-2 sensor to prevent fire accidents.
* **Environmental Tracking:** Continuous tracking of ambient temperature and humidity using the DHT11 sensor.
* **Cloud Integration:** Live data synchronization and visualization on the Blynk Console dashboard.
* **Instant Alerts & Monitoring:** Remote access to sensor parameters from anywhere using the Blynk mobile/web application.

---

##  Hardware Components
1. **ESP32 Microcontroller Module** (Wi-Fi enabled)
2. **MQ-2 Gas / Smoke Sensor** (for safety and leak detection)
3. **DHT11 Sensor** (for temperature and humidity monitoring)
4. **Connecting Wires & Breadboard**
5. **Power Supply / USB Cable**

---

##  Software & Cloud Platform
* **Arduino IDE** (for compiling and uploading firmware)
* **Blynk IoT Cloud Console** (for setting up gauges and hazard alerts)
* **Required Libraries:**
  * `WiFi.h`
  * `BlynkSimpleEsp32.h`
  * `DHT.h`

---

##  Blynk Dashboard Preview
* Real-time metrics display current **Temperature (°C)** and **Humidity (%)**.
* Gas sensor monitoring status displayed dynamically on the cloud console.

---

## 📄 Repository Structure
* `/firmware/` - Contains the source code files (`.ino`) for the ESP32.
* `/assets/` - Contains Blynk dashboard screenshots and circuit references.

---
Project code

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

//================= WiFi =================
#define WIFI_SSID "Ai-BO8"
#define WIFI_PASSWORD "Dipanshu1"

//================ Firebase ==============
#define API_KEY "AIzaSyD6DJR0ZQa78WCCeT4Zwm3VYMo-ntaIFfE"

#define DATABASE_URL "https://firedetector-9c17f-default-rtdb.firebaseio.com/"

#define USER_EMAIL "test@gmail.com"
#define USER_PASSWORD "12345678"

//=============== MQ2 ====================
#define MQ2 A0
#define BUZZER D6
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(MQ2, INPUT);
  pinMode(BUZZER,OUTPUT);
  digitalWrite(BUZZER,LOW);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}
void loop()
{
  if (Firebase.ready() &&
      (millis() - sendDataPrevMillis > 2000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

    int gasValue = analogRead(MQ2);

    String status;

    if (gasValue > 1100)
    {
      status = "GAS DETECTED";
    digitalWrite(BUZZER,HIGH);
  }
    else
    {
      status = "SAFE";
    
    digitalWrite(BUZZER,LOW);
  }

    Serial.print("Gas Value : ");
    Serial.println(gasValue);

    Serial.print("Status : ");
    Serial.println(status);

    if (Firebase.setInt(fbdo, "/GasSensor/GasValue", gasValue))
      Serial.println("Gas Value Uploaded");
    else
      Serial.println(fbdo.errorReason());

    if (Firebase.setString(fbdo, "/GasSensor/Status", status))
      Serial.println("Status Uploaded");
    else
      Serial.println(fbdo.errorReason());








  

    Serial.println("-------------------------");
  }
}
