#ifndef __APP_MESH_COM__
#define __APP_MESH_COM__

    typedef struct sub_Adr{
	    unsigned short int* subAdr;
	    int num;
    }sub_Adr;
    
    typedef struct sensor_copy{
        char *name;
	    int Cond;
	    int Span;
	    char index;
	    unsigned int sensor_dt;
        sub_Adr SubAdr;
    }sensor_copy;

    extern sensor_copy Light_sensor;
    extern void sensor_copy_init(sensor_copy *s, char *name);
    extern void sensor_copy_send_sensor_data(sensor_copy s, unsigned int sensor_dt);
    extern void sensor_copy_send_onoff_cmd(sensor_copy s, char onoff);
    extern void sensor_copy_set_lum_cmd(sensor_copy s, int lum);
    extern void sensor_copy_update_sensor_data(sensor_copy *s, unsigned int sensor_dt);

    extern void send_onoff_cmd(unsigned short int addr, char onoff);
    extern void set_lum_cmd(unsigned short int addr, int lum);
#endif