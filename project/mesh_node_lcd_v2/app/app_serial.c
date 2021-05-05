#include "app_serial.h"
#include "proj/drivers/uart.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "proj/common/tstring.h"
#include "app_lcd_handler.h"
#include "user_lib/user_fifo.h"
#include "user_lib/serial.h"

uart_mess_t uart_last_mess_sent = {0};

u16 find_first_valid_uart_mess_header(){
	u16 count = 0;
	u16 i = 0;
	u16 r_point = user_fifo_get_r_point();
	while (count <= user_fifo_get_number_bytes_written())
	{
		u16 i1 = (i + r_point);
		u16 i2 = (i + r_point + 1);
		u16 i3 = (i + r_point + 2);
		if(i1 > MAX_QUEUE_LEN -1){
			i1 = i1 - MAX_QUEUE_LEN;
		}
		if(i2 > MAX_QUEUE_LEN -1){
			i2 = i2 - MAX_QUEUE_LEN;
		}if(i1 > MAX_QUEUE_LEN -1){
			i3 = i3 - MAX_QUEUE_LEN;
		}
		if((user_fifo_get(i1) == UART_HEADER_DATA_0) && (user_fifo_get(i2) == UART_HEADER_DATA_0) && (user_fifo_get(i3) == UART_HMI_TO_MODULE) && (i1 != r_point)){
			return i;
		}
		i = i + 1;
		count = count + 1;
		if(i == MAX_QUEUE_LEN){
			i = 0;
		}
	}
	return 0;
}

u8 create_crc_check(u8 *d){
	int i = 0;
	uint16_t temp = 0;
	for(i=2; i<7; i++){
		temp = temp + d[i];
	}
	return temp & 0x00ff;
}

int is_uart_data_reiv_valid(u8 *para, int len){
	if((para[0] == UART_HEADER_DATA_0) && (para[1] == UART_HEADER_DATA_1) && (para[2] == UART_HMI_TO_MODULE) && (para[len-1] == create_crc_check(para))){
		return 1;
	}

	return 0;
}

int add_data_to_queue(u8 *p, int len){
	for(int i = 0; i< len ; i++){
		if(user_fifo_get_number_bytes_written() == MAX_QUEUE_LEN){
			user_fifo_pop();
		}
		user_fifo_push(p[i]);
	}
	return 0;
}

u8 is_uart_mess_response_waiting(){
	return uart_last_mess_sent.flag;
}

void uart_handler_by_act(u8 *para, int len){
	u16 act = para[4] << 8 | para[5];
	switch (act)
	{
	case LCD_CALL_SCENE_CMD:
		module_handler_scene_btn_onclick(para[7], para[8]);
		break;
	default:
		break;
	}
}

void uart_handler_data_in_main_loop(u8 *para, int len){
	u16 act = para[4] << 8 | para[5];
	u16 temp = uart_last_mess_sent.dt[4] << 8 | uart_last_mess_sent.dt[5];
	if(is_uart_mess_response_waiting()){
		if((para[3] == UART_TYPE_RES) && (act == temp) && (len == uart_last_mess_sent.len)){
			uart_last_mess_sent.flag = 0;
		}
	}else{
		if(para[3] == UART_TYPE_CMD){
			para[2] = UART_MODULE_TO_HMI;
			para[3] = UART_TYPE_RES;
			para[len - 1] = create_crc_check(para);
			uart_Send(para, len);
		}
		uart_handler_by_act(para, len);
	}
	return;
}

// int uart_lcd_handler_data_t(u8 *para, int len){
// 	uart_com_data r = {0};
// 	u16 act = para[4] << 8 | para[5];
// 	u16 temp = uart_last_mess_sent.dt[4] << 8 | uart_last_mess_sent.dt[5];
// 	if(is_uart_mess_response_waiting()){
// 		if((para[3] == UART_TYPE_RES) && (act == temp) && (len == uart_last_mess_sent.len)){
// 			uart_last_mess_sent.flag = 0;
// 		}
// 	}else{
// 		if(para[3] == UART_TYPE_CMD){
// 			para[2] = UART_MODULE_TO_HMI;
// 			para[3] = UART_TYPE_RES;
// 			para[len - 1] = create_crc_check(para);
// 			uart_Send(para, len);
// 		}
// 		add_data_to_queue(para, len);
// 	}
// 	return 0;
// }

int uart_data_handler(u8 *para, int len){
	add_data_to_queue(para, len);
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