#ifndef _RTC_H_
#define _RTC_H_

#include <ch.h>

#define EXT32768 1

extern EventSource rtcEvtSecond;
extern EventSource rtcEvtAlarm;

void rtcInit(uint32_t time);
void rtcSetCounter(int counter);
int  rtcGetCounter(void);

#endif /* _RTC_H_ */

