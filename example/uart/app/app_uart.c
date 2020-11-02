#include "tl_common.h"
#include "drivers.h"
/* uart in TSLR 825x*/


/* Define uart mode */
#define UART_DMA    1   //read and write data from DMA
#define UART_NDMA   2   //read and write data from MCU
#define UART_MODE   UART_DMA


/* define led pin */
#define PWM_R GPIO_PC4
#define PWM_G GPIO_PC3
#define PWM_B GPIO_PC2

// define infastructure of data recived and data transported
#if (UART_MODE == UART_DMA)

    // data len
    #define UART_DATA_LEN 12
    typedef struct 
    {
        unsigned int dma_len;   // dma_len must be 4 byte
        unsigned char data[UART_DATA_LEN];
    }uart_data_t;

    uart_data_t rec_buff = {0, {0, } };
    uart_data_t trans_buff = {10, {'T', 'E', 'S', 'T', 'D', 'A', 'T', 'A', '\r', '\n'} };
    
#endif

#define BuffAdd 0x90

// init uart app
void app_uart_init(){

    //leave enough time for SWS_reset when power on
    WaitMs(100);

    // init receive buff to send data.
    uart_recbuff_init( (unsigned char *)&rec_buff, sizeof(rec_buff), BuffAdd);


    // uart tx/rx pin set
	// TB01 : UART_TX_PD7  UART_RX_PA0
    uart_gpio_set(UART_TX_PD7, UART_RX_PA0);

    //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset
    uart_reset();  

    // This function initializes the UART module, it is defined in uart.c
	#if (CLOCK_SYS_CLOCK_HZ == 16000000)
		uart_init(118, 13, PARITY_NONE, STOP_BIT_ONE);    // baudrate 9600
	#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
		uart_init(249, 9, PARITY_NONE, STOP_BIT_ONE);   // baudrate 9600
	#endif

    // setting active method of uart
    #if (UART_MODE == UART_DMA)

        //dma uart enable
        uart_dma_enable(1, 1);  // must be enable tx_dma and rx_dma when using dma mode
        irq_set_mask(FLD_IRQ_DMA_EN);

        //dma_irq enable
        dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 1);

        //uar_irq disable
        uart_irq_enable(0, 0);
    #endif
        // serve irq
        irq_enable();
}

// this function set main action of uart
void app_uart_loop()
{
    WaitMs(1000);

    #if (UART_MODE == UART_DMA)

        // send data from uart
        uart_dma_send((unsigned char*)&rec_buff);
        WaitMs(300);    
    #endif
}

// those variable are used to count the number of message received or transmited
volatile unsigned char uart_dmairq_tx_cnt=0;
volatile unsigned char uart_dmairq_rx_cnt=0;

// this function is used to recieve data from uart 
void app_uart_irq_proc()
{
    #if (UART_MODE == UART_DMA)
        unsigned char uart_dma_irq_sts;

        //get status of uart
        uart_dma_irq_sts = dma_chn_irq_status_get();

        if(uart_dma_irq_sts & FLD_DMA_CHN_UART_RX)
        {
            dma_chn_irq_status_clr(FLD_DMA_CHN_UART_RX);
        	uart_dmairq_rx_cnt++;

            /*
                can copy data received in rec_buf here
            */
        
        }
    
        if(uart_dma_irq_sts & FLD_DMA_CHN_UART_TX){
            dma_chn_irq_status_clr(FLD_DMA_CHN_UART_TX);
            uart_dmairq_tx_cnt++;
	    }
    #endif
}