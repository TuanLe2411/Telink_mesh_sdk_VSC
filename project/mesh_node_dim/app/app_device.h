#ifndef __APP_DEVICE_H__
#define __APP_DEVICE_H__
#include "proj/common/types.h"
#include "app_mesh_com.h"

typedef struct dim{
	u16 L_adc_value;
	u16 C_adc_value; 
	int lum;
	int delay;  //us
	SubAdr SentAddr;
}dim;

extern dim Dim;
extern void dim_init(dim *d);
extern void dim_ctrl_process(dim *d, int adc_in);

#endif