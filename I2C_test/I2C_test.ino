#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 or 0x3F (depending on your LCD module's I2C address)
// Adjust if necessary; common addresses are 0x27 and 0x3F.
LiquidCrystal_I2C lcd(0x27, 2, 16);

void setup() {
  lcd.init();    // Initialize the LCD
  lcd.backlight();     
  // lcd.noBacklight();   // Turn on the backlight
  lcd.clear();
}

void loop() {
  
  lcd.setCursor(1, 1); 
  lcd.printstr(1231); 

  // lcd.display();

}