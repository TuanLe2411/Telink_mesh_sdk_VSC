#ifndef __APP_ROUTES_H__
#define __APP_ROUTES_H__

    typedef struct handle{
        char *name;
        char cmd_flag;
    }handle;

    typedef struct light_control_command{
        unsigned short int dest;
        unsigned char level;
        unsigned char onoff;
    }light_control_command;

    typedef struct meshHandle{
        handle h;
        light_control_command lig_com;
    }meshHandle;

    extern meshHandle MeshHandle;
    extern void mesh_command_handle_init(meshHandle *h, char *name);
    extern void mesh_command_handle(char *dt, unsigned int len);
#endif
