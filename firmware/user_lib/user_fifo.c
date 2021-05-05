#include "user_fifo.h"
#include "user_lib/serial.h"
user_fifo_t user_fifo;

void user_fifo_init(){
    if(user_fifo.is_init == 1){
        return;
    }
    user_fifo.num_bytes_written = user_fifo.r = user_fifo.w = 0;
    user_fifo.is_init = 1;
}

u8 user_fifo_pop(){
    if(user_fifo.num_bytes_written == 0){
        return 0x00;
    }
    u8 temp = user_fifo.dt[user_fifo.r];
    user_fifo.r = user_fifo.r + 1;
    if(user_fifo.r == MAX_QUEUE_LEN){
        user_fifo.r = 0;
    }
    user_fifo.num_bytes_written = user_fifo.num_bytes_written - 1;
    return temp;
}
void user_fifo_push(u8 d){
    if(user_fifo.num_bytes_written == MAX_QUEUE_LEN){
        return;
    }
    user_fifo.dt[user_fifo.w] = d;
    user_fifo.w = user_fifo.w + 1;
    if(user_fifo.w == MAX_QUEUE_LEN){
        user_fifo.w = 0;
    }
    user_fifo.num_bytes_written = user_fifo.num_bytes_written + 1;
}
u8 user_fifo_get(u8 loc){
    return user_fifo.dt[loc];
}

void user_fifo_reset(){
    user_fifo.num_bytes_written = user_fifo.r = user_fifo.w = 0;
}

u8 user_fifo_get_r_point(){
    return user_fifo.r;
}

u16 user_fifo_get_w_point(){
    return user_fifo.w;
}

u16 user_fifo_get_number_bytes_written(){
    return user_fifo.num_bytes_written;
}

