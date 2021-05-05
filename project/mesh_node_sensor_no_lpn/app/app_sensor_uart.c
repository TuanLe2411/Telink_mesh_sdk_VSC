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

int reset_flag;
u8 current_frame;
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
	set_gw_res_glag(1);
	if(d->data1 == SENSOR_DETECTED){
		sensor_on_detected_report();
		set_module_res_st(1);
	}else{
		sensor_no_detected_report();
		set_module_res_st(0);
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
				current_frame = uart_res.frame_number;
				if(reset_flag == 0){
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
				module_on_reset();
				break;
			}else{
				if(current_frame != uart_res.frame_number){
					module_call_chip_wakeup();
					module_send_response(&uart_res, sizeof(uart_res));
					current_frame = uart_res.frame_number;
				}
			}
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

int add_data_invalid_to_queue(u8 *p, int len){
	for(int i = 0; i< len ; i++){
		user_fifo_push(p[i]);
	}
	return 0;
}

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
		if((user_fifo_get(i1) << 8 | user_fifo_get(i2) == UART_DATA_HEADER) && (user_fifo_get(i3) == UART_DATA_TYPE_CHIP_TO_MODULE) && (i1 != r_point)){
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


void uart_data_handler_t(u8 *para, int len){

	if(user_fifo_get_number_bytes_written() == MAX_QUEUE_LEN){
		user_fifo_reset();
	}

	if(is_data_on_valid(para, len) == 0){
		valid_data_handler(para, len);
		user_fifo_reset();
		return;
	}

	add_data_invalid_to_queue(para, len);

	u16 i = find_first_valid_uart_mess_header();
	u8 buff[UART_MAX_LEN];
	if(i == 0){
		return;
	}
	for(int j = 0; j< i; j++){
		buff[j] = user_fifo_pop();
	}
	if(is_data_on_valid(buff, i) == 0){
		valid_data_handler(buff, i);
		user_fifo_reset();
		return;
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
            uart_data_handler_t(p->data, rx_len);
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
	for(int i = 0; i< 0x100; i = i + 0x10){
		dt[3] = i;
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

void init_current_frame(){
	current_frame = 0xff;
}

void init_reset_flag(){
	reset_flag = 0;
}

void init_all_para(){
	init_bind_state();
	init_current_frame();
	init_reset_flag();
}
