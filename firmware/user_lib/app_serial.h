#ifndef __APP_SERIAL_H__
#define __APP_SERIAL_H__
#include "proj/common/types.h"
///////////uart//////////////////
extern int rx_from_uart_cb(void);
extern int uart_print_data(u8 *para, int n, u8 *head, u8 head_len);
extern int uart_print_char(u8 para);
#endif