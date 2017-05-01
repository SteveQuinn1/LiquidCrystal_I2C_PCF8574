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

// LiquidCrystal_I2C_PCF8574 V1.0

// Foreword: To successfully use this library you will to ensure the following.
// 1. Your PCF8574 LCD adaptor board is mapped either the same as outlined below or you have correctly re-assigned the pins for your board. I used the following;
//    http://www.ebay.co.uk/itm/1602LCD-Display-IIC-I-SPI-Serial-Board-Module-Port-For-Arduino-K6-/291572452594?hash=item43e3128cf2
// 2. Anywhere you may typically need to make changes have been tagged with 'TODO Adapt'
// Note : It was written and tested using the the Arduino Uno. IDE v1.6.5. Having a bus speed of 100KHz
//        Some of the inspiration for the library was taken from the Arduino LiquidCrystal_I2C library
//        See here : http://hmario.home.xs4all.nl/arduino/LiquidCrystal_I2C/
// Data sheets and useful information can be found here;
// Arduino    : http://playground.arduino.cc/Code/LCDAPI
// LCD        : https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
// PCF8574    : http://www.nxp.com/documents/data_sheet/PCF8574.pdf
// I2C        : http://www.nxp.com/documents/application_note/AN10216.pdf
// TI         : http://www.ti.com/lit/an/slva704/slva704.pdf
// TI         : http://www.ti.com/lit/an/slva689/slva689.pdf
// TI         : http://www.ti.com/lit/an/scaa106/scaa106.pdf
// MICROCHIP  : http://ww1.microchip.com/downloads/en/DeviceDoc/i2c.pdf

#ifndef LiquidCrystal_I2C_PCF8574_h
#define LiquidCrystal_I2C_PCF8574_h

#include <inttypes.h>
#include "Print.h"
#include <Wire.h>

//
// Code was written with the following assumptions as to PCF8574 -> Parallel 4bit convertor interconnections
// controlling an LCD display. Assumes A0...A2 on PCF8574 are all pulled high. Giving address of 0x4E or B01001110 (0x27)
// (the last bit [b0] is I2C R/nW bit)
//
// Pin out for LCD display (16 pins)
// ---------------------------------
// 1  - Gnd
// 2  - Vcc
// 3  - VContrast
// 4  - RS - P0 - Pin 4 PCF8574
// 5  - RW - P1 - Pin 5 PCF8574
// 6  - En - P2 - Pin 6 PCF8574
// 7  - D0 - Don't Care
// 8  - D1 - Don't Care
// 9  - D2 - Don't Care
// 10 - D3 - Don't Care
// 11 - D4 - P4 - Pin 9  PCF8574
// 12 - D6 - P5 - Pin 10 PCF8574
// 13 - D6 - P6 - Pin 11 PCF8574
// 14 - D7 - P7 - Pin 12 PCF8574
// 15 - Anode   LED
// 16 - Cathode LED
//
// PCF8574 register and pin mapping
// Bit 0 - RS  - P0 - Pin 4  PCF8574
// Bit 1 - RW  - P1 - Pin 5  PCF8574
// Bit 2 - En  - P2 - Pin 6  PCF8574
// Bit 3 - Led - P3 - Pin 7  PCF8574 (Active High, Led turned on)
// Bit 4 - D4  - P4 - Pin 9  PCF8574
// Bit 5 - D5  - P5 - Pin 10 PCF8574
// Bit 6 - D6  - P6 - Pin 11 PCF8574
// Bit 7 - D7  - P7 - Pin 12 PCF8574
//



// The display is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 4-bit interface data
//    N = 0; 2-line display
//    F = 0; 5x7 dot character font
// 3. Display on/off control:
//    D = 1; Display on
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//


#define LCD_FONT_5_10               0x04        // Sub Mode of FUNCTION_SET : Selects 5 x 10 Dot Matrix Font (F)
#define LCD_FONT_5_7                0x00        // Sub Mode of FUNCTION_SET : Selects 5 x 7 Dot Matrix Font  (F)




class LiquidCrystal_I2C_PCF8574 : public Print {

public:
  LiquidCrystal_I2C_PCF8574(uint8_t lcd_addr,uint8_t lcd_cols,uint8_t lcd_rows, uint8_t lcd_font = LCD_FONT_5_7);

  virtual size_t write(uint8_t);

  void init();
  void writeChar(char message);
  void writeStr(const char *message);

  void clear();
  void home();

  void displayOff();
  void displayOn();
  void blinkOff();
  void blinkOn();
  void cursorOff();
  void cursorOn();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void noBacklight();
  void backlight();
  void autoscroll();
  void noAutoscroll();
  void createChar(uint8_t location, uint8_t charmap[]);
  void setCursor(uint8_t col, uint8_t row);

  void commandWrite(uint8_t value);
  uint8_t commandRead();
  void dataWrite(uint8_t value);
  uint8_t dataRead();
  uint8_t busy();
  uint8_t addressCounter();
  uint8_t readDDRam(uint8_t address);
  uint8_t readCGRam(uint8_t address);

  // To maintain compatibility with API function aliases
  void blink_on();						            // alias for blinkOn()
  void blink_off();       					        // alias for blinkOff()
  void cursor_on();      	 				        // alias for cursorOn()
  void cursor_off();      					        // alias for cursorOff()
  void setBacklight(uint8_t new_val);				// alias for backlight() and noBacklight()
  void load_custom_character(uint8_t char_num, uint8_t *rows);	// alias for createChar()
  void printstr(const char[]);

  // Unsupported API functions (not implemented in this library)
  uint8_t status();
  void setContrast(uint8_t new_val);
  uint8_t keypad();
  void setDelay(int,int);
  void on();
  void off();
  uint8_t init_bargraph(uint8_t graphtype);
  void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);
  void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);


private:
  void send(uint8_t value, uint8_t mode);
  uint8_t receive(uint8_t RsMode);
  void write4bits(uint8_t value);
  uint8_t read4bits(uint8_t RsEnMode);
  void pulseEnableNeg(uint8_t value);
  void pulseEnablePos(uint8_t value);
  void writePCF8574(uint8_t value);
  uint8_t readPCF8574();

  uint8_t _backlightval = 0;
  uint8_t _functionset = 0;
  uint8_t _entrymodeset = 0;
  uint8_t _displayfunction = 0;
  uint8_t _displaycontrol = 0;
  uint8_t _numlines = 0;
  uint8_t _addr = 0;
  uint8_t _cols = 0;
  uint8_t _rows = 0;
  uint8_t _font = 0;
};


#endif //LiquidCrystal_I2C_PCF8574_h


