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

enum class LedState { ON, OFF };
enum class CurrentMode { PHOTORESISTOR, POTENTIOMETER, SERVO };

class Led {
private:
    uint8_t pin;

public:
    constexpr Led(uint8_t ledPin) : pin(ledPin) {}

    void init() const {
        pinMode(pin, OUTPUT);
    }

    void set(LedState state) const {
        if (state == LedState::ON) {
            digitalWrite(pin, HIGH);
        } else {
            digitalWrite(pin, LOW);
        }
    }
    
    void toggle() const {
        digitalWrite(pin, !digitalRead(pin));
    }
};

void applyMode(CurrentMode mode, int photoInput, float voltage, int potValue, Servo &servo, Led &led);

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_INPUT, INPUT);
  // pinMode(CONTROL_LED_PIN_GREEN, OUTPUT);
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
  static bool isInitialized = false;
  static Led myLed(CONTROL_LED_PIN_GREEN);

  if(!isInitialized) {
    myServo.attach(SERVO_OUTPUT);
    myLed.init();
    isInitialized = true;
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

  applyMode(currentMode, rawValue, voltage, potValue, myServo, myLed);

  lastButtonState = reading;

  end_cycles = esp_cpu_get_ccount();
  uint32_t spent_cycles = end_cycles - start_cycles;
  float seconds = spent_cycles / (ESP.getCpuFreqMHz() * 1000000.0);

  Serial.printf("Cycles: %u | Freq: %u MHz | Core: %d | Time spent: %.9f s\n", spent_cycles, ESP.getCpuFreqMHz(), xPortGetCoreID(), seconds);
}

void applyMode(CurrentMode mode, int photoResistorInput, float voltage, int potValue, Servo &servo, Led &led) {
  switch(mode) {
    case CurrentMode::PHOTORESISTOR: { 
        int onTimePhotoresistor = map(photoResistorInput, 500, 2200, 0, 10000); 
        int offTimePhotoresistor = 10000 - onTimePhotoresistor;

        if (onTimePhotoresistor > 0) {
          led.set(LedState::ON);
          delayMicroseconds(onTimePhotoresistor);
        }
        if (offTimePhotoresistor > 0) {
          led.set(LedState::OFF);
          delayMicroseconds(offTimePhotoresistor);
        };
      break;
    }
    case CurrentMode::POTENTIOMETER: {
      int onTime = map(potValue, 0, 4095, 0, 10000); 
      int offTime = 10000 - onTime;
      if (onTime > 0) {
        led.set(LedState::ON);
        delayMicroseconds(onTime);
      }
      if (offTime > 0) {
        led.set(LedState::OFF);
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