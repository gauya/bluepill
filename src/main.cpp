#include <Arduino.h>

int ledPin = PC13;

// put function declarations here:
int myFunction(int, int);

void setup1() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);

  Serial.begin(115200);
  Serial.println("Hello");
  pinMode(ledPin, OUTPUT);
}

void loop1() {
  static int cnt=0, sw=0;
  delay(100);

  Serial.println(cnt++);
  digitalWrite(ledPin,sw);
  sw = !sw;
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}