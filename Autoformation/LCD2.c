#include "LCD2.h"
#include "timer_config.h"

#define SLAVE_ADDRESS_LCD 0x4E // change this according to ur setup


void lcd_write2(char saddr,uint8_t *buffer, uint8_t length){
	
	I2C_start2();
	I2C_address2(saddr);
	I2C_MultiSend2(buffer,length);
	I2C_STOP2();
	
}
void lcd_write(char saddr,uint8_t *buffer, uint8_t length)
{	
while (I2C1->SR2 & I2C_SR2_BUSY);           //wait until bus not busy
I2C1->CR1 |= I2C_CR1_START;                   //generate start
while (!(I2C1->SR1 & I2C_SR1_SB)){;}					//wait until start is generated
volatile int Temp;														
I2C1->DR = saddr<< 1;                 	 			// Send slave address
while (!(I2C1->SR1 & I2C_SR1_ADDR)){;}        //wait until address flag is set
Temp = I2C1->SR2; 														//Clear SR2
//sending the data
for (uint8_t i=0;i<length;i++)
 { 
 I2C1->DR=buffer[i]; 													//filling buffer with command or data
	while (!(I2C1->SR1 & I2C_SR1_BTF));
 }	
                             
I2C1->CR1 |= I2C_CR1_STOP;										//wait until transfer finished

}

void lcd_send_cmd2 (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	lcd_write2(SLAVE_ADDRESS_LCD,(uint8_t *)data_t,4);
}
void lcd_send_data2 (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
	lcd_write2(SLAVE_ADDRESS_LCD,(uint8_t *)data_t,4);
}

void lcd_init2 (void)
{
	// 4 bit initialisation
	Delay_ms(50);  // wait for >40ms
	lcd_send_cmd2 (0x30);
	Delay_ms(5);  // wait for >4.1ms
	lcd_send_cmd2 (0x30);
	Delay_ms(1);  // wait for >100us
	lcd_send_cmd2 (0x30);
	Delay_ms(10);
	lcd_send_cmd2 (0x20);  // 4bit mode
	Delay_ms(10);

  // dislay initialisation
	lcd_send_cmd2 (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	Delay_ms(1);
	lcd_send_cmd2 (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	Delay_ms(1);
	lcd_send_cmd2 (0x01);  // clear display
	Delay_ms(1);
	Delay_ms(1);
	lcd_send_cmd2 (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	Delay_ms(1);
	lcd_send_cmd2 (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_send_string2 (char *str)
{
	while (*str) lcd_send_data2 (*str++);
}

void setCursor2(int a, int b)
{
	int i=0;
	switch(b){
	case 0:lcd_send_cmd2(0x80);break;
	case 1:lcd_send_cmd2(0xC0);break;
	case 2:lcd_send_cmd2(0x94);break;
	case 3:lcd_send_cmd2(0xd4);break;}
	for(i=0;i<a;i++)
	lcd_send_cmd2(0x14);
}

void lcd_clear2 (void)
{

	#define LCD_CLEARDISPLAY 0x01
	lcd_send_cmd2(LCD_CLEARDISPLAY);
	Delay_ms(100);
	
}