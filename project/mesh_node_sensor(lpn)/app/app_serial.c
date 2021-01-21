#include "app_serial.h"
#include "proj/drivers/uart.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "app_routes.h"

#define UART_CMD_LEN    (1)
#define UART_MESH_CMD   0x01

u8 mesh_get_hci_tx_fifo_cnt()
{
	return hci_tx_fifo.size;
}

int uart_print_data(u8 *para, int n, u8 *head, u8 head_len){
	u8 fifoSize = mesh_get_hci_tx_fifo_cnt();
	if(n > (fifoSize - 2 - 1)){ // 2: size of length,  1: size of type
        return -1;
    }
	return my_fifo_push_hci_tx_fifo(para, n, head, head_len);
}

void serial_data_handle(unsigned char *dt, u16 len){
    unsigned char cmd = dt[0];
    unsigned char *serial_data = dt + UART_CMD_LEN;
    unsigned int serial_data_len = len - UART_CMD_LEN;
    switch (cmd)
    {
        case UART_MESH_CMD:
            if(MeshHandle.cmd_flag == 0){
                MeshHandle.cmd_flag = 1;
                mesh_command_handle(serial_data, serial_data_len);
            }
            break;
        default:
            break;
    }
    return;
}

void rx_from_uart_cb(){
	uart_ErrorCLR();
	
	uart_data_t* p = (uart_data_t *)my_fifo_get(&hci_rx_fifo);
	if(p){
		u32 rx_len = p->len & 0xff; //usually <= 255 so 1 byte should be sufficient
		if (rx_len)
		{
			serial_data_handle(p->data, p->len);
		}
	}
	return;
}



