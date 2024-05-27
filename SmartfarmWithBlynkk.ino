#define BLYNK_TEMPLATE_ID "TMPL6hBCNVxxc" //you can change this based on your blink 
#define BLYNK_TEMPLATE_NAME "Smart farm with IOT" //you can change this based on your blink 
#define BLYNK_AUTH_TOKEN "evpWkJUQw1tok6ieYh8OQmbG6xe-uvOt" //you can change this based on your blink 

#define BLYNK_PRINT Serial
#include <WiFi.h> //becarefull when installing library
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "";  // type your wifi name
char pass[] = "";  // type your wifi password

BlynkTimer timer;

#define DHTPIN 4 // Connect DHT sensor to GPIO 4 (D2) in ESP32
#define SOIL_MOISTURE_PIN 34 // Connect soil moisture sensor to GPIO 34 (ADC1_CH6) in ESP32
#define DHTTYPE DHT11
#define WATER_PUMP_PIN 5 // Define the GPIO pin for the water pump

DHT dht(DHTPIN, DHTTYPE);

void sendSensor()
{
  int soilmoisture = analogRead(SOIL_MOISTURE_PIN);  // Read from the soil moisture sensor
  int soilmoisturepercentage = map(soilmoisture, 3500, 4095, 100, 0);
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Control the water pump based on soil moisture level
  if (soilmoisturepercentage < 20) {
    digitalWrite(WATER_PUMP_PIN, HIGH); // Turn on the water pump
  } else {
    digitalWrite(WATER_PUMP_PIN, LOW); // Turn off the water pump
  }
  
  Blynk.virtualWrite(V0, soilmoisturepercentage);
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
  
  Serial.print("Soil Moisture : ");
  Serial.print(soilmoisturepercentage);
  Serial.print(" Temperature : ");
  Serial.print(t);
  Serial.print("    Humidity : ");
  Serial.println(h);
}

void setup()
{
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  dht.begin();
  
  pinMode(WATER_PUMP_PIN, OUTPUT); // Initialize the water pump pin as an output
  
  timer.setInterval(100L, sendSensor); // Set the timer to call sendSensor every 100 ms
}

void loop()
{
  Blynk.run();
  timer.run();
}
