#include"header.h"

#define LEDG (1<<17)
#define LEDR (1<<19)
#define BUZ (1<<22)
#define SW ((IOPIN0>>14)&1)
char s[20];

int main()
{
	float t;
	int hr,hrs=0,i,spo2=0;

	lcd_init();
	lcd_string("PATIENT HEALTH");
	lcd_cmd(0XC0);
	lcd_string("MONITERING");
	delay_ms(1000);
	uart0_init(9600);
	uart1_init(9600);
	adc_init();
	i2c_init();
	max_init();
	esp_init();
	IODIR0|=LEDG|LEDR|BUZ;
	IOSET0|=LEDR;
	IOCLR0|=LEDG|BUZ;

	while(1){	
		t=temp();
		hrs=spo2=0;
		for(i=0;i<3;i++){		
		spo2=max_read(&hr);
		hrs+=hr;
		}
		hr=hrs/3;
		esp_send_data(t,hr,spo2);            //sending data to cloud

		if(t>38)
		{
			IOCLR0|=LEDR;
			IOSET0|=LEDG|BUZ;
			lcd_cmd(0x1);
			sprintf(s,"HIGH TEMP");
			lcd_string(s);
			lcd_cmd(0xc0);
			sprintf(s,"ALERT: %.2f C",t);
			lcd_string(s);
			delay_ms(3000);
			lcd_cmd(0x01);
		}
		else{
			IOSET0|=LEDR;
			IOCLR0|=LEDG|BUZ;
			lcd_cmd(0x1);
			sprintf(s,"Temp:%.2f C",t);
			lcd_string(s);
		}
		if(hr>100||hr<60)
		{
			IOCLR0|=LEDR;
			IOSET0|=LEDG|BUZ;
			lcd_cmd(0x01);
			sprintf(s,"HEART RATE");
			lcd_string(s);
			lcd_cmd(0xc0);
			sprintf(s,"ABNORMAL: %d BPM",hr);
			lcd_string(s);
			delay_ms(3000);
			lcd_cmd(0x01);
		}
		else
		{
			IOSET0|=LEDR;
			IOCLR0|=LEDG|BUZ;
			lcd_cmd(0xc0);
			sprintf(s,"HR : %d BPM",hr);
			lcd_string(s);
			delay_ms(2000);
		}
		if(spo2<=91)
		{
			IOCLR0|=LEDR;
			IOSET0|=LEDG|BUZ;
			lcd_cmd(0x01);
			sprintf(s,"LOW SPO2");
			lcd_string(s);
			lcd_cmd(0xc0);
			sprintf(s,"ALERT: %d %%",spo2);
			lcd_string(s);
			delay_ms(3000);
			lcd_cmd(0x01);
		}
		else
		{
			IOSET0|=LEDR;
			IOCLR0|=LEDG|BUZ;
			lcd_cmd(0x80);
			sprintf(s,"SpO2: %d %%       ",spo2);
			lcd_string(s);
			delay_ms(2000);
		}

		if(spo2<85||(hr>=120)||(hr<=50)||(t>40))
		{
			lcd_cmd(0x1);
			lcd_string("EMERGENCY");
			lcd_cmd(0xc0);
			lcd_string("press SW to clear");
			while(1){
				if(SW==0){
					while(SW==0);
					break;
				}
				IOCLR0|=LEDR;
				IOSET0|=LEDG|BUZ;
				delay_ms(100);
				IOSET0|=LEDR;
				delay_ms(100);	
			}
			IOSET0|=LEDR;
			IOCLR0|=LEDG|BUZ;
		}
		lcd_cmd(0x1);
		sprintf(s,"Temp:%.2fC",t);
		lcd_string(s);
		lcd_cmd(0xc0);
		sprintf(s,"HR:%dBPM O2:%d%%",hr,spo2);
		lcd_string(s);
		delay_sec(8);
		lcd_cmd(0x1);
		lcd_string("........");
		delay_sec(1);

	}
}
