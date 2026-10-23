#include<lpc21xx.h>
#include"header.h"
void adc_init()
{
	PINSEL1|=0x15400000;
	ADCR=0x00200400;
}
#define DONE ((ADDR>>31)&1)
unsigned int adc_read(int ch_num)
{
	unsigned int result=0;
	ADCR|=(1<<ch_num);	//select channel number
	ADCR|=(1<<24); //start ADC
	while(DONE==0); //wait for adc conversion;
	ADCR&=~(1<<24); //or ADCR^=1<<24;//stop adc
	ADCR&=~(1<<ch_num); //Diselect ADC
	result=(ADDR>>6&0x3FF); //extract 10 bit ADC result
	return result;
}