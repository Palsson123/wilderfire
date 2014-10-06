#!/usr/bin/python
########################################################################
#
# A Python script for controlling the Adafruit 1.8" TFT LCD module
# from a Raspbery Pi.
#
# Author : Bruce E. Hall, W8BH <bhall66@gmail.com>
# Date : 19 Feb 2014
#
# This module uses the ST7735 controller and SPI data interface
# PART 2 --- HARDWARE SPI
#
# For more information, see w8bh.net
#
########################################################################
import RPi.GPIO as GPIO
import time
import spidev #hardware SPI
import os, sys
import Image
GPIO.setmode(GPIO.BOARD)
#TFT to RPi connections
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
DC = 25
#RGB888 Color constants
BLACK = 0x000000
RED = 0xFF0000
GREEN = 0x00FF00
BLUE = 0x0000FF
WHITE = 0xFFFFFF
COLORSET = [RED,GREEN,BLUE,WHITE]
#ST7735 commands
SWRESET = 0x01 #software reset
SLPOUT = 0x11 #sleep out
DISPON = 0x29 #display on
CASET = 0x2A #column address set
RASET = 0x2B #row address set
RAMWR = 0x2C #RAM write
MADCTL = 0x36 #axis control
COLMOD = 0x3A #color mode


## KONSTANTER
HX8357D = 0xD
HX8357B = 0xB

HX8357_TFTWIDTH = 320
HX8357_TFTHEIGHT = 480

HX8357B_NOP    = 0x00
HX8357B_SWRESET = 0x01
HX8357B_RDDID  = 0x04
HX8357B_RDDST  = 0x09

HX8357B_RDPOWMODE = 0x0A
HX8357B_RDMADCTL = 0x0B
HX8357B_RDCOLMOD = 0x0C
HX8357B_RDDIM = 0x0D
HX8357B_RDDSDR = 0x0F

HX8357_SLPIN  = 0x10
HX8357_SLPOUT = 0x11
HX8357B_PTLON  = 0x12
HX8357B_NORON  = 0x13

HX8357_INVOFF = 0x20
HX8357_INVON  = 0x21
HX8357_DISPOFF= 0x28
HX8357_DISPON = 0x29

HX8357_CASET  = 0x2A
HX8357_PASET  = 0x2B
HX8357_RAMWR  = 0x2C
HX8357_RAMRD  = 0x2E

HX8357B_PTLAR  = 0x30
HX8357_TEON = 0x35
HX8357_TEARLINE = 0x44
HX8357_MADCTL = 0x36
HX8357_COLMOD = 0x3A

HX8357_SETOSC= 0xB0
HX8357_SETPWR1= 0xB1
HX8357B_SETDISPLAY= 0xB2
HX8357_SETRGB= 0xB3
HX8357D_SETCOM = 0xB6

HX8357B_SETDISPMODE = 0xB4
HX8357D_SETCYC = 0xB4
HX8357B_SETOTP = 0xB7
HX8357D_SETC = 0xB9

HX8357B_SET_PANEL_DRIVING = 0xC0
HX8357D_SETSTBA = 0xC0
HX8357B_SETDGC = 0xC1
HX8357B_SETID = 0xC3
HX8357B_SETDDB = 0xC4
HX8357B_SETDISPLAYFRAME = 0xC5
HX8357B_GAMMASET= 0xC8
HX8357B_SETCABC = 0xC9
HX8357_SETPANEL = 0xCC

HX8357B_SETPOWER = 0xD0
HX8357B_SETVCOM = 0xD1
HX8357B_SETPWRNORMAL = 0xD2

HX8357B_RDID1  = 0xDA
HX8357B_RDID2  = 0xDB
HX8357B_RDID3  = 0xDC
HX8357B_RDID4  = 0xDD

HX8357D_SETGAMMA = 0xE0

HX8357B_SETGAMMA = 0xC8
HX8357B_SETPANELRELATED = 0xE9

