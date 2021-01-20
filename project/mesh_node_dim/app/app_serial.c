#include "app_serial.h"
#include "proj/drivers/uart.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "app_routes.h"
#include "proj/common/types.h"
#define UART_NAME "uart"

#define CMD_LEN             (1)
#define CMD_MESH_MES_SENT   0x01

serial Uart;

void serial_init(serial *s, char *name){
    s->name = name;
    return;
}

void rx_from_uart_cb(){
	uart_ErrorCLR();
	
	uart_data_t* p = (uart_data_t *)my_fifo_get(&hci_rx_fifo);
	if(p){
		u32 rx_len = p->len & 0xff; //usually <= 255 so 1 byte should be sufficient
		if (rx_len)
		{
			serial_data_handle(&Uart, p->data, p->len);
		}
	}
	return;
}

void serial_print_data(serial *s, char *data, u16 len){
    if(s->name == UART_NAME){
        uart_print(data, len);
        return;
    }
    
}

void serial_data_recv(serial *s){
    if(s->name == UART_NAME){
        return;
    }};

void serial_data_handle(serial *s, char *dt, u16 len){
    if(s->name = UART_NAME){
        char cmd = dt[0];
        char *serial_data = dt + CMD_LEN;
        unsigned int serial_data_len = len - CMD_LEN;
        switch (cmd)
        {
            case CMD_MESH_MES_SENT:
                mesh_command_handle(serial_data, serial_data_len);
                break;
            default:
                break;
        }
    }
    
    return;
}

u8 mesh_get_hci_tx_fifo_cnt()
{
	return hci_tx_fifo.size;
}

int uart_print(u8 *para, int n){
	u8 fifoSize = mesh_get_hci_tx_fifo_cnt();
	if(n > (fifoSize - 2 - 1)){ // 2: size of length,  1: size of type
        return -1;
    }
	u8 head[1] = {HCI_RSP_USER};
	return my_fifo_push_hci_tx_fifo(para, n, head, 1);
}