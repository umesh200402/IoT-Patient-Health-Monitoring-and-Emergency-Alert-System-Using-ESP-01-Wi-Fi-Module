#include"header.h"
void spi0_init(void)
{
	PINSEL0|=0x00001500;//P0.4-->CLK0 //P0.5-->MISO0 //P0.6-->MOSI0	

	IODIR0|=(1<<7);//P0.7-->Out dir(CS0) 	

	IOSET0=(1<<7);//*CS0=1

	S0SPCCR=15;//spi0 speed==1Mbps

	S0SPCR=0x20;//CPOL=CPHA=0
}
#define SPIF ((S0SPSR>>7)&1)

unsigned char spi0(unsigned char data)
{

	S0SPDR=data;//data to slave

	while(SPIF==0);

	return S0SPDR;//data from slave

}