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


unsigned char hex[]         = {"0123456789ABCDEF"};
unsigned char bell[8]       = {0x04,0x0e,0x0e,0x0e,0x1f,0x00,0x04,0x00};
unsigned char note[8]       = {0x02,0x03,0x02,0x0e,0x1e,0x0c,0x00,0x00};
unsigned char cclock[8]     = {0x00,0x0e,0x15,0x17,0x11,0x0e,0x00,0x00};
unsigned char heart[8]      = {0x00,0x0a,0x1f,0x1f,0x0e,0x04,0x00,0x00};
unsigned char duck[8]       = {0x00,0x0c,0x1d,0x0f,0x0f,0x06,0x00,0x00};
unsigned char check[8]      = {0x00,0x01,0x03,0x16,0x1c,0x08,0x00,0x00};
unsigned char cross[8]      = {0x00,0x1b,0x0e,0x04,0x0e,0x1b,0x00,0x00};
unsigned char retarrow[8]   = {0x01,0x01,0x05,0x09,0x1f,0x08,0x04,0x00};
unsigned char * charmap[8]  = {bell, note, cclock, heart, duck, check, cross, retarrow};

#define MAX_COLS 20
#define MAX_ROWS 4
 


void displayLeftRightScroll(void);
void autoIncrement(void);
void displayOnOff(void);
void backLightControl(void);
void printAt(void);
void cursorControl(void);
void printLeftAndRight(void);
void controlMemoryReading(void);
void dataMemoryReading(void);
void putHexString(LiquidCrystal_I2C_PCF8574 lcd, unsigned char c, unsigned char r, unsigned char hexval);

LiquidCrystal_I2C_PCF8574 lcd1(0x27, MAX_COLS, MAX_ROWS, LCD_FONT_5_7);  // set the lcd. address to 0x27 for a 20 chars and 4 line display (Left 20x4)
LiquidCrystal_I2C_PCF8574 lcd2(0x23, MAX_COLS, MAX_ROWS, LCD_FONT_5_7);  // set the lcd. address to 0x27 for a 20 chars and 4 line display (Right 20x4)

void setup()
{
  lcd1.init();                      // initialize the lcd1
  lcd2.init();                      // initialize the lcd2
  lcd1.backlight();
  lcd2.backlight();
}

void loop()
{
    displayLeftRightScroll();
    autoIncrement();
    displayOnOff();
    backLightControl();
    printAt();
    cursorControl();
    printLeftAndRight();
    controlMemoryReading();
    dataMemoryReading();
}



void autoIncrement(void)
{
  char Autoscroll[] = "Autoscroll";
  char NoAutoscroll[] = "No Autoscroll";
  char * p;

  lcd1.clear();
  lcd2.clear();
  lcd1.cursorOff();
  lcd2.cursorOff();
  lcd1.blinkOff();
  lcd2.blinkOff();
  lcd1.autoscroll();
  lcd2.autoscroll();
  p = Autoscroll;
  lcd1.setCursor(15,1);
  lcd2.setCursor(15,1);
  while (*p)
  {
      lcd1.writeChar((char) *p);
      lcd2.writeChar((char) *p++);
      delay(300);
  }
  delay(1000);
  lcd1.clear();
  lcd2.clear();
  lcd1.noAutoscroll();
  lcd2.noAutoscroll();
  p = NoAutoscroll;
  lcd1.setCursor(3,1);
  lcd2.setCursor(3,1);
  while (*p)
  {
      lcd1.writeChar((char) *p);
      lcd2.writeChar((char) *p++);
      delay(150);
  }
  delay(2000);
}


