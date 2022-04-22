#include <game.h>

/*
#define KEYNAME(key) \
  [AM_KEY_##key] = #key,
static const char *key_names[] = {
  AM_KEYS(KEYNAME)
};
*/

//void print_key() {
//  AM_INPUT_KEYBRD_T event = { .keycode = AM_KEY_NONE };
//  ioe_read(AM_INPUT_KEYBRD, &event);
//  if (event.keycode != AM_KEY_NONE && event.keydown) {
//    puts("Key pressed: ");
//    puts(key_names[event.keycode]);
//    puts("\n");
//  }
//}

int read_key(){
    AM_INPUT_KEYBRD_T event = io_read(AM_INPUT_KEYBRD);
    if(event.keycode != AM_KEY_NONE && event.keydown){
        //printf("Key pressed: ");
        //printf("%s\n", key_names[event.keycode]);
        return event.keycode;
    }
    return AM_KEY_NONE;
}

void kbd_event(int key){
    switch(key){ 
        case AM_KEY_I:
            snake.direc = UP;
            break;
        case AM_KEY_K:
            snake.direc = DOWN;
            break;
        case AM_KEY_J:
            snake.direc = LEFT;
            break;
        case AM_KEY_L:
            snake.direc = RIGHT;
            break;
        case AM_KEY_ESCAPE:
            halt(1);
            break;
    }
}
