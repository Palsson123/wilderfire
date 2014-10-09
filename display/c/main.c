/*import RPi.GPIO as GPIO
import time
import spidev #hardware SPI
GPIO.setmode(GPIO.BOARD)*/
/*TFT to RPi connections
# PIN TFT RPi
# 1 backlight=V3
# 2 MISO <none>
#= CLK SCLK (GPIO 11)
# 4 MOSI MOSI (GPIO 10)
# 5 CS-TFT GND
# 6 CS-CARD <none>
# 7 D/C GPIO 25
# 8 RESET <none>
# 9 VCC=V3
# 10 GND GND
*/
//#include <avr/io.h>
//#include <util/delay.h>
#include <wiringPi.h>
#include "spi.h"

int spi_cs_fd;
//#include <spi.h>
//volatile uint8_t data;

#define DC 0	// change to whatever port

//DC = 25 // gpio 25 change this
//RGB888 Color constants
#define BLACK 0x000000
#define RED 0xFF0000
#define GREEN  0x00FF00
#define BLUE  0x0000FF
#define WHITE  0xFFFFFF
//const uint32_t COLORSET[4] = {RED, GREEN ,BLUE ,WHITE} // kanske stoppa in värdena?
//#define COLORSET [RED,GREEN,BLUE,WHITE]
//ST7735 commands
#define SWRESET 0x01 //software reset
#define SLPOUT 0x11 //sleep out
#define DISPON 0x29 //display on
#define CASET 0x2A //column address set
#define RASET 0x2B //row address set
#define RAMWR 0x2C //RAM write
#define MADCTL 0x36 //axis control
#define COLMOD 0x3A //color mode


// constants
#define HX8357D 0xD
#define HX8357B 0xB

#define HX8357_TFTWIDTH 320
#define HX8357_TFTHEIGHT 480

#define HX8357B_NOP 0x00
#define HX8357B_SWRESET 0x01
#define HX8357B_RDDID  0x04
#define HX8357B_RDDST  0x09

#define HX8357B_RDPOWMODE  0x0A
#define HX8357B_RDMADCTL  0x0B
#define HX8357B_RDCOLMOD  0x0C
#define HX8357B_RDDIM  0x0D
#define HX8357B_RDDSDR  0x0F

#define HX8357_SLPIN   0x10
#define HX8357_SLPOUT  0x11
#define HX8357B_PTLON   0x12
#define HX8357B_NORON   0x13

#define HX8357_INVOFF 0x20
#define HX8357_INVON   0x21
#define HX8357_DISPOFF 0x28
#define HX8357_DISPON  0x29

#define HX8357_CASET   0x2A
#define HX8357_PASET   0x2B
#define HX8357_RAMWR   0x2C
#define HX8357_RAMRD   0x2E

#define HX8357B_PTLAR   0x30
#define HX8357_TEON  0x35
#define HX8357_TEARLINE  0x44
#define HX8357_MADCTL  0x36
#define HX8357_COLMOD  0x3A

#define HX8357_SETOSC 0xB0
#define HX8357_SETPWR1 0xB1
#define HX8357B_SETDISPLAY 0xB2
#define HX8357_SETRGB 0xB3
#define HX8357D_SETCOM  0xB6

#define HX8357B_SETDISPMODE  0xB4
#define HX8357D_SETCYC  0xB4
#define HX8357B_SETOTP  0xB7
#define HX8357D_SETC  0xB9

#define HX8357B_SET_PANEL_DRIVING  0xC0
#define HX8357D_SETSTBA  0xC0
#define HX8357B_SETDGC  0xC1
#define HX8357B_SETID  0xC3
#define HX8357B_SETDDB  0xC4
#define HX8357B_SETDISPLAYFRAME  0xC5
#define HX8357B_GAMMASET 0xC8
#define HX8357B_SETCABC  0xC9
#define HX8357_SETPANEL  0xCC

#define HX8357B_SETPOWER  0xD0
#define HX8357B_SETVCOM  0xD1
#define HX8357B_SETPWRNORMAL  0xD2

#define HX8357B_RDID1   0xDA
#define HX8357B_RDID2   0xDB
#define HX8357B_RDID3  0xDC
#define HX8357B_RDID4   0xDD

#define HX8357D_SETGAMMA  0xE0

#define HX8357B_SETGAMMA  0xC8
#define HX8357B_SETPANELRELATED  0xE9

