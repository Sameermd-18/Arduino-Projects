#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
int sensor_pin = A0;
int relay_pin = 7;
char Data;
int Mode = 0; // 0 = Manual, 1 = Auto

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.backlight();
  pinMode(sensor_pin, INPUT);
  pinMode(relay_pin, OUTPUT);
}

void loop() {
  // Read Serial Input
  if (Serial.available() > 0) {
    Data = Serial.read();
    Serial.println(Data);
    
    if (Data == 'A') {
      Mode = 0;
      digitalWrite(relay_pin, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Manual: Motor ON ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
    } else if (Data == 'B') {
      Mode = 0;
      digitalWrite(relay_pin, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Manual: Motor OFF");
      lcd.setCursor(0, 1);
      lcd.print("                ");
    } else if (Data == 'Z') {
      Mode = 1;
      lcd.setCursor(0, 0);
      lcd.print("Auto Mode Active ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
  }

  // Auto Mode Logic
  if (Mode == 1) {
    int data = analogRead(sensor_pin);
    Serial.println(data);

    if (data >= 950) {
      Serial.println("No moisture, soil is dry");
      digitalWrite(relay_pin, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Soil Dry        ");
      lcd.setCursor(0, 1);
      lcd.print("Motor ON        ");
    } else if (data <= 700 && data >= 400) {
      Serial.println("Little moisture, soil is wet");
      digitalWrite(relay_pin, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Soil Wet        ");
      lcd.setCursor(0, 1);
      lcd.print("Motor OFF       ");
    } else if (data < 400) {
      Serial.println("Full moisture, soil is very wet");
      digitalWrite(relay_pin, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Soil Very Wet   ");
      lcd.setCursor(0, 1);
      lcd.print("Motor OFF       ");
    }

    delay(1000); // To avoid flickering and overloading LCD
  }

  delay(100); // Slow down loop a little in Manual mode too
}
