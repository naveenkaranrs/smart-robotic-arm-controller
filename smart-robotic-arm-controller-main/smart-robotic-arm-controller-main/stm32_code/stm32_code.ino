#include <Servo.h>

// Array of 4 servos
Servo servos[4];

// UART pins: PA9 (TX), PA10 (RX) - default HardwareSerial
HardwareSerial Serial1(PA10, PA9);  // RX, TX

int currentAngles[4] = {0, 0, 0, 0};  // Default positions (0-based index)

void setup() {
  // Initialize servos on PWM pins
  servos[0].attach(PA8);   // Servo 1
  servos[1].attach(PB6);   // Servo 2
  servos[2].attach(PB7);   // Servo 3
  servos[3].attach(PA15);  // Servo 4
  
  // Center all servos
  for (int i = 0; i < 4; i++) {
    servos[i].write(currentAngles[i]);
  }
  
  // Initialize UART at 115200 baud
  Serial1.begin(115200);
  
  // Use Serial for debugging (USB to PC)
  Serial.begin(115200);
  Serial.println("STM32 4-Servo Ready - Waiting for data...");
}

void loop() {
  // Check for incoming data from ESP8266
  if (Serial1.available()) {
    String receivedData = Serial1.readStringUntil('\n');  // Read full line
    receivedData.trim();  // Remove whitespace
    
    // Log reception with timestamp
    Serial.print("Data received at ");
    Serial.print(millis());
    Serial.print("ms: '");
    Serial.print(receivedData);
    Serial.println("'");
    
    // Parse "X:Y" format (X=1-4, Y=0-180)
    int colonIndex = receivedData.indexOf(':');
    if (colonIndex > 0) {
      int servoId = receivedData.substring(0, colonIndex).toInt() - 1;  // 0-based
      int angle = receivedData.substring(colonIndex + 1).toInt();
      
      Serial.print("  Parsed: Servo ID=");
      Serial.print(servoId + 1);
      Serial.print(", Angle=");
      Serial.print(angle);
      Serial.println();
      
      // Validate and set
      if (servoId >= 0 && servoId < 4 && angle >= 0 && angle <= 180) {
        currentAngles[servoId] = angle;
        servos[servoId].write(angle);
        Serial.println("  -> Valid - Servo " + String(servoId + 1) + " set to " + String(angle) + " degrees");
      } else {
        Serial.println("  -> Invalid servo ID or angle - Ignored");
      }
    } else {
      Serial.println("  -> Invalid format (expected 'X:Y') - Ignored");
    }
    Serial.println("---");
  }
  
  delay(10);  // Small delay for stability
}
stm code