/**************************************************************************/
/*
  Example of the ADS1100 library for
  Analog to digital converter. The unit uses VCC as a reference voltage 
  making it ideal to measure resistance - thermistors are an example
  
  The library updated on 18 May 2019 by Nikolay Cherkasov
  Github: https://github.com/cherkasovn/ADS1100

  The example reads ADC data from the unit and performs conversion
  only when requested.continuous reading from the unit. 
  The output is performed only when conversion is performed 
  actively checking for this condition. 
*/
/**************************************************************************/

#include <Wire.h>
#include <ADS1100.h>

#define ADC_ADRESS 0x48   // It is defined by the unit. 0x48 - ADS1100, see datasheet for other addresses

ADS1100 adc=ADS1100(ADC_ADRESS);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  adc.begin();
  adc.configure(ADS1100_CONVERSION_SINGLE,ADS1100_DATA_RATE_8SPS,ADS1100_GAIN_1X);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1); // See datasheet for minimum timing. It is ~125 ms for maximum resolution
  if (adc.conversionDone())  // the function reads the value and the status. The status remails constant until the conversion started
  {
    Serial.print("ADC value =");
    Serial.println(adc.value);
    
    adc.startSingleConversion(); // the function starts the conversion process.
    delay(30);
  }
}
