#include <am.h>
#include <amdev.h>
#include <klib.h>
#include <klib-macros.h>

#define GRID_SIZE               10
#define GRID_COLOR              0xffffff //white
// time
uint64_t update_time();

// video
void draw_init();
void draw_background();
void draw_snake();
int window_width();
int window_height();

// keyboard
void print_key();
int read_key();
void kbd_event(int key);

// snake
#define SNAKE_INIT_SIZE         3
#define SNAKE_MAX_SIZE          10
#define SNAKE_COLOR             0x0000ff //blue

typedef struct Body Body;

struct Body{
    int x, y;
};

typedef enum Direction Direction;

enum Direction{
    NONE, UP, DOWN, LEFT, RIGHT
};

typedef struct Snake Snake;

struct Snake{
    int size;
    Direction direc;
    Body body[SNAKE_MAX_SIZE];
};

void snake_init();
void move_snake();

extern Snake snake;