#define HX8357_BLACK   0x0000
#define HX8357_BLUE    0x001F
#define HX8357_RED     0xF800
#define HX8357_GREEN   0x07E0
#define HX8357_CYAN    0x07FF
#define HX8357_MAGENTA 0xF81F
#define HX8357_YELLOW  0xFFE0  
#define HX8357_WHITE   0xFFFF

#define true HIGH
#define false LOW

void SetPin(uint8_t pinNumber, uint8_t value)
{
	digitalWrite(pinNumber, value);
}
uint8_t temp = 0;
void WriteByte(uint8_t value, uint8_t data)
{
	SetPin(DC, data);
//	uint8_t temp;
	transfer(spi_cs_fd, &value, &temp);
}
void WriteCmd(uint8_t value)
{
	WriteByte(value, false); //set D/C line to 0 = command
}

void HX_SWRESET()
{
	WriteCmd(HX8357B_SWRESET); //software reset, puts display into sleep
}

void HX_SETC()
{
	WriteCmd(HX8357D_SETC);
	WriteByte(0xFF, true);
	WriteByte(0x83, true);
	WriteByte(0x57, true);
	delay(300);
}

void HX_SETRGB()
{
	WriteCmd(HX8357_SETRGB);
	WriteByte(0x00, true);
	WriteByte(0x00, true);
	WriteByte(0x06, true);
	WriteByte(0x06, true);
}

void HX_SETCOM()
{
	WriteCmd(HX8357D_SETCOM);
	WriteByte(0x25, true);
}

void HX_SETOSC()
{
	WriteCmd(HX8357_SETOSC);
	WriteByte(0x68, true);
}

void HX_SETPANEL()
{
	WriteCmd(HX8357_SETPANEL);
	WriteByte(0x05, true);
}

void HX_SETPWR1()
{
	WriteCmd(HX8357_SETPWR1);
	WriteByte(0x00, true);
	WriteByte(0x15, true);
	WriteByte(0x1C, true);
	WriteByte(0x1C, true);
	WriteByte(0x83, true);
	WriteByte(0xAA, true);
}

void HX_SETSTBA()
{
	WriteCmd(HX8357D_SETSTBA);
	WriteByte(0x50, true);
	WriteByte(0x50, true);
	WriteByte(0x01, true);
	WriteByte(0x3C, true);
	WriteByte(0x1E, true);
	WriteByte(0x08, true);
}

void HX_SETCYC()
{
	WriteCmd(HX8357D_SETCYC);
	WriteByte(0x02, true);
	WriteByte(0x40, true);
	WriteByte(0x00, true);
	WriteByte(0x2A, true);
	WriteByte(0x2A, true);
	WriteByte(0x0D, true);
	WriteByte(0x78, true);
}

void HX_SETGAMMA()
{
	WriteCmd(HX8357D_SETGAMMA);
	WriteByte(0x02, true);
	WriteByte(0x0A, true);
	WriteByte(0x11, true);
	WriteByte(0x1d, true);
	WriteByte(0x23, true);
	WriteByte(0x35, true);
	WriteByte(0x41, true);
	WriteByte(0x4b, true);
	WriteByte(0x4b, true);
	WriteByte(0x42, true);
	WriteByte(0x3A, true);
	WriteByte(0x27, true);
	WriteByte(0x1B, true);
	WriteByte(0x08, true);
	WriteByte(0x09, true);
	WriteByte(0x03, true);
	WriteByte(0x02, true);
	WriteByte(0x0A, true);
	WriteByte(0x11, true);
	WriteByte(0x1d, true);
	WriteByte(0x23, true);
	WriteByte(0x35, true);
	WriteByte(0x41, true);
	WriteByte(0x4b, true);
	WriteByte(0x4b, true);
	WriteByte(0x42, true);
	WriteByte(0x3A, true);
	WriteByte(0x27, true);
	WriteByte(0x1B, true);
	WriteByte(0x08, true);
	WriteByte(0x09, true);
	WriteByte(0x03, true);
	WriteByte(0x00, true);
	WriteByte(0x01, true);
}

void HX_COLMOD()
{
	WriteCmd(HX8357_COLMOD);
	WriteByte(0x07, true); //Sets 16-bit color mode
}

void HX_MADCTL()
{
	WriteCmd(HX8357_MADCTL);
	WriteByte(0xC0, true);
}

