#include "app_lcd_model.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "proj/common/types.h"
#include "vendor/common/mesh_node.h"
#include "vendor/common/cmd_interface.h"
#include "app_serial.h"

u16 find_sceneId_from_btn_onclick(u8 btn_onclick, u8 btn_mode){
    for(int i = 0; i< MAX_SCENE_SAVE; i++){
        if((model_vd_btn_scene.btn[0][i].bid == btn_onclick)&&(model_vd_btn_scene.btn[0][i].mid == btn_mode))
        {
            return model_vd_btn_scene.btn[0][i].sceneId;
        }
    }
    return 0;
}

u16 find_appId_from_btn_onclick(u8 btn_onclick, u8 btn_mode){
    for(int i = 0; i< MAX_SCENE_SAVE; i++){
        if((model_vd_btn_scene.btn[0][i].bid == btn_onclick)&&(model_vd_btn_scene.btn[0][i].mid == btn_mode))
        {
            return model_vd_btn_scene.btn[0][i].appId;
        }
    }
    return 0;
}

void module_send_btn_onclick_to_hc(u8 btn_onclick, u8 btn_mode){
    btn_onclick_data_status btn_onclick_status = {0};
    btn_onclick_status.header = REMOTE_AC_MODULE_TYPE;
    btn_onclick_status.bid = btn_onclick;
    btn_onclick_status.mid = btn_mode;
    btn_onclick_status.sceneId = find_sceneId_from_btn_onclick(btn_onclick, btn_mode);
    btn_onclick_status.appId = find_appId_from_btn_onclick(btn_onclick, btn_mode);
    #if 1
        u8 buff[20];
        sprintf(buff, "%d %d %d %d %d\n", btn_onclick_status.header,  btn_onclick_status.bid, btn_onclick_status.mid, btn_onclick_status.sceneId, btn_onclick_status.appId);
        uart_print_data(buff, strlen(buff), 0, 0);
    #endif

    #if 1
        SendOpParaDebug(0xffff, 0, VENDOR_LCD_SEND_BTN_ONCLICK, (u8 *)&btn_onclick_status, sizeof(btn_onclick_status));
    #else
        SendOpParaDebug(GATEWAY_ADDR, 0, VENDOR_LCD_SEND_BTN_ONCLICK, (u8 *)&btn_onclick_status, sizeof(btn_onclick_status));
    #endif

}