HX8357_BLACK  = 0x0000
HX8357_BLUE   = 0x001F
HX8357_RED    = 0xF800
HX8357_GREEN  = 0x07E0
HX8357_CYAN   = 0x07FF
HX8357_MAGENTA= 0xF81F
HX8357_YELLOW = 0xFFE0  
HX8357_WHITE  = 0xFFFF














######################################################################################################
######################################################################################################
######################################################################################################
######################################################################################################
def HX_SWRESET():
	WriteCmd (HX8357B_SWRESET) #software reset, puts display into sleep
	
def HX_SETC():
	WriteCmd(HX8357D_SETC)
	WriteByte(0xFF)
	WriteByte(0x83)
	WriteByte(0x57)
	time.sleep(0.3)
def HX_SETRGB():
	WriteCmd(HX8357_SETRGB)
	WriteByte(0x00)
	WriteByte(0x00)
	WriteByte(0x06)
	WriteByte(0x06)
def HX_SETCOM():
	WriteCmd(HX8357D_SETCOM)
	WriteByte(0x25)
def HX_SETOSC():
	WriteCmd(HX8357_SETOSC)
	WriteByte(0x68)
def HX_SETPANEL():
	WriteCmd(HX8357_SETPANEL)
	WriteByte(0x05)
def HX_SETPWR1():
	WriteCmd(HX8357_SETPWR1)
	WriteByte(0x00)
	WriteByte(0x15)
	WriteByte(0x1C)
	WriteByte(0x1C)
	WriteByte(0x83)
	WriteByte(0xAA)
def HX_SETSTBA():
	WriteCmd(HX8357D_SETSTBA)
	WriteByte(0x50)
	WriteByte(0x50)
	WriteByte(0x01)
	WriteByte(0x3C)
	WriteByte(0x1E)
	WriteByte(0x08)
	
def HX_SETCYC():
	WriteCmd(HX8357D_SETCYC)
	WriteByte(0x02)
	WriteByte(0x40)
	WriteByte(0x00)
	WriteByte(0x2A)
	WriteByte(0x2A)
	WriteByte(0x0D)
	WriteByte(0x78)
def HX_SETGAMMA():
	WriteCmd(HX8357D_SETGAMMA)
	WriteByte(0x02)
	WriteByte(0x0A)
	WriteByte(0x11)
	WriteByte(0x1d)
	WriteByte(0x23)
	WriteByte(0x35)
	WriteByte(0x41)
	WriteByte(0x4b)
	WriteByte(0x4b)
	WriteByte(0x42)
	WriteByte(0x3A)
	WriteByte(0x27)
	WriteByte(0x1B)
	WriteByte(0x08)
	WriteByte(0x09)
	WriteByte(0x03)
	WriteByte(0x02)
	WriteByte(0x0A)
	WriteByte(0x11)
	WriteByte(0x1d)
	WriteByte(0x23)
	WriteByte(0x35)
	WriteByte(0x41)
	WriteByte(0x4b)
	WriteByte(0x4b)
	WriteByte(0x42)
	WriteByte(0x3A)
	WriteByte(0x27)
	WriteByte(0x1B)
	WriteByte(0x08)
	WriteByte(0x09)
	WriteByte(0x03)
	WriteByte(0x00)
	WriteByte(0x01)

def HX_COLMOD():
	WriteCmd(HX8357_COLMOD)
	WriteByte(0x07) #Sets 16-bit color mode
	
def HX_MADCTL():
	WriteCmd(HX8357_MADCTL)
	WriteByte(0xC0)

def HX_TEON():
	WriteCmd(HX8357_TEON)
	WriteByte(0x00)
	
def HX_TEARLINE():
	WriteCmd(HX8357_TEARLINE)
	WriteByte(0x00)
	WriteByte(0x02)
def HX_SLPOUT():
	WriteCmd(HX8357_SLPOUT)
	time.sleep(0.15)
	
def HX_DISPON():
	WriteCmd(HX8357_DISPON)
	time.sleep(0.05)




######################################################################################################
######################################################################################################
######################################################################################################























########################################################################
## Low-level routines
# These routines access GPIO directly
#
def SetPin(pinNumber,value):
	#sets the GPIO pin to desired value (1=on,0=off)
	GPIO.output(pinNumber,value)
