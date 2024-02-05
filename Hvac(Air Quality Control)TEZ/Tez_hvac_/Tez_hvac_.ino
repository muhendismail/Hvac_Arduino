#define BLYNK_TEMPLATE_ID "TMPL6XRtt2x-B"
#define BLYNK_TEMPLATE_NAME "nodemcu"
#define BLYNK_AUTH_TOKEN "tumGM7TSzZaT2NzaPrs13VL7vayfITqr"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define BLYNK_PRINT Serial

char auth[] = BLYNK_AUTH_TOKEN;

//char ssid[] = "İsmail iPhone’u";  //  wifi ismi
//char pass[] = "ismailcep";  // wifi şifre
char ssid[] = "ASUS";
char pass[] = "asus1234";


#define fan_co2 D5  //For decreasing Co2 with a fan.
#define DecrHum D6   //For decreasing humidity with a fan.
#define IncrHum D8  //For increasing humidity with a fan.
#define Heater D2   //LED was used instead of heater. 
#define Cooling D4  //LED was used instead of cooling. 

#define DHTPIN D1
#define Co2 A0
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

bool manualControl = false; //Button first false(Automatic control active).

BLYNK_WRITE(V2) {
  manualControl = param.asInt() == HIGH; //Button control
}

BLYNK_WRITE(V3) {                    //Manuel control of heater.
  if (manualControl) {
    int Value = param.asInt();
    analogWrite(Heater, Value);
  }
}
BLYNK_WRITE(V1) {                    //Manuel control of cooler.
  if (manualControl) {
    int Value = param.asInt();
    analogWrite(Cooling, Value);
  }
}

void readSensors() {
  // Read sensor data
  float MQ_135 = analogRead(A0);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t) || isnan(MQ_135)) {
    Serial.println("Failed to read data from sensor!");
    return;
  }
  // Send data to Blynk app
  Blynk.virtualWrite(V6, h);
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V4, MQ_135);

  Serial.print("   Temperature : ");
  Serial.println(t);
  Serial.print("   Humidity : ");
  Serial.println(h);
  Serial.print("   Co2 : ");
  Serial.println(MQ_135);

  //Humidity control
  if (h > 75) {
    int humiditySetting = map(h, 0, 100, 0, 255);
    analogWrite(DecrHum, 250);
  }
  else if (h <= 50) {
    int humiditySetting = map(h, 0, 100, 0, 255);
    analogWrite(IncrHum, 250);

    // Heater setting
    int heaterSetting = map(t, 0, 100, 0, 255);
    // Cooler Setting
    int coolerSetting = map(t, 0, 100, 0, 255);
  }
}

void AutomaticControl() {
  if (!manualControl) {
    float Co2 = analogRead(A0);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int Co2Setting = map(Co2, 0, 3000, 0, 255);


    //if Co2 is smaller from 1000
    if (Co2 <= 1000) {
      if (t <= 18) {
        analogWrite(fan_co2, 100);
        analogWrite(Heater, 250);
        analogWrite(Cooling, 0);
      }
      else if (t > 18 && t <= 21) {
        analogWrite(fan_co2, 100);
        analogWrite(Heater, 100);
        analogWrite(Cooling, 0);
      }
      else if (t > 21 && t <= 25) {
        analogWrite(fan_co2, 100);
        analogWrite(Heater, 0);
        analogWrite(Cooling, 100);
      }
      else if (t > 25) {
        analogWrite(fan_co2, 100);
        analogWrite(Heater, 0);
        analogWrite(Cooling, 200);
      }
    }
    //if Co2 is bigger than 1000
    if (Co2 >= 1000) {
      if (t <= 18) {
        analogWrite(fan_co2, 200);
        analogWrite(Heater, 250);
        analogWrite(Cooling, 0);
      }
      else if (t > 18 && t <= 21) {
        analogWrite(fan_co2, 200);
        analogWrite(Heater, 100);
        analogWrite(Cooling, 0);
      }
      else if (t > 21 && t <= 25) {
        analogWrite(fan_co2, 200);
        analogWrite(Heater, 0);
        analogWrite(Cooling, 100);
      }
      else if (t > 25 && t <= 30) {
        analogWrite(fan_co2, 200);
        analogWrite(Heater, 0);
        analogWrite(Cooling, 200);
      }
      else if (t > 30) {
        analogWrite(fan_co2, 200);
        analogWrite(Heater, 0);
        analogWrite(Cooling, 250);
      }
    }
    delay(5000);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(fan_co2, OUTPUT);
  pinMode(DecrHum, OUTPUT);
  pinMode(IncrHum, OUTPUT);
  pinMode(Heater, OUTPUT);
  pinMode(Cooling, OUTPUT);

  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(2500L, readSensors);
}

void loop() {
  Blynk.run();
  timer.run();
  AutomaticControl();
}
