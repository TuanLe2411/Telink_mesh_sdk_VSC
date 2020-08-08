/* 
    This section define what protocol used to communicate with gateway
    ----------writer by LeTuan------------
*/

#include "app_transport.h"

/*****************************************uart**************************************/

/* uart define */
#define uart_tx_pin         UART_TX_PD7
#define uart_rx_pin         UART_RX_PB7

/* uart function */
void Transceiver_init(struct Transceiver trans){
    if( trans.Name == "serial" ){
        WaitMs(100);  //leave enough time for SWS_reset when power on

        //note: dma addr must be set first before any other uart initialization! (confirmed by sihui)
        uart_recbuff_init( (unsigned short *)my_fifo_wptr(&rx_fifo), MAX_LEN_DATA, (char *)&trans_buff);

        uart_gpio_set(uart_tx_pin, uart_rx_pin);// uart tx/rx pin set

        uart_reset();  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset

        //baud rate:
        if(trans.Baudrate == 9600)
            uart_init(118, 13, PARITY_NONE, STOP_BIT_ONE);
        if(trans.Baudrate == 19200)
            uart_init(118, 6, PARITY_NONE, STOP_BIT_ONE);
        if(trans.Baudrate == 115200)
            uart_init(9, 13, PARITY_NONE, STOP_BIT_ONE);
        
        uart_dma_enable(1, 1); 	//uart data in hardware buffer moved by dma, so we need enable them first

        irq_set_mask(FLD_IRQ_DMA_EN);

        dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 1);   	//uart Rx/Tx dma irq enable
    }
    
    if(0){
        /*
            code for another transceive protocol 
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
            code for another transceive protocol 
        */
    }
}

unsigned char hextab[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
void Transceiver_print_hexstr(struct Transceiver trans, char* data, unsigned int len){
    if( trans.Name == "serial" ){
        unsigned char buf[MAX_LEN_DATA] = { 0 };
        for(int i =0; i < len; i ++)
        {
            buf[i*2] = hextab[(data[i] >> 4)];
            buf[i*2 +1] = hextab[(data[i]&0xf)];
        }
        Transceiver_print(trans, (char*)buf);
    }

    if(0){
        /*
            code for another transceive protocol 
        */
    }
}

void Transceiver_print_array(struct Transceiver trans, char* data, unsigned int len){
    if( trans.Name == "serial" ){
        unsigned char buf[MAX_LEN_DATA] = { 0 };
        for(int i =0; i < len; i ++)
        {
            buf[i*2] = hextab[(data[i] >> 4)];
            buf[i*2 +1] = hextab[(data[i]&0xf)];
        }
        Transceiver_print(trans, (char*)buf);
    }

    if(0){
        /*
            code for another transceive protocol 
        */
    }
}

void Transceiver_send(struct Transceiver trans, char* data, unsigned int len){
    if( trans.Name == "serial" ){
        while(len > MAX_LEN_DATA)
        {
            memcpy(trans_buff.data, data,  MAX_LEN_DATA);
            data += MAX_LEN_DATA;
            len -= MAX_LEN_DATA;

            trans_buff.data_length = MAX_LEN_DATA;

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
            code for another transceive protocol 
        */
    }
}

void Transceiver_irq_proc(struct Transceiver trans){
    if( trans.Name == "serial" ){

        unsigned char uart_dma_irqsrc;
        //1. UART irq
        uart_dma_irqsrc = dma_chn_irq_status_get();///in function,interrupt flag have already been cleared,so need not to clear DMA interrupt flag here

        if(uart_dma_irqsrc & FLD_DMA_CHN_UART_RX)
        {
            //Received uart data in rec_buff, user can copy data here
            //u_sprintf(print_buff,"%d", rec_buff.dma_len);
            //at_print("uart data\r\n");

            u8* w = my_fifo_wptr(&rx_fifo);
            if((w[0]!=0) || (w[1]!=0))
            {
                my_fifo_next(&rx_fifo); //写指针前移
                u8* p = my_fifo_wptr(&rx_fifo); //获取当前写指针
                reg_dma_uart_rx_addr = (u16)((u32)p);  //switch uart RX dma address
            }

            dma_chn_irq_status_clr(FLD_DMA_CHN_UART_RX);
        }

        if(uart_dma_irqsrc & FLD_DMA_CHN_UART_TX)
        {
            dma_chn_irq_status_clr(FLD_DMA_CHN_UART_TX);
        }
    }

    if(0){
        /*
            code for another transceive protocol 
        */
    }
}

char * data = NULL;

void Transceiver_loop(struct Transceiver trans){
    if( trans.Name == "serial" ){

        data = my_fifo_get(&rx_fifo);
	    if(data != NULL){
            
        }

    }

    if(0){
        /*
            code for another transceive protocol 
        */
    }
}