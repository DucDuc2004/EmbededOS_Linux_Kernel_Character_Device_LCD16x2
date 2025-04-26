#include <linux/gpio.h>
#include <linux/delay.h>
#include "lcd_driver.h"

static void lcd_pulse_enable(void) {
    gpio_set_value(LCD_E, 1);
    udelay(1);                 
    gpio_set_value(LCD_E, 0);
    udelay(100);              
}

static void lcd_send_nibble(uint8_t nibble) {
    gpio_set_value(LCD_D4, (nibble >> 0) & 0x1);
    gpio_set_value(LCD_D5, (nibble >> 1) & 0x1);
    gpio_set_value(LCD_D6, (nibble >> 2) & 0x1);
    gpio_set_value(LCD_D7, (nibble >> 3) & 0x1);
    lcd_pulse_enable();
}

void lcd_send_command(uint8_t cmd) {
    gpio_set_value(LCD_RS, 0);         
    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd & 0x0F);
    mdelay(2);                        
}

void lcd_send_data(uint8_t data) {
    gpio_set_value(LCD_RS, 1);        
    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data & 0x0F);
    mdelay(1);
}

void lcd_init(void) {
    gpio_request(LCD_RS, "LCD_RS");
    gpio_request(LCD_E,  "LCD_E");
    gpio_request(LCD_D4, "LCD_D4");
    gpio_request(LCD_D5, "LCD_D5");
    gpio_request(LCD_D6, "LCD_D6");
    gpio_request(LCD_D7, "LCD_D7");

    gpio_direction_output(LCD_RS, 0);
    gpio_direction_output(LCD_E,  0);
    gpio_direction_output(LCD_D4, 0);
    gpio_direction_output(LCD_D5, 0);
    gpio_direction_output(LCD_D6, 0);
    gpio_direction_output(LCD_D7, 0);

    mdelay(20); 

    lcd_send_nibble(0x03);  
    mdelay(5);
    lcd_send_nibble(0x03);
    udelay(150);
    lcd_send_nibble(0x03);
    lcd_send_nibble(0x02);  

    lcd_send_command(LCD_CMD_FUNCTION_SET);
    lcd_send_command(LCD_CMD_DISPLAY_ON);
    lcd_send_command(LCD_CMD_CLEAR_DISPLAY);
    lcd_send_command(LCD_CMD_ENTRY_MODE);
}

void lcd_clear(void) {
    lcd_send_command(LCD_CMD_CLEAR_DISPLAY);
    mdelay(2);
}
