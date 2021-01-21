#include "app_mesh_com.h"
#include "vendor/common/mesh_node.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "vendor/common/cmd_interface.h"

SubAdr sub;

int get_subAdr(){
	sub.subAdr = model_sig_g_onoff_level.onoff_srv[0].com.sub_list;
	sub.num = sizeof(model_sig_g_onoff_level.onoff_srv[0].com.sub_list)/sizeof(model_sig_g_onoff_level.onoff_srv[0].com.sub_list[0]);
	if(sub.num == 0){
		return 0;
	} 
	return 1;
}


void mesh_send_onoff_cmd(unsigned short int addr, char onoff){
	access_cmd_onoff(addr, 0, onoff, CMD_NO_ACK, 0);
	return;
};

void mesh_set_lum_cmd(unsigned short int addr, int lum){
	access_set_lum(addr, 0, lum, CMD_NO_ACK);
	return;
};

SubAdr mesh_get_sub_addr(){
	get_subAdr();
	SubAdr temp = sub;
	return temp;
}

