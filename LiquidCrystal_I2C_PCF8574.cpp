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


#include "LiquidCrystal_I2C_PCF8574.h"
#include <inttypes.h>
#include "Wire.h"
#include "Arduino.h"

#define LCD_CLEAR_DISPLAY           0x01    // Mode : Clears display

#define LCD_RETURN_HOME             0x02    // Mode : Returns cursor to home posn.

// Entry Mode Set
#define LCD_ENTRY_MODE_SET          0x04    // Mode : Entry Mode Set, Sets the cursor move dir and specs whether or not to shift the display
#define LCD_INCREMENT               0x02        // Sub Mode of ENTRY_MODE_SET : Increment DDRAM (I/D), Entry Left
#define LCD_DECREMENT               0x00        // Sub Mode of ENTRY_MODE_SET : Decrement DDRAM (I/D), Entry Right
#define LCD_SHIFT_ON                0x01        // Sub Mode of ENTRY_MODE_SET : Shift On  (S), Shift Display when byte written. Display Shift
#define LCD_SHIFT_OFF               0x00        // Sub Mode of ENTRY_MODE_SET : Shift Off (S), Don't shift display when byte written. Cursor Move

// Display Function
#define LCD_DISPLAY_ON_OFF          0x08    // Mode : Display On/Off, Sets on/off of all display, Cursor on/off, Cursor Blink on/off
#define LCD_DISPLAY_ON              0x04        // Sub Mode of DISPLAY_ON_OFF : Puts display on  (D)
#define LCD_DISPLAY_OFF             0x00        // Sub Mode of DISPLAY_ON_OFF : Puts display off (D)
#define LCD_CURSOR_ON               0x02        // Sub Mode of DISPLAY_ON_OFF : Puts cursor on   (C)
#define LCD_CURSOR_OFF              0x00        // Sub Mode of DISPLAY_ON_OFF : Puts cursor off  (C)
#define LCD_BLINKING_ON             0x01        // Sub Mode of DISPLAY_ON_OFF : Blinking cursor  (B)
#define LCD_BLINKING_OFF            0x00        // Sub Mode of DISPLAY_ON_OFF : Solid cursor     (B)

// Display Control
#define LCD_MV_CUR_SHIFT_DISPLAY    0x10    // Mode : Move the cursor and shifts the display
#define LCD_DISPLAY_SHIFT           0x08        // Sub Mode of CURSOR_SHFT_DIS : Display shifts after char print   (SC)
#define LCD_CURSOR_SHIFT            0x00        // Sub Mode of CURSOR_SHFT_DIS : Cursor shifts after char print    (SC)
#define LCD_SHIFT_RIGHT             0x04        // Sub Mode of CURSOR_SHFT_DIS : Cursor or Display shifts to right (RL)
#define LCD_SHIFT_LEFT              0x00        // Sub Mode of CURSOR_SHFT_DIS : Cursor or Display shifts to left  (RL)

// Function Set
#define LCD_FUNCTION_SET            0x20    // Mode : Set the type of interface that the display will use
#define LCD_INTF8BITS               0x10        // Sub Mode of FUNCTION_SET : Select 8 bit interface         (DL)
#define LCD_INTF4BITS               0x00        // Sub Mode of FUNCTION_SET : Select 4 bit interface         (DL)
#define LCD_TWO_LINES               0x08        // Sub Mode of FUNCTION_SET : Selects two char line display  (N)
#define LCD_ONE_LINE                0x00        // Sub Mode of FUNCTION_SET : Selects one char line display  (N)
//#define LCD_FONT_5_10               0x04        // Sub Mode of FUNCTION_SET : Selects 5 x 10 Dot Matrix Font (F)
//#define LCD_FONT_5_7                0x00        // Sub Mode of FUNCTION_SET : Selects 5 x 7 Dot Matrix Font  (F)

#define LCD_CG_RAM_ADDRESS          0x40        // Mode : Enables the setting of the Char Gen (CG) Ram Address, to be or'ed with require address
#define LCD_CG_RAM_ADDRESS_MASK     B00111111  // Used to mask off the lower 6 bits of valid CG Ram Addresses

