#include <game.h>

uint64_t update_time(){
    return io_read(AM_TIMER_UPTIME).us / 1000;
}
