#include "app_serial.h"
#include "proj/drivers/uart.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "app_routes.h"
#include "jsmn/jsmn.h"
#include "proj/common/tstring.h"
#include <stdio.h>

#define UART_CMD_LEN         (1)
#define UART_MESH_CMD       0x01
#define UART_TEST_JSON_CMD  0x30

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

void uart_data_handle(u8 *dt, u16 len){
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
        case UART_TEST_JSON_CMD:
            break;
        default:
            break;
    }

    return;
}

typedef struct json_data{
    char *type;
    u8 *dt;
}json_data;

json_data jsonData;

void rx_from_uart_cb(){
	uart_ErrorCLR();
	
	uart_data_t* p = (uart_data_t *)my_fifo_get(&hci_rx_fifo);
	if(p){
		u32 rx_len = p->len & 0xff; //usually <= 255 so 1 byte should be sufficient
		if (rx_len)
		{
			//uart_data_handle(p->data, p->len);
            uart_json_data_handle(p->data, &jsonData);
		}
	}
	return;
}



static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

int uart_json_data_handle(const char *dt, json_data *j){
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
        return 1;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT) {
        //printf("Object expected\n");
        return 1;
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {
        if (jsoneq(dt, &t[i], "type") == 0) {
            u8 buff[t[i + 1].end - t[i + 1].start];
            memcpy(buff, dt + t[i + 1].start, t[i + 1].end - t[i + 1].start);
            j->type = (char *)buff;
            uart_print_data((u8 *)(j->type), t[i + 1].end - t[i + 1].start, 0, 0);
            i++;
        } else if (jsoneq(dt, &t[i], "data") == 0) {
            u8 buff[t[i + 1].end - t[i + 1].start];
            memcpy(buff, dt + t[i + 1].start, t[i + 1].end - t[i + 1].start);
            j->dt = (u8 *)buff;
            uart_print_data((u8 *)(j->dt), t[i + 1].end - t[i + 1].start, 0, 0);
            i++;
        }else {
        //printf("Unexpected key: %.*s\n", t[i].end - t[i].start, JSON_STRING + t[i].start);
        }
    }
    return 0;
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
