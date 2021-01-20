#include "app_mesh_com.h"
#include "vendor/common/mesh_node.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "vendor/common/cmd_interface.h"

#define SENSOR_CONDITION 0
#define MAX_CONDITION 100
#define DEFAULT_CONDITION 200
#define DEFAULT_SPAN 5        //s
#define LIGHT_SENSOR_NAME "lightSensor"

static int index = 0;
sensor_copy Light_sensor;
void sensor_copy_init(sensor_copy *s, char *name){
    s->Cond = DEFAULT_CONDITION;
	s->Span = DEFAULT_SPAN;
	s->name = name;
	s->index = index;
	index++;
};

sub_Adr Sub_Adr;

void sensor_copy_send_sensor_data(sensor_copy s, unsigned int sensor_dt){
	return;
};
void sensor_copy_send_onoff_cmd(sensor_copy s, char onoff){
	int ok = update_subAdr();
	if(ok){
		for(int i = 0; i< s.SubAdr.num; i++){
			if(s.SubAdr.subAdr[i] == ADR_ALL_NODES){
				break;
			}
	    access_cmd_onoff(s.SubAdr.subAdr[i], 0, onoff, CMD_NO_ACK, 0);
		}
	}
	return;
};
void sensor_copy_set_lum_cmd(sensor_copy s, int lum){
	int ok = update_subAdr();
	if(ok){
		for(int i = 0; i< s.SubAdr.num; i++){
			if(s.SubAdr.subAdr[i] == ADR_ALL_NODES){
				break;
			}
			access_set_lum(s.SubAdr.subAdr[i], 0, lum, CMD_NO_ACK);
		}
	}
	return;
};
void sensor_copy_update_sensor_data(sensor_copy *s, unsigned int sensor_dt){
	if(s->name = LIGHT_SENSOR_NAME){
		s->sensor_dt = sensor_dt;
	}
};

void sensor_copy_update_subAdr(sensor_copy *s){
	int ok = update_subAdr();
	if(ok){	
		s->SubAdr = Sub_Adr;
	}
	return;
};

void send_onoff_cmd(unsigned short int addr, char onoff){
	access_cmd_onoff(addr, 0, onoff, CMD_NO_ACK, 0);
	return;
};

void set_lum_cmd(unsigned short int addr, int lum){
	access_set_lum(addr, 0, lum, CMD_NO_ACK);
	return;
};

int update_subAdr(){
	Sub_Adr.subAdr = model_sig_g_onoff_level.onoff_srv[0].com.sub_list;
	Sub_Adr.num = sizeof(model_sig_g_onoff_level.onoff_srv[0].com.sub_list)/sizeof(model_sig_g_onoff_level.onoff_srv[0].com.sub_list[0]);
	if(Sub_Adr.num == 0){
		return 0;
	} 
	return 1;
}