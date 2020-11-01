/* 
    This section define how command received from Gateway was handled
    ----------writer by LeTuan------------
*/

#include "app.h"
#include "app_command.h"
#include "app_transport.h"

void app_cmd_from_rec_handler(u8 *buff, int n){
	u16 type = buff[0] + (buff[1] << 8);
	u8 *hci_data = buff + HCI_CMD_LEN;
    u16 hci_data_len = n - HCI_CMD_LEN;

	if (HCI_CMD_GATEWAY_CTL == type){
		#if GATEWAY_ENABLE
    	gateway_cmd_from_host_ctl(hci_data, hci_data_len);
    	#endif
	}
}

int rx_from_trans_cb(){

    uart_ErrorCLR();
	
	rec_data_t* p = (rec_data_t *)my_fifo_get(&hci_rx_fifo);
	if(p){
		unsigned int rx_len = p->data_length & 0xff; //usually <= 255 so 1 byte should be sufficient
		if (rx_len)
		{
			app_cmd_from_rec_handler(p->data, rx_len);
			my_fifo_pop(&hci_rx_fifo);
		}
	}
	return 0;
}


