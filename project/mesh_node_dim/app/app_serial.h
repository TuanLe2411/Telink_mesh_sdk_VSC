#ifndef __APP_SERIAL_H__
#define __APP_SERIAL_H__

typedef struct serial{
    char *name;
}serial;

extern serial Uart;

extern void rx_from_uart_cb();
extern void serial_init(serial *s, char *name);
extern void serial_sent_data(serial *s, char *data, unsigned int len);
extern void serial_data_recv(serial *s);

#endif