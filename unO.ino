#include <DHT.h>
#include <Ultrasonic.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define DHTPIN 2               // Pin connected to DHT11 data pin
#define DHTTYPE DHT11          // DHT11 sensor type
#define SOIL_MOISTURE_PIN A0   // Soil moisture sensor analog pin
#define RAIN_SENSOR_PIN A1     // Rain sensor analog pin
#define VIBRATION_SENSOR_PIN 4 // Vibration sensor digital pin
#define TRIG_PIN 6             // Ultrasonic sensor trigger pin
#define ECHO_PIN 7             // Ultrasonic sensor echo pin

// Initialize Sensors
DHT dht(DHTPIN, DHTTYPE);  
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);

// Initialize I2C LCD (address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables for Sensor Data
float temperature, humidity;
int soilMoistureValue;
int rainSensorValue;
int vibrationSensorValue;
long distance;

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);

  // Initialize Sensors
  dht.begin();
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(VIBRATION_SENSOR_PIN, INPUT);

  // Initialize LCD
  lcd.begin();
  lcd.backlight();  // Turn on LCD backlight

  // Initial Message on LCD
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);  // Allow sensors to stabilize
  lcd.clear();
}

void loop() {
  // Read Sensor Data
  soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  rainSensorValue = analogRead(RAIN_SENSOR_PIN);
  vibrationSensorValue = digitalRead(VIBRATION_SENSOR_PIN);
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  distance = ultrasonic.read();

  // Check for valid temperature and humidity readings
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading DHT sensor!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DHT Error!");
    delay(2000);
    return;
  }

  // Display Data on I2C LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C H:");
  lcd.print(humidity);
  lcd.print("% V:");
  lcd.print(vibrationSensorValue);

  lcd.setCursor(0, 1);
  lcd.print("S:");
  lcd.print(soilMoistureValue);
  lcd.print(" D:");
  lcd.print(distance);
  lcd.print(" R:");
  lcd.print(rainSensorValue < 800 ? "1" : "0");

  // Output Data to Serial Monitor
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  Serial.print(soilMoistureValue);
  Serial.print(",");
  Serial.print(rainSensorValue < 500 ? 1 : 0); // 1 for Rain, 0 otherwise
  Serial.print(",");
  Serial.print(vibrationSensorValue);
  Serial.print(",");
  Serial.println(distance);

  // Delay for stability
  delay(2000);
}
