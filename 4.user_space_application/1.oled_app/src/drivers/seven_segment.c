#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const unsigned char digit_patterns[10] = {
    0x3F,   // 0
    0x06,   // 1
    0x5B,   // 2
    0x4F,   // 3
    0x66,   // 4
    0x6D,   // 5
    0x7D,   // 6
    0x07,   // 7
    0x7F,   // 8
    0x6F,   // 9
};

int seven_segment_init(void)
{
    // Initialize GPIO pins for 7-segments output here
    // Return 0 if success, -1 on error

    return 0;
}

void seven_segment_display_digit(int number)
{
    if( number < 0 || number > 9 ) return;  // Invalid digit
    unsigned char pattern = digit_patterns[number];
    // Write pattern bits to GPIO pins controlling segments

}

void seven_segment_display_number(int number)
{
    // For multi-digit 7-seg hardware, implement digit multiplexing here
    // For simple single digit
    seven_segment_display_digit(number%10);
}

void seven_segment_clear(void)
{
    // Turn off all segment GPIOs
}

int seven_segment_close(void)
{
    // Clean up GPIO if necessary
    return 0;
}