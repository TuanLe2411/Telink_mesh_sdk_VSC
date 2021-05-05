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

void user_lcd_data_debug(u8 btn_onclick, u8 btn_mode){
   uart_print_char(btn_onclick);
   uart_print_char(btn_mode);
}

void module_change_lcd_page(u8 pageId){
    u8 uart_response[10] = {0xAA, 0xAA, 0x01, 0x82, 0x00, 0x84, 0x5A, 0x01, 0x00 ,pageId};
    uart_print_data(uart_response, sizeof(uart_response)/sizeof(char), 0, 0);
}

void module_send_response_to_lcd(MODULE_RESPONSE_TO_LCD_OPCODE op){

    if(op == SCENE_CALL_SUCCESS){
        module_change_lcd_page(SCENE_CALL_SUCCESS_PAGE_ID);
    }else{
        module_change_lcd_page(SCENE_CALL_FAILURE_PAGE_ID);
    }

  
}

//handler a button onclick
void module_handler_scene_btn_onclick(u8 btn_onclick, u8 btn_mode){
    module_handler_btn_onclick(btn_onclick, btn_mode);
    //module_send_response_to_lcd(SCENE_CALL_SUCCESS);
}
