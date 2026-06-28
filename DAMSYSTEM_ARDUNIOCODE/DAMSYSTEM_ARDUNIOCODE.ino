#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// SoftwareSerial for ESP8266
SoftwareSerial esp(6, 7); // RX, TX

// Sensors & LEDs
const int trigPin = 9;
const int echoPin = 8;
const int floatPin = A0;
const int ledLow = A1;
const int ledHigh = A2;

long duration;
int distance;
bool lastHigh = false;
bool lastLow = false;

void setup() {
  Serial.begin(9600);
  esp.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(floatPin, INPUT_PULLUP);
  pinMode(ledLow, OUTPUT);
  pinMode(ledHigh, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Hi Welcome");
  delay(1500);
  lcd.clear();
  lcd.print("Dam Monitoring");
  lcd.setCursor(0, 1);
  lcd.print("System Loading..");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Ultrasonic
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 30000);
  distance = (duration * 0.034) / 2;

  int fs = digitalRead(floatPin);

  lcd.setCursor(0, 0);
  lcd.print("Dist:");
  lcd.print(distance);
  lcd.print("cm  ");

  lcd.setCursor(0, 1);

  if (fs == LOW) {  // Float low → water low
    lcd.print("LOW LEVEL - SAFE ");
    digitalWrite(ledLow, HIGH);
    digitalWrite(ledHigh, LOW);

    if (!lastLow) {
      esp.println("LOW");         // send to ESP
      Serial.println("Sent: LOW");
      lastLow = true;
      lastHigh = false;
    }
  } else {           // Float high → water high
    lcd.print("HIGH LEVEL-UNSAFE");
    digitalWrite(ledLow, LOW);
    digitalWrite(ledHigh, HIGH);

    if (!lastHigh) {
      esp.println("HIGH");        // send to ESP
      Serial.println("Sent: HIGH");
      lastHigh = true;
      lastLow = false;
    }
  }

  delay(2000);
}
