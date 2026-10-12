#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// -------- MOTOR PINS --------
#define IN1 12
#define IN2 13
#define ENA 15

// -------- SENSOR PINS --------
#define PH_PIN 34
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// -------- PWM SETUP --------
const int pwmChannel = 0;
const int pwmFreq = 5000;
const int pwmResolution = 8;

int speedValue = 0;
String command = "";

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // ESP32 PWM (fixed for your error)
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(ENA, pwmChannel);

  sensors.begin();

  Serial.println("READY");
  Serial.println("Commands:");
  Serial.println("forward | reverse | stop | speed 0-255");
}

void loop() {

  // -------- SERIAL CONTROL --------
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    command.trim();

    // Direction
    if (command == "forward") {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      Serial.println("Motor FORWARD");
    }

    else if (command == "reverse") {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      Serial.println("Motor REVERSE");
    }

    else if (command == "stop") {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      Serial.println("Motor STOP");
    }

    // Speed control
    else if (command.startsWith("speed")) {
      int val = command.substring(6).toInt();

      if (val >= 0 && val <= 255) {
        speedValue = val;
        ledcWrite(pwmChannel, speedValue);

        Serial.print("Speed set to: ");
        Serial.println(speedValue);
      } else {
        Serial.println("Invalid speed (0-255)");
      }
    }
  }

  // -------- TEMPERATURE --------
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  // -------- PH SENSOR --------
  int phRaw = analogRead(PH_PIN);
  float voltage = phRaw * (3.3 / 4095.0);
  float pH = 7 + ((2.5 - voltage) / 0.18);

  // -------- PRINT DATA --------
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" °C | pH: ");
  Serial.println(pH);

  delay(2000);
}