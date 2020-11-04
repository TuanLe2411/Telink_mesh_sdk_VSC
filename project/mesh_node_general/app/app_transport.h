#pragma once

#ifndef __APP_TRANSPORT_H__
#define __APP_TRANSPORT_H__

#include <stdio.h>
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "proj/common/static_assert.h"

typedef struct Transceiver{
    char Name[20];
    void (*trans_init)(struct Transceiver trans, int Baud);
    void (*trans_print)(struct Transceiver trans, char* str);
    void (*trans_print_hexstr)(struct Transceiver trans, char* data, unsigned int len);
    void (*trans_print_array)(struct Transceiver trans, char* data, unsigned int len);
    void (*trans_send)(struct Transceiver trans, char* data, unsigned int len);
    void (*trans_rec_data_print)(struct Transceiver trans);
    void (*transceiver_irq_proc)(struct Transceiver trans);
    void (*transceiver_loop)(struct Transceiver trans);
    void (*transceiver_data_proc)(char *pbuf, int len);
    void (*transceiver_res_result)(struct Transceiver trans, unsigned char result_code);
}Transceiver;

void Transceiver_init(struct Transceiver trans, int Baud);
void Transceiver_print(struct Transceiver trans, char* str);
void Transceiver_print_hexstr(struct Transceiver trans, char* data, unsigned int len);
void Transceiver_print_array(struct Transceiver trans, char* data, unsigned int len);
void Transceiver_send(struct Transceiver trans, char* data, unsigned int len);
void Transceiver_rec_data_print(struct Transceiver trans);
void Transceiver_irq_proc(struct Transceiver trans);
void Transceiver_loop(struct Transceiver trans);
void Transceiver_data_proc(char *pbuf, int len);
void Transceiver_res_result(struct Transceiver trans, unsigned char result_code);

extern Transceiver Serial;

typedef enum {
    CMD_RESULT_CODE_OK = 0,
    CMD_RESULT_CODE_ERROR,
    CMD_RESULT_CODE_INVALID,
    CMD_RESULT_CODE_MAX,
} cmd_result_code_string_index_t;

#endif