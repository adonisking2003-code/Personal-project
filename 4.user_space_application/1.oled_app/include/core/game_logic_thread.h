#ifndef GAME_LOGIC_THREAD_H
#define GAME_LOGIC_THREAD_H

#include "ssd1306.h"
#include <pthread.h> 

#define MAP_WIDTH 	128 // width oled
#define MAP_HEIGHT	64	// Height oled
#define BIRD_HEIGHT	5
#define COLUMN_GAP	10	// Khoảng trống cột
#define COLUMN_WIDTH 5

typedef enum {
	GAME_STATE_READY 	= 0,
	GAME_STATE_PLAYING,
	GAME_STATE_OVER
} eGameState;

typedef struct stBirdInfo {
	uint8_t bird_x;
	uint8_t bird_y;
	uint8_t bird_acceleration;
	uint8_t bird_height;
	uint8_t bird_width;
} stBirdInfo;

typedef struct stColumnInfo {
	int column_top_y;    // Vị trí Y của phần trên cột
	int column_bottom_y; // Vị trí Y của phần dưới cột
	int column_x;        // Vị trí X của từng cột
} stColumnInfo;

typedef struct stColumnList stColumnList;  // forward declare tên struct

struct stColumnList {
    stColumnInfo col;
    stColumnList *next_column;  // trỏ tới chính struct kiểu này
};

typedef struct stGameInfo {
	int points;
	eGameStatus status;
	int time_play;
} stGameInfo;

void *game_logic_thread_func(void *arg);

void move_up(struct stBirdInfo *bird);
void move_down(struct stBirdInfo *bird);
int check_bird_collision(struct stBirdInfo *bird, struct stColumnList *col_list);

void create_column();
void update_column(struct stColumnList *col_list);

void increase_point(struct stGameInfo *game_point);

stBirdInfo init_bird(struct stBirdInfo *bird, uint8_t bird_x, uint8_t bird_y, uint8_t bird_h, uint8_t bird_w, uint8_t bird_accel);
void init_game(uint8_t game_vel);

#endif // GAME_LOGIC_THEAD_H