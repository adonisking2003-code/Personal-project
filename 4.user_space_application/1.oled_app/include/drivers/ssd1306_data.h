#ifndef SSD1306_DATA
#define SSD1306_DATA

#include "stdio.h"
#include "stdint.h"

// Image VietNam map 80x60 px
// display.drawBitmap(0, 0, myBitmap[0], 60, 80, WHITE); // height = 60px, width = 80px
extern const unsigned char VietNam_map[];
extern const unsigned char flappyBird_bitmap[];
extern const unsigned char column_bitmap[];
#endif  // SSD1306_DATA