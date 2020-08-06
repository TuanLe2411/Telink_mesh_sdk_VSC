/* 
    This section define what protocol used to communicate with gateway
    ----------writer by LeTuan------------
*/
#include <stdio.h>

#define MaxLengthName 20
#define MaxLengthData 1000

struct Transceiver{
    char Name[MaxLengthName];
    int Baudrate;
    char rec_data[MaxLengthData];
    char trans_data[MaxLengthData];
    void (*init)(struct Transceiver);
};

/*****************************************uart**************************************/

/* uart define */
#define uart_tx_pin         UART_TX_PD7
#define uart_rx_pin
#define UART_DATA_LEN       1000   

typedef struct{
    unsigned int data_length;        
    unsigned char data[UART_DATA_LEN];
}uart_data_t;

uart_data_t tx_data_buf;
uart_data_t rx_data_buf;

/* uart function */
void Transceiver_uart_init(struct Transceiver tran){}
void 