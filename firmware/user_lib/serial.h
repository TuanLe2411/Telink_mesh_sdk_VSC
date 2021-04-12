#ifndef __SERIAL_H__
#define __SERIAL_H__
#include "proj/common/types.h"
///////////uart//////////////////

extern int uart_print_data(u8 *para, int n, u8 *head, u8 head_len);
extern int uart_print_char(u8 para);
#endif