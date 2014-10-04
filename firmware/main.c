//MSP430 UART Example
//http://msp430.gorillaembedded.com
// By Silver Back [Gorilla Embedded ]
// MSP430G2553 P1.1 and P1.2 as TX and RX



#include "msp430g2553.h"

//function prototype

void send_uart(char *data);



void main( void )
{
   //Initialization

    WDTCTL = WDTPW + WDTHOLD;               // Stop WDT , not used
   
   //Calibrate DCO 
    BCSCTL1 = CALBC1_1MHZ;                  // DCO at 1 MHz
    DCOCTL = CALDCO_1MHZ;                   // DCO at 1 MHz
  
   // UART Set-Up
    P1SEL |= 0x06;                       // Use P1.1 and P1.2 as USCI_A0
    P1SEL2|= 0x06;                        // Use P1.1 and P1.2 as USCI_A0
    P1DIR |= 0x04;                          // Set 1.2 as output
    UCA0CTL1 |= UCSSEL_2;                   // Use SMCLK / DCO 
    UCA0BR0 = 104;                          // 1 MHz -> 9600   N=Clock/Baud
    UCA0BR1 = 0;                            // 1 MHz -> 9600
    UCA0MCTL = UCBRS1;                      // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI  

   //Indicator LED 
    P1DIR |= BIT6;

    //General Interrupt Enable
      _BIS_SR(GIE);
      


    
    //Repeat  this section
    
    while(1)
    {
      send_uart("ATD0046706948464\r");
     __delay_cycles(500000);
     while(1)
     {
     }
    }
}



void send_uart(char *data)
{
    P1OUT ^= BIT6;  // Toggle LED 

    unsigned int i;
    unsigned int size = strlen(data);      //get length of data to be sent
    for (i = 0; i < size; i++) {
        
        while (!(IFG2 & UCA0TXIFG));      //Wait UART to finish before next send
        UCA0TXBUF = data[i];
                    }

}