void HX_TEON()
{
	WriteCmd(HX8357_TEON);
	WriteByte(0x00, true);
}

void HX_TEARLINE()
{
	WriteCmd(HX8357_TEARLINE);
	WriteByte(0x00, true);
	WriteByte(0x02, true);
}

void HX_SLPOUT()
{
	WriteCmd(HX8357_SLPOUT);
	delay(150);
}

void HX_DISPON()
{
	WriteCmd(HX8357_DISPON);
	delay(50);
}
//"Send command byte to display"
void WriteWord (uint16_t value)
{
	//"sends a 16-bit word to the display as data"
	WriteByte(value >> 8, true); //write upper 8 bits
	WriteByte(value & 0xFF, true); //write lower 8 bits
}

/*void WriteList (uint8_t byteList[], uint8_t size)
{
	//"Send list of bytes to display, as data"
	int i;
	for (i = 0; i < size; i++){
		WriteByte(byteList[i], true);
	}
}*/


void Write888(uint32_t value, uint8_t width, uint8_t count)
{
	//"sends a 24-bit RGB pixel data to display, with optional repeat"
	/*uint8_t red = value>>16; //#red = upper 8 bits
	uint8_t green = (value>>8) & 0xFF; //#green = middle 8 bits
	uint8_t blue = value & 0xFF; //#blue = lower 8 bits
	uint8_t RGB = [red,green,blue]; //#assemble RGB as= byte list
	SetPin(DC,1);
	for a in range(count):
		spi.writebytes(RGB*width)*/
}

//ST7735 driver routines:
void InitDisplay()
{
	HX_SWRESET();
	HX_SETC();
	HX_SETRGB();
	HX_SETCOM();
	HX_SETOSC();
	HX_SETPANEL();
	HX_SETPWR1();
	HX_SETSTBA();
	HX_SETCYC();
	HX_SETGAMMA();
	HX_COLMOD();
	HX_MADCTL();
	HX_TEON();
	HX_TEARLINE();
	HX_SLPOUT();
	HX_DISPON();
}

void SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	//"sets a rectangular display window into which pixel data is placed"
	WriteCmd(CASET); //set column range (x0,x1)
	WriteWord(x0);
	WriteWord(x1);
	WriteCmd(RASET); //set row range (y0,y1)
	WriteWord(y0);
	WriteWord(y1);
}

void FillRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint32_t color)
{
	//"fills rectangle with given color"
	uint8_t width = x1 - x0 + 1;
	uint8_t height = y1 - y0 + 1;
	SetAddrWindow(x0, y0, x1, y1);
	WriteCmd(RAMWR);
	Write888(color, width ,height);
}

/*void FillScreen(uint8_t color)
{
	//"Fills entire screen with given color"
	FillRect(0, 0, HX8357_TFTWIDTH - 1, HX8357_TFTHEIGHT - 1, color);
}*/

/*void ClearScreen()
{
	FillRect(0, 0, HX8357_TFTWIDTH - 1, HX8357_TFTHEIGHT - 1, BLACK)
}*/
	//"Fills entire screen with black"


//Testing routines

/*void TimeDisplay()
{
	//"Measures time required to fill display twice"
	startTime=time.time()
	//print " Now painting screen GREEN"
	FillScreen(GREEN)
	//print " Now clearing screen"
	ClearScreen()
	elapsedTime=time.time()-startTime
	//print " Elapsed time %0.1f seconds" % (elapsedTime)
}*/

void SetPixel()
{
	SetAddrWindow(1, 1,2,2);
	WriteCmd(RAMWR);
	WriteByte(0xFF, true);
	WriteByte(0x00, true);
	WriteByte(0x00, true);
		
	
}

int main(void)
{
	//spi_cs_fd = open(std::string("/dev/spidev0.1").c_str(), O_RDWR);
	//InitSPI();
	//InitDisplay();
	wiringPiSetup();
	pinMode(0, OUTPUT);
	//void pabort(const char *s);
	//spi_set_mode(spi_cs_fd, 0);
	//spi_set_word(int, int);
	//spi_set_speed(int, int);
	//spi_set_delay(int);
	//spiOpen(const char*);
	//spiClose(int);
	spi_cs_fd = (spiOpen("/dev/spidev0.0"));
	spi_init(spi_cs_fd);
	printSpiDetails();
	InitDisplay();
	SetPixel();

	for(;;)
	{

	}
}
