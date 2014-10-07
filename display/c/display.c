// make all
// MSP430Flasher.exe -n msp430g2xx3 -w "main.hex" -v -z [VCC] -m SBW2
#include <msp430.h>				

void drawPixel(int16_t x, int16_t y, uint16_t color) {

  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

  setAddrWindow(x,y,x+1,y+1);

  //digitalWrite(_dc, HIGH);
  *dcport |=  dcpinmask;
  //digitalWrite(_cs, LOW);
  *csport &= ~cspinmask;

  /* 18 bit hack for testing */
  /*
  uint8_t r = (color >> 10) & 0x1F, g = (color >> 5) & 0x3F, b = color & 0x1F;
  r <<= 3;
  g <<= 2;
  b <<= 3;

  spiwrite(r);
  spiwrite(g);
  spiwrite(b);
  */

  spiwrite(color >> 8);
  spiwrite(color);

  *csport |= cspinmask;
  //digitalWrite(_cs, HIGH);
}

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	P1DIR |= 0x01;					// Set P1.0 to output direction

//--------------SPI------------
	P1OUT |= BIT5;
    P1DIR |= BIT5;  
    P1SEL = BIT1 | BIT2 | BIT4;
    P1SEL2 = BIT1 | BIT2 | BIT4;
 
    UCA0CTL1 = UCSWRST;
    UCA0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;  // 3-pin, 8-bit SPI master
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 |= 0x02;                          // /2
    UCA0BR1 = 0;                              //
    UCA0MCTL = 0;                             // No modulation
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

    P1OUT &= (~BIT5); // Select Device

    while (!(IFG2 & UCA0TXIFG));   // USCI_A0 TX buffer ready? 
    UCA0TXBUF = 0xAA;              // Send 0xAA over SPI to Slave
}

	for(;;) {
		
	}
	
	return 0;
}
