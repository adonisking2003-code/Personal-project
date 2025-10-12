#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

int seven_segment_init(void);
void seven_segment_display_digit(int number);
void seven_segment_display_number(int number);
void seven_segment_clear(void);
int seven_segment_close(void);

#endif // SEVEN_SEGMENT