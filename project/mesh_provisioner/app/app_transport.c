/* 
    This section define what protocol used to communicate with gateway
    ----------writer by LeTuan------------
*/

#include "app_transport.h"
/*****************************************uart**************************************/

/* uart define */
trans_data_t trans_buff = {0, {0, } };
u8 uart_hw_tx_buf[52];
/* uart function */

void Transceiver_init(struct Transceiver trans, int Baud){
    if( trans.Name == "serial" ){
        WaitMs(100);  //leave enough time for SWS_reset when power on
        u8 *uart_rx_addr = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
        //note: dma addr must be set first before any other uart initialization! (confirmed by sihui)
        uart_recbuff_init(uart_rx_addr, hci_rx_fifo.size, uart_hw_tx_buf);

        uart_gpio_set(uart_tx_pin, uart_rx_pin);// uart tx/rx pin set

        uart_reset();  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset

        //baud rate:
        if(Baud == 9600)
            uart_init(118, 13, PARITY_NONE, STOP_BIT_ONE);
        if(Baud == 19200)
            uart_init(118, 6, PARITY_NONE, STOP_BIT_ONE);
        if(Baud == 115200)
            uart_init(9, 13, PARITY_NONE, STOP_BIT_ONE);
        
        uart_dma_enable(1, 1); 	//uart data in hardware buffer moved by dma, so we need enable them first

        irq_set_mask(FLD_IRQ_DMA_EN);

        dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 1);   	//uart Rx/Tx dma irq enable
    }
    
    if(0){
        /*
            code for another transceiver protocol 
        */
    }
}
void Transceiver_print(struct Transceiver trans, char* str){
    if( trans.Name == "serial" ){
        while(*str)
        {
            trans_buff.data[trans_buff.data_length] = *str++;
            trans_buff.data_length += 1;
            if(trans_buff.data_length == 12)
            {
                uart_dma_send((unsigned char*)&trans_buff);
                trans_buff.data_length = 0;
                WaitMs(20);
            }
        }

        if(trans_buff.data_length)
        {
            uart_dma_send((unsigned char*)&trans_buff);
            trans_buff.data_length = 0;
            WaitMs(20);
        }
    }

    if(0){
        /*
            code for another transceiver protocol 
        */
    }
}

unsigned char hextab[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
void Transceiver_print_hexstr(struct Transceiver trans, char* data, unsigned int len){
    if( trans.Name == "serial" ){
        unsigned char buf[DATA_LEN] = { 0 };
        for(int i =0; i < len; i ++)
        {
            buf[i*2] = hextab[(data[i] >> 4)];
            buf[i*2 +1] = hextab[(data[i]&0x0f)];
        }
        Transceiver_print(trans, (char*)buf);
    }

    if(0){
        /*
            code for another transceiver protocol 
        */
    }
}

void Transceiver_print_array(struct Transceiver trans, char* data, unsigned int len){
    if( trans.Name == "serial" ){
        unsigned char buf[128] = { 0 };
        for(int i =0; i < len; i ++)
        {
            buf[i*3] = hextab[(data[i] >> 4)];
            buf[i*3 +1] = hextab[(data[i]&0xf)];
            buf[i*3 +2] = ' ';
        }
        Transceiver_print(trans, (char*)buf);
    }

    if(0){
        /*
            code for another transceiver protocol 
        */
    }
}

void Transceiver_send(struct Transceiver trans, char* data, unsigned int len){
    if( trans.Name == "serial" ){
        while(len > DATA_LEN)
        {
            memcpy(trans_buff.data, data,  DATA_LEN);
            data += DATA_LEN;
            len -= DATA_LEN;

            trans_buff.data_length = DATA_LEN;

            uart_dma_send((unsigned char*)&trans_buff);
            trans_buff.data_length = 0;
            WaitMs(10);
            
        }

        if(len > 0)
        {
            memcpy(trans_buff.data, data,  len);
            trans_buff.data_length = len;
            uart_dma_send((unsigned char*)&trans_buff);
            trans_buff.data_length = 0;
            WaitMs(2);
        }
    }

    if(0){
        /*
            code for another transceiver protocol 
        */
    }
}

void Transceiver_rec_data_print(struct Transceiver trans){
    if( trans.Name == "serial" ){
        rec_data_t* p = (rec_data_t *)my_fifo_get(&hci_rx_fifo);
	    if(p){
            Transceiver_print(trans, (char *)(p->data));
        }
        free(p);
    }

    if(0){
        /*
            code for another transceiver protocol 
        */
    }
}