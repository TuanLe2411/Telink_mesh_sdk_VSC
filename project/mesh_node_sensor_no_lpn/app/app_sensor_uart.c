#include "app_sensor_uart.h"
#include "proj/drivers/uart.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "proj/common/tstring.h"
#include "app_gpio_func.h"
#include "user_lib/serial.h"
#include "drivers/8258/timer.h"
#include "user_lib/user_fifo.h"
#include "drivers/8258/timer.h"
#include "vendor/common/cmd_interface.h"
#include "app_sensor_model.h"
const char uart_message_header[2] = {0xaa, 0xaa};

int reset_flag = 0xff;
u8 current_frame = 0xff;
int bind_state;


void module_send_response(void *c, int len){
	uart_print_data((u8 *)c, len, uart_message_header, 2);
}

u8 create_crc_check(u8 *para){
	int s = 0;
	for(int i = 0; i< 11; i++){
		s = s + para[i];
	}
	return (s & 0xff);
}

u8 message_crc_check(u8 *para){
	int s = 0;
	for(int i = 2; i< 13; i++){
		s = s + para[i];
	}
	return (s & 0xff);
}

void module_on_reset(){
	factory_reset();
	start_reboot();
}

void module_send_led_control(u8 cmd){
	current_frame = current_frame + 0x10;
	u8 dt[12] = {0xF5, 0x0A, 0x03, current_frame, 0x00, 0x00, 0x03, 0x01, 0x51, cmd, 0x00, 0 };
	dt[11] = create_crc_check(dt);
	module_call_chip_wakeup();
	uart_print_data(dt, 12, uart_message_header, 2);
}

void module_on_detec_send_mesh_mes(uart_data_response *d){
	if(d->data1 == SENSOR_DETECTED){
		sensor_on_detected_report();
	}else{
		sensor_no_detected_report();
	}
	return;
}

void module_create_response(uart_data_response *uart_res, uart_data_receive *r, u8 act){
	uart_res->type = UART_DATA_TYPE_MODULE_TO_CHIP;
	uart_res->len = UART_DATA_DEFAULT_LEN;
	uart_res->action = act;
	uart_res->frame_number = r->frame_number;
	u8 id[] = UART_DATA_ID1;
	memcpy(uart_res->id1, id, 3);
	uart_res->id2 = UART_DATA_ID2;
	uart_res->deviceId = UART_DATA_deviceId;
	uart_res->data1 = r->data1;
	uart_res->data2 = r->data2;

	crc_check_input crc_check = {0};
	crc_check.type = uart_res->type;
	crc_check.len = uart_res->len ;
	crc_check.action = uart_res->action;
	crc_check.frame_number = uart_res->frame_number;
	memcpy(crc_check.id1, uart_res->id1, 3);
	crc_check.id2 = uart_res->id2;
	crc_check.deviceId = uart_res->deviceId;
	crc_check.data1 = uart_res->data1;
	crc_check.data2 = uart_res->data2;

	uart_res->crcCheck = create_crc_check((u8 *)&crc_check);
	//module_on_detec_send_mesh_mes(uart_res);
}



void module_cmd_handler(uart_data_receive *r){
	uart_data_response uart_res = {0};
	switch (r->action)
	{
		case UART_DATA_ACTION_DETECTED:
			module_create_response(&uart_res, r, UART_DATA_ACTION_DETECTED_ACK);
			if(current_frame != uart_res.frame_number){
				module_call_chip_wakeup();
				module_send_response(&uart_res, sizeof(uart_res));
				current_frame = uart_res.frame_number;
				module_on_detec_send_mesh_mes(&uart_res);
			}

			break;
		
		case UART_DATA_ACTION_ON_RESET:
			module_create_response(&uart_res, r, UART_DATA_ACTION_ON_RESET_ACK);
			if(current_frame != uart_res.frame_number){
				module_call_chip_wakeup();
				module_send_response(&uart_res, sizeof(uart_res));
				if(reset_flag == 0xff){
					sleep_ms(30);
					module_send_led_control(UART_LED_CONTROL_BLINK);
					reset_flag = 1;
				}
			}
			//module_on_reset();
			break;
		case UART_DATA_ACTION_LED_CONTROL:
			module_create_response(&uart_res, r, UART_DATA_ACTION_LED_CONTROL_ACK);
			if(reset_flag == 1){
				module_call_chip_wakeup();
				module_send_response(&uart_res, sizeof(uart_res));
				current_frame = uart_res.frame_number;
				module_on_reset();
				break;
			}else{
				if(current_frame != uart_res.frame_number){
					module_call_chip_wakeup();
					module_send_response(&uart_res, sizeof(uart_res));
					current_frame = uart_res.frame_number;
				}
			}

			// module_call_chip_wakeup();
			// module_send_response(&uart_res, sizeof(uart_res));
			// current_frame = uart_res.frame_number;
			// if(reset_flag == 1){
			// 	module_on_reset();
			// }
			break;
		default:
			break;
	}
	return;
}