void displayLeftRightScroll(void)
{
  lcd1.clear();
  lcd2.clear();
  lcd1.cursorOff();
  lcd2.cursorOff();
  lcd1.blinkOff();
  lcd2.blinkOff();
  lcd1.setCursor(0,0);
  lcd2.setCursor(0,0);
  lcd1.writeStr("Scroll Right");
  lcd2.writeStr("Scroll Right");
  lcd1.setCursor(0,1);
  lcd2.setCursor(0,1);
  lcd1.writeStr("---->");
  lcd2.writeStr("---->");
  for (unsigned char x = 0; x< 8; x++)
  {
    lcd1.scrollDisplayRight();
    lcd2.scrollDisplayRight();
    delay(300);
  }
  lcd1.clear();
  lcd2.clear();
  lcd1.setCursor(8,0);
  lcd2.setCursor(8,0);
  lcd1.writeStr("Scroll Left ");
  lcd2.writeStr("Scroll Left ");
  lcd1.setCursor(8,1);
  lcd2.setCursor(8,1);
  lcd1.writeStr("<----");
  lcd2.writeStr("<----");
  for (unsigned char x = 0; x< 8; x++)
  {
    lcd1.scrollDisplayLeft();
    lcd2.scrollDisplayLeft();
    delay(300);
  }
  delay(1000);
}



void displayOnOff(void)
{
  lcd1.clear();
  lcd2.clear();
  lcd1.cursorOff();
  lcd2.cursorOff();
  lcd1.blinkOff();
  lcd2.blinkOff();
  for (unsigned char x = 0; x < 6; x++)
  {
      if (x%2) {
        lcd1.home();
        lcd1.writeStr("           ");
        lcd1.home();
        lcd1.writeStr("Display On ");
        lcd1.displayOn();
        lcd2.home();
        lcd2.writeStr("           ");
        lcd2.home();
        lcd2.writeStr("Display On ");
        lcd2.displayOn();
      } else {
        lcd1.setCursor(0,0);
        lcd1.writeStr("           ");
        lcd1.setCursor(0,0);
        lcd1.writeStr("Display Off");
        lcd1.displayOff();
        lcd2.setCursor(0,0);
        lcd2.writeStr("           ");
        lcd2.setCursor(0,0);
        lcd2.writeStr("Display Off");
        lcd2.displayOff();
      }
      delay(550);
  }
}

void backLightControl(void)
{
  lcd1.clear();
  lcd2.clear();
  lcd1.cursorOff();
  lcd2.cursorOff();
  lcd1.backlight();
  lcd2.backlight();
  lcd1.writeStr("Backlight On");
  lcd2.writeStr("Backlight On");
  delay(1000);
  lcd1.clear();
  lcd2.clear();
  //lcd1.setCursor(0,0);
  //lcd2.setCursor(0,0);
  lcd1.noBacklight();
  lcd2.noBacklight();
  lcd1.writeStr("Backlight Off");
  lcd2.writeStr("Backlight Off");
  delay(1000);
  lcd1.clear();
  lcd2.clear();
  //lcd1.setCursor(0,0);
  //lcd2.setCursor(0,0);
  lcd1.backlight();
  lcd2.backlight();
  lcd1.writeStr("Backlight On");
  lcd2.writeStr("Backlight On");
  delay(1000);
  for (unsigned char x = 0; x < 6; x++)
  {
      if (x%2) {
        lcd1.setCursor(0,0);
        lcd1.writeStr("             ");
        lcd1.setCursor(0,0);
        lcd1.writeStr("Backlight On ");
        lcd1.backlight();
        lcd2.setCursor(0,0);
        lcd2.writeStr("             ");
        lcd2.setCursor(0,0);
        lcd2.writeStr("Backlight On ");
        lcd2.backlight();
      } else {
        lcd1.clear();
        lcd1.writeStr("Backlight Off");
        lcd1.noBacklight();
        lcd2.clear();
        lcd2.writeStr("Backlight Off");
        lcd2.noBacklight();
      }
      delay(750);
  }
}

void printAt(void)
{
  lcd1.clear();
  lcd2.clear();
  lcd1.cursorOff();
  lcd2.cursorOff();
  //lcd1.setCursor(0,0);
  //lcd2.setCursor(0,0);
  lcd1.writeStr("@:0,0");
  lcd2.setCursor(1,0);
  lcd2.writeStr("@:1,0");
  lcd1.setCursor(1,1);
  lcd1.writeStr("@:1,1");
  lcd2.setCursor(2,1);
  lcd2.writeStr("@:2,1");
  lcd1.setCursor(2,2);
  lcd1.writeStr("@:2,2");
  lcd2.setCursor(3,2);
  lcd2.writeStr("@:3,2");
  lcd1.setCursor(3,3);
  lcd1.writeStr("@:3,3");
  lcd2.setCursor(4,3);
  lcd2.writeStr("@:4,3");
  lcd1.setCursor(12,0);
  lcd1.writeStr("Print at");
  lcd2.setCursor(12,0);
  lcd2.writeStr("Print at");
  delay(3000);
}