def InitIO():
	GPIO.setmode(GPIO.BCM)
	GPIO.setwarnings(False)
	GPIO.setup(DC,GPIO.OUT)
 
 
########################################################################
#
# Hardware SPI routines:
# 
# 
def WriteByte(value, data=True):
	SetPin(DC,data)
	spi.writebytes([value])
	
def WriteCmd(value):
	"Send command byte to display"
	WriteByte(value,False) #set D/C line to 0 = command
def WriteWord (value):
	"sends a 16-bit word to the display as data"
	WriteByte(value >> 8) #write upper 8 bits
	WriteByte(value & 0xFF) #write lower 8 bits
	
def WriteList (byteList):
	"Send list of bytes to display, as data"
	for byte in byteList: #grab each byte in list
		WriteByte(byte) #and send it
		
def Write888(value,width,count):
	"sends a 24-bit RGB pixel data to display, with optional repeat"
	red = value>>16 #red = upper 8 bits
	green = (value>>8) & 0xFF #green = middle 8 bits
	blue = value & 0xFF #blue = lower 8 bits
	RGB = [int(red),int(green),int(blue)] #assemble RGB as= byte list
	SetPin(DC,1)
	for a in range(count):
		spi.writebytes(RGB*width)
		
########################################################################
#
# ST7735 driver routines:
# 
# 
def InitDisplay():
	HX_SWRESET()
	HX_SETC()
	HX_SETRGB()
	HX_SETCOM()
	HX_SETOSC()
	HX_SETPANEL()
	HX_SETPWR1()
	HX_SETSTBA()
	HX_SETCYC()
	HX_SETGAMMA()
	HX_COLMOD()
	HX_MADCTL()
	HX_TEON()
	HX_TEARLINE()
	HX_SLPOUT()
	HX_DISPON()
	
def SetAddrWindow(x0,y0,x1,y1):
	"sets a rectangular display window into which pixel data is placed"
	WriteCmd(CASET) #set column range (x0,x1)
	WriteWord(x0)
	WriteWord(x1)
	WriteCmd(RASET) #set row range (y0,y1)
	WriteWord(y0)
	WriteWord(y1)
def FillRect(x0,y0,x1,y1,color):
	"fills rectangle with given color"
	width = x1-x0+1
	height = y1-y0+1
	SetAddrWindow(x0,y0,x1,y1)
	WriteCmd(RAMWR)
	Write888(color,width,height)
def FillScreen(color):
	"Fills entire screen with given color"
	FillRect(0,0,HX8357_TFTWIDTH-1,HX8357_TFTHEIGHT-1,color)
def ClearScreen():
	"Fills entire screen with black"
	FillRect(0,0,HX8357_TFTWIDTH-1,HX8357_TFTHEIGHT-1,BLACK)
def drawImage(x,y,imageName):
	im = Image.open(imageName)
	#x=30
	#y=30
	pix = im.load()
	width = im.size[1]
	height = im.size[0]
	for x in range(0, height):
		for y in range(0, width):
			color =  (pix[x,y][0]<<16) & 0xff0000 
			color += (pix[x,y][1]<<8) & 0x00ff00
			color  += pix[x,y][2]
			FillRect(x,y,x+1,y+1,color)
	#print pix[x,y]
########################################################################
#
# Testing routines:
# 
# 
def TimeDisplay():
	"Measures time required to fill display twice"
	startTime=time.time()
	print " Now painting screen GREEN"
	FillScreen(GREEN)
	print " Now clearing screen"
	ClearScreen()
	elapsedTime=time.time()-startTime
	print " Elapsed time %0.2f seconds" % (elapsedTime)
########################################################################
#
# Main Program
#
print "Adafruit 1.8 TFT display demo with hardware SPI"
spi = spidev.SpiDev()
spi.open(0,0)
spi.mode = 0
spi.max_speed_hz = 27777000
InitIO()
InitDisplay()
TimeDisplay()
#drawImage(0,0, "b.jpg")
spi.close()
print "Done."
# END ###############################################################
