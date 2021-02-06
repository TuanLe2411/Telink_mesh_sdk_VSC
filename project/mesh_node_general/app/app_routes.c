#include "app_routes.h"
#include "app_serial.h"
#include "proj/common/types.h"

#define AT_RESULT_CODE_MAX 3

enum {
    MESH_COMMAND_RESULT_CODE_OK = 0,
    MESH_COMMAND_RESULT_CODE_ERROR,
    MESH_COMMAND_RESULT_CODE_INVALID,
};

const u8 *mesh_result_string[AT_RESULT_CODE_MAX] = 
{
    "\r\nMesh command handler successfully\r\n",         //MESH_COMMAND_RESULT_CODE_OK         = 0x00,
    "\r\nMesh command handler error\r\n",                //MESH_COMMAND_RESULT_CODE_ERROR      = 0x01,
    "\r\nMesh command invalid\r\n",                      //MESH_COMMAND_RESULT_CODE_INVALID   = 0x02,
};

#define MESH_CMD_TYPE_LEN (1)
#define MESH_LIGHT_CMD 0x01

#define MESH_CMD_LIGHT_TYPE_LEN (1)
#define MESH_CMD_LIGHT_LUM_SET 0x01
#define MESH_CMD_LIGHT_ONOFF_SET 0x02

meshHandle MeshHandle;

void mesh_light_command_handle(u8 *dt, unsigned int len){
    u8 mesh_light_cmd_type = dt[0];
    unsigned int mesh_light_cmd_len = len - MESH_CMD_LIGHT_TYPE_LEN;
    u8 *mesh_light_cmd_data = dt + MESH_CMD_LIGHT_TYPE_LEN;

    light_control_command *com = (light_control_command* )mesh_light_cmd_data;
    MeshHandle.lig_com = *com;
    if((MeshHandle.lig_com.dest!= 0)){
        MeshHandle.lig_com.cmd_flag = 1;
        MeshHandle.lig_com.cmd_type = mesh_light_cmd_type;
    }
    return;
}

void mesh_command_handle(u8 *dt, unsigned int len){
    u8 mesh_cmd_type = dt[0];
    unsigned int mesh_cmd_len = len - MESH_CMD_TYPE_LEN;
    u8 *mesh_cmd_data = dt + MESH_CMD_TYPE_LEN;
    const mesh_command *cmd_ptr = Mesh_Command; 

    switch (cmd_ptr->cmd_opcode)
    {
    case MESH_LIGHT_CMD:
        cmd_ptr->Mesh_func_handle(mesh_cmd_data, mesh_cmd_len);
        break;
    default:
        break;
    }
    return;
}


void mesh_command_handle_init(meshHandle *m){
    m->cmd_flag = 0;
    return;
}

void mesh_command_response(char response_code){
     if (response_code < AT_RESULT_CODE_MAX) 
    {
        uart_print_data(mesh_result_string[response_code], sizeof(mesh_result_string[response_code])/sizeof(mesh_result_string[response_code][0]), 0, 0);
    }
    return;
}

mesh_command Mesh_Command[] = {
    {0x01, mesh_light_command_handle},
};
