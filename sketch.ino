#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int ledPin = 2;
const int buttonPin = 4;
const int buzzerPin = 5;

bool ledOn = false;
bool lastButtonState = HIGH;

int XP = 0;
unsigned long startTime = 0;
unsigned long reactionTime = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(115200);
  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Reaction Game");
  lcd.setCursor(0,1);
  lcd.print("Press Button");
}

void loop() {
  bool currentButtonState = digitalRead(buttonPin);

  //  BUTTON PRESSED (HIGH → LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW && !ledOn) {
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000, 200);
    startTime = millis();
    ledOn = true;
  }

  // BUTTON RELEASED (LOW → HIGH)
  if (lastButtonState == LOW && currentButtonState == HIGH && ledOn) {
    reactionTime = millis() - startTime;

    digitalWrite(ledPin, LOW);
    ledOn = false;

    addXP(50);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time:");
    lcd.print(reactionTime);
    lcd.print(" ms");

    lcd.setCursor(0,1);
    lcd.print("XP:");
    lcd.print(XP);

    Serial.print("Reaction Time: ");
    Serial.println(reactionTime);

    delay(1500);
    lcd.clear();
    lcd.print("Press Button");
  }

  lastButtonState = currentButtonState;
  delay(20); // debounce
}

void addXP(int value) {
  XP += value;
  Serial.print("XP = ");
  Serial.println(XP);
}
