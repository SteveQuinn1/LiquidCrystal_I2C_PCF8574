// Copyright 2017 Steve Quinn
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

/*
 * Displays text sent over the serial port (e.g. from the Serial Monitor) on
 * an attached LCD.
 * Adapted from : http://hmario.home.xs4all.nl/arduino/LiquidCrystal_I2C/
 */
#include <Wire.h> 
#include <LiquidCrystal_I2C_PCF8574.h>

// Drop this code into Arduino development environment	
	
#define Blank0 0	
#define Blank1 1	
#define LineFeed 2	
#define Blank3 3	
#define Blank4 4	
#define CarriageReturn 5	
#define Blank6 6	
#define Blank7 7	
	
typedef struct {	
	const unsigned char array1[8];
} binaryArrayType;	
	
binaryArrayType binaryArray[8] =	
{	
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // Blank0, 0
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // Blank1, 1
	{0x10,0x10,0x10,0x1C,0x07,0x04,0x06,0x04},  // LineFeed, 2
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // Blank3, 3
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // Blank4, 4
	{0x1C,0x10,0x10,0x1C,0x07,0x05,0x06,0x05},  // CarriageReturn, 5
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // Blank6, 6
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}  // Blank7, 7
};	


LiquidCrystal_I2C_PCF8574 lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  // load in chars
  //for (unsigned char p = 0; p < (sizeof(charmap)/sizeof(unsigned char *)) ; p++)
  for (unsigned char p = 0; p < 8 ; p++)
  {
      lcd.createChar((uint8_t)p, (uint8_t *)&binaryArray[p]);
  }
  
  Serial.begin(9600);
}

void loop()
{
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      lcd.write(Serial.read());
    }
  }
}