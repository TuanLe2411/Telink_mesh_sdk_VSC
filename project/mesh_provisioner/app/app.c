#include "proj/tl_common.h"
#include "proj_lib/sig_mesh/app_mesh.h"


MYFIFO_INIT(blt_rxfifo, 64, 16);
MYFIFO_INIT(blt_txfifo, 40, 32);


int app_event_handler(){}
void proc_ui(){}
void main_loop(){
    static u32 tick_loop;
	tick_loop ++;

	factory_reset_cnt_check();
    mesh_loop_proc_prior();

    //BLE entry
    blt_sdk_main_loop ();

    //UI entry
    proc_ui();
    proc_led();

    
    mesh_loop_process();
}
void user_init(){}