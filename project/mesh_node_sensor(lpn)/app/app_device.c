#include "app_device.h"
#include "drivers.h"

const int lux_condition[MAX_CONDITION] = {0, 50, 100, 150, 200, 250, 300,
								   350, 400, 450, 500};
const int span_getVal[MAX_CONDITION] = {60, 600, 1800, 3600, 7200, 14400};

char ind = 0;

sensor lightSensor;

void sensor_init(sensor *s){
	s->Cond = DEFAULT_CONDITION;
	s->Span = DEFAULT_SPAN;
	s->index = ind;
	s->L_adc_value = 0;
	s->C_adc_value = 0;
	ind++;
}

void sensor_set_lum_to_SentAdr(sensor *s, int lum){
		for(int i = 0; i< s->SentAddr.num; i++){
			if(s->SentAddr.subAdr[i] == 0xffff){
				break;
			}
			mesh_set_lum_cmd(s->SentAddr.subAdr[i], lum);
		}
	
	return;
}

void sensor_set_onoff_to_SentAdr(sensor *s, char onoff){
	
		for(int i = 0; i< s->SentAddr.num; i++){
			if(s->SentAddr.subAdr[i] == 0xffff){
				break;
			}
	    	mesh_send_onoff_cmd(s->SentAddr.subAdr[i], onoff);
		}
	
	return;
}

void sensor_update_SentAddr(sensor *s){
	s->SentAddr = mesh_get_sub_addr();
	return;
}

void sensor_update_sensor_data(sensor *s){
	s->sensor_dt = (int)((s->C_adc_value)/10);
	return;
}

void sensor_send_data(sensor *s){
	return;
}

#if SENSOR_CONDITION
	void sensor_update_cond(sensor *s){
		u16 temp = model_sig_sensor.sensor_states[0].setting[0].setting_raw
		u8 loc_cond = temp && 0x00ff;
		u8 loc_span = temp >> 8;
		int cond = lux_condition[loc_cond];
		int span = span_getVal[loc_span];
		if(cond != s->Cond){
			s->Cond = cond;
		}
		if(span != s->Span){
			s->Span = span;
		}
		return;
	}
#endif

void sensor_light_ctrl_process(sensor *s, int adc_data){
	sensor_update_SentAddr(s);
	s->C_adc_value = adc_data;
	if(s->L_adc_value != s->C_adc_value){
		s->L_adc_value = s->C_adc_value;
		sensor_update_sensor_data(s);
		sensor_send_data(s);
		if(s->sensor_dt >= s->Cond){
			sensor_set_onoff_to_SentAdr(s, 0);
		}else{
			sensor_set_onoff_to_SentAdr(s, 1);
			sensor_set_lum_to_SentAdr(s, 100 - (char)((s->sensor_dt)/5));
		}
	}
	
	return;
}

void sensor_ctrl_proc(sensor *s){
	static u32 sensor_check_time, sensor_update_time;

    if(clock_time_exceed(sensor_check_time, lightSensor.Span*1000*1000)){
        sensor_check_time = clock_time();
		static u16 T_adc_val;
		T_adc_val = adc_sample_and_get_result();
		sensor_light_ctrl_process(&lightSensor, T_adc_val);
    }

	if(clock_time_exceed(sensor_update_time, 10*1000*1000)){
        sensor_update_time = clock_time();
		#if SENSOR_CONDITION
			sensor_update_cond(s);
		#endif
    }    
}