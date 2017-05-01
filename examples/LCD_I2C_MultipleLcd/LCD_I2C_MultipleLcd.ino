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

#include <Wire.h> 
#include <LiquidCrystal_I2C_PCF8574.h>

// Adapted from : http://hmario.home.xs4all.nl/arduino/LiquidCrystal_I2C/ 
 
LiquidCrystal_I2C_PCF8574 lcd1(0x27,20,4);  // set the LCD address of the first lcd to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C_PCF8574 lcd2(0x23,20,4);  // set the LCD address of the second lcd to 0x23 for a 20 chars and 4 line display
 
void setup()
{
  lcd1.init();                      // initialize the first lcd 
  lcd2.init();                      // initialize the second lcd 
 
  // Print a message on the first LCD.
  lcd1.backlight();
  lcd1.print("Hello, #1 world!");
 
  // Print a message on the second LCD.
  lcd2.backlight();
  lcd2.print("Hello, #2 world!");
 
}
 
void loop()
{
}
