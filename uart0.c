#include"header.h"
void uart0_init(unsigned int baud)
{
	int result,pclk;
	if(VPBDIV==0)
		pclk=15000000;
	else if(VPBDIV==1)
		pclk=60000000;
	else if(VPBDIV==2)
		pclk=30000000;
	result=pclk/(16*baud);
	PINSEL0|=0X00000005;
	U0LCR=0x83;
	U0DLL=result&0xFF;
	U0DLM=(result>>8)&0xFF;
	U0LCR=0x03;
}
#define THRE ((U0LSR>>5)&1)
void uart0_tx(unsigned char data)
{
	U0THR=data;
	while(THRE==0);
}
#define RDR (U0LSR&1)
unsigned char uart0_rx()
{
	while(RDR==0);
	return U0RBR;

}
void uart0_tx_string(char *ptr)	 //to transmit string
{
	while(*ptr)
	{
		uart0_tx(*ptr);
		ptr++;
	}
}
void uart0_rx_string(char *s,int len)//to receive string
{
	int i=0;
	for(i=0;i<len;i++)
	{
		while(RDR==0);
		s[i]=U0RBR;
		if(s[i]=='\r')
			break;
	}
	s[i]=0;
}

int uart0_int()	 //to conver received characters to integer
{
	int sum=0;
	char ch;
	while((ch=uart0_rx())!=13)
	{
		uart0_tx(ch);
		sum=sum*10+(ch-48);
	}
	uart0_tx('\n');
	return sum;
}
		
