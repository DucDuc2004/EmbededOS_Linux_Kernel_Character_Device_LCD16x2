#ifndef _LCD_DRIVER_H_
#define _LCD_DRIVER_H_

#include <linux/types.h>
#include <linux/gpio.h>
#include <linux/delay.h>

#define LCD_RS     26
#define LCD_E      19
#define LCD_D4     13
#define LCD_D5     6
#define LCD_D6     5
#define LCD_D7     11

#define LCD_CMD_CLEAR_DISPLAY  0x01
#define LCD_CMD_RETURN_HOME    0x02
#define LCD_CMD_ENTRY_MODE     0x06
#define LCD_CMD_DISPLAY_ON     0x0C
#define LCD_CMD_FUNCTION_SET   0x28  
#define LCD_CMD_SET_CURSOR     0x80 

#define LCD_NUM_COLS 16
#define LCD_NUM_ROWS 2

void lcd_send_command(uint8_t cmd);
void lcd_send_data(uint8_t data);
void lcd_init(void);
void lcd_clear(void);

#endif 
