#include "app_gpio_func.h"
#include"drivers/8258/gpio.h"
#include "drivers/8258/timer.h"
#include "drivers/8258/pm.h"

#define MODULE_WAKEUP_CONTROL  GPIO_PC3
#define CHIP_WAKEUP_CONTROL  GPIO_PC2

void module_wakeup_chip_control_init(){

	gpio_set_func(CHIP_WAKEUP_CONTROL, AS_GPIO);
	gpio_set_output_en(CHIP_WAKEUP_CONTROL, 1);
	gpio_set_input_en(CHIP_WAKEUP_CONTROL, 0);
	gpio_setup_up_down_resistor(CHIP_WAKEUP_CONTROL, PM_PIN_PULLUP_1M);
}

void module_call_chip_wakeup(){
	gpio_write(CHIP_WAKEUP_CONTROL, 0);
	sleep_ms(24);
	gpio_write(CHIP_WAKEUP_CONTROL, 1);
}

void module_on_sleep(){
	cpu_set_gpio_wakeup(MODULE_WAKEUP_CONTROL, 0, 1);	//set the specified pin as GPIO wakeup source
	gpio_setup_up_down_resistor(MODULE_WAKEUP_CONTROL, PM_PIN_PULLUP_1M);	//enable 100k pull-down resistor of the specified pin
	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K , PM_WAKEUP_PAD, 0);
}