#define LCD_DD_RAM_ADDRESS          0x80        // Mode : Enables the setting of the Display Data (DD) Ram Address, to be or'ed with require address
#define LCD_DD_RAM_ADDRESS_MASK     B01111111    // Used to mask off the lower 6 bits of valid DD Ram Addresses

//#define USE_BUSY_FLAG               // Define this if you wish to use busy flag polling on slow LCD activities

// Change here for your I2C to 16 pin parallel interface // TODO Adapt
#define Bl B00001000  // Backlight enable bit (On = 1, Off =0)
#define En B00000100  // Enable bit (Enable on low edge)
#define Rw B00000010  // Read/Write bit (Read = 1, Write = 0)
#define Rs B00000001  // Register select bit (Data = 1, Control = 0)

// Change here for your I2C to 16 pin parallel interface // TODO Adapt
#define LCD_INIT      ((B00000000 | En) & ~Rs) & (~Rw) // Used to set all the O/Ps on the PCF8574 to initialise the LCD
#define LCD_8BIT_INIT B00110000 // Used to initialise the interface at the LCD
#define LCD_4BIT_INIT B00100000 // Used to initialise the interface at the LCD

//#define LCD_PCF8574_ADDR         (0x27<<1)  // Modify this if the default address is altered
#define LCD_PCF8574_WEAK_PU      B11110000 // Used to turn on PCF8574 Bits 7-4 on. To allow for read of LCD.

#define LCD_BUSY_FLAG_MASK       B10000000 // Used to mask off the status of the busy flag
#define LCD_ADDRESS_COUNTER_MASK B01111111 // Used to mask off the value of the Address Counter

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

#define LCD_LINE1	0x00		// Constant used to point to start of LCD Line 1
#define LCD_LINE2	0x40		// Constant used to point to start of LCD Line 2
#define LCD_LINE3	0x14		// Constant used to point to start of LCD Line 3
#define LCD_LINE4	0x54		// Constant used to point to start of LCD Line 4


LiquidCrystal_I2C_PCF8574::LiquidCrystal_I2C_PCF8574(uint8_t lcd_addr,uint8_t lcd_cols,uint8_t lcd_rows, uint8_t lcd_font)
{
  _addr = lcd_addr;
  _cols = lcd_cols;
  _rows = lcd_rows;
  _font = lcd_font;
}


void LiquidCrystal_I2C_PCF8574::init(){
	uint8_t lines = LCD_TWO_LINES;
	Wire.begin();
    //_backlightval &= ~Bl; // Off at start up
    _backlightval |= Bl; // On at start up
    _numlines = _rows;

    // Ensure supply rails are up before config sequence
    delayMicroseconds(50000);

    // Set all control and data lines low. D4 - D7, En (High=1), Rw (Low = 0 or Write), Rs (Control/Instruction) (Low = 0 or Control)
    //I2C_Write_Byte_Single_Reg(_addr, LCD_INIT); // Backlight off (Bit 3 = 0)
	Wire.beginTransmission(_addr);
	Wire.write((int) LCD_INIT); // Backlight off (Bit 3 = 0)
	Wire.endTransmission();
    delayMicroseconds(100);

    // Sequence to put the LCD into 4 bit mode this is according to the hitachi HD44780 datasheet page 109

    // we start in 8bit mode
    write4bits(LCD_8BIT_INIT);
    delayMicroseconds(4500);  // wait more than 4.1ms

    // second write
    write4bits(LCD_8BIT_INIT);
    delayMicroseconds(150); // wait > 100us

    // third write
    write4bits(LCD_8BIT_INIT);
    delayMicroseconds(150);

    // now set to 4-bit interface
    write4bits(LCD_4BIT_INIT);
    delayMicroseconds(150);

	if (_rows < 2)
		lines = LCD_ONE_LINE;

    // set # lines, font size, etc.
    if (_font != LCD_FONT_5_7)
    	_functionset = LCD_INTF4BITS | lines | LCD_FONT_5_10;
    else
    	_functionset = LCD_INTF4BITS | lines | LCD_FONT_5_7;
    commandWrite(LCD_FUNCTION_SET | _functionset);
    //delayMicroseconds(150);

    _displayfunction = LCD_DISPLAY_OFF | LCD_CURSOR_OFF | LCD_BLINKING_OFF;
    displayOff();

    // turn the display on with no cursor or blinking default
    displayOn();

    // set the entry mode
    _entrymodeset = LCD_INCREMENT | LCD_SHIFT_OFF; // Initialize to default text direction (for roman languages)
    commandWrite(LCD_ENTRY_MODE_SET | _entrymodeset);

    // Display Function set
    // _displayfunction = LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINKING_OFF;
    commandWrite(LCD_DISPLAY_ON_OFF | _displayfunction);

    // Display Control set
    _displaycontrol = LCD_DISPLAY_SHIFT | LCD_SHIFT_LEFT;
    commandWrite(LCD_MV_CUR_SHIFT_DISPLAY | _displaycontrol);

    // clear display and return cursor to home position. (Address 0)
    clear();
}





