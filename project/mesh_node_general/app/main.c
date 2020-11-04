/********************************************************************************************************
 * @file     main.c 
 *
 * @brief    for TLSR chips
 *
 * @author	 telink
 * @date     Sep. 30, 2010
 *
 * @par      Copyright (c) 2010, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *           
 *			 The information contained herein is confidential and proprietary property of Telink 
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms 
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai) 
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in. 
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this 
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided. 
 *           
 *******************************************************************************************************/
#include "proj/tl_common.h"
#include "proj/mcu/watchdog_i.h"
#include "vendor/common/user_config.h"
#include "proj_lib/rf_drv.h"
#include "proj_lib/pm.h"
#include "proj_lib/ble/blt_config.h"
#include "proj_lib/ble/ll/ll.h"
#include "app_transport.h"

extern void user_init();
extern void main_loop();


Transceiver Serial = {
	.Name = "serial",
	.trans_init = &Transceiver_init,
	.trans_print = &Transceiver_print,
	.trans_print_hexstr = &Transceiver_print_hexstr,
	.trans_print_array = &Transceiver_print_array,
	.trans_send = &Transceiver_send,
	.trans_rec_data_print = &Transceiver_rec_data_print,
	.transceiver_irq_proc = &Transceiver_irq_proc,
	.transceiver_loop = &Transceiver_loop,
	.transceiver_data_proc = &Transceiver_data_proc,
	.transceiver_res_result = &Transceiver_res_result
};

#if (HCI_ACCESS == HCI_USE_UART)
	#include "proj/drivers/uart.h"
	extern my_fifo_t hci_rx_fifo;
	u16 uart_tx_irq=0, uart_rx_irq=0;
	void uart_irq_proc(){
		unsigned char irqS = reg_dma_rx_rdy0;
		if(irqS & FLD_DMA_CHN_UART_RX)	
		{
			uart_rx_irq++;
			reg_dma_rx_rdy0 = FLD_DMA_CHN_UART_RX;
			u8* w = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
			if(w[0]!=0)
			{
				my_fifo_next(&hci_rx_fifo);
				u8* p = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
				reg_dma0_addr = (u16)((u32)p);
			}
		}

		if(irqS & FLD_DMA_CHN_UART_TX)	//tx
		{
			uart_tx_irq++;
			reg_dma_rx_rdy0 = FLD_DMA_CHN_UART_TX;
		}
	}
#endif

#if IRQ_TIMER1_ENABLE
_attribute_ram_code_ void irq_timer_handle()
{
    u32 src = reg_irq_src;
    static u32 A_debug_irq_cnt =0;
    if(src & FLD_IRQ_TMR1_EN){
       A_debug_irq_cnt++;
       reg_tmr_sta = FLD_TMR_STA_TMR1;
       gpio_write(GPIO_PA1,A_debug_irq_cnt%2);
    }
}
#endif

#if	IRQ_GPIO_ENABLE
static u32 irq_gpio_user_cnt=0;
void gpio_irq_user_handle()
{
	irq_gpio_user_cnt++;
	return;
}

void gpio_risc0_user_handle()
{
	return;
}

void gpio_risc1_user_handle()
{
	return;
}

void gpio_risc2_user_handle()
{
	return;
}

void irq_gpio_handle()
{
	u32 src = reg_irq_src;
	if(src & FLD_IRQ_GPIO_EN){
		gpio_irq_user_handle();
		reg_irq_src = FLD_IRQ_GPIO_EN;        // clear irq_gpio irq flag		
	}

	/************* gpio irq risc0 *************/
	if(src & FLD_IRQ_GPIO_RISC0_EN){
		gpio_risc0_user_handle();
		reg_irq_src = FLD_IRQ_GPIO_RISC0_EN;        // clear irq_gpio irq flag				
	}

	/************* gpio irq risc1 *************/
	if(src & FLD_IRQ_GPIO_RISC1_EN){
		gpio_risc1_user_handle();
		reg_irq_src = FLD_IRQ_GPIO_RISC1_EN;        // clear irq_gpio irq flag		
	}
	#if (!(__TL_LIB_8258__ || (MCU_CORE_TYPE && MCU_CORE_TYPE == MCU_CORE_8258) || (MCU_CORE_TYPE == MCU_CORE_8278)))
	if(src & FLD_IRQ_GPIO_RISC2_EN){
		gpio_risc2_user_handle();
		reg_irq_src = FLD_IRQ_GPIO_RISC2_EN;
	}
	#endif
}
#endif

_attribute_ram_code_ void irq_handler(void)
{
	irq_blt_sdk_handler ();  //ble irq proc

	#if IRQ_TIMER1_ENABLE
		irq_timer_handle();
	#endif

	#if (HCI_ACCESS == HCI_USE_UART)
		uart_irq_proc();
	#endif

	#if	IRQ_GPIO_ENABLE
		irq_gpio_handle();
	#endif

	Serial.transceiver_irq_proc(Serial);
}

FLASH_ADDRESS_DEFINE;

_attribute_ram_code_ int main (void)    //must run in ramcode
{
	FLASH_ADDRESS_CONFIG;
	blc_pm_select_internal_32k_crystal();
	cpu_wakeup_init();
	
	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

	rf_drv_init(RF_MODE_BLE_1M);

	gpio_init( !deepRetWakeUp );  //analog resistance will keep available in deepSleep mode, so no need initialize again

	#if (CLOCK_SYS_CLOCK_HZ == 16000000)
		clock_init(SYS_CLK_16M_Crystal);
	#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
		clock_init(SYS_CLK_24M_Crystal);
	#elif (CLOCK_SYS_CLOCK_HZ == 32000000)
		clock_init(SYS_CLK_32M_Crystal);
	#elif (CLOCK_SYS_CLOCK_HZ == 48000000)
		clock_init(SYS_CLK_48M_Crystal);
	#endif

	
	#if	(PM_DEEPSLEEP_RETENTION_ENABLE)
		if( pm_is_MCU_deepRetentionWakeup() ){
			user_init_deepRetn ();
		}
		else
	#endif
	{
		user_init();
	}


    irq_enable();

	while (1) {
		#if (MODULE_WATCHDOG_ENABLE)
			wd_clear(); //clear watch dog
		#endif
			main_loop ();
	}
}
