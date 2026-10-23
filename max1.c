#include<lpc21xx.h>
#include"header.h"
#define MAX 200

unsigned char wrptr,rdptr;
unsigned int ir[MAX],red[MAX],count,i,spo2,dcir=0,acred,dcred=0,acir;
float R;
unsigned char a[6];
int b[10];

void max_init()
{
	i2c_send(0xae,0x09,0x40);//reset max sensor
	delay_ms(20);
	i2c_send(0xae,0x02,0x40);//disable intr
	i2c_send(0xae,0x03,0x00);//disable intr
	i2c_send(0xae,0x04,0x00);//fw ptr 
	i2c_send(0xae,0x05,0x00);//ovr counter 0
	i2c_send(0xae,0x06,0x00);//fr ptr
	i2c_send(0xae,0x08,0x00);  
	i2c_send(0xae,0x09,0x03);//spo2 mode
	i2c_send(0xae,0x0a,0x27);//range 15pA TO 4096mA, 100 samples per sec, 18 bit ADC
	i2c_send(0xae,0x0c,0x24);//led current
	i2c_send(0xae,0x0d,0x24);//led current



	//delay_ms(100);
}

int max_read(int *p)
{
	unsigned int i;
	read();
	for(i=0;i<MAX;i++)
	{
		dcred+=red[i];
		dcir+=ir[i];
	}
//	for(i=0;i<MAX;i=i+3)
//	{
//			sprintf(s,"%d, ir=%d,red=%d  %d, ir=%d,red=%d  %d, ir=%d,red=%d\r\n",i+1,ir[i], red[i],i+1+1,ir[i+1],red[1+i],i+3,ir[i+2], red[2+i]);
//		uart0_tx_string(s);
//	}
	dcred =dcred/MAX;
	dcir =dcir/MAX;	
	acred=acr(red,MAX);
	acir=acr(ir,MAX);


	R=((((float)acred*(float)dcir)/((float)dcred*(float)acir)));
	spo2=(-45.060*R*R)+(30.354*R)+94.845;


	filter(ir,MAX);
	*p=pulse();



//	sprintf(s,"r=%.2f spo2=%d per\r\n",(R/25),spo2);
//	uart0_tx_string(s);
	if(spo2<=100)
		return spo2;
	else
		return 100;

}
float pulse(void)
{

	int j,last_peak;
	l:
	j=0;
	last_peak=-50;
	for(i=10;i<MAX-10;i++)
	{
		if((ir[i]>ir[i-10])&&(ir[i]>ir[i+10])){
			if(i-last_peak>=40){
				b[j++]=i;
				last_peak=i;
			}
		}
	}
	if(j<2){
		read();
		goto l;
	}

		R=b[1]-b[0];
	
	R=(60*100)/R;
//  	sprintf(s,"hr=%.2f bpm\r\n",R);
//	uart0_tx_string(s);
	return (int)R;
}
int acr(unsigned int *a,int l)
{
	int i,min=1000000,max=0;
	for(i=25;i<195;i++)
	{
		if(a[i]<min)
			min=a[i];
		else if(a[i]>max)
			max=a[i];
	}
	return ((max-min)/2);
}
void filter(unsigned int *a,int len)
{
	int i;
	for(i=2;i<(len-2);i++)
		a[i]=((a[i-2]+a[i-1]+a[i]+a[i+1]+a[i+2])/5);
}
void read(void)
{
	unsigned int i;
	i2c_send(0xae,0x09,0x03);
	
	for(i=0;i<35;i++)
	{
		while(1)
		{
			wrptr=i2c_read(0xaf,0x04)&0x0f;
			rdptr=i2c_read(0xaf,0x06)&0x0f;
			if(wrptr!=rdptr)
				break;
		}

	}
	for(i=0;i<MAX;i++)
	{
		while(1)
		{
			wrptr=i2c_read(0xaf,0x04)&0x0f;
			rdptr=i2c_read(0xaf,0x06)&0x0f;
			if(wrptr!=rdptr)
				break;
		}
		i2c_read_max(0xaf,0x07,a);
		red[i]=((a[0]<<16)|(a[1]<<8)|a[2]);
		red[i]&=0x3ffff;
		ir[i]=((a[3]<<16)|(a[4]<<8)|a[5]);
		ir[i]&=0x3ffff;
	}
}
