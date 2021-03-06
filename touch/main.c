#define cspin
#include <SPI.h>
#include <Wire.h>

#include "STMPE610.h"
#define STMPE_CS 10

//Fixad
void writeRegister8(uint8_t reg, uint8_t val) {
	digitalWrite(_CS, LOW);
	spiOut(reg); 
	spiOut(val);
	digitalWrite(_CS, HIGH);
}

//Fixad
void Adafruit_STMPE610(uint8_t cspin) {
	_CS = cspin;
	_MOSI = _MISO = _CLK = -1;
  	wiringPiSPISetup (0, 1000000);
  }

//Fixad
void begin() {  	
    // hardware SPI
	pinMode(_CS, OUTPUT);
	digitalWrite(_CS, HIGH);
		

  	writeRegister8(STMPE_SYS_CTRL1, STMPE_SYS_CTRL1_RESET);
  	delay(10);

  	for (uint8_t i=0; i<65; i++) {
  		readRegister8(i);
  	}

  writeRegister8(STMPE_SYS_CTRL2, 0x0); // turn on clocks!
  writeRegister8(STMPE_TSC_CTRL, STMPE_TSC_CTRL_XYZ | STMPE_TSC_CTRL_EN); // XYZ and enable!
  //Serial.println(readRegister8(STMPE_TSC_CTRL), HEX);
  writeRegister8(STMPE_INT_EN, STMPE_INT_EN_TOUCHDET);
  writeRegister8(STMPE_ADC_CTRL1, STMPE_ADC_CTRL1_10BIT | (0x6 << 4)); // 96 clocks per conversion
  writeRegister8(STMPE_ADC_CTRL2, STMPE_ADC_CTRL2_6_5MHZ);
  writeRegister8(STMPE_TSC_CFG, STMPE_TSC_CFG_4SAMPLE | STMPE_TSC_CFG_DELAY_1MS | STMPE_TSC_CFG_SETTLE_5MS);
  writeRegister8(STMPE_TSC_FRACTION_Z, 0x6);
  writeRegister8(STMPE_FIFO_TH, 1);
  writeRegister8(STMPE_FIFO_STA, STMPE_FIFO_STA_RESET);
  writeRegister8(STMPE_FIFO_STA, 0);    // unreset
  writeRegister8(STMPE_TSC_I_DRIVE, STMPE_TSC_I_DRIVE_50MA);
  writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints
  writeRegister8(STMPE_INT_CTRL, STMPE_INT_CTRL_POL_HIGH | STMPE_INT_CTRL_ENABLE);
}

// kolla i main if != 0
uint8_t touched(void) {
	return (readRegister8(STMPE_TSC_CTRL) & 0x80);
}
// kolla i main if != 0
uint8_t bufferEmpty(void) {
	return (readRegister8(STMPE_FIFO_STA) & STMPE_FIFO_STA_EMPTY);
}

uint8_t bufferSize(void) {
	return readRegister8(STMPE_FIFO_SIZE);
}

uint16_t getVersion() {
	uint16_t v;
  //Serial.print("get version");
	v = readRegister8(0);
	v <<= 8;
	v |= readRegister8(1);
  //Serial.print("Version: 0x"); Serial.println(v, HEX);
	return v;
}






void readData(uint16_t *x, uint16_t *y, uint8_t *z) {
	uint8_t data[4];

	for (uint8_t i=0; i<4; i++) {
    	data[i] = readRegister8(0xD7); //SPI.transfer(0x00); 
   // Serial.print("0x"); Serial.print(data[i], HEX); Serial.print(" / ");
	}
	*x = data[0];
	*x <<= 4;
	*x |= (data[1] >> 4);
	*y = data[1] & 0x0F; 
	*y <<= 8;	
	*y |= data[2]; 
	*z = data[3];

	if (bufferEmpty()){
		writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints
	}
    
}



uint8_t spiIn() {
		uint8_t d=0;
		wiringPiSPIDataRW(0, &d, 1);
		return d;
	
}


void spiOut(uint8_t x) {  
		wiringPiSPIDataRW(0, &x, 1);
}


//Fixad
uint8_t readRegister8(uint8_t reg) {
	uint8_t x ;

	SPI.beginTransaction(mySPISettings);
	
	digitalWrite(_CS, LOW);
	reg |= 0x80;
	spiOut(reg);
	spiOut(0x00);
	x = spiIn(); 
	digitalWrite(_CS, HIGH);
	return x;
}

//Fixad
uint16_t readRegister16(uint8_t reg) {
	uint16_t x;
    // hardware SPI
	digitalWrite(_CS, LOW);
	spiOut(0x80 | reg); 
	spiOut(0x00);
	x = spiIn(); 
	x<<=8;
	x |= spiIn(); 
	digitalWrite(_CS, HIGH);
	return x;
}
//Fixad
void writeRegister8(uint8_t reg, uint8_t val) {

	
	digitalWrite(_CS, LOW);
	spiOut(reg); 
	spiOut(val);
	digitalWrite(_CS, HIGH);
	
	}
delay(10);
}

int main(void)
{
	Adafruit_STMPE610(cspin);
	begin();
	


	uint16_t x, y;
	uint8_t z;
	if (touched()) {
    // read x & y & z;
		while (!bufferEmpty()) {
			printf("%d", bufferSize());
			readData(&x, &y, &z);
			printf("->("); 
			printf("%d", x); 
			printf(", "); 
			printf("%d", y); 
			printf(", "); 
			printf("%d", z);
			printf(")\n");
			}
    	writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints
}