#include "app_serial.h"
#include "proj/drivers/uart.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "app_routes.h"
#include "jsmn/jsmn.h"
#include "proj/common/tstring.h"
#include <stdio.h>
#include <stdlib.h>

#define UART_CMD_LEN         (1)
#define UART_MESH_CMD       0x01

u8 mesh_get_hci_tx_fifo_cnt()
{
	return hci_tx_fifo.size;
}

int uart_print_data(u8 *para, int n, u8 *head, u8 head_len){
	u8 fifoSize = mesh_get_hci_tx_fifo_cnt();
	if(n > (fifoSize - 2 - 1)){ // 2: size of length,  1: size of type
        return -1;
    }
	return my_fifo_push_hci_tx_fifo(para, n, head, head_len);
}

void uart_byte_data_handle(u8 *dt, u16 len){
    u8 cmd = dt[0];
    u8 *serial_data = dt + UART_CMD_LEN;
    unsigned int serial_data_len = len - UART_CMD_LEN;
    switch (cmd)
    {
        case UART_MESH_CMD:
            if(MeshHandle.cmd_flag == 0){
                MeshHandle.cmd_flag = 1;
                mesh_command_handle(serial_data, serial_data_len);
            }
            break;
        default:
            break;
    }

    return;
}

char byte[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
u8* uart_convert_string_to_byte(u8* dt, u16 len){
    static u8 r[126];
	u8 a[2];
	int i = 0;
	while(i < len){
		a[0] = dt[i];
		a[1] = dt[i+1];
		u8 number = (u8)strtol(a, NULL, 16);
		r[i/2] = number;
		i = i + 2;  
	}
	return r;
};

json_data jsonData;

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

int uart_json_data_handle(const char *dt, u16 len){
    uart_print_data((u8 *)dt, strlen(dt), 0, 0);
    int i;
    int r;
    jsmn_parser p;
    jsmntok_t t[128]; /* We expect no more than 128 tokens */

    jsmn_init(&p);
    r = jsmn_parse(&p, dt, strlen(dt), t,
                    sizeof(t) / sizeof(t[0]));
    if (r < 0) {
        //printf("Failed to parse JSON: %d\n", r);
        return 0;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        //printf("Object expected\n");
        return 0;
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {
        if (jsoneq(dt, &t[i], "type") == 0) {
            u8* buff;
            memcpy(buff, dt + t[i + 1].start, t[i + 1].end - t[i + 1].start);
            uart_print_data((u8 *)buff, t[i + 1].end - t[i + 1].start, 0, 0);
            u8* temp = uart_convert_string_to_byte((u8 *)buff, t[i + 1].end - t[i + 1].start);
            for(int j = 0; j< (t[i + 1].end - t[i + 1].start)/2; j++){
                jsonData.type[j] = temp[j];
            }
            i++;
        } else if (jsoneq(dt, &t[i], "data") == 0) {
            u8* buff;
            memcpy(buff, dt + t[i + 1].start, t[i + 1].end - t[i + 1].start);
            uart_print_data((u8 *)buff, t[i + 1].end - t[i + 1].start, 0, 0);
            u8* temp = uart_convert_string_to_byte((u8 *)buff, t[i + 1].end - t[i + 1].start);
            for(int j = 0; j< (t[i + 1].end - t[i + 1].start)/2; j++){
                jsonData.dt[j] = temp[j];
            }
            if(jsonData.type[0] == UART_MESH_CMD){
                mesh_command_handle(jsonData.dt, (t[i + 1].end - t[i + 1].start)/2);
            }
            i++;
        }else {
        //printf("Unexpected key: %.*s\n", t[i].end - t[i].start, JSON_STRING + t[i].start);
        }
    }
    return 1;
}

#if 1

typedef struct jsontest {
  u8 *user;
}jsontest;

jsontest jsonTest;

static const char *JSON_STRING =
    "{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
    "\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";

int uart_json_test() {
    int i;
    int r;
    jsmn_parser p;
    jsmntok_t t[128]; /* We expect no more than 128 tokens */

    jsmn_init(&p);
    r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t,
                    sizeof(t) / sizeof(t[0]));
    if (r < 0) {
        //printf("Failed to parse JSON: %d\n", r);
        return 1;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        //printf("Object expected\n");
        return 1;
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {
        if (jsoneq(JSON_STRING, &t[i], "user") == 0) {
        /* We may use strndup() to fetch string value */
        //printf("- User: %.*s\n", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
        u8 buff[t[i + 1].end - t[i + 1].start];
        memcpy(buff, JSON_STRING + t[i + 1].start, t[i + 1].end - t[i + 1].start);
        jsonTest.user = (u8 *)buff;
        uart_print_data(jsonTest.user, t[i + 1].end - t[i + 1].start, 0, 0);
        i++;
        } else if (jsoneq(JSON_STRING, &t[i], "admin") == 0) {
        /* We may additionally check if the value is either "true" or "false" */
        //printf("- Admin: %.*s\n", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
        i++;
        } else if (jsoneq(JSON_STRING, &t[i], "uid") == 0) {
        /* We may want to do strtol() here to get numeric value */
        //printf("- UID: %.*s\n", t[i + 1].end - t[i + 1].start, JSON_STRING + t[i + 1].start);
        i++;
        } else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
        int j;
        //printf("- Groups:\n");
        if (t[i + 1].type != JSMN_ARRAY) {
            continue; /* We expect groups to be an array of strings */
        }
        for (j = 0; j < t[i + 1].size; j++) {
            jsmntok_t *g = &t[i + j + 2];
            //printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
        }
        i += t[i + 1].size + 1;
        } else {
        //printf("Unexpected key: %.*s\n", t[i].end - t[i].start, JSON_STRING + t[i].start);
        }
    }
    return 0;
}

#endif

void rx_from_uart_cb(){
	uart_ErrorCLR();
	
	uart_data_t* p = (uart_data_t *)my_fifo_get(&hci_rx_fifo);
	if(p){
		u32 rx_len = p->len & 0xff; //usually <= 255 so 1 byte should be sufficient
		if (rx_len)
		{
			//uart_data_handle(p->data, p->len);
            uart_json_data_handle(p->data, p->len);
            my_fifo_pop(&hci_rx_fifo);
		}
	}
	return;
}