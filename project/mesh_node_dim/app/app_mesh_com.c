#include "app_mesh_com.h"
#include "vendor/common/mesh_node.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "vendor/common/cmd_interface.h"

void send_onoff_cmd(unsigned short int addr, char onoff){
	access_cmd_onoff(addr, 0, onoff, CMD_NO_ACK, 0);
	return;
};

void set_lum_cmd(unsigned short int addr, int lum){
	access_set_lum(addr, 0, lum, CMD_NO_ACK);
	return;
};
