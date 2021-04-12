#include "app_serial.h"
#include "proj/drivers/uart.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "proj/common/tstring.h"

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