/********** high level commands, for the user! */


void LiquidCrystal_I2C_PCF8574::writeChar(char message)
{
    dataWrite((uint8_t) message);
}

void LiquidCrystal_I2C_PCF8574::writeStr(const char *message)
{
    uint8_t *message_ptr = (uint8_t *) message;

    while (*message_ptr)
        dataWrite((uint8_t) (*message_ptr++));
}


void LiquidCrystal_I2C_PCF8574::clear(){
    commandWrite(LCD_CLEAR_DISPLAY);// clear display, set cursor position to zero
#ifdef USE_BUSY_FLAG
    while (busy()){};
#else
    delayMicroseconds(30000);  // this command takes a long time!
#endif
}

void LiquidCrystal_I2C_PCF8574::home(){
    commandWrite(LCD_RETURN_HOME);  // set cursor position to zero
#ifdef USE_BUSY_FLAG
    while (busy()){};
#else
    delayMicroseconds(30000);  // this command takes a long time!
#endif
}

void LiquidCrystal_I2C_PCF8574::setCursor(uint8_t col, uint8_t row)
{
  int row_offsets[] = { LCD_LINE1, LCD_LINE2, LCD_LINE3, LCD_LINE4 };
  if ( row >= _numlines ) {
    row = _numlines-1;    // we count rows starting w/0
  }

  commandWrite(LCD_DD_RAM_ADDRESS | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal_I2C_PCF8574::displayOff() {
    _displayfunction &= ~LCD_DISPLAY_ON;
    commandWrite(LCD_DISPLAY_ON_OFF | _displayfunction);
}

void LiquidCrystal_I2C_PCF8574::displayOn() {
    _displayfunction |= LCD_DISPLAY_ON;
    commandWrite(LCD_DISPLAY_ON_OFF | _displayfunction);
}

// Turns the underline cursor on/off
void LiquidCrystal_I2C_PCF8574::cursorOff() {
    _displayfunction &= ~LCD_CURSOR_ON;
    commandWrite(LCD_DISPLAY_ON_OFF | _displayfunction);
}

void LiquidCrystal_I2C_PCF8574::cursorOn() {
    _displayfunction |= LCD_CURSOR_ON;
    commandWrite(LCD_DISPLAY_ON_OFF | _displayfunction);
}

// Turn on and off the blinking cursor
void LiquidCrystal_I2C_PCF8574::blinkOff() {
    _displayfunction &= ~LCD_BLINKING_ON;
    commandWrite(LCD_DISPLAY_ON_OFF | _displayfunction);
}

void LiquidCrystal_I2C_PCF8574::blinkOn() {
    _displayfunction |= LCD_BLINKING_ON;
    commandWrite(LCD_DISPLAY_ON_OFF | _displayfunction);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal_I2C_PCF8574::scrollDisplayLeft() {
    _displaycontrol &=  ~LCD_SHIFT_RIGHT;
    _displaycontrol |=   LCD_DISPLAY_SHIFT;
    commandWrite(LCD_MV_CUR_SHIFT_DISPLAY | _displaycontrol);
}

void LiquidCrystal_I2C_PCF8574::scrollDisplayRight() {
    _displaycontrol |=  LCD_SHIFT_RIGHT;
    _displaycontrol |=  LCD_DISPLAY_SHIFT;
    commandWrite(LCD_MV_CUR_SHIFT_DISPLAY | _displaycontrol);
}


// This is for text that flows Left to Right
void LiquidCrystal_I2C_PCF8574::leftToRight() {
    _entrymodeset |= LCD_INCREMENT;
    //_entrymodeset |= LCD_SHIFT_ON;
    commandWrite(LCD_ENTRY_MODE_SET | _entrymodeset);
}

// This is for text that flows Right to Left
void LiquidCrystal_I2C_PCF8574::rightToLeft() {
    _entrymodeset &= ~LCD_INCREMENT;
    //_entrymodeset &= ~LCD_SHIFT_ON;
    commandWrite(LCD_ENTRY_MODE_SET | _entrymodeset);
}

// This will 'right justify' text from the cursor. Display shift
void LiquidCrystal_I2C_PCF8574::autoscroll() {
    _entrymodeset |= LCD_SHIFT_ON;
    //_entrymodeset |= LCD_INCREMENT;
    commandWrite(LCD_ENTRY_MODE_SET | _entrymodeset);
}

// This will 'left justify' text from the cursor. Cursor Move
void LiquidCrystal_I2C_PCF8574::noAutoscroll() {
    _entrymodeset &= ~LCD_SHIFT_ON;
    //_entrymodeset &= ~LCD_INCREMENT;
    commandWrite(LCD_ENTRY_MODE_SET | _entrymodeset);
}

// Allows filling of the first 8 CGRAM locations with custom characters
void LiquidCrystal_I2C_PCF8574::createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7; // we only have 8 locations 0-7
    commandWrite(LCD_CG_RAM_ADDRESS | (location << 3));
    for (int i=0; i<8; i++)
        dataWrite(charmap[i]);
}

// Turn the (optional) backlight off/on
void LiquidCrystal_I2C_PCF8574::noBacklight() {
    _backlightval &= ~Bl;
    writePCF8574(readPCF8574());  // Dummy write to LCD, only led control bit is of interest
}

void LiquidCrystal_I2C_PCF8574::backlight() {
    _backlightval |= Bl;
    writePCF8574(readPCF8574());  // Dummy write to LCD, only led control bit is of interest
}



/*********** mid level commands, for sending/receiving data/cmds */

void LiquidCrystal_I2C_PCF8574::commandWrite(uint8_t value) {
    send(value, Rs & ~Rs);
}

uint8_t LiquidCrystal_I2C_PCF8574::commandRead() {
    return receive(Rs & ~Rs);
}

inline size_t LiquidCrystal_I2C_PCF8574::write(uint8_t value) {
    send(value, Rs);
    return 0;
}

void LiquidCrystal_I2C_PCF8574::dataWrite(uint8_t value) {
    send(value, Rs);
}

uint8_t LiquidCrystal_I2C_PCF8574::dataRead() {
    return receive(Rs);
}

uint8_t LiquidCrystal_I2C_PCF8574::busy() {
    return commandRead() & LCD_BUSY_FLAG_MASK;
}

uint8_t LiquidCrystal_I2C_PCF8574::addressCounter() {
    return commandRead() & LCD_ADDRESS_COUNTER_MASK;
}

uint8_t LiquidCrystal_I2C_PCF8574::readDDRam(uint8_t address)
{
    commandWrite(LCD_DD_RAM_ADDRESS | (address & LCD_DD_RAM_ADDRESS_MASK));
    return dataRead();
}

uint8_t LiquidCrystal_I2C_PCF8574::readCGRam(uint8_t address)
{
    commandWrite(LCD_CG_RAM_ADDRESS | (address & LCD_CG_RAM_ADDRESS_MASK));
    return dataRead();
}

/************ low level data read/write commands **********/

// Change this routine for your I2C to 16 pin parallel interface, if your pin interconnects are different to that outlined above // TODO Adapt

// write either command or data
void LiquidCrystal_I2C_PCF8574::send(uint8_t value, uint8_t RsMode) {
    uint8_t highnib = value & 0xF0;

    uint8_t lownib  = value << 4;
    lownib &= 0xF0;

    write4bits((highnib) | En | RsMode);
    write4bits((lownib ) | En | RsMode);
}

// Change this routine for your I2C to 16 pin parallel interface, if your pin interconnects are different to that outlined above // TODO Adapt

// read either command or data
uint8_t LiquidCrystal_I2C_PCF8574::receive(uint8_t RsMode) {
    uint8_t highnib;
    uint8_t lownib;

    writePCF8574(LCD_PCF8574_WEAK_PU | (En & ~En) | RsMode); // Set P7..P4 = 1, En = 0, RnW = 0, Rs = XX
    highnib = read4bits(LCD_PCF8574_WEAK_PU | En | RsMode);
    lownib = read4bits(LCD_PCF8574_WEAK_PU | En | RsMode);
    writePCF8574((LCD_PCF8574_WEAK_PU & ~LCD_PCF8574_WEAK_PU) | En | RsMode); // Set P7..P4 = 1, En = 1, RnW = 0, Rs = XX
    return (uint8_t) ((highnib & 0xF0) | ((lownib & 0xF0) >> 4));
}



void LiquidCrystal_I2C_PCF8574::write4bits(uint8_t nibEnRsMode) {
    writePCF8574(nibEnRsMode & ~Rw);
    pulseEnableNeg(nibEnRsMode & ~Rw);
}


uint8_t LiquidCrystal_I2C_PCF8574::read4bits(uint8_t RsEnMode) {
    uint8_t b;
    pulseEnablePos(RsEnMode | Rw);
    b = readPCF8574(); // Read the data from the LCD just after the rising edge. NOT WELL DOCUMENTED!
    pulseEnableNeg(RsEnMode | Rw);
    return b;
}

void LiquidCrystal_I2C_PCF8574::pulseEnableNeg(uint8_t _data){
    writePCF8574(_data | En);	// En high
    delayMicroseconds(1);		// enable pulse must be >450ns

    writePCF8574(_data & ~En);	// En low
    delayMicroseconds(50);		// commands need > 37us to settle
}

void LiquidCrystal_I2C_PCF8574::pulseEnablePos(uint8_t _data){
    writePCF8574(_data & ~En);	// En low
    delayMicroseconds(1);		// enable pulse must be >450ns

    writePCF8574(_data | En);	// En high
    delayMicroseconds(50);		// commands need > 37us to settle
}


void LiquidCrystal_I2C_PCF8574::writePCF8574(uint8_t value) {
    //I2C_Write_Byte_Single_Reg(LCD_PCF8574_ADDR, value | _backlightval);
	Wire.beginTransmission(_addr);
	Wire.write((int)(value) | _backlightval);
	Wire.endTransmission();

}


uint8_t LiquidCrystal_I2C_PCF8574::readPCF8574() {
	uint8_t value;
    //return I2C_Read_Byte_Single_Reg(LCD_PCF8574_ADDR);
	Wire.requestFrom((int)_addr, (int)1);    // request 1 byte from slave device #addr
	value = (char) Wire.read();
}


// Alias functions

void LiquidCrystal_I2C_PCF8574::blink_on(){
	blinkOn();
}

void LiquidCrystal_I2C_PCF8574::blink_off(){
	blinkOff();
}

void LiquidCrystal_I2C_PCF8574::cursor_on(){
	cursorOn();
}

void LiquidCrystal_I2C_PCF8574::cursor_off(){
	cursorOff();
}

void LiquidCrystal_I2C_PCF8574::load_custom_character(uint8_t char_num, uint8_t *rows){
		createChar(char_num, rows);
}

void LiquidCrystal_I2C_PCF8574::setBacklight(uint8_t new_val){
	if(new_val){
		backlight();		// turn backlight on
	}else{
		noBacklight();		// turn backlight off
	}
}

void LiquidCrystal_I2C_PCF8574::printstr(const char c[]){
	// Required so user sketch doesn't have to be adapted
	print(c);
}

// unsupported API functions
void LiquidCrystal_I2C_PCF8574::off(){}
void LiquidCrystal_I2C_PCF8574::on(){}
void LiquidCrystal_I2C_PCF8574::setDelay (int cmdDelay,int charDelay) {}
uint8_t LiquidCrystal_I2C_PCF8574::status(){return 0;}
uint8_t LiquidCrystal_I2C_PCF8574::keypad (){return 0;}
uint8_t LiquidCrystal_I2C_PCF8574::init_bargraph(uint8_t graphtype){return 0;}
void LiquidCrystal_I2C_PCF8574::draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end){}
void LiquidCrystal_I2C_PCF8574::draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_row_end){}
void LiquidCrystal_I2C_PCF8574::setContrast(uint8_t new_val){}
