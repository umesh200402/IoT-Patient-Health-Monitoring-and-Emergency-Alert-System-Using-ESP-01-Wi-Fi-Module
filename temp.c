#include<lpc21xx.h>
#include"header.h"
float temp()
{
	int adc=0,i;
	float temp;

	for(i=0;i<20;i++)
	{
		adc+=adc_read(3);
	}
	temp=((float)(adc)/20)*0.3226;
	return temp;
}