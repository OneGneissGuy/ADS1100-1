/**************************************************************************/
/*
	Library for ADS1100 Analog to digital converter
	It uses VCC as a reference voltage making it ideal to measure
	resistance - thermistors are an example.
	
		The unit is made by Texas Instruments, 
	16-Bit, 128SPS, 1-Ch Delta-Sigma ADC w/ PGA, Oscillator & I2C
	http://www.ti.com/product/ADS1100#
	
	The library updated on 18 May 2019 by Nikolay Cherkasov
	Github: https://github.com/cherkasovn/ADS1100
	
	GNU General Public License v3.0
	Distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/
/**************************************************************************/

#include "ADS1100.h"
#if ARDUINO >= 100
	#include "Arduino.h"
	#else
	#include "WProgram.h"
#endif

#include <Wire.h>

/**************************************************************************/
/*
	Initializing the ADS1100 class
*/
/**************************************************************************/
ADS1100::ADS1100(byte address) 
{
	ads1100_i2cAddress=address;
}


/**************************************************************************/
/*
	Starting the I2C communication with the ADS1100 unit
*/
/**************************************************************************/
void ADS1100::begin(void)
{
	Wire.begin();
}

/**************************************************************************/
/*
	Reading the ADC dara regardless of the fact it is avaliable
*/
/**************************************************************************/
int16_t ADS1100::getvalue(void)
{
	int8_t highbyte;
	uint8_t lowbyte;
	Wire.requestFrom((uint8_t) ads1100_i2cAddress, (uint8_t) 2);  
	while(Wire.available())						// ensure all the data comes in
	{ 
		highbyte  = Wire.read();				// high byte * B11111111
		lowbyte   = Wire.read();				// low byte
	}
	value=(highbyte*256 + lowbyte);
	return value;
}

/**************************************************************************/
/*
	Configuring the operation mode of the ADS1100 unit
*/
/**************************************************************************/
void ADS1100::configure(uint8_t singleMode, uint8_t dataRate, uint8_t gain) {
	singleMode	&= ADS1100_CONVERSION_MASK;
	dataRate		&= ADS1100_DATA_RATE_MASK;
	gain			&= ADS1100_GAIN_MASK;
	currentRegister = singleMode+dataRate+gain;
	Wire.beginTransmission(ads1100_i2cAddress);
	#if ARDUINO >= 100
		Wire.write((uint8_t)currentRegister);
		#else
		Wire.send(currentRegister);
	#endif
	Wire.endTransmission();
}

/**************************************************************************/
/*
	Checking if the conversion is completed and reading the ADC value as well
	If the conversion is not completed yet, the ADC value is the old one
*/
/**************************************************************************/
uint8_t ADS1100::conversionDone() { 	// check if conversion was completed. Reasonable only in the single conversion mode
	int8_t highbyte;
	uint8_t lowbyte,regbyte;
	Wire.requestFrom((uint8_t) ads1100_i2cAddress, (uint8_t) 3);
	while(Wire.available()) // ensure all the data comes in
	{   
		highbyte = Wire.read(); // high byte * B11111111
		lowbyte  = Wire.read(); // low byte
		regbyte  = Wire.read(); // register byte
	}
	value=(highbyte*256+lowbyte);
	regbyte &= ADS1100_START_SINGLE_CONVERSION;
	return (regbyte==0);
}   

/**************************************************************************/
/*
	Requesting to start the reading the ADC value. The reading actually 
	takes the whole reading time
*/
/**************************************************************************/
void ADS1100::startSingleConversion() {
	// start the conversion mode
	currentRegister=currentRegister|ADS1100_START_SINGLE_CONVERSION;
	Wire.beginTransmission(ads1100_i2cAddress);
	#if ARDUINO >= 100
		Wire.write((uint8_t) currentRegister);
		#else
		Wire.send(currentRegister);
		#endif
	Wire.endTransmission();
}
