#include "display_hw_routine.h"

#include <ch.h>
#include <hal.h>
#include <inttypes.h>
#include <stdlib.h>

static void _mcuInit(void);

static uint16_t _lcdReadStatus(void);
static void _lcdWriteCmd(uint16_t cmd);

static uint16_t _lcdReadData(void);
static void _lcdWriteData(uint16_t data);

static void _lcdWriteReg(uint16_t reg, uint16_t data);
static uint16_t _lcdReadReg(uint16_t reg);

static void dispInitILI9325(void);

display_state_t dispInit(void)
{
    GPIOE->BSRR = (1<<1); // PE1
    chThdSleepMilliseconds(10);
    GPIOE->BRR = (1<<1); // reset PE1
    _mcuInit();
    chThdSleepMilliseconds(50);
    GPIOE->BSRR = (1<<1); // PE1
    chThdSleepMilliseconds(50);

    int deviceCode = _lcdReadReg(0);

    if((deviceCode == 0xFFFF) || (deviceCode == 0x0000))
        return dsNoDevice;

    if((deviceCode == 0x9325) || (deviceCode == 0x9328)) /* ILI9325 */
    {
        dispInitILI9325();
    }
    else
        return dsUnknowDevice;

    return dsOk;
}

void dispBox(display_coord_t x1, display_coord_t y1, display_coord_t x2, display_coord_t y2,
        display_color_t color)
{
    display_coord_t x, y;
    _lcdWriteReg(0x50, x1); // Horizontal frame bounds
    _lcdWriteReg(0x51, x2);
    _lcdWriteReg(0x52, y1); // Vertical frame bounds
    _lcdWriteReg(0x53, y2);
    _lcdWriteReg(0x20, x1); // Cursor
    _lcdWriteReg(0x21, y1);
    _lcdWriteCmd(0x22); // Write datas
    for(y=y1; y<=y2; y++)
        for(x=x1; x<=x2; x++)
            _lcdWriteData(color);
}

void dispFillRect(display_coord_t x1, display_coord_t y1, display_coord_t x2, display_coord_t y2,
        display_coord_t border, display_color_t rect_color, display_color_t fill_color)
{
    display_coord_t x, y;
    _lcdWriteReg(0x50, x1); // Horizontal frame bounds
    _lcdWriteReg(0x51, x2);
    _lcdWriteReg(0x52, y1); // Vertical frame bounds
    _lcdWriteReg(0x53, y2);
    _lcdWriteReg(0x20, x1); // Cursor
    _lcdWriteReg(0x21, y1);
    _lcdWriteCmd(0x22); // Write datas
    for(y=y1; y<=y2; y++)
        for(x=x1; x<=x2; x++)
        {
            if((x<(x1+border)) ||
               (x>(x2-border)) ||
               (y<(y1+border)) ||
               (y>(y2-border)))
                _lcdWriteData(rect_color);
            else
                _lcdWriteData(fill_color);
        }
}

void dispPixel(display_coord_t x, display_coord_t y, display_color_t color)
{
    _lcdWriteReg(0x20, x); // Cursor
    _lcdWriteReg(0x21, y);
    _lcdWriteReg(0x22, color); // Write data
}

