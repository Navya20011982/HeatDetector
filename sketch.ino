#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int relayPin = 6;    // Relay for heater
const int redPin = 3;      // RGB LED Red
const int greenPin = 4;    // RGB LED Green
const int bluePin = 5;     // RGB LED Blue

// Temperature thresholds (°C)
const float targetTemp = 30.0;
const float stabilizeRange = 1.0; // ±1°C around target
const float overheatTemp = 35.0;

// State machine
enum State { Idle, Heating, Stabilizing, TargetReached, Overheat };
State currentState = Idle;

// Timing for periodic updates
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 2000; // Update every 2s (DHT22 needs time)

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  digitalWrite(relayPin, LOW); // Heater off
  setRGB(0, 0, 0); // RGB LED off
  
  Serial.begin(9600);
  dht.begin();
  Serial.println("Heater Control System Started");
}

void setRGB(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

float readTemperature() {
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1; // Indicate error
  }
  return temp;
}

void updateState(float temp) {
  if (temp < 0) return; // Skip if reading failed
  
  switch (currentState) {
    case Idle:
      if (temp < targetTemp - stabilizeRange) {
        currentState = Heating;
        digitalWrite(relayPin, HIGH); // Turn heater on
        setRGB(0, 0, 255); // Blue for Heating
      }
      break;
    case Heating:
      if (temp >= targetTemp - stabilizeRange) {
        currentState = Stabilizing;
        setRGB(0, 255, 255); // Cyan for Stabilizing
      } else if (temp >= overheatTemp) {
        currentState = Overheat;
        digitalWrite(relayPin, LOW); // Turn heater off
        setRGB(255, 0, 0); // Red for Overheat
      }
      break;
    case Stabilizing:
      if (temp >= targetTemp + stabilizeRange) {
        digitalWrite(relayPin, LOW); // Turn heater off
        currentState = TargetReached;
        setRGB(0, 255, 0); // Green for TargetReached
      } else if (temp < targetTemp - stabilizeRange) {
        currentState = Heating;
        digitalWrite(relayPin, HIGH); // Turn heater on
        setRGB(0, 0, 255); // Blue for Heating
      } else if (temp >= overheatTemp) {
        currentState = Overheat;
        digitalWrite(relayPin, LOW); // Turn heater off
        setRGB(255, 0, 0); // Red for Overheat
      }
      break;
    case TargetReached:
      if (temp < targetTemp - stabilizeRange) {
        currentState = Heating;
        digitalWrite(relayPin, HIGH); // Turn heater on
        setRGB(0, 0, 255); // Blue for Heating
      } else if (temp >= overheatTemp) {
        currentState = Overheat;
        digitalWrite(relayPin, LOW); // Turn heater off
        setRGB(255, 0, 0); // Red for Overheat
      }
      break;
    case Overheat:
      if (temp < targetTemp - stabilizeRange) {
        currentState = Heating;
        digitalWrite(relayPin, HIGH); // Turn heater on
        setRGB(0, 0, 255); // Blue for Heating
      } else if (temp < overheatTemp) {
        currentState = Stabilizing;
        setRGB(0, 255, 255); // Cyan for Stabilizing
      }
      break;
  }
}

void logStatus(float temp) {
  String stateStr;
  switch (currentState) {
    case Idle: stateStr = "Idle"; setRGB(0, 0, 0); break;
    case Heating: stateStr = "Heating"; break;
    case Stabilizing: stateStr = "Stabilizing"; break;
    case TargetReached: stateStr = "TargetReached"; break;
    case Overheat: stateStr = "Overheat"; break;
  }
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" C, State: ");
  Serial.print(stateStr);
  Serial.print(", Heater: ");
  Serial.println(digitalWrite(relayPin) ? "ON" : "OFF");
}

void loop() {
  if (millis() - lastUpdate >= updateInterval) {
    float temp = readTemperature();
    updateState(temp);
    logStatus(temp);
    lastUpdate = millis();
  }
}