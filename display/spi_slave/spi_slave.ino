// Written by Nick Gammon
// February 2011
/**
 * Send arbitrary number of bits at whatever clock rate (tested at 500 KHZ and 500 HZ).
 * This script will capture the SPI bytes, when a '\n' is recieved it will then output
 * the captured byte stream via the serial.
 */

#include <SPI.h>

volatile byte c;
volatile byte pos;
volatile boolean process_it;

void setup (void)
{
  Serial.begin (9600);   // debugging

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // get ready for an interrupt 
  process_it = false;

  // now turn on interrupts
  SPI.attachInterrupt();

}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
  c = SPDR;  // grab byte from SPI Data Register

  // add to buffer if room
    // example: newline means time to process buffer
    process_it = true;

    // end of room available
}  // end of interrupt routine SPI_STC_vect

// main loop - wait for flag set in interrupt routine
void loop (void)
{
  if (process_it)
  {
    Serial.println(c, HEX);
    process_it = false;
  }  // end of flag set

}  // end of loop

