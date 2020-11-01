#pragma once

#ifndef __APP_TRANSPORT_H__
#define __APP_TRANSPORT_H__

#include <stdio.h>
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "proj/common/static_assert.h"

#define uart_tx_pin         UART_TX_PD7
#define uart_rx_pin         UART_RX_PA0
#define DATA_LEN            76   
#define MAX_CMD_NUM			8
#define FIFO_RX_SIZE        (DATA_LEN + 4)

STATIC_ASSERT((FIFO_RX_SIZE % 16) == 0);

typedef struct{
    unsigned int data_length;        
    unsigned char data[1];
}rec_data_t;

typedef struct{
    unsigned int data_length;        
    unsigned char data[DATA_LEN];
}trans_data_t;

typedef struct Transceiver{
    char Name[20];
    void (*trans_init)(struct Transceiver, int Baud);
    void (*trans_print)(struct Transceiver, char* str);
    void (*trans_print_hexstr)(struct Transceiver trans, char* data, unsigned int len);
    void (*trans_print_array)(struct Transceiver trans, char* data, unsigned int len);
    void (*trans_send)(struct Transceiver trans, char* data, unsigned int len);
    void (*trans_rec_data_print)(struct Transceiver trans);
}Transceiver;

void Transceiver_init(struct Transceiver trans, int Baud);
void Transceiver_print(struct Transceiver trans, char* str);
void Transceiver_print_hexstr(struct Transceiver trans, char* data, unsigned int len);
void Transceiver_print_array(struct Transceiver trans, char* data, unsigned int len);
void Transceiver_send(struct Transceiver trans, char* data, unsigned int len);
void Transceiver_rec_data_print(struct Transceiver trans);



#endif