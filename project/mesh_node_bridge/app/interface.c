#include "interface.h"
#include "proj/common/types.h"
#include "drivers.h"
#include "app_routes.h"
#include "app_serial.h"
#include "app_mesh_com.h"

void user_set_onoff_to_SentAdr(char onoff){
	SubAdr SentAddr = mesh_get_sub_addr();
		for(int i = 0; i< SentAddr.num; i++){
			if(SentAddr.subAdr[i] == 0xffff){
				break;
			}
	    	mesh_send_onoff_cmd(SentAddr.subAdr[i], onoff);
		}
	
	return;
}

void user_func_init(){
    mesh_command_handle_init(&MeshHandle);
};

void user_func_proc(){
    #if 0
        static u32 test_time;
        if(clock_time_exceed(test_time, 3000*1000)){
            test_time = clock_time();
            uart_json_test(); 
        }
    #endif

    static u32 serial_check_time;
    if(clock_time_exceed(serial_check_time, 200*1000)){
        if(MeshHandle.cmd_flag == 1){
            if(MeshHandle.lig_com.cmd_flag == 1){
                switch (MeshHandle.lig_com.cmd_type)
                {
                    case MESH_CMD_LIGHT_ONOFF_SET:
                        if(MeshHandle.lig_com.dest){
                            mesh_send_onoff_cmd(MeshHandle.lig_com.dest, MeshHandle.lig_com.onoff);
                            mesh_command_response(MESH_COMMAND_RESULT_CODE_OK);
                        }else{
                            user_set_onoff_to_SentAdr( MeshHandle.lig_com.onoff);
                            mesh_command_response(MESH_COMMAND_RESULT_CODE_OK);
                        }
                        MeshHandle.cmd_flag = 0;
                        MeshHandle.lig_com.cmd_flag = 0;
                        break;
                    case MESH_CMD_LIGHT_LUM_SET:
                        break;
                    default:
                        break;
                }
            }
        }
    }
    serial_check_time = clock_time();
};