void cursorControl(void)
{
  lcd1.clear();
  lcd2.clear();
  lcd1.cursorOff();
  lcd2.cursorOff();
  lcd1.writeStr("Cursor Off");
  lcd2.writeStr("Cursor Off");
  delay(1500);
  lcd1.clear();
  lcd2.clear();
  lcd1.cursorOn();
  lcd2.cursorOn();
  lcd1.writeStr("Cursor On");
  lcd2.writeStr("Cursor On");
  delay(1500);
  lcd1.clear();
  lcd2.clear();
  lcd1.blinkOn();
  lcd2.blinkOn();
  lcd1.writeStr("Blink On");
  lcd2.writeStr("Blink On");
  delay(1500);
  lcd1.clear();
  lcd2.clear();
  lcd1.blinkOff();
  lcd2.blinkOff();
  lcd1.writeStr("Blink Off");
  lcd2.writeStr("Blink Off");
  delay(1500);
  lcd1.clear();
  lcd2.clear();
  lcd1.writeStr("Cursor Home");
  lcd2.writeStr("Cursor Home");
  lcd1.home();
  lcd2.home();
  delay(1500);
  lcd1.clear();
  lcd2.clear();
  lcd1.writeStr("Cursor Home & Blink");
  lcd2.writeStr("Cursor Home & Blink");
  lcd1.home();
  lcd2.home();
  lcd1.blinkOn();
  lcd2.blinkOn();
  delay(1500);
}


void printLeftAndRight(void)
{
  lcd1.clear();
  lcd2.clear();
  lcd1.cursorOff();
  lcd2.cursorOff();
  lcd1.blinkOff();
  lcd2.blinkOff();
  //lcd1.setCursor(0,0);
  //lcd2.setCursor(0,0);
  lcd1.leftToRight();
  lcd2.leftToRight();
  lcd1.writeStr("Print left to right");
  lcd2.writeStr("Print left to right");
  lcd1.setCursor(0,1);
  lcd2.setCursor(0,1);
  lcd1.writeStr("0123456789ABCDEFGHIJ");
  lcd2.writeStr("0123456789ABCDEFGHIJ");
  lcd1.setCursor(0,2);
  lcd2.setCursor(0,2);
  lcd1.writeStr("Print right to left");
  lcd2.writeStr("Print right to left");
  lcd1.rightToLeft();
  lcd2.rightToLeft();
  lcd1.setCursor(19,4);
  lcd2.setCursor(19,4);
  lcd1.writeStr("0123456789ABCDEFGHIJ");
  lcd2.writeStr("0123456789ABCDEFGHIJ");
  lcd1.leftToRight();
  lcd2.leftToRight();
  delay(4000);
}

