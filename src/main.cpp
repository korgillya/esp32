#include <Arduino.h>


#define CONTROL_LED_PIN_BLUE 14
#define CONTROL_LED_PIN_YELL 13
#define CONTROL_LED_PIN_GREEN 12
#define CONTROL_LED_PIN_RED 11

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  Serial.begin(115200);

  pinMode(CONTROL_LED_PIN_BLUE , OUTPUT);
  pinMode(CONTROL_LED_PIN_YELL , OUTPUT);
  pinMode(CONTROL_LED_PIN_GREEN , OUTPUT);
  pinMode(CONTROL_LED_PIN_RED , OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.begin(115200);

   // put your main code here, to run repeatedly:
  Serial.println("LED on");
  digitalWrite(CONTROL_LED_PIN_BLUE , HIGH); // Turn the LED on
  delay(500);                     // Wait for a second
  Serial.println("LED off");
  digitalWrite(CONTROL_LED_PIN_BLUE , LOW);  // Turn the LED off
  delay(500);


    digitalWrite(CONTROL_LED_PIN_YELL , HIGH); // Turn the LED on
  delay(500);                     // Wait for a second
  Serial.println("LED off");
  digitalWrite(CONTROL_LED_PIN_YELL , LOW);  // Turn the LED off
  delay(500);

    digitalWrite(CONTROL_LED_PIN_GREEN , HIGH); // Turn the LED on
  delay(500);                     // Wait for a second
  Serial.println("LED off");
  digitalWrite(CONTROL_LED_PIN_GREEN , LOW);  // Turn the LED off
  delay(500);

    digitalWrite(CONTROL_LED_PIN_RED , HIGH); // Turn the LED on
  delay(500);                     // Wait for a second
  Serial.println("LED off");
  digitalWrite(CONTROL_LED_PIN_RED , LOW);  // Turn the LED off
  delay(500);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}