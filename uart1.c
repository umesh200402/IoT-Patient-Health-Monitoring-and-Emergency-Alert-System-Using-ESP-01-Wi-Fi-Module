#include"header.h"
void uart1_init(unsigned int baud)
{
	int result,pclk;
	if(VPBDIV==0)
		pclk=15000000;
	else if(VPBDIV==1)
		pclk=60000000;
	else if(VPBDIV==2)
		pclk=30000000;
	result=pclk/(16*baud);
	PINSEL0|=(0X00000005<<16);
	U1LCR=0x83;
	U1DLL=result&0xFF;
	U1DLM=(result>>8)&0xFF;
	U1LCR=0x03;
}
#define THRE ((U1LSR>>5)&1)
void uart1_tx(unsigned char data)
{
	U1THR=data;
	while(THRE==0);
}
#define RDR (U1LSR&1)
unsigned char uart1_rx()
{
	while(RDR==0);
	return U1RBR;

}
void uart1_tx_string(char *ptr)	 //to transmit string
{
	while(*ptr)
	{
		uart1_tx(*ptr);
		ptr++;
	}
}
void uart1_rx_string(char *s,int len)//to receive string
{
	int i=0;
	for(i=0;i<len;i++)
	{
		while(RDR==0);
		s[i]=U1RBR;
		//if(s[i]=='\r')
		if(U1RBR=='\r')
			break;
	}
	s[i]=0;
}

int uart1_int()	 //to conver received characters to integer
{
	int sum=0;
	char ch;
	while((ch=uart1_rx())!=13)
	{
		uart1_tx(ch);
		sum=sum*10+(ch-48);
	}
	uart1_tx('\n');
	return sum;
}
		