int is_data_on_valid(u8 *para, int len){
	u16 header = para[0] << 8 | para[1];
	u8 type = para[2];
	u8 data_len = para[3];
	if((header == UART_DATA_HEADER) && (type == UART_DATA_TYPE_CHIP_TO_MODULE) && (data_len == len - 4) && (para[len - 1] == message_crc_check(para)) ){
		return 0;
	}
	return -1;
}

// int is_data_on_receiving_valid(u8 *para, int len){
// 	return is_data_on_valid(para, len);
// }

int add_data_invalid_to_queue(u8 *p, int len){
	for(int i = 0; i< len ; i++){
		user_fifo_push(p[i]);
	}
	return 0;
}

u16 find_next_uart_mess_head_point(){
	u16 count = 0;
	u16 i = 0;
	u16 r_point = user_fifo_get_r_point();
	while (count <= user_fifo_get_number_bytes_written())
	{
		u16 i1 = ((i + r_point) > (MAX_QUEUE_LEN - 1))?(i + r_point) : (i + r_point - MAX_QUEUE_LEN);
		u16 i2 = ((i + r_point + 1) > (MAX_QUEUE_LEN - 1))?(i + r_point + 1) : (i + r_point + 1 - MAX_QUEUE_LEN);
		u16 i3 = ((i + r_point + 2) > (MAX_QUEUE_LEN - 1))?(i + r_point + 2) : (i + r_point + 2 - MAX_QUEUE_LEN);
		if((user_fifo_get(i1) << 8 | user_fifo_get(i2) != UART_DATA_HEADER) && (user_fifo_get(i3) != UART_DATA_TYPE_CHIP_TO_MODULE)){
			return i1;
		}
		i = i + 1;
		count = count + 1;
		if(i = MAX_QUEUE_LEN){
			i = 0;
		}
	}
	return -1;
}

int find_data_valid_in_queue(u8 *buff, int *len){
	while(1){
		u16 i = find_next_uart_mess_head_point();
		if(i < 0){
			return;
		}
		u16 data_lenght = 0;
		for(int j = 0; j< i; j++){
			buff[j] = user_fifo_pop();
			data_lenght = data_lenght + 1;
		}
		if(is_data_on_valid(buff, data_lenght)){
			*len = data_lenght;
			return 0;
		}
	}
	return -1;
}

int valid_data_handler(u8 *p, int len){
	uart_data_receive uart_data_reiv = {0};
	uart_data_reiv.header = (p[0] << 8) | p[1];
	uart_data_reiv.type = p[2];
	uart_data_reiv.action = p[4];
	uart_data_reiv.frame_number = p[5];
	uart_data_reiv.data1 = p[11];
	uart_data_reiv.data2 = p[12];
	uart_data_reiv.data_explaned_len = p[3] - UART_DATA_DEFAULT_LEN;
	memcpy(uart_data_reiv.data_explanded, p + 13, uart_data_reiv.data_explaned_len);
	module_cmd_handler(&uart_data_reiv);
	return 0;
}


// void uart_data_handler(u8 *para, int len){
// 	if(user_fifo.num_bytes_written == MAX_QUEUE_LEN){
// 		user_fifo_reset();
// 	}

// 	int is_data_on_receiving_valid = is_data_on_valid(para, len);
// 	if(is_data_on_receiving_valid == 0){
// 		valid_data_handler(para, len);
// 		user_fifo_reset();
// 		return;
// 	}

// 	for(int i = 0; i< len ; i++){
// 		user_fifo_push(para[i]);
// 	}

