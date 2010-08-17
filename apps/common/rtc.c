#include "rtc.h"

#include <ch.h>
#include <hal.h>
#include "board.h"

#include <nvic.h>
//#include "stm32f10x_nvic.h"

EventSource rtcEvtSecond;
EventSource rtcEvtAlarm;

/* RTC vector handler */
CH_IRQ_HANDLER(Vector4C)
{
    if(RTC->CRL & RTC_CRL_OWF)
    {
        RTC->CRL &= ~RTC_CRL_OWF;
        // TODO overflow GPIOB->BSRR = GPIOB_LED1;
    }
    if(RTC->CRL & RTC_CRL_ALRF)
    {
        RTC->CRL &= ~RTC_CRL_ALRF;
        chEvtBroadcastI(&rtcEvtAlarm);
    }
    if(RTC->CRL & RTC_CRL_SECF)
    {
        RTC->CRL &= ~RTC_CRL_SECF;
        chEvtBroadcastI(&rtcEvtSecond);
    }
}


void rtcInit(uint32_t time)
{
    RCC->APB1ENR |= RCC_APB1ENR_BKPEN | RCC_APB1ENR_PWREN; // Enable PWR and RTC perif
    PWR->CR |= PWR_CR_DBP; // enable access to RTC, BDC registers

#ifdef EXT32768
    RCC->BDCR |= RCC_BDCR_LSEON;
    while( (RCC->BDCR & RCC_BDCR_LSERDY) == 0 ) ; // Wait for LSERDY = 1 (LSE is ready)
#else
    RCC->CSR |= RCC_CSR_LSION;
    while( (RCC->CSR & RCC_CSR_LSIRDY) == 0 ) ; // Wait for LSIRDY = 1 (iternal 40khz rtc oscillator ready)
#endif

    RCC->BDCR |= RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_0;
    while( (RTC->CRL & RTC_CRL_RTOFF) == 0 ) ; // Poll RTOFF, wait until its value goes to ‘1’

    RTC->CRL |= RTC_CRL_CNF;
    RTC->PRLH = 0;
    RTC->PRLL = 32768-1;
    if(time)
    {
        RTC->CNTL = time & 0xFFFF;
        RTC->CNTH = time >> 16;
        RTC->ALRL = 0xFFFF;
        RTC->ALRH = 0xFFFF;
    }

    RTC->CRH = RTC_CRH_SECIE;

    RTC->CRL &= ~RTC_CRL_CNF;
    while( (RTC->CRL & RTC_CRL_RTOFF) == 0 ) ; // Poll RTOFF, wait until its value goes to ‘1’ to check the end of the write operation.

    PWR->CR &= ~PWR_CR_DBP; // disable access to RTC registers

    // Init OS depended functions
    chEvtInit(&rtcEvtSecond);
    chEvtInit(&rtcEvtAlarm);

    // Enable interrupt
    NVICEnableVector(RTC_IRQn, 0xC0);
}

void rtcSetCounter(int counter)
{
    PWR->CR |= PWR_CR_DBP; // enable access to RTC, BDC registers
    RTC->CRL &= ~RTC_CRL_RSF; // Wait syncronization
    while( (RTC->CRL & RTC_CRL_RSF) == 0 ) ;

    GPIOB->BSRR = (1<<14);

    RTC->CRL |= RTC_CRL_CNF; // Enter configuration mode
    RTC->CNTH = counter>>16;
    RTC->CNTL = counter&0xFFFF;
    RTC->CRL &= ~RTC_CRL_CNF; // Leave configuration mode
    // TODO dirty hack while( (RTC->CRL & RTC_CRL_RTOFF) == 0 ) ; // Poll RTOFF, wait until its value goes to ‘1’ to check the end of the write operation.
    PWR->CR &= ~PWR_CR_DBP; // disable access to RTC registers
}

int  rtcGetCounter(void)
{
    RTC->CRL &= ~RTC_CRL_RSF; // Wait syncronization
    while( (RTC->CRL & RTC_CRL_RSF) == 0 ) ;

    int r = RTC->CNTL;
    r |= RTC->CNTH<<16;
    return r;
}

