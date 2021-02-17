#include "interface.h"
#include "app_device.h"
#include "drivers.h"
#include "proj/common/types.h"

void user_func_init(){
    sensor_init(&lightSensor);
}

void user_func_proc(){
	sensor_ctrl_proc(&lightSensor);
}