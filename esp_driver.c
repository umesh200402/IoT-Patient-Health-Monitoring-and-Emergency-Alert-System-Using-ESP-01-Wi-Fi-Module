//esp driver//

#include"header.h"
#include<string.h>

#define WNAME "umesh"
#define WPASS "@1234567"
		
	void esp_send_data(float temp,int bpm,int spo2)
	{			
		char data[500],data_len[50];
		int len; 
		uart0_tx_string("starting sending data\r\n");

		sprintf(data,"GET /update?api_key=JIB14QYZBF3G7R0M&field1=%.2f&field2=%d&field3=%d HTTP/1.1\r\nHost: api.thingspeak.com\r\nConnection: close\r\n\r\n",temp,bpm,spo2);
		
		len=strlen(data);
		
		sprintf(data_len,"AT+CIPSEND=%d\r\n",len);
		
		/*create connection*/
		uart1_tx_string("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
		delay_ms(500);

		/*send data length*/
		uart1_tx_string(data_len);	      	
		delay_ms(500);

		/*send data*/
		uart1_tx_string(data);

		//delay_sec(15);
		uart0_tx_string("Updated\r\n");
	
	}	 
		  

void esp_init()
{ 
		char s[10],c[40];

		uart0_tx_string("Wifi Connecting...\r\n");
		lcd_cmd(0x1);
		lcd_string("Wifi Connecting");

		uart1_tx_string("AT\r\n");
		uart1_rx_string(s,10);
		uart0_tx_string(s);
	

		if(strstr(s,"AT\r\n\r\nOK")!=0){
			uart0_tx_string("Device detected\r\n");
			lcd_cmd(0x1);
			lcd_string("Device detected");
		}
		delay_ms(500);

		uart1_tx_string("AT+CWMODE=1\r\n");

		delay_ms(500);
	W:		
		uart1_tx_string("AT+CWJAP=\"shivam\",\"@1234567\"\r\n");
		delay_sec(5);	  

		uart1_tx_string("AT+CWJAP?\r\n");
		uart1_rx_string(c,32);


		if(strstr(c,"shivam")!=0){
			uart0_tx_string("WIFI connect\r\n");
			lcd_cmd(0x1);
			lcd_string("WIFI connect");
		}
		else
		{
			uart0_tx_string("WIFI not connect\r\n");
			lcd_cmd(0x1);
			lcd_string("WIFI not connect");
			goto W;
		}
				
}

