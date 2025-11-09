#ifndef UTILS_H
#define UTILS_H

/* definition declare*/
#define PRINTF_DEBUG(...)    printf("[DEBUG]: " __VA_ARGS__);
#define PRINTF_ERROR(...)    printf("[ERROR]: " __VA_ARGS__);
#define PRINTF_INFO(...)     printf("[INFO]: " __VA_ARGS__);
/* variable declare*/

/* function declare*/
int clamp(int value, int min, int max);
unsigned int random_unit(unsigned int max);
void delay_ms(unsigned int milliseconds);

#endif // UTILS_H