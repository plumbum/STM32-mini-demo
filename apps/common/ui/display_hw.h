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
#define DISPLAY_WIDTH   320
#define DISPLAY_HEIGHT  240

#define displayGetRgbColor(r, g, b) ((display_color_t)((((r)&0xF8)<<8) | (((g)&0xFC)<<3) | (b>>3)))

#endif /* _DISPLAY_HW_H_ */