void dispLine(display_coord_t x1, display_coord_t y1, display_coord_t x2, display_coord_t y2,
        display_color_t color)
{
    display_coord_t dx, dy;
    dx = abs(x2-x1);
    dy = abs(y2-y1);
    if((dx == 0) && (dy == 0)) // Point
        dispPixel(x1, y1, color);
    else if(dy == 0) // Horizontal
    {
        display_coord_t x;
        for(x=x1; x<=x2; x++)
            dispPixel(x, y1, color);
    }
    else if(dx == 0) // Vertical
    {
        display_coord_t y;
        for(y=y1; y<=y2; y++)
            dispPixel(x1, y, color); // TODO Optimize it!
    }
    else if(dy < dx) // Horizontal longest
    {
        if(x2 < x1)
        { // Swap coordinates
            display_coord_t t;
            t = x1; x1 = x2; x2 = t;
            t = y1; y1 = y2; y2 = t;
        }
        display_coord_t x, y, sy;
        if(y1 < y2) sy = 1; else sy = -1;
        display_coord_t e = 0;
        y = y1;
        for(x=x1; x<=x2; x++)
        {
            dispPixel(x, y, color);
            e += dy;
            if(2*e >= dx)
            {
                y += sy;
                e -= dx;
            }
        }
    }
    else // Vertical longest
    {
        if(y2 < y1)
        { // Swap coordinates
            display_coord_t t;
            t = x1; x1 = x2; x2 = t;
            t = y1; y1 = y2; y2 = t;
        }
        display_coord_t x, y, sx;
        if(x1 < x2) sx = 1; else sx = -1;
        display_coord_t e = 0;
        x = x1;
        for(y=y1; y<=y2; y++)
        {
            dispPixel(x, y, color);
            e += dx;
            if(2*e >= dy)
            {
                x += sx;
                e -= dy;
            }
        }
    }
}

/*
 * Internal routines
 */
inline static void _mcuInit()
{
    RCC->AHBENR |= RCC_AHBENR_FSMCEN;
    //FSMC_Bank1->BTCR[0] = 0;
    //FSMC_Bank1E->BWTR[0] = 0;

    // Register clear
    // Bank1 have NE1 ~ 4, each has a BCR + TCR, so a total of eight registers.
    // Here we use the NE2, also corresponds to BTCR [6], [7].
    FSMC_Bank1->BTCR[2] = 0x00000000;
    FSMC_Bank1->BTCR[3] = 0x00000000;
    FSMC_Bank1E->BWTR[2] = 0x00000000;
    // Register to use asynchronous mode of operation of BCR
    FSMC_Bank1->BTCR[2] |= FSMC_BCR1_WREN; // memory write enable
    FSMC_Bank1->BTCR[2] |= FSMC_BCR1_MWID_0; // memory data width is 16bit
    // Operation BTR register
    FSMC_Bank1->BTCR[3] |= 1<<9; // data retention time for 3 HCLK
    // Flash Write Timing Register
    FSMC_Bank1E->BWTR[2] = 0x0FFFFFFF; // default values
    // Enable BANK4 (PC card Device)
    FSMC_Bank1->BTCR[2] |= FSMC_BCR1_MBKEN;

}

static void dispInitILI9325(void)
{
	_lcdWriteReg(0x00E3, 0x3008); // Set internal timing
	_lcdWriteReg(0x00E7, 0x0012); // Set internal timing
	_lcdWriteReg(0x00EF, 0x1231); // Set internal timing
	_lcdWriteReg(0x0000, 0x0001); // Start Oscillation
	_lcdWriteReg(0x0001, 0x0100); // set SS and SM bit
	_lcdWriteReg(0x0002, 0x0700); // set 1 line inversion

	_lcdWriteReg(0x0003, 0x1018); // set GRAM write direction and BGR=0,262K colors,1 transfers/pixel.
	_lcdWriteReg(0x0004, 0x0000); // Resize register
	_lcdWriteReg(0x0008, 0x0202); // set the back porch and front porch
	_lcdWriteReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
	_lcdWriteReg(0x000A, 0x0000); // FMARK function
	_lcdWriteReg(0x000C, 0x0000); // RGB interface setting
	_lcdWriteReg(0x000D, 0x0000); // Frame marker Position
	_lcdWriteReg(0x000F, 0x0000); // RGB interface polarity
//Power On sequence 
	_lcdWriteReg(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	_lcdWriteReg(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	_lcdWriteReg(0x0012, 0x0000); // VREG1OUT voltage
	_lcdWriteReg(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
	chThdSleepMilliseconds(200); // Dis-charge capacitor power voltage
	_lcdWriteReg(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
	_lcdWriteReg(0x0011, 0x0227); // R11h=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]
	chThdSleepMilliseconds(50); // delay 50ms
	_lcdWriteReg(0x0012, 0x001C); // External reference voltage= Vci;
	chThdSleepMilliseconds(50); // delay 50ms
	_lcdWriteReg(0x0013, 0x1800); // R13=1200 when R12=009D;VDV[4:0] for VCOM amplitude
	_lcdWriteReg(0x0029, 0x001C); // R29=000C when R12=009D;VCM[5:0] for VCOMH
	_lcdWriteReg(0x002B, 0x000D); // Frame Rate = 91Hz
	chThdSleepMilliseconds(50); // delay 50ms
	_lcdWriteReg(0x0020, 0x0000); // GRAM horizontal Address
	_lcdWriteReg(0x0021, 0x0000); // GRAM Vertical Address
// ----------- Adjust the Gamma Curve ----------//
	_lcdWriteReg(0x0030, 0x0007);
	_lcdWriteReg(0x0031, 0x0302);
	_lcdWriteReg(0x0032, 0x0105);
	_lcdWriteReg(0x0035, 0x0206);
	_lcdWriteReg(0x0036, 0x0808);
	_lcdWriteReg(0x0037, 0x0206);
	_lcdWriteReg(0x0038, 0x0504);
	_lcdWriteReg(0x0039, 0x0007);
	_lcdWriteReg(0x003C, 0x0105);
	_lcdWriteReg(0x003D, 0x0808);
//------------------ Set GRAM area ---------------//
	_lcdWriteReg(0x0050, 0x0000); // Horizontal GRAM Start Address
	_lcdWriteReg(0x0051, 0x00EF); // Horizontal GRAM End Address
	_lcdWriteReg(0x0052, 0x0000); // Vertical GRAM Start Address
	_lcdWriteReg(0x0053, 0x013F); // Vertical GRAM Start Address
	_lcdWriteReg(0x0060, 0xA700); // Gate Scan Line
	_lcdWriteReg(0x0061, 0x0001); // NDL,VLE, REV
	_lcdWriteReg(0x006A, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
	_lcdWriteReg(0x0080, 0x0000);
	_lcdWriteReg(0x0081, 0x0000);
	_lcdWriteReg(0x0082, 0x0000);
	_lcdWriteReg(0x0083, 0x0000);
	_lcdWriteReg(0x0084, 0x0000);
	_lcdWriteReg(0x0085, 0x0000);
//-------------- Panel Control -------------------//
	_lcdWriteReg(0x0090, 0x0010);
	_lcdWriteReg(0x0092, 0x0000);
	_lcdWriteReg(0x0093, 0x0003);
	_lcdWriteReg(0x0095, 0x0110);
	_lcdWriteReg(0x0097, 0x0000);
	_lcdWriteReg(0x0098, 0x0000);
	_lcdWriteReg(0x0007, 0x0133); // 262K color and display ON
}

#define Bank1_LCD_D    ((uint32_t)0x60020000)    //display Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //display Reg ADDR

static uint16_t _lcdReadStatus(void)
{
    return *(__IO uint16_t *) (Bank1_LCD_C);
}

static void _lcdWriteCmd(uint16_t cmd)
{
    *(__IO uint16_t *) (Bank1_LCD_C) = cmd;	
}

static uint16_t _lcdReadData(void)
{
    return *(__IO uint16_t *) (Bank1_LCD_D);
}

static void _lcdWriteData(uint16_t data)
{
    *(__IO uint16_t *) (Bank1_LCD_D) = data;	
}

static void _lcdWriteReg(uint16_t reg, uint16_t data)
{
    *(__IO uint16_t *) (Bank1_LCD_C) = reg;	
    *(__IO uint16_t *) (Bank1_LCD_D) = data;	
}

static uint16_t _lcdReadReg(uint16_t reg)
{
    *(__IO uint16_t *) (Bank1_LCD_C) = reg;	
    return *(__IO uint16_t *) (Bank1_LCD_D);
}


