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
    void (*trans_init)(struct Transceiver, int Baud);
    void (*trans_print)(struct Transceiver, char* str);
    void (*trans_print_hexstr)(struct Transceiver trans, char* data, unsigned int len);
    void (*trans_print_array)(struct Transceiver trans, char* data, unsigned int len);
    void (*trans_send)(struct Transceiver trans, char* data, unsigned int len);
    void (*trans_rec_data_print)(struct Transceiver trans);
    void (*transceiver_irq_proc)(struct Transceiver trans);
    void (*transceiver_loop)(struct Transceiver trans);
}Transceiver;

void Transceiver_init(struct Transceiver trans, int Baud);
void Transceiver_print(struct Transceiver trans, char* str);
void Transceiver_print_hexstr(struct Transceiver trans, char* data, unsigned int len);
void Transceiver_print_array(struct Transceiver trans, char* data, unsigned int len);
void Transceiver_send(struct Transceiver trans, char* data, unsigned int len);
void Transceiver_rec_data_print(struct Transceiver trans);
void Transceiver_irq_proc(struct Transceiver trans);
void Transceiver_loop(struct Transceiver trans);

extern Transceiver Serial;

#endif