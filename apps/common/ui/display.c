#include "display.h"
#include "display_hw_routine.h"

static display_color_t fgcolor;
static display_color_t bgcolor;

display_state_t displayInit(void)
{
    dispInit();
    fgcolor = BLACK;
    bgcolor = WHITE;
    displayClear();
    return dsOk;
}

void displayOn(void)
{
    // TODO write displayOn code
}

void displayOff(void)
{
    // TODO write displayOff code
}

void displaySetPenColor(display_coord_t color)
{
    fgcolor = color;
}

display_coord_t displayGetPenColor(void)
{
    return fgcolor;
}

void displaySetCanvasColor(display_coord_t color)
{
    bgcolor = color;
}

display_coord_t displayGetCanvasColor(void)
{
    return bgcolor;
}

void displayClear(void)
{
    dispBox(DISPLAY_LEFT, DISPLAY_TOP,
            DISPLAY_LEFT+DISPLAY_WIDTH-1, DISPLAY_TOP+DISPLAY_HEIGHT-1, bgcolor);
}

void displayBox(display_coord_t x1, display_coord_t y1, 
                display_coord_t x2, display_coord_t y2)
{
    dispBox(x1, y1, x2, y2, bgcolor);
}

void displayFillRect(display_coord_t x1, display_coord_t y1, 
                     display_coord_t x2, display_coord_t y2, display_coord_t border)
{
    dispFillRect(x1, y1, x2, y2, border, fgcolor, bgcolor);
}

void displayPlot(display_coord_t x, display_coord_t y)
{
    dispPixel(x, y, fgcolor);
}

void displayLine(display_coord_t x1, display_coord_t y1, 
                 display_coord_t x2, display_coord_t y2)
{
    dispLine(x1, y1, x2, y2, fgcolor);
}

void displayTextXY(display_coord_t x, display_coord_t y,
                    char* text)
{
}

void displaySetTextPos(display_coord_t row, display_coord_t col)
{
}

display_coord_t displayGetRow(void)
{
}

display_coord_t displayGetCol(void)
{
}


display_coord_t displayGetMaxRow(void)
{
}

display_coord_t displayGetMaxCol(void)
{
}


void displaySetFont(int fn)
{
}


int displayGetFont(void)
{
}


void displayPutchar(char c)
{
}


void displayPuts(const char* c)
{
}


