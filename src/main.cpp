#include <Arduino.h>

#define BUTTON_INPUT 13
#define PHOTORESISTOR_INPUT 12
#define CONTROL_LED_PIN_GREEN 14
#define POTENTIOMETER_INPUT 11
#define SERVO_OUTPUT 10

#define U_REFERENCE 3.1
#define MAX_DIGITAL 4095.0
#define THRESHOLD_VOLTAGE 1.1

const int freq = 5000;    // Частота 5 кГц
const int ledChannel = 0; // Канал ШІМ
const int resolution = 8; // Роздільна здатність 8 біт (0-255)

int currentMode = 0;        // Поточний режим
const int maxModes = 2;     // Кількість режимів (наприклад, 0, 1)

bool lastButtonState = LOW; // Попередній стан кнопки (HIGH через INPUT_PULLUP)


void applyMode(int, int, float, int);

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_INPUT, INPUT); // Використовуємо вбудовану підтяжку
  pinMode(CONTROL_LED_PIN_GREEN, OUTPUT);
  pinMode(POTENTIOMETER_INPUT, INPUT);

  analogSetAttenuation(ADC_11db);
  Serial.println("Система готова. Режим: 0");
}

void loop() {
 bool reading = digitalRead(BUTTON_INPUT);

  int rawValue = analogRead(PHOTORESISTOR_INPUT);

  int potValue = analogRead(POTENTIOMETER_INPUT); // 0 - 4095

  // Convert raw value to voltage (approx)
  float voltage = (rawValue / MAX_DIGITAL) * U_REFERENCE; 

  // Перевіряємо перехід з HIGH (натиснуто) в LOW (відпущено)
  if (lastButtonState == HIGH && reading == LOW) {
    delay(50); // Простий антибрязкіт
    
    currentMode++;
    if (currentMode > 1) currentMode = 0; // Наприклад, 3 режими
    
    Serial.print("Кнопку відпущено! Поточний режим: ");
    Serial.println(currentMode);
  }

  applyMode(currentMode, rawValue, voltage, potValue);

  // Запам'ятовуємо стан для наступного циклу
  lastButtonState = reading;
}

// void handleMode(int photoResistorInput, float voltage) {
  
// }

void applyMode(int mode, int photoResistorInput, float voltage, int potValue) {
  switch(mode) {
    case 0: { 

        Serial.println(photoResistorInput);
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
    case 1: 
      // Перетворюємо значення в мікросекунди для періоду в 10мс (100 Гц)
      // 10000 мкс - це повний цикл. 
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
}