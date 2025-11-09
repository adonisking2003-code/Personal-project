#ifndef SSD1306_DATA
#define SSD1306_DATA

#include "stdio.h"
#include "stdint.h"

typedef struct stBitmapInfo {
    uint8_t width;
    uint8_t height;
    const unsigned char *bitmap_data;
} stBitmapInfo;

extern struct stBitmapInfo st_VietNam;
extern struct stBitmapInfo st_FlappyBird;

// Image VietNam map 80x60 px
// display.drawBitmap(0, 0, myBitmap[0], 60, 80, WHITE); // height = 60px, width = 80px
extern const unsigned char VietNam_map[];
extern const unsigned char flappyBird_bitmap[];
extern const unsigned char column_bitmap[];
extern const unsigned char column_simple[];
#endif  // SSD1306_DATA