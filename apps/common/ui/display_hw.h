/**
 * @brief 240x320 LCD hardware specified header
 *
 * @author Ivan A-R <ivan@tuxotronic.org>
 */

#ifndef _DISPLAY_HW_H_
#define _DISPLAY_HW_H_

#include <inttypes.h>

typedef uint16_t display_color_t;
typedef int16_t display_coord_t;

#define DISPLAY_LEFT    0
#define DISPLAY_TOP     0
#define DISPLAY_WIDTH   240
#define DISPLAY_HEIGHT  320

#define displayGetRgbColor(r, g, b) ((display_color_t)((((r)&0xF8)<<8) | (((g)&0xFC)<<3) | (b>>3)))

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

#endif /* _DISPLAY_HW_H_ */

