#include "app_sensor_model.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "vendor/common/mesh_node.h"
#include "vendor/common/cmd_interface.h"
#include "drivers/8258/flash.h"

void sensor_on_detected_report(){
    sensor_detect_report_t rep = {0};
    rep.data = SENSOR_DETECTED;
    rep.header = SENSOR_REPORT_ST;

    SendOpParaDebug(ADR_ALL_NODES , 0, SENSOR_TO_GATEWAY_OP, (u8 *)&rep, sizeof(rep));
}

void sensor_no_detected_report(){
    sensor_detect_report_t rep = {0};
    rep.data = SENSOR_NO_DETECTED;
    rep.header = SENSOR_REPORT_ST;

    SendOpParaDebug(ADR_ALL_NODES , 0, SENSOR_TO_GATEWAY_OP, (u8 *)&rep, sizeof(rep));
}

void sensor_update_type_device(){
    sensor_type_device t = {0};
    t.header = SENSOR_TYPE_DEVICE_UPDATE_TO_GW;
    t.type = SMOKE_SENSOR_TYPE;
    t.feature = SMOKE_SENSOR_FEATURE;
    t.app = 0x00;

    SendOpParaDebug(ADR_ALL_NODES , 0, SENSOR_UPDATE_TYPE_DEVICE_OP, (u8 *)&t, sizeof(t));
}