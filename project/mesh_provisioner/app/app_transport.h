#ifndef __APP_TRANSPORT_H__
#define __APP_TRANSPORT_H__

#include <stdio.h>
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include <string.h>

#define MAX_LEN_NAME            20
#define MAX_LEN_DATA            500   
#define MAX_CMD_NUM			    8

#define SerialName              "serial"

typedef struct{
    unsigned int data_length;        
    unsigned char data[MAX_LEN_DATA];
}data_t;

data_t trans_buff = {0, {0,} };

_attribute_data_retention_  u8 		 	rx_fifo_b[MAX_LEN_DATA * MAX_CMD_NUM] = {0};
_attribute_data_retention_	my_fifo_t	rx_fifo = {
												MAX_LEN_DATA,
												MAX_CMD_NUM,
												0,
												0,
												rx_fifo_b,};

struct Transceiver{
    char Name[MAX_LEN_NAME];
    int Baudrate;
    void (*trans_init)(struct Transceiver);
    void (*trans_print)(struct Transceiver, char* str);
    void (*trans_print_hexstr)(struct Transceiver trans, char* data, unsigned int len);
    void (*trans_print_array)(struct Transceiver trans, char* data, unsigned int len);
    void (*trans_send)(struct Transceiver trans, char* data, unsigned int len);
    void (*trans_irq_proc)(struct Transceiver trans);
    void (*trans_loop)(struct Transceiver trans);
};

void Transceiver_init(struct Transceiver trans);
void Transceiver_print(struct Transceiver trans, char* str);
void Transceiver_print_hexstr(struct Transceiver trans, char* data, unsigned int len);
void Transceiver_print_array(struct Transceiver trans, char* data, unsigned int len);
void Transceiver_send(struct Transceiver trans, char* data, unsigned int len);
void Transceiver_irq_proc(struct Transceiver trans);
void Transceiver_loop(struct Transceiver trans);

#endif