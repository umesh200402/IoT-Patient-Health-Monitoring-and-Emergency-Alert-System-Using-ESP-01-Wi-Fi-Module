#include<lpc21xx.h>
#include<stdio.h>
#include<string.h>


extern void adc_init(void);
extern unsigned int adc_read(int);
extern void lcd_data(unsigned char);
extern void lcd_cmd(unsigned char);
extern void lcd_string(char *);
extern void lcd_init(void);
extern void uart0_init(unsigned int);
extern void uart0_tx(unsigned char);
extern unsigned char uart0_rx(void);
extern void uart0_tx_string(char *);
extern void uart0_rx_string(char *,int);
extern int uart0_int(void);

extern void uart1_init(unsigned int);
extern void uart1_tx(unsigned char);
extern unsigned char uart1_rx(void);
extern void uart1_tx_string(char *);
extern void uart1_rx_string(char *,int);
extern int uart1_int(void);

extern void delay_sec(unsigned int);
extern void delay_ms(unsigned int);
void lcd_integer(int );
float temp_read(void);
typedef unsigned char u8;
void i2c_init(void);
void i2c_send(u8 , u8 , u8 );
int bcdtodec(int );
int dectobcd(int );
void eint0_init(void);
struct clk
{
	u8 h,m,s,dd,mm,yy,am_pm,day;
};
void settings(struct clk);
void lcd_time(struct clk);
void lcd_date(struct clk);
void max_init(void);
int max_read(int *p);
float pulse(void);
int acr(unsigned int *a,int l);
void filter(unsigned int *a,int len);
void i2c_read_max(u8 sa, u8 mr,u8 *data);
u8 i2c_read(u8 sa, u8 mr);
float temp(void);
void read(void);

void esp_init(void);
void esp_send_data(float,int,int);
