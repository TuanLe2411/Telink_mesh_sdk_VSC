#ifndef __APP_SERIAL_H__
#define __APP_SERIAL_H__
#include "proj/common/types.h"

typedef struct json_data{
    u8 *type;
    u8 *dt;
}json_data;

extern json_data jsonData;

///////////uart//////////////////
extern void rx_from_uart_cb();
extern int uart_print_data(u8 *para, int n, u8 *head, u8 head_len);
extern int uart_json_test();

#endif