#include "interface.h"
#include "app_device.h"
#include "drivers.h"
#include "proj/common/types.h"

void user_func_init(){
    dim_init(&Dim);
}

void user_func_proc(){
    static u32 adc_check_time;
    if(clock_time_exceed(adc_check_time, Dim.delay*1000)){
        adc_check_time = clock_time();
        static u16 T_adc_val;
        T_adc_val = adc_sample_and_get_result();  
		dim_ctrl_process(&Dim, T_adc_val);
    }
}