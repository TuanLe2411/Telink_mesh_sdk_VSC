#include "app_routes.h"

#define MESH_CMD_TYPE_LEN (1)
#define MESH_LIGHT_CMD 0x01

#define MESH_CMD_LIGHT_TYPE_LEN (1)
#define MESH_CMD_LIGHT_LUM_SET 0x01
#define MESH_CMD_LIGHT_ONOFF_SET 0x02

meshHandle MeshHandle;

void mesh_light_command_handle(unsigned char *dt, unsigned int len){
    unsigned char mesh_light_cmd_type = dt[0];
    unsigned int mesh_light_cmd_len = len - MESH_CMD_LIGHT_TYPE_LEN;
    unsigned char *mesh_light_cmd_data = dt + MESH_CMD_LIGHT_TYPE_LEN;

    light_control_command *com = (light_control_command* )mesh_light_cmd_data;
    MeshHandle.lig_com = *com;
    if((MeshHandle.lig_com.dest!= 0)){
        MeshHandle.lig_com.cmd_flag = 1;
        MeshHandle.lig_com.cmd_type = mesh_light_cmd_type;
    }
    return;
}

void mesh_command_handle(unsigned char *dt, unsigned int len){
    unsigned char mesh_cmd_type = dt[0];
    unsigned int mesh_cmd_len = len - MESH_CMD_TYPE_LEN;
    unsigned char *mesh_cmd_data = dt + MESH_CMD_TYPE_LEN;
    switch (mesh_cmd_type)
    {
    case MESH_LIGHT_CMD:
        mesh_light_command_handle(mesh_cmd_data, mesh_cmd_len);
        break;
    default:
        break;
    }
    return;
}



void mesh_command_handle_init(meshHandle *m){
    m->cmd_flag = 0;
    return;
};
