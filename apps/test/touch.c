#include "touch.h"

#include <ch.h>
#include <hal.h>

static const SPIConfig touchSpiConf = {
    IOPORT2,
    7,
    (6<<3), // Baud rate Fpclk/128
//    0,
//    0,
};

void touchInit(void)
{
    spiStart(&SPID1, &touchSpiConf);
}

#define CHX 0x90
#define CHY 0xD0

const uint8_t touchRequest[] =
    { CHX, 0, CHX, 0, CHY, 0, CHY, 0, 0 };

int touchGetXY(int* x, int* y)
{
    uint8_t touchResult[sizeof(touchRequest)];
    spiSelect(&SPID1);
    spiExchange(&SPID1, sizeof(touchRequest), touchRequest, touchResult);
    spiUnselect(&SPID1);
    *x = ((touchResult[1]<<8 | touchResult[2]) + (touchResult[3]<<8 | touchResult[4]))/2;
    *y = ((touchResult[5]<<8 | touchResult[6]) + (touchResult[7]<<8 | touchResult[8]))/2;
    return 0;
}

