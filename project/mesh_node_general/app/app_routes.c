#include "app_routes.h"

#define MESH_CMD_HANDLE "mesh_cmd"
#define MESH_CMD_TYPE_LEN 1

#define MESH_CMD_LIGHT_HANDLE 0x01

meshHandle MeshHandle;

void mesh_command_handle(char *dt, unsigned int len){
    char mesh_cmd_type_len = dt[0];
    unsigned int mesh_cmd_len = len - MESH_CMD_TYPE_LEN;
    char *mesh_cmd_data = dt + MESH_CMD_TYPE_LEN;
    switch (mesh_cmd_type_len)
    {
    case MESH_CMD_LIGHT_HANDLE:
        mesh_light_command_handle(mesh_cmd_data, mesh_cmd_len);
        break;
    default:
        break;
    }
    return;
}

void mesh_light_command_handle(char *dt, unsigned int len){
    light_control_command *com = (light_control_command* )dt;
    MeshHandle.lig_com = *com;
    if((MeshHandle.lig_com.dest!= 0)){
        MeshHandle.h.cmd_flag = 1;
    }
}

void mesh_command_handle_init(meshHandle *hl, char *name){
    hl->h.name = name;
    return;
};
