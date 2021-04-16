#include "app_gpio_func.h"
#include"drivers/8258/gpio.h"
#include "drivers/8258/timer.h"
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
