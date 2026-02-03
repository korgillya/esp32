#include <Arduino.h>

#define CONTROL_LED_PIN_BLUE 14
#define CONTROL_LED_PIN_YELL 13
#define CONTROL_LED_PIN_GREEN 12
#define CONTROL_LED_PIN_RED 11

// put function declarations here:
int myFunction(int, int);

void setup()
{
  // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  Serial.begin(115200);

  pinMode(CONTROL_LED_PIN_BLUE, OUTPUT);
  pinMode(CONTROL_LED_PIN_YELL, OUTPUT);
  pinMode(CONTROL_LED_PIN_GREEN, OUTPUT);
  pinMode(CONTROL_LED_PIN_RED, OUTPUT);
}

void loop()
{

  //array of leds
  uint16_t pins[] = {CONTROL_LED_PIN_BLUE, CONTROL_LED_PIN_YELL, CONTROL_LED_PIN_GREEN, CONTROL_LED_PIN_RED};

  for (int i = 0; i < 4; i++) {
    digitalWrite(pins[i], HIGH); // Turn the LED on
    delay(500); // Wait for a half of second
    Serial.println("LED off");
    digitalWrite(pins[i], LOW); // Turn the LED off
    delay(500); // Wait for another half of second            
  }

}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}