#include <ch.h>
#include <hal.h>

#include "terminal.h"
#include "touch.h"

#include "display_hw_routine.h"

/*
 * Blue LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThdLed, 128);
static msg_t ThdLed(void *arg)
{
    (void)arg;
    for(;;)
    {
        LED_ON();
        chThdSleepMilliseconds(500);
        LED_OFF();
        chThdSleepMilliseconds(500);
    }
    return 0;
}

unsigned int tParse(const char* ibuf, int lbuf, char** errorMsg)
{
    (void)ibuf;
    (void)lbuf;
    *errorMsg = 0;

    int x, y;
    touchGetXY(&x, &y);

    tPuts("--- Touch (");
    tPutHex(x, 4);
    tPuts(",");
    tPutHex(y, 4);
    tPuts(")" EOLN);

    return 200;
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    chThdCreateStatic(waThdLed, sizeof(waThdLed), NORMALPRIO, ThdLed, NULL);
    touchInit();
    tInit(1);

    dispInit();
    /*
    int st = dispGetStatus();
    tPuts("--- Disp status: ");
    tPutHex(st, 4);
    tPuts(EOLN);
    */

    for (;;)
    {
        /*
        if (palReadPad(IOPORT1, GPIOA_BUTTON))
            TestThread(&SD2);
        */
        chThdSleepMilliseconds(500);
    }
    return 0;
}

