#include "interface.h"
#include "proj/common/types.h"
#include "drivers.h"

void user_func_init();

void user_func_proc(){
    static u32 adc_check_time;
        if(clock_time_exceed(adc_check_time, 3000*1000)){
            adc_check_time = clock_time();
            uart_json_test();
        }      
};