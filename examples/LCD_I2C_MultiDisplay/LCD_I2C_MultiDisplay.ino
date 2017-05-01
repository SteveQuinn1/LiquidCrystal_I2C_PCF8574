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

#include <LiquidCrystal_I2C_PCF8574.h>

#include <Wire.h> 

// Assumes 4 off 20 by 4 LCDs are connected

void updateDisplays(LiquidCrystal_I2C_PCF8574 lcdDisplay, int y);
void initLCD(LiquidCrystal_I2C_PCF8574 lcdDisplay);

LiquidCrystal_I2C_PCF8574 lcd1(0x27,20,4,LCD_FONT_5_7);  // set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C_PCF8574 lcd2(0x26,20,4,LCD_FONT_5_7);  // set the LCD address to 0x26 for a 20 chars and 4 line display
LiquidCrystal_I2C_PCF8574 lcd3(0x23,20,4,LCD_FONT_5_7);  // set the LCD address to 0x23 for a 20 chars and 4 line display
LiquidCrystal_I2C_PCF8574 lcd4(0x25,20,4,LCD_FONT_5_7);  // set the LCD address to 0x25 for a 20 chars and 4 line display


void setup()
{
  lcd1.init();                      // initialize the lcd 
  lcd2.init();                      // initialize the lcd 
  lcd3.init();                      // initialize the lcd 
  lcd4.init();                      // initialize the lcd 
  initLCD(lcd1);
  initLCD(lcd2);
  initLCD(lcd3);
  initLCD(lcd4);
 }


int x=0;

void loop()
{
  String s;
  char sptr[255];
  x++;  
  updateDisplays(lcd1,x);
  updateDisplays(lcd2,x);
  updateDisplays(lcd3,x);
  updateDisplays(lcd4,x);
  delay(250);              // wait for a second
  if (x>98) x=0;
}


void updateDisplays(LiquidCrystal_I2C_PCF8574 lcdDisplay, int y)
{

  String s;
  char sptr[255];
  lcdDisplay.setCursor(7,1);
  lcdDisplay.writeStr("   ");
  lcdDisplay.setCursor(7,2);
  lcdDisplay.writeStr("   ");
  lcdDisplay.setCursor(7,3);
  lcdDisplay.writeStr("   ");
  lcdDisplay.setCursor(17,1);
  lcdDisplay.writeStr("   ");
  lcdDisplay.setCursor(17,2);
  lcdDisplay.writeStr("   ");
  lcdDisplay.setCursor(17,3);
  lcdDisplay.writeStr("   ");
  s = String(y);
  s.toCharArray(sptr, sizeof(sptr));
  lcdDisplay.setCursor(7,1);
  lcdDisplay.writeStr(sptr);
  lcdDisplay.setCursor(7,2);
  lcdDisplay.writeStr(sptr);
  lcdDisplay.setCursor(7,3);
  lcdDisplay.writeStr(sptr);
  lcdDisplay.setCursor(17,1);
  lcdDisplay.writeStr(sptr);
  lcdDisplay.setCursor(17,2);
  lcdDisplay.writeStr(sptr);
  lcdDisplay.setCursor(17,3);
  lcdDisplay.writeStr(sptr);
}


void initLCD(LiquidCrystal_I2C_PCF8574 lcdDisplay)
{
  // writeStr a message to the LCD.
  lcdDisplay.writeStr("Hello, world!######");
  lcdDisplay.setCursor(0,1);
  lcdDisplay.writeStr("Count :");
  lcdDisplay.setCursor(0,2);
  lcdDisplay.writeStr("Count :");
  lcdDisplay.setCursor(0,3);
  lcdDisplay.writeStr("Count :");
  lcdDisplay.setCursor(10,1);
  lcdDisplay.writeStr("Count :");
  lcdDisplay.setCursor(10,2);
  lcdDisplay.writeStr("Count :");
  lcdDisplay.setCursor(10,3);
  lcdDisplay.writeStr("Count :");
  lcdDisplay.backlight();
}
