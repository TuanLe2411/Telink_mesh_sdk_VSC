#ifndef __APP_ROUTES_H__
#define __APP_ROUTES_H__
#include "proj/common/types.h"

    typedef u8 (*mesh_func_handle)(u8 *dt, unsigned int len);

    typedef struct mesh_command{
        const char cmd_opcode;
        mesh_func_handle Mesh_func_handle;
    }mesh_command;

    extern mesh_command Mesh_Command[];

    typedef struct light_control_command{
        unsigned short int dest;
        u8 level;
        u8 onoff;
        char cmd_flag;
        char cmd_type;
    }light_control_command;

    typedef struct meshHandle{
        char cmd_flag;
        light_control_command lig_com;
    }meshHandle;

    extern meshHandle MeshHandle;
    extern void mesh_command_handle_init(meshHandle *h);
    extern void mesh_command_handle(u8 *dt, unsigned int len);
#endif
