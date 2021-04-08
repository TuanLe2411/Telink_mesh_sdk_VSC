#ifndef __APP_LCD_HANDLER_H__
#define __APP_LCD_HANDLER_H__
#include "proj/common/types.h"

#ifdef LCD_TYPE
    extern void lcd_control_init();
#endif

extern void uart_lcd_handler_data(u8 *para, int len);
    
#endif