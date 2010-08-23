#include "display.h"
#include "display_hw_routine.h"


display_state_t displayInit(void)
{
    dispInit();
    return dsOk;
}

void displayOn(void)
{
}

void displayOff(void)
{
}

void displayClear(display_color_t color)
{
}

void displayBox(
    display_coord_t x1, display_coord_t y1, 
    display_coord_t x2, display_coord_t y2, display_color_t color)
{
}

