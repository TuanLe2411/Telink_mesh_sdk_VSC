#include "app_lcd_model.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "proj/common/types.h"
#include "vendor/common/mesh_node.h"
#include "vendor/common/cmd_interface.h"
#include "app_serial.h"
#include "vendor/common/scene.h"
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

void module_send_btn_onclick_to_hc(u8 btn_onclick, u8 btn_mode, u16 sceneId, u16 appId){
    btn_onclick_data_status btn_onclick_status = {0};
    btn_onclick_status.header = REMOTE_AC_MODULE_TYPE;
    btn_onclick_status.bid = btn_onclick;
    btn_onclick_status.mid = btn_mode;
    btn_onclick_status.sceneId = sceneId;
    btn_onclick_status.appId = appId;
    #if 1
        SendOpParaDebug(ADR_ALL_NODES, 0, VENDOR_LCD_SEND_BTN_ONCLICK, (u8 *)&btn_onclick_status, sizeof(btn_onclick_status));
    #else
        SendOpParaDebug(GATEWAY_ADDR, 0, VENDOR_LCD_SEND_BTN_ONCLICK, (u8 *)&btn_onclick_status, sizeof(btn_onclick_status));
    #endif
    return;
}

void module_handler_btn_onclick_itself(u16 sceneId, u16 appId){
    if(sceneId != 0){
        access_cmd_scene_recall(ADR_ALL_NODES, 0, sceneId, CMD_NO_ACK, 0);
    }
    if(appId != 0){
        rgb_scene_call rgb = {0};
        SendOpParaDebug(ADR_ALL_NODES, 0, VENDOR_LCD_SEND_BTN_ONCLICK, (u8 *)&rgb, sizeof(rgb));
    }
    return;
}

void module_handler_btn_onclick(u8 btn_onclick, u8 btn_mode){
    u16 sceneId = find_sceneId_from_btn_onclick(btn_onclick, btn_mode);
    u16 appId   = find_appId_from_btn_onclick(btn_onclick, btn_mode);
    #if 1
        u8 buff[20];
        sprintf(buff, "%d %d %d %d\n", btn_onclick, btn_mode, sceneId, appId);
        uart_print_data(buff, strlen(buff), 0, 0);
    #endif
    module_send_btn_onclick_to_hc(btn_onclick, btn_mode, sceneId, appId);
    module_handler_btn_onclick_itself(sceneId, appId);
}


