#include "app_lcd_handler.h"
#include "app_serial.h"
#include "user_lib/serial.h"
#include "proj/common/tstring.h"
#include "vendor/common/scene.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "app_lcd_model.h"

#define SCENE_CALL_SUCCESS_PAGE_ID     0x09     
#define SCENE_CALL_FAILURE_PAGE_ID     0x08

typedef enum {
    SCENE_CALL_SUCCESS=0,
    SCENE_CALL_FAILURE
}MODULE_RESPONSE_TO_LCD_OPCODE;

void module_handler_scene_btn_onclick(u8 btn_onclick){
    module_handler_btn_onclick(btn_onclick);
}

void module_on_reset(){
    factory_reset();
	start_reboot();
}