// 	u8 buff[UART_MAX_LEN];	
// 	int buff_len;
// 	int find_data_success = find_data_valid_in_queue(buff, &buff_len);
// 	if(find_data_success == 0){
// 		valid_data_handler(buff, buff_len);
// 		user_fifo_reset();
// 	}

// 	return;
// }

void uart_data_handler(u8 *para, int len){
	if(user_fifo.num_bytes_written == MAX_QUEUE_LEN){
		user_fifo_reset();
	}
	for(int i = 0; i< len ; i++){
		user_fifo_push(para[i]);
	}
	while(1){
		u16 header = user_fifo_get(user_fifo.r) << 8 | user_fifo_get(user_fifo.r + 1);
		if(header == UART_DATA_HEADER){
			if(user_fifo_get(user_fifo.r + 2) == UART_DATA_TYPE_CHIP_TO_MODULE){
				u8 data_len = user_fifo_get(user_fifo.r + 3);
				u16 header_t = user_fifo_get(user_fifo.r + 4 + data_len) << 8 | user_fifo_get(user_fifo.r + 5 + data_len);
				if(((header_t == UART_DATA_HEADER) && (user_fifo_get(user_fifo.r + 6 + data_len) == UART_DATA_TYPE_CHIP_TO_MODULE))  || (user_fifo.num_bytes_written == (4 + data_len)) || (( user_fifo_get(user_fifo.r + 3 + data_len) != 0xaa) && (user_fifo.num_bytes_written > (4 + data_len)))){
					u8 p[UART_MAX_LEN];
					if((header_t == UART_DATA_HEADER) && (user_fifo_get(user_fifo.r + 5 + data_len) == UART_DATA_TYPE_CHIP_TO_MODULE)){
						int i = 0;
						
						while(((user_fifo_get(i) << 8) | (user_fifo_get(i+1) != UART_DATA_HEADER))&&( user_fifo_get(i + 2) != UART_DATA_TYPE_CHIP_TO_MODULE)&&(i > 0)){
							p[i] = user_fifo_pop();
							i++;
						}
						if(p[i] != message_crc_check(p)){
							return;
						}
					}else if ((user_fifo.num_bytes_written == (4 + data_len)))
					{	
						for (int i = 0; i< (4 + data_len); i++){
							p[i] = user_fifo_pop();
						}
						if(p[3 + data_len] != message_crc_check(p)){
							return;
						}
					}
					uart_data_receive uart_data_reiv = {0};
					uart_data_reiv.header = (p[0] << 8) | p[1];
					uart_data_reiv.type = p[2];
					uart_data_reiv.action = p[4];
					uart_data_reiv.frame_number = p[5];
					uart_data_reiv.data1 = p[11];
					uart_data_reiv.data2 = p[12];
					uart_data_reiv.data_explaned_len = p[3] - UART_DATA_DEFAULT_LEN;
					memcpy(uart_data_reiv.data_explanded, p + 13, uart_data_reiv.data_explaned_len);
					module_cmd_handler(&uart_data_reiv);
					
				}else{
					return;
				}
			}else {
				return;
			}
		}else{
			while ((user_fifo_get(user_fifo.r) << 8 | user_fifo_get(user_fifo.r + 1)) == UART_DATA_HEADER){
				u8 d = user_fifo_pop();
			}
			return;
		}
	}
	return;
}

void rx_from_uart_cb(void){
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
	return;
}

void module_send_message_to_chip(){
	return;
}

int is_on_reset(){
	return reset_flag;
}

u8 get_current_frame_number(){
	return current_frame;
}

void module_control_led_off_after_reset(){
	u8 dt[12] = {0xF5, 0x0A, 0x03, 0, 0x00, 0x00, 0x03, 0x01, 0x51, 0x00, 0x00, 0 };
	for(int i = 0; i<= 0xf0; i = i + 0x10){
		current_frame = current_frame + 0x10;
		dt[3] = current_frame;
		dt[11] = create_crc_check(dt);
		uart_print_data(dt, 12, uart_message_header, 2);
	}
	sleep_ms(400);
	user_fifo_reset();
}

int get_bind_state(){
	return bind_state;
}

void set_bind_state(int st){
	bind_state = st;
}

void init_bind_state(){
	if(get_provision_state() == STATE_DEV_PROVED){
		bind_state = 0;
	}
	bind_state = 1;
}

