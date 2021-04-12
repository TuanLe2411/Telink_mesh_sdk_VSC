#include "app_serial.h"
#include "proj/drivers/uart.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "proj/common/tstring.h"
#include "app_lcd_handler.h"

#define UART_LCD_DATA_HEADER	0x5AA5

#define LCD_DATA_LEN 			5

int uart_data_handler(u8* para, int len){
	u16 uart_data_type = (para[0] << 8) | para[1];
	u8 lcd_data_handler[LCD_DATA_LEN] = {para[len-5], para[len-4], para[len-3], para[len-2], para[len-1]};

	if(uart_data_type == UART_LCD_DATA_HEADER) {
		uart_lcd_handler_data(lcd_data_handler, LCD_DATA_LEN);
	}

    return 0;
}

int rx_from_uart_cb(void){
	uart_ErrorCLR();
	
	uart_data_t* p = (uart_data_t *)my_fifo_get(&hci_rx_fifo);
	if(p){
		u32 rx_len = p->len & 0xff; //usually <= 255 so 1 byte should be sufficient
		if (rx_len)
		{
            uart_data_handler(p->data, rx_len);
            my_fifo_pop(&hci_rx_fifo);
		}
	}
	return 0;
}