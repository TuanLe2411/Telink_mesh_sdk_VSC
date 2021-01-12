/********************************************************************************************************
 * @file     main.c
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 18, 2018
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
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

#include "tl_common.h"
#include "drivers.h"
#include "drivers/8258/gpio_8258.h"
#include "app_config_blink.h"

// define function of this pin, AS_GPIO mean gpio
void user_init()
{
	
	gpio_set_func(PWM_R, AS_GPIO);
	
	
	gpio_set_output_en(PWM_R, 1);

	
	gpio_set_input_en(PWM_R, 0); 

	/* if you want to set a button to control led, use the following code*/
	
	// gpio_set_func(GPIO_PD2, AS_GPIO);
	// gpio_setup_up_down_resistor(GPIO_PD2, PM_PIN_PULLUP_10K);
	// gpio_set_input_en(GPIO_PD2, 1); 
	// gpio_set_output_en(GPIO_PD2, 0);	
}

void my_key_proocess()
{
	int c = 20;
	while (c--)
	{	
		/* if you want to set a button to control led, use the following code*/

		// if(gpio_read(GPIO_PD2) == 0)
		// {
		// 	while (gpio_read(GPIO_PD2) == 0){sleep_ms(1);};

		// 	sleep_ms(20);
		// 	while (1)
		// 	{
		// 		if(gpio_read(GPIO_PD2) == 0) 
		// 		{
		// 			while (gpio_read(GPIO_PD2) == 0){sleep_ms(1);};
		// 			return;
		// 		}
		// 		sleep_ms(1);
		// 	}
		// }
		sleep_ms(10);
	}
}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop ()
{
	
	gpio_write(PWM_R, 0); 
    my_key_proocess();

	
	gpio_write(PWM_R, 0); 
    my_key_proocess();

	
	gpio_write(PWM_R, 1); 
	my_key_proocess();
}

void irq_handler(void)
{
	return;
}

void system_init()
{
	blc_pm_select_internal_32k_crystal();

	cpu_wakeup_init();

	//int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

	rf_drv_init(RF_MODE_BLE_1M);

	gpio_init(1);

#if (CLOCK_SYS_CLOCK_HZ == 16000000)
	clock_init(SYS_CLK_16M_Crystal);
#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
	clock_init(SYS_CLK_24M_Crystal);
#endif

}

int main (void)    //must run in ramcode
{
	system_init();

	user_init();

    irq_enable();

	while (1) 
	{
#if (MODULE_WATCHDOG_ENABLE)
		wd_clear(); //clear watch dog
#endif
		main_loop ();
	}
}


