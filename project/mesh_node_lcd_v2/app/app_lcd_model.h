#ifndef __APP_LCD_MODEL_H__
#define __APP_LCD_MODEL_H__
#include "proj/common/types.h"

#define REMOTE_AC_MODULE_TYPE              0x0003
#define MODULE_SEND_RGB_HEADER             0x0100

#define VENDOR_LCD_SEND_BTN_ONCLICK        0x52
#define MODULE_SEND_SGBID_ITSELF_OP        0xE3

typedef struct btn_onclick_data_status{
    u16 header;
    u8 bid;
    u16 sceneId; 
    u16 appId;
}btn_onclick_data_status;

typedef struct {
    u16 header;
    u16 appId;
}rgb_scene_call;

extern void module_handler_btn_onclick(u8 btn_onclick);

#endif
