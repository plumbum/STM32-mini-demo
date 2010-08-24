#ifndef _DISPLAY_HW_ROUTINE_H_
#define _DISPLAY_HW_ROUTINE_H_

#include "display.h"

display_state_t dispInit(void);

void dispBox(display_coord_t x1, display_coord_t y1, display_coord_t x2, display_coord_t y2,
        display_color_t color);

void dispFillRect(display_coord_t x1, display_coord_t y1, display_coord_t x2, display_coord_t y2,
        display_coord_t border, display_color_t rect_color, display_color_t fill_color);

void dispPixel(display_coord_t x, display_coord_t y, display_color_t color);

void dispLine(display_coord_t x1, display_coord_t y1, display_coord_t x2, display_coord_t y2,
        display_color_t color);

#endif /* _DISPLAY_HW_ROUTINE_H_ */

