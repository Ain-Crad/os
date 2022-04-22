#include <game.h>
#include <limits.h>

// Operating system is a C program!
//int main(const char *args) {
//    ioe_init();
//    
//    puts("mainargs = \"");
//    puts(args); // make run mainargs=xxx
//    puts("\"\n");
//
//    splash();
//
//    puts("Press any key to see its key code...\n");
//    while (1) {
//        print_key();
//    }
//    return 0;
//}

const int FPS = 10;

Snake snake;

void game_init(){
    draw_init();
    snake_init(); 
    
    draw_background(); 
    draw_snake();
}

void game_progress(){
    move_snake();
}

void screen_update(){
    draw_background();
    draw_snake();
}

int main(){
    ioe_init();
    game_init();
    
    uint64_t next_frame = update_time();
    
    int key = AM_KEY_NONE;
    while(1){
        while(update_time() < next_frame);
        while((key = read_key()) != AM_KEY_NONE){
            kbd_event(key);
        }

        game_progress();
        screen_update();
        next_frame += 1000 / FPS;
    }
}
