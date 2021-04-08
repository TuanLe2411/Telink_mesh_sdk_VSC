#include "app_serial.h"
#include "proj/drivers/uart.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "proj/common/tstring.h"
#include "app_lcd_handler.h"

#define UART_LCD_DATA_HEADER	0x5AA5

#define LCD_DATA_LEN 			5

u8 mesh_get_hci_tx_fifo_cnt()
{
	return hci_tx_fifo.size;
}

int uart_print_char(u8 para){
	u8 fifoSize = mesh_get_hci_tx_fifo_cnt();
	if(1 > (fifoSize - 2 - 1)){ // 2: size of length,  1: size of type
        return -1;
    }
	u8 data[1] = {para};
	return my_fifo_push_hci_tx_fifo(data, 1, 0, 0);
}

int uart_print_data(u8 *para, int n, u8 *head, u8 head_len){
	u8 fifoSize = mesh_get_hci_tx_fifo_cnt();
	if(n > (fifoSize - 2 - 1)){ // 2: size of length,  1: size of type
        return -1;
    }
	return my_fifo_push_hci_tx_fifo(para, n, head, head_len);
}

char hex_char[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
void uart_print_hex_data(u8 *para, int n){
	
}

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