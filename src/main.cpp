#include <Arduino.h>
#include <ESP32Servo.h>
#include "esp_cpu.h"


constexpr uint8_t BUTTON_INPUT =  13;
constexpr uint8_t PHOTORESISTOR_INPUT = 12;
constexpr uint8_t CONTROL_LED_PIN_GREEN = 14;
constexpr uint8_t POTENTIOMETER_INPUT = 11;
constexpr uint8_t SERVO_OUTPUT = 10;

static const float U_REFERENCE = 3.1;
static const float MAX_DIGITAL = 4095.0;

enum class CurrentMode { PHOTORESISTOR, POTENTIOMETER, SERVO };

void applyMode(CurrentMode mode, int photoInput, float voltage, int potValue, Servo &servo);

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_INPUT, INPUT);
  pinMode(CONTROL_LED_PIN_GREEN, OUTPUT);
  pinMode(POTENTIOMETER_INPUT, INPUT);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  //moved global variables to local
  static uint32_t start_cycles;
  static uint32_t end_cycles;
  static CurrentMode currentMode = CurrentMode::PHOTORESISTOR;
  static bool lastButtonState = LOW;

  static Servo myServo;
  static bool isServoInitialized = false;
  if(!isServoInitialized) {
    myServo.attach(SERVO_OUTPUT);
    isServoInitialized = true;
  }

  start_cycles = esp_cpu_get_ccount();

  bool reading = digitalRead(BUTTON_INPUT);

  int rawValue = analogRead(PHOTORESISTOR_INPUT);

  int potValue = analogRead(POTENTIOMETER_INPUT); // 0 - 4095

  // Convert raw value to voltage (approx)
  float voltage = (rawValue / MAX_DIGITAL) * U_REFERENCE; 

  if (lastButtonState == HIGH && reading == LOW) {
    int nextMode = static_cast<int>(currentMode) + 1;
    if (nextMode > static_cast<int>(CurrentMode::SERVO)) nextMode = 0;
    currentMode = static_cast<CurrentMode>(nextMode);
    Serial.println(nextMode);
  }

  applyMode(currentMode, rawValue, voltage, potValue, myServo);

  lastButtonState = reading;

  end_cycles = esp_cpu_get_ccount();
  uint32_t spent_cycles = end_cycles - start_cycles;
  float seconds = spent_cycles / (ESP.getCpuFreqMHz() * 1000000.0);

  Serial.printf("Cycles: %u | Freq: %u MHz | Core: %d | Time spent: %.9f s\n", spent_cycles, ESP.getCpuFreqMHz(), xPortGetCoreID(), seconds);
}

void applyMode(CurrentMode mode, int photoResistorInput, float voltage, int potValue, Servo &servo) {
  switch(mode) {
    case CurrentMode::PHOTORESISTOR: { 
        int onTimePhotoresistor = map(photoResistorInput, 500, 2200, 0, 10000); 
        int offTimePhotoresistor = 10000 - onTimePhotoresistor;

        if (onTimePhotoresistor > 0) {
            digitalWrite(CONTROL_LED_PIN_GREEN, HIGH);
            delayMicroseconds(onTimePhotoresistor);
        }
        if (offTimePhotoresistor > 0) {
            digitalWrite(CONTROL_LED_PIN_GREEN, LOW);
            delayMicroseconds(offTimePhotoresistor);
        };
      break;
    }
    case CurrentMode::POTENTIOMETER: {
      int onTime = map(potValue, 0, 4095, 0, 10000); 
      int offTime = 10000 - onTime;
      if (onTime > 0) {
          digitalWrite(CONTROL_LED_PIN_GREEN, HIGH);
          delayMicroseconds(onTime);
      }
      if (offTime > 0) {
          digitalWrite(CONTROL_LED_PIN_GREEN, LOW);
          delayMicroseconds(offTime);
      }
     break;
    }
    case CurrentMode::SERVO: {
      int angle = map(potValue, 0, 4095, 0, 180);
      Serial.println(angle);
      servo.write(angle);
    }
  }
}