void controlMemoryReading(void)
{
    unsigned char x1 = 0;
    unsigned char x2 = 0;

    lcd1.clear();
    lcd2.clear();
    lcd1.cursorOn();
    lcd2.cursorOn();
    lcd1.blinkOn();
    lcd2.blinkOn();
    lcd1.setCursor(1,1); // At position 1,1 the Address Counter will read 0x41
    lcd2.setCursor(2,1); // At position 1,1 the Address Counter will read 0x42
    delay(2000);
    x1 = lcd1.addressCounter();
    x2 = lcd2.addressCounter();
    lcd1.cursorOff();
    lcd2.cursorOff();
    lcd1.blinkOff();
    lcd2.blinkOff();
    lcd1.setCursor(0,2);
    lcd2.setCursor(0,2);
    lcd1.writeStr("Addr Counter : ");
    lcd2.writeStr("Addr Counter : ");
    lcd1.setCursor(15,2);
    lcd2.setCursor(15,2);
    lcd1.writeStr("0x");
    lcd2.writeStr("0x");
    lcd1.setCursor(17,2);
    lcd2.setCursor(17,2);
    lcd1.writeChar((char) hex[(x1>>4)& 0x0f]);
    lcd2.writeChar((char) hex[(x2>>4)& 0x0f]);
    lcd1.setCursor(18,2);
    lcd2.setCursor(18,2);
    lcd1.writeChar((char) hex[x1 & 0x0f]);
    lcd2.writeChar((char) hex[x2 & 0x0f]);
    //delay(5000);

    x1 = lcd1.busy();
    x2 = lcd2.busy();
    lcd1.setCursor(0,3);
    lcd2.setCursor(0,3);
    lcd1.writeStr("Busy Flag : ");
    lcd2.writeStr("Busy Flag : ");
    lcd1.setCursor(12,3);
    lcd2.setCursor(12,3);
    lcd1.writeStr("0x");
    lcd2.writeStr("0x");
    lcd1.setCursor(14,3);
    lcd2.setCursor(14,3);
    lcd1.writeChar((char) hex[(x1>>4)& 0x0f]);
    lcd2.writeChar((char) hex[(x2>>4)& 0x0f]);
    lcd1.setCursor(15,3);
    lcd2.setCursor(15,3);
    lcd1.writeChar((char) hex[x1 & 0x0f]);
    lcd2.writeChar((char) hex[x2 & 0x0f]);
    delay(3000);
}


void dataMemoryReading(void)
{
    unsigned char x1 = 0;
    unsigned char x2 = 0;

    lcd1.clear();
    lcd2.clear();
    lcd1.cursorOff();
    lcd2.cursorOff();
    lcd1.blinkOff();
    lcd2.blinkOff();
    //for (unsigned char p = 0; p < (sizeof(charmap)/sizeof(unsigned char *)) ; p++)
    for (unsigned char p = 0; p < 8 ; p++)
    {
        lcd1.createChar(p, charmap[p]);
        lcd2.createChar(p, charmap[p]);
    }
    lcd1.home();
    lcd2.home();

    lcd1.setCursor(0,0);
    lcd1.writeStr("S = ");
    lcd2.setCursor(0,0);
    lcd2.writeStr("Q = ");

    x1 = lcd1.readDDRam(0x00);
    x2 = lcd2.readDDRam(0x00);
    lcd1.setCursor(4,0);
    lcd2.setCursor(4,0);
    lcd1.writeStr("0x");
    lcd2.writeStr("0x");
    lcd1.setCursor(6,0);
    lcd2.setCursor(6,0);
    lcd1.writeChar((char) hex[(x1>>4)& 0x0f]);
    lcd2.writeChar((char) hex[(x1>>4)& 0x0f]);
    lcd1.setCursor(7,0);
    lcd2.setCursor(7,0);
    lcd1.writeChar((char) hex[x1 & 0x0f]);
    lcd2.writeChar((char) hex[x2 & 0x0f]);
    lcd1.setCursor(8,0);
    lcd2.setCursor(8,0);
    lcd1.writeChar(',');
    lcd2.writeChar(',');

    for (unsigned char i = 0; i < 8; i++)
    {
        lcd1.setCursor(i+10,0);
        lcd1.writeChar(i);
        lcd2.setCursor(i+10,0);
        lcd2.writeChar(i);
    }

    lcd1.setCursor(0,1);
    lcd1.writeStr("Bell char Hex rep;");
    lcd2.setCursor(0,1);
    lcd2.writeStr("Note char Hex rep;");

    x1 = 0;
    for (unsigned char r = 0; r < 2; r++)
        for (unsigned char c = 0; c < 4; c++)
        {
            putHexString(lcd1, c, r, lcd1.readCGRam(x1));
            putHexString(lcd2, c, r, lcd2.readCGRam(8+x1++));
        }
   delay(5000);
}



void putHexString(LiquidCrystal_I2C_PCF8574 lcd, unsigned char c, unsigned char r, unsigned char hexval)
{
    lcd.setCursor(0+(5*c),2+(2*r));
    lcd.writeStr("0x");
    lcd.setCursor(2+(5*c),2+(2*r));
    lcd.writeChar((char) hex[(hexval>>4)& 0x0f]);
    lcd.setCursor(3+(5*c),2+(2*r));
    lcd.writeChar((char) hex[hexval & 0x0f]);
}

