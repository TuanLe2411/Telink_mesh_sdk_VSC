#ifndef __APP_SENSOR_MODEL_H__
#define __APP_SENSOR_MODEL_H__

#include "proj/common/types.h"

#define SENSOR_TO_GATEWAY_OP                0x52
#define SENSOR_UPDATE_TYPE_DEVICE_OP        0xE1

#define SENSOR_TYPE_DEVICE_UPDATE_TO_GW     0x0003
#define SMOKE_SENSOR_TYPE                   0x03
#define SMOKE_SENSOR_FEATURE                0x03

#define SENSOR_DETECTED                     0x10
#define SENSOR_NO_DETECTED                  0x00

#define SENSOR_REPORT_ST                    0x0108
#define SENSOR_REPORT_POWER                 0x0208

typedef struct {
    u16 header;
    u8 data;
}sensor_detect_report_t;

typedef struct {
    u16 header;
    u8 data;
}sensor_power_report_t;

typedef struct {
    u16 header;
    u8 type;
    u8 feature;
    u8 app;
}sensor_type_device;

extern void sensor_on_detected_report();
extern void sensor_no_detected_report();
extern void sensor_update_type_device();

#endif