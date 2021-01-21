#ifndef __APP_ROUTES_H__
#define __APP_ROUTES_H__

    typedef struct light_control_command{
        unsigned short int dest;
        unsigned char level;
        unsigned char onoff;
        char cmd_flag;
        char cmd_type;
    }light_control_command;

    typedef struct meshHandle{
        char cmd_flag;
        light_control_command lig_com;
    }meshHandle;

    extern meshHandle MeshHandle;
    extern void mesh_command_handle_init(meshHandle *h);
    extern void mesh_command_handle(unsigned char *dt, unsigned int len);
#endif
