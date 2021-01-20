#include "app_serial.h"
#include "proj/drivers/uart.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "app_routes.h"

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

void serial_sent_data(serial *s, char *data, int len){
    if(s->name == UART_NAME){
        return;
    }
    
}

void serial_data_recv(serial *s){
    if(s->name == UART_NAME){
        return;
    }};

void serial_data_handle(serial *s, char *dt, unsigned int len){
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