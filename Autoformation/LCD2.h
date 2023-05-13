#include "stm32f401xe.h"
#include "I2C_config.h"

void lcd_send_cmd2 (char cmd);

void lcd_send_data2 (char data);

void lcd_init2 (void);

void lcd_send_string2 (char *str);

void lcd_clear2 (void);

void setCursor2(int a, int b);

void lcd_write2(char saddr,uint8_t *buffer, uint8_t length);

void lcd_write(char saddr,uint8_t *buffer, uint8_t length);