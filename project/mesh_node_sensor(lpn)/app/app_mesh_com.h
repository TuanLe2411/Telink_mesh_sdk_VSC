#ifndef __APP_MESH_COM__
#define __APP_MESH_COM__
#include "proj/common/types.h"

typedef struct SubAdr{
	u16* subAdr;
	int num;
}SubAdr;

extern void mesh_send_onoff_cmd(unsigned short int addr, char onoff);
extern void mesh_set_lum_cmd(unsigned short int addr, int lum);
extern SubAdr mesh_get_sub_addr();

#endif