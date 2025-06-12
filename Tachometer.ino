#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change to 0x27 if needed

volatile unsigned long lastPulseTime = 0;
volatile unsigned long pulseInterval = 0;
float rpm = 0;
unsigned long lastDisplayTime = 0;
unsigned long lastValidPulseTime = 0;

void isr() {
  unsigned long now = micros();
  if (now - lastPulseTime > 100000) { // Debounce: 100ms = max 600 RPM
    pulseInterval = now - lastPulseTime;
    lastPulseTime = now;
    lastValidPulseTime = millis(); // Record last time a real pulse happened
  }
}

void setup() {
  lcd.begin();
  lcd.backlight();
  attachInterrupt(digitalPinToInterrupt(2), isr, RISING);
  lcd.setCursor(0, 0);
  lcd.print("  RPM Meter");
  delay(1000);
  lcd.clear();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("  Tachometer");

  if (millis() - lastValidPulseTime <= 2000 && pulseInterval > 0) {
    rpm = 60000000.0 / pulseInterval;
  } else {
    rpm = 0;
  }

  lcd.setCursor(0, 1);
  lcd.print("RPM: ");
  lcd.print((int)rpm);
  lcd.print("     ");  // Clear leftover digits

  delay(500);
}
