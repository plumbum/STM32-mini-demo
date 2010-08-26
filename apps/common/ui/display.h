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


#define BLACK   displayGetRgbColor(0x00, 0x00, 0x00)
#define GRAY    displayGetRgbColor(0x99, 0x99, 0x99)
#define SILVER  displayGetRgbColor(0xCC, 0xCC, 0xCC)
#define WHITE   displayGetRgbColor(0xFF, 0xFF, 0xFF)
#define MAROON  displayGetRgbColor(0x99, 0x00, 0x00)
#define RED     displayGetRgbColor(0xFF, 0x00, 0x00)
#define PURPLE  displayGetRgbColor(0x99, 0x00, 0x99)
#define FUCHSIA displayGetRgbColor(0xFF, 0x00, 0xFF)
#define GREEN   displayGetRgbColor(0x00, 0x99, 0x00)
#define LIME    displayGetRgbColor(0x00, 0xFF, 0x00)
#define OLIVE   displayGetRgbColor(0x99, 0x99, 0x00)
#define YELLOW  displayGetRgbColor(0xFF, 0xFF, 0x00)
#define NAVY    displayGetRgbColor(0x00, 0x00, 0x99)
#define BLUE    displayGetRgbColor(0x00, 0x00, 0xFF)
#define TEAL    displayGetRgbColor(0x00, 0x99, 0x99)
#define AQUA    displayGetRgbColor(0x00, 0xff, 0xff)


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

