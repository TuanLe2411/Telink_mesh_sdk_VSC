#include "app_device.h"
#include "proj/common/types.h"
#include "app_config_8258.h"
#include "app_mesh_com.h"

dim Dim;

void dim_init(dim *d){
	d->delay = DEFAULT_DELAY;
	d->C_adc_value = 0;
	d->L_adc_value = 0;
	return;
}

void dim_set_lum_to_SentAddr(dim *d){

	for(int i = 0; i< d->SentAddr.num; i++){
		if(d->SentAddr.subAdr[i] == 0xffff){
			break;
		}
		mesh_set_lum_cmd(d->SentAddr.subAdr[i], d->lum);
	}
	return;
}

void dim_update_SentAddr(dim *d){
	d->SentAddr = mesh_get_sub_addr();
}

void dim_update_lum(dim *d, int lum){
	d->lum = lum;
	return;
}

void dim_convert_adc_val(dim *d){
	d->lum = (char)((d->C_adc_value)/32);
	return;
}

void dim_sent_ctrl_cmd(dim *d){
	dim_convert_adc_val(d);
	dim_set_lum_to_SentAddr(d);
	return;
}

void dim_ctrl_process(dim *d, int adc_in){

	dim_update_SentAddr(d);
	d->C_adc_value = adc_in;
	if(d->L_adc_value != d->C_adc_value){
		d->L_adc_value = d->C_adc_value;
		dim_sent_ctrl_cmd(d);
	}
}