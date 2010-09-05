#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "display_hw.h"

/**
 * Display state codes
 */
typedef enum {
    dsOk = 0,               /**< Display inited sussefully */
    dsFail = -1,            /**< Abstract error */
    dsNoDevice = -2,        /**< Not found display on line */
    dsUnknowDevice = -3,    /**< Unsupported display module (unknow device code readed) */
} display_state_t;

/**
 * Library features flags definition
 */
typedef struct {
    int has_text:1;         /**< Library support text printing */
    int has_graph:1;        /**< Library support graphical primitives */
} display_features_t;

/**
 * Display control flags definition
 */
typedef struct {
    int pos_inc:1;  /**< Automatic cursor position increment */
    int wrap:1;     /**< Automatic line wrap (actually if pos_inc enabled */
    int scroll:1;   /**< Automatic screen scroll after wrap latest line */
    int text_transparent:1;     /**< Print text witch transparent background */
} display_control_t;

/*
 * Display control routines
 */

/**
 * @brief Init display hardware
 * @details
 * @return initial routine state
 */
display_state_t displayInit(void);

/**
 * @brief Power on display after displayOff
 * @see displayOff
 */
void displayOn(void);

/**
 * @brief Power off display
 * @see displayOn
 */
void displayOff(void);

/**
 * @brief Clear display
 * @details Routine fill display with current canvas color
 * @see displaySetCanvasColor
 */
void displayClear(void);

/*
 * Display color routines
 */

/**
 * @brief Set current drawing pen color
 * @param color - new pen color
 * @see displayGetRgbColor
 * @see displayGetPenColor
 * @see displaySetCanvasColor
 */
void displaySetPenColor(display_coord_t color);

/**
 * @brief Get current pen color
 * @return current pen color
 * @see displaySetPenColor
 */
display_coord_t displayGetPenColor(void);

/**
 * @brief Set canvas color
 * @param color - new canvas color
 * @see displayGetRgbColor
 * @see displayGetCanvasColor
 * @see displaySetPenColor
 */
void displaySetCanvasColor(display_coord_t color);

/**
 * @brief Get current canvas color
 * @return current canvas color
 * @see displaySetCanvasColor
 */
display_coord_t displayGetCanvasColor(void);

/*
 * Graphical routines
 */

/**
 * @brief Draw filled rect (box) with current pen color
 * @param x1 - left corner coordinate
 * @param y1 - top corner coordinate
 * @param x2 - right corner coordinate
 * @param y2 - bottom corner coordinate
 * @see displaySetPenColor
 */
void displayBox(display_coord_t x1, display_coord_t y1, 
                display_coord_t x2, display_coord_t y2);

/**
 * @brief Draw rect with current pen color
 *          filled with current canvas color
 * @param x1 - left corner coordinate
 * @param y1 - top corner coordinate
 * @param x2 - right corner coordinate
 * @param y2 - bottom corner coordinate
 * @param border - rect border width
 * @see displaySetPenColor
 * @see displaySetCanvasColor
 */
void displayFillRect(display_coord_t x1, display_coord_t y1, 
                     display_coord_t x2, display_coord_t y2, display_coord_t border);

/**
 * @brief Draw one point with current pen color
 * @param x - point coordinate
 * @param y - point coordinate
 * @see displaySetPenColor
 */
void displayPlot(display_coord_t x, display_coord_t y);

/**
 * @brief Draw line with current pen color
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @see displaySetPenColor
 */
void displayLine(display_coord_t x1, display_coord_t y1, 
                 display_coord_t x2, display_coord_t y2);


/**
 * @brief Draw ring
 * @param x
 * @param y
 * @param radius
 * @see displaySetPenColor
 */
void displayCircle(display_coord_t x, display_coord_t y,
                    display_coord_t radius);

/**
 * @brief Draw filled circle
 * @param x
 * @param y
 * @param radius
 * @see displaySetPenColor
 */
void displayFillCircle(display_coord_t x, display_coord_t y,
                    display_coord_t radius);


/**
 * @brief Draw text at specified coordinates
 * @param x
 * @param y
 * @param text
 * @see displaySetFont
 */
void displayTextXY(display_coord_t x, display_coord_t y,
                    char* text);

/*
 * Text routines
 */

/**
 * @brief Get maximal row number
 * @return Maximal row number
 * @see displayGetMaxCol
 */
display_coord_t displayGetMaxRow(void);

/**
 * @brief Get maximal col number
 * @return Maximal col number
 * @see displayGetMaxRow
 */
display_coord_t displayGetMaxCol(void);

/**
 * @brief Set new current cursor coordinates
 * @param row - coordinate. Must be between 1 and displayGetMaxRow
 * @param col - coordinate. Must be between 1 and displayGetMaxCol
 * @see displayGetMaxCol
 * @see displayGetMaxRow
 */
void displaySetTextPos(display_coord_t row, display_coord_t col);

/**
 * @brief Get current row number
 * @return Current row number
 */
display_coord_t displayGetRow(void);

/**
 * @brief Get current col number
 * @return Current col number
 */
display_coord_t displayGetCol(void);

void displaySetFont(int fn);

int displayGetFont(void);

void displayPutchar(char c);

void displayPuts(const char* c);

/*
 * Verbose color names
 */
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

#endif /* _DISPLAY_H_ */

