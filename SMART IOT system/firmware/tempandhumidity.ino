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