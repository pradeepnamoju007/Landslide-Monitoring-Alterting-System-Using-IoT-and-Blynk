#define BLYNK_TEMPLATE_ID "TMPL35FjEJVLx"
#define BLYNK_TEMPLATE_NAME "LandSlide Monitoring"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Blynk authentication token (You will get this from the Blynk app)
char auth[] = "rxAoDeQiWz8tLwhYueYZgFwTW8Fi4tAZ";

// WiFi credentials
char ssid[] = "Jarvis";
char pass[] = "12345678";

// Variables for Sensor Data
float temperature, humidity;
int soilMoistureValue, rainSensorValue, vibrationSensorValue;
long distance;

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);

  // Small delay for stability
  delay(2000);
}

void loop() {
  // Run Blynk
  Blynk.run();

  // Read data from Arduino
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    parseAndSendToBlynk(data);
  }
}

void parseAndSendToBlynk(String data) {
  // Parse CSV data received from Arduino
  int index = 0;
  String values[6];
  for (int i = 0; i < data.length(); i++) {
    if (data[i] == ',') {
      index++;
    } else {
      values[index] += data[i];
    }
  }

  // Convert values to respective types
  temperature = values[0].toFloat();
  humidity = values[1].toFloat();
  soilMoistureValue = values[2].toInt();
  rainSensorValue = values[3].toInt();
  vibrationSensorValue = values[4].toInt();
  distance = values[5].toInt();

  // Send data to Blynk
  Blynk.virtualWrite(V1, temperature);        // Temperature
  Blynk.virtualWrite(V2, humidity);          // Humidity
  Blynk.virtualWrite(V3, soilMoistureValue); // Soil Moisture
  Blynk.virtualWrite(V4, vibrationSensorValue);   // Rain
  Blynk.virtualWrite(V5, rainSensorValue); // Vibration
  Blynk.virtualWrite(V6, distance);          // Ultrasonic Distance
  
}

