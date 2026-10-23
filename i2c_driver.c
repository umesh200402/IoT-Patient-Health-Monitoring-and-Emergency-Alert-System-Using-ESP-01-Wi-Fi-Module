/*i2c_driver.c*/

#include <LPC21xx.H>

#include "header.h"


void i2c_init(void){

	PINSEL0|=0x50;//P0.2-> SCL, P0.3-> SDA

	I2CONSET=(1<<6);//I2EN=1, AA=0 (Master mode selecte, I2C interface is enabled)

	I2SCLH=I2SCLL=75;//Std speed mode

}


#define SI ((I2CONSET>>3)&1)

void i2c_send(u8 sa, u8 mr, u8 data){

	/*step1: generate start condi*/

	I2CONSET=(1<<5);//STA=1

	I2CONCLR=(1<<3);//clear SI* 

	while(SI==0);//waiting for start condi

							 //to generate

	I2CONCLR=(1<<5);//STA=0


	/*step2: send SA+W & check ack*/

	I2DAT=sa;//send SA+W

	I2CONCLR=(1<<3);//SI=0

	while(SI==0);

	if(I2STAT==0x20){

	 uart0_tx_string("ERR: SA+W\r\n");

	 goto exit; 

	}


	/*step3: send mr addr & check ack*/

	I2DAT=mr;//send memory addr

	I2CONCLR=(1<<3);//SI=0

	while(SI==0);

	if(I2STAT==0x30){

	 uart0_tx_string("ERR: Memory Addr\r\n");

	 goto exit; 

	}

	

	/*step4: send data & check ack*/

	I2DAT=data;//send data

	I2CONCLR=(1<<3);//SI=0

	while(SI==0);

	if(I2STAT==0x30){

	 uart0_tx_string("ERR: Data\r\n");

	 goto exit; 

	}

	

	/*step5: generate stop condi*/

	exit:

			I2CONSET=(1<<4);//STO=1

			I2CONCLR=(1<<3);//clear SI*


}


u8 i2c_read(u8 sa, u8 mr){

	u8 temp;

	/*step1: generate start condi*/

	I2CONSET=(1<<5);//STA=1

	I2CONCLR=(1<<3);//clear SI* 

	while(SI==0);//waiting for start condi

							 //to generate

	I2CONCLR=(1<<5);//STA=0

	

	/*step2: send SA+W & check ack*/

	I2DAT=sa^1;//send SA+W

	I2CONCLR=(1<<3);//SI=0

	while(SI==0);

	if(I2STAT==0x20){

	 uart0_tx_string("ERR: SA+W\r\n");

	 goto exit; 

	}

	

	/*step3: send mr addr & check ack*/

	I2DAT=mr;//send memory addr

	I2CONCLR=(1<<3);//SI=0

	while(SI==0);

	if(I2STAT==0x30){

	 uart0_tx_string("ERR: Memory Addr\r\n");

	 goto exit; 

	}

	

	/*step4: generate restart condi*/

	I2CONSET=(1<<5);//STA=1

	I2CONCLR=(1<<3);//clear SI* 

	while(SI==0);//waiting for start condi

							 //to generate

	I2CONCLR=(1<<5);//STA=0

	

	/*step5: send SA+R & check ack*/

	I2DAT=sa;//send SA+R

	I2CONCLR=(1<<3);

	while(SI==0);

	if(I2STAT==0x48){

		uart0_tx_string("ERR: SA+R\r\n");

		goto exit;

	}

	

	

	/*step6: read data from slave & send noack*/

	I2CONCLR=(1<<3);//SI=0

	while(SI==0);//waiting for data to receive

	temp=I2DAT;

	

	

	/*step7: generate stop condi*/

	exit:

			I2CONSET=(1<<4);//STO=1

			I2CONCLR=(1<<3);//clear SI*

	

	/*step8: return data*/

	return temp;

}




void i2c_read_max(u8 sa, u8 mr,u8 *data){
	int i;
	/*step1: generate start condi*/

	I2CONSET=(1<<5);//STA=1

	I2CONCLR=(1<<3);//clear SI* 

	while(SI==0);//waiting for start condi

							 //to generate

	I2CONCLR=(1<<5);//STA=0

	

	/*step2: send SA+W & check ack*/

	I2DAT=sa^1;//send SA+W

	I2CONCLR=(1<<3);//SI=0

	while(SI==0);

	if(I2STAT==0x20){

	 uart0_tx_string("ERR: SA+W\r\n");
	 goto exit; 

	}

	

	/*step3: send mr addr & check ack*/

	I2DAT=mr;//send memory addr

	I2CONCLR=(1<<3);//SI=0

	while(SI==0);

	if(I2STAT==0x30){

	 uart0_tx_string("ERR: Memory Addr\r\n");
	 goto exit; 

	}

	

	/*step4: generate restart condi*/

	I2CONSET=(1<<5);//STA=1

	I2CONCLR=(1<<3);//clear SI* 

	while(SI==0);//waiting for start condi

							 //to generate

	I2CONCLR=(1<<5);//STA=0

	

	/*step5: send SA+R & check ack*/

	I2DAT=sa;//send SA+R

	I2CONCLR=(1<<3);

	while(SI==0);

	if(I2STAT==0x48){

		uart0_tx_string("ERR: SA+R\r\n");

		goto exit;

	}

	for(i=0;i<5;i++)
	{
		I2CONSET=(1<<2);
		I2CONCLR=(1<<3);
		while(SI==0);
		data[i]=I2DAT;
	}	

	

	I2CONCLR=(1<<2);
	I2CONCLR=(1<<3);//SI=0

	while(SI==0);//waiting for data to receive

	data[5]=I2DAT;

	

	

	/*step7: generate stop condi*/

	exit:

			I2CONSET=(1<<4);//STO=1

			I2CONCLR=(1<<3);//clear SI*

	

	/*step8: return data*/
}




