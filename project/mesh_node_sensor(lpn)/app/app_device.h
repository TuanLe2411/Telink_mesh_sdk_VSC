#ifndef __APP_DEVICE_H__
#define __APP_DEVICE_H__
#include "app_mesh_com.h"
#include "app_config_8258.h"

typedef struct sensor{
	int Cond;
	int Span;
	int sensor_dt;
	char index;
	u16 L_adc_value;
	u16 C_adc_value;
	SubAdr SentAddr;
}sensor;

extern sensor lightSensor;
extern void sensor_init(sensor *s);
extern void sensor_ctrl_proc(sensor* s);

#endif