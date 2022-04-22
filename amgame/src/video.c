#include <game.h>

#define SIDE 16
static int w, h;

void draw_init() {
  AM_GPU_CONFIG_T info = {0};
  ioe_read(AM_GPU_CONFIG, &info);
  w = info.width;
  h = info.height;
}

static void draw_tile(int x, int y, int w, int h, uint32_t color) {
  uint32_t pixels[w * h]; // WARNING: large stack-allocated memory
  AM_GPU_FBDRAW_T event = {
    .x = x, .y = y, .w = w, .h = h, .sync = 1,
    .pixels = pixels,
  };
  for (int i = 0; i < w * h; i++) {
    pixels[i] = color;
  }
  ioe_write(AM_GPU_FBDRAW, &event);
}

void draw_background(){
    for(int x = 0; x * GRID_SIZE <= w; x++){
        for(int y = 0; y * GRID_SIZE <= h; y++){
            draw_tile(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, GRID_COLOR);
        }
    }
}

void draw_snake(){
    for(int i = 0; i < snake.size; ++i){
        int x = snake.body[i].x, y = snake.body[i].y;
        draw_tile(x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE, SNAKE_COLOR);
    }    
}

int window_width(){return w;}
int window_height(){return h;}
