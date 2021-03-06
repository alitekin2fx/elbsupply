#include <avr/io.h>
#include "adc.h"

void ADC_init(void)
{
	ADMUX |= 0x00;		// ADC uses the internal 2.5V band gap reference from AREF
	ADCSRA = 0x87;		// 125kHz conversion clock (F_CPU/128), ADC enabled
}

uint16_t ADC_readRaw(uint8_t channel)
{
	uint16_t temp;
	ADMUX &= ~0x0F;				// clear channel MUX bits
	ADMUX |= (channel & 0x0F);	// select channel
	ADCSRA |= (1<<ADSC);		// start conversion
	while(ADCSRA & (1<<ADSC));
	temp = ADCW;				// read ADC value
	return temp;
}

/* Reads the output voltage of the linear PSU section
 * Returns scaled integer value of actual output voltage
 * of PSU 
 * i.e. return value is 100*Vout, e.g. 20V = 2000
 */
uint16_t ADC_readPSUOutV(void)
{
	uint32_t temp;
	temp = ADC_readRaw(6);	// read from ADC channel 6 
	temp *= 1500;
	temp = temp>>10;
	//temp = (uint16_t)((float)temp*voltageConstant);			
	return temp;
}

/* Reads the output current of the PSU
 * Returns scaled integer value of actual output current
 * of PSU 
 * return value is 1000*Iout, i.e. Iout in mA
 */
uint16_t ADC_readPSUOutI(void)
{
	uint16_t temp;
	temp = ADC_readRaw(7);	// read from ADC channel 7
	temp = temp * 3;
	return temp;
}
