#include <stdlib.h>
#include <unistd.h>
#include <utils.h>

/*
    * Purpose: Limits value to be within the range [min, max].
*/
int clamp(int value, int min, int max)
{
    if( value < min ) return min;
    else if( value > max ) return max;
    else return value;
}

/*
    * Purpose: Returns a random unsigned integer between 0 and max-1 .
*/
unsigned int random_unit(unsigned int max)
{
    return rand() % max;
}

/*
    * Purpose: Pause execution for the given milliseconds.
*/
void delay_ms(unsigned int milliseconds)
{
    usleep(milliseconds * 1000);
}