#ifndef __APP_SERIAL_H__
#define __APP_SERIAL_H__
#include "proj/common/types.h"

///////////uart//////////////////
extern void rx_from_uart_cb();
extern int uart_print_data(u8 *para, int n, u8 *head, u8 head_len);

#endif