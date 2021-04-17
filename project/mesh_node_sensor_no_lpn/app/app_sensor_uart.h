#ifndef __APP_SENSOR_UART_H__
#define __APP_SENSOR_UART_H__

#include "proj/common/types.h"
extern void rx_from_uart_cb(void);
extern void module_send_message_to_chip();
extern void module_send_led_control(u8 frame_number, u8 cmd);
extern int is_on_reset();
extern u8 get_current_frame_number();

#define UART_DATA_HEADER                   0xaaaa
#define UART_DATA_DEFAULT_LEN              0x0a
#define UART_MAX_LEN                       30

#define UART_DATA_TYPE_MODULE_TO_CHIP      0xf5
#define UART_DATA_TYPE_CHIP_TO_MODULE      0xfa

#define UART_DATA_ACTION_BTN_ONCLICK       0x01
#define UART_DATA_ACTION_BTN_ONCLICK_ACK   0x01

#define UART_DATA_ACTION_ON_RESET          0x02
#define UART_DATA_ACTION_ON_RESET_ACK      0x02

#define UART_DATA_ACTION_LED_CONTROL       0x03
#define UART_DATA_ACTION_LED_CONTROL_ACK   0x03

#define UART_DATA_ID1                      {0x00, 0x00, 0x03}
#define UART_DATA_ID2                      0x01
#define UART_DATA_deviceId                 0x51

#define UART_LED_CONTROL_OFF            0x00
#define UART_LED_CONTROL_ON             0xff
#define UART_LED_CONTROL_BLINK          0xaa

typedef struct {
    u8 type;
    u8 len;
    u8 action;
    u8 frame_number;
    u8 id1[3];
    u8 id2;
    u8 deviceId;
    u8 data1;
    u8 data2;
    u8 crcCheck;
}uart_data_response;

typedef struct {
    u16 header;
    u8 type;
    u8 action;
    u8 frame_number;
    u8 data1;
    u8 data2;
    u16 data_explaned_len;
    u8 *data_explanded;
}uart_data_receive;

typedef struct {
    u8 type;
    u8 len;
    u8 action;
    u8 frame_number;
    u8 id1[3];
    u8 id2;
    u8 deviceId;
    u8 data1;
    u8 data2;
}crc_check_input;

#endif