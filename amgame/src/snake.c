#include <game.h>

void snake_init(){
    int w = window_width();
    int h = window_height();
    int x_grid_num = w / GRID_SIZE;
    int y_grid_num = h / GRID_SIZE;

    snake.direc = NONE;
    snake.size = SNAKE_INIT_SIZE;
    snake.body[0] = (Body){x_grid_num / 2 - 1, y_grid_num / 2};
    snake.body[1] = (Body){x_grid_num / 2, y_grid_num / 2}; 
    snake.body[2] = (Body){x_grid_num / 2 + 1, y_grid_num / 2};
}

void move_snake(){
    for(int i = snake.size - 2; i >= 0; --i){
        snake.body[i + 1] = snake.body[i];
    }
    
    int x = snake.body[1].x, y = snake.body[1].y;
    switch(snake.direc){
        case UP:
            y = snake.body[1].y - 1;
            break;
        case DOWN:
            y = snake.body[1].y + 1;
            break;
        case LEFT:
            x = snake.body[1].x - 1;
            break;
        case RIGHT:
            x = snake.body[1].x + 1;
            break;
        case NONE:
            break;
    }
    snake.direc = NONE;
    
    snake.body[0] = (Body){x, y};
}
