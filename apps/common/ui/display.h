#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "display_hw.h"

typedef enum {
    dsOk = 0,
    dsFail = -1,
    dsNoDevice = -2,
    dsUnknowDevice = -3,
} display_state_t;

typedef struct {
    int hastext:1;
    int hasgraph:1;
} display_features_t;

display_state_t displayInit(void);

void displayOn(void);
void displayOff(void);

void displaySetPenColor(display_coord_t color);
display_coord_t displayGetPenColor(void);
void displaySetCanvasColor(display_coord_t color);
display_coord_t displayGetCanvasColor(void);

void displayClear(void);

void displayBox(display_coord_t x1, display_coord_t y1, 
                display_coord_t x2, display_coord_t y2);

void displayFillRect(display_coord_t x1, display_coord_t y1, 
                     display_coord_t x2, display_coord_t y2, display_coord_t border);

void displayPlot(display_coord_t x, display_coord_t y);

void displayLine(display_coord_t x1, display_coord_t y1, 
                 display_coord_t x2, display_coord_t y2);

#endif /* _DISPLAY_H_ */

