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

#define FALSE 0
#define TRUE  ~FALSE

#define MAX_CHARS 5

LiquidCrystal_I2C_PCF8574 lcd1(0x27,20,4,LCD_FONT_5_7);  // set the LCD address to 0x27 for a 20 chars and 4 line display

///////////////////////////////
// Drop code in between here //
///////////////////////////////

// Drop this code into Arduino development environment  
  
#define PackR0 0  
#define PackR1 1  
#define PackR2 2  
#define PackR3 3  
#define PackR4 4  
#define PackR5 5  
#define PackR6 6  
#define PackR7 7  
  
typedef struct {  
  const unsigned char array1[8];
} binaryArrayType;  
  
binaryArrayType binaryArray[8] =  
{ 
  {0x00,0x0E,0x1F,0x1F,0x1F,0x0E,0x00,0x00},  // PackR0, 0
  {0x00,0x0E,0x1F,0x1E,0x1F,0x0E,0x00,0x00},  // PackR1, 1
  {0x00,0x0E,0x1F,0x1C,0x1F,0x0E,0x00,0x00},  // PackR2, 2
  {0x00,0x0E,0x1C,0x18,0x1C,0x0E,0x00,0x00},  // PackR3, 3
  {0x00,0x0C,0x18,0x10,0x18,0x0C,0x00,0x00},  // PackR4, 4
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // PackR5, 5
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  // PackR6, 6
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}  // PackR7, 7
};  



//////////////
// and here //
//////////////


void setup()
{
    lcd1.init();                      // initialize the lcd 

    lcd1.clear();
    lcd1.cursorOff();
    lcd1.blinkOff();

    // load in chars
    //for (unsigned char p = 0; p < (sizeof(charmap)/sizeof(unsigned char *)) ; p++)
    for (unsigned char p = 0; p < 8 ; p++)
    {
        lcd1.createChar((uint8_t)p, (uint8_t *)&binaryArray[p]);
    }

    // Print out Chars
    for (unsigned char i = 0; i < 8; i++)
    {
        lcd1.setCursor(i+2,0);
        lcd1.writeChar(i);
    }
}


void loop()
{
    while(TRUE)
    {
      // Repeatedly print out user defined chars at 0,0
      for (unsigned char j = 0; j < 10; j++)
      {
        for (unsigned char i = 0; i < (MAX_CHARS); i++)
        {

            lcd1.setCursor(0,0);
            lcd1.writeChar(i);
            delay(60);
        }    
        for (unsigned char i = 0; i < (MAX_CHARS); i++)
        {

            lcd1.setCursor(0,0);
            lcd1.writeChar(MAX_CHARS-i);
            delay(30);
        }    
      }
    }
}


