#include "app_lcd_model.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "proj/common/types.h"
#include "vendor/common/mesh_node.h"
#include "vendor/common/cmd_interface.h"
#include "app_serial.h"
#include "vendor/common/scene.h"
#include "user_lib/serial.h"
#include "drivers/8258/flash.h"

#define GW_ADDR_SAVE 0x29500

u16 get_gw_addr(){
   u8 buf[2];
   flash_read_page(GW_ADDR_SAVE, 2, buf);
   return buf[0] << 8 | buf[1]; 
}

u16 find_sceneId_from_btn_onclick(u8 btn_onclick){
    for(int i = 0; i< MAX_SCENE_SAVE; i++){
        if(model_vd_btn_scene.btn[0][i].bid == btn_onclick)
        {
            return model_vd_btn_scene.btn[0][i].sceneId;
        }
    }
    return 0;
}

u16 find_appId_from_btn_onclick(u8 btn_onclick){
    for(int i = 0; i< MAX_SCENE_SAVE; i++){
        if(model_vd_btn_scene.btn[0][i].bid == btn_onclick)
        {
            return model_vd_btn_scene.btn[0][i].appId;
        }
    }
    return 0;
}

void module_send_btn_onclick_to_hc(u8 btn_onclick, u16 sceneId, u16 appId){
    btn_onclick_data_status btn_onclick_status = {0};
    btn_onclick_status.header = REMOTE_AC_MODULE_TYPE;
    btn_onclick_status.bid = btn_onclick;
    btn_onclick_status.sceneId = sceneId;
    btn_onclick_status.appId = appId;

    u16 gw_addr = get_gw_addr();
    #if 1
        SendOpParaDebug(ADR_ALL_NODES, 0, VENDOR_LCD_SEND_BTN_ONCLICK, (u8 *)&btn_onclick_status, sizeof(btn_onclick_status));
    #else
        SendOpParaDebug(gw_addr, 0, VENDOR_LCD_SEND_BTN_ONCLICK, (u8 *)&btn_onclick_status, sizeof(btn_onclick_status));
    #endif
    return;
}

void module_handler_btn_onclick_itself(u16 sceneId, u16 appId){
    if(sceneId != 0){
        access_cmd_scene_recall(ADR_ALL_NODES, 0, sceneId, CMD_NO_ACK, 0);
    }
    if(appId != 0){
        rgb_scene_call rgb = {0};
        rgb.appId = sceneId;
        rgb.header = MODULE_SEND_RGB_HEADER;
        SendOpParaDebug(ADR_ALL_NODES, 0, MODULE_SEND_SGBID_ITSELF_OP, (u8 *)&rgb, sizeof(rgb));
    }
    return;
}

void module_handler_btn_onclick(u8 btn_onclick){
    u16 sceneId = find_sceneId_from_btn_onclick(btn_onclick);
    u16 appId   = find_appId_from_btn_onclick(btn_onclick);
    #if 1
        u8 buff[20];
        sprintf(buff, "%d %d %d", btn_onclick, sceneId, appId);
        uart_print_data(buff, strlen(buff), 0, 0);
    #endif
    module_send_btn_onclick_to_hc(btn_onclick, sceneId, appId);
    module_handler_btn_onclick_itself(sceneId, appId);
}


