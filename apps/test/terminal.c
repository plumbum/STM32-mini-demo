#include "terminal.h"

#include <stdlib.h>
#include <string.h>

#include "utils.h"


/*
 * Use Serial1 as terminal port
 */
#define TSD SD1

#define TERM_INPUT_BUFFER 256

static const char cszHello[] = EOLN EOLN "Build date: " __DATE__ " " __TIME__ EOLN EOLN;
static const char cszPrompt[] = "> ";

static int tEcho;
static MUTEX_DECL(mtxTerminalLock);

static void _tPuts(const char* str)
{
    int len = strlen(str);
    if(len)
        sdWrite(&TSD, (const uint8_t*)str, len);
}

static void _tPutLn(void)
{
    sdWrite(&TSD, (const uint8_t*)EOLN, sizeof(EOLN));
}

static void _tPutc(char c)
{
    sdPut(&TSD, c);
}

void tPuts(const char* str)
{
    chMtxLock(&mtxTerminalLock);
    _tPuts(str);
    chMtxUnlock();
}

void tPutsLn(const char* str)
{
    chMtxLock(&mtxTerminalLock);
    _tPuts(str);
    _tPutLn();
    chMtxUnlock();
}

static void _tPutui(uint32_t val)
{
    uint32_t ipow;
    uint32_t power;
    char atmp;
    char prefix_flag = 0;
    for(ipow=0; ipow<(sizeof(power10)/sizeof(power10[0])); ipow++)
    {
        atmp = '0';
        power = power10[ipow];
        while(val >= power)
        {
            atmp++;
            val -= power;
        }
        if(atmp > '0')
            prefix_flag = -1;
        if(prefix_flag)
            sdPut(&TSD, atmp);
    }
    sdPut(&TSD, val+'0');
}

void tPutui(uint32_t val)
{
    chMtxLock(&mtxTerminalLock);
    _tPutui(val);
    chMtxUnlock();
}

static void _tPuti(int32_t val)
{
    if(val<0)
    {
        sdPut(&TSD, '-');
        val = -val;
    }
    tPutui(val);
}

void tPuti(int32_t val)
{
    chMtxLock(&mtxTerminalLock);
    _tPuti(val);
    chMtxUnlock();
}

static char _encHex(uint8_t v)
{
    if(v<10)
        return '0'+v;
    else
        return 'A'+v-10;
}

static void _tPutHex(uint32_t val, int digits)
{
    if(digits >= 8) sdPut(&TSD, _encHex((val>>28) & 0x0F));
    if(digits >= 7) sdPut(&TSD, _encHex((val>>24) & 0x0F));
    if(digits >= 6) sdPut(&TSD, _encHex((val>>20) & 0x0F));
    if(digits >= 5) sdPut(&TSD, _encHex((val>>16) & 0x0F));
    if(digits >= 4) sdPut(&TSD, _encHex((val>>12) & 0x0F));
    if(digits >= 3) sdPut(&TSD, _encHex((val>>8) & 0x0F));
    if(digits >= 2) sdPut(&TSD, _encHex((val>>4) & 0x0F));
    sdPut(&TSD, _encHex((val>>0) & 0x0F));
}


void tPutHex(uint32_t val, int digits)
{
    chMtxLock(&mtxTerminalLock);
    _tPutHex(val, digits);
    chMtxUnlock();
}

static char backBuf[TERM_INPUT_BUFFER];

int tGetLn(char* str, int maxlen)
{
    int i = 0;
    char c;

    while(i < maxlen-1)
    {
        c = sdGet(&TSD);
        if((c == '\b') || (c == 0x7F))
        {
            if(i>0)
            {
                str[--i] = 0;
                if(tEcho)
                    tPuts("\b \b");
            }
            else
                if(tEcho)
                    tPuts("\a");
        }
        else if((c == '\n') || (c == '\r'))
        {
            if(tEcho)
                tPuts(EOLN);
            break;
        }
        else if(c == 0x03) // Cancel
        {
            tPuts(EOLN);
            i = 0;
            break;
        }
        else if(c == 0x10) // Copy from backBuf
        {
            if(tEcho && (backBuf[0] != 0))
            {
                strcpy(str, backBuf);
                i = strlen(str);
                chMtxLock(&mtxTerminalLock);
                _tPutLn();
                _tPuts(cszPrompt);
                _tPuts(str);
                chMtxUnlock();
            }
        }
        else if(c == 0x01) // Clean to home
        {
            if(tEcho)
            {
                chMtxLock(&mtxTerminalLock);
                while(i--) _tPuts("\b \b");
                chMtxUnlock();
                i = 0;
                str[0] = 0;
            }
        }
        else if(c >= ' ')
        {
            str[i++] = c;
            if(tEcho)
            {
                chMtxLock(&mtxTerminalLock);
                _tPutc(c);
                chMtxUnlock();
            }
        }
        else
        {
            if(tEcho)
            {
                chMtxLock(&mtxTerminalLock);
                _tPutc('\r');
                _tPutHex(c, 4);
                _tPutLn();
                _tPuts(cszPrompt);
                str[i] = 0;
                _tPuts(str);
                chMtxUnlock();
            }
        }
        c = 0;
    }
    str[i] = 0;
    if(str[0] != 0)
        strcpy(backBuf, str);
    return i;
}

static char ibuf[TERM_INPUT_BUFFER];

static WORKING_AREA(waThdTerminal, 1024);
static msg_t ThdTerminal(void *arg)
{
    (void)arg;
    int lbuf;

    tPuts(cszHello);

    backBuf[0] = 0;

    for(;;)
    {
        if(tEcho)
            tPuts(cszPrompt);
        
        lbuf = tGetLn(ibuf, TERM_INPUT_BUFFER);
        if((lbuf>0) && (lbuf<TERM_INPUT_BUFFER-1))
        {
            char* errorMsg = 0;
            unsigned int response = tParse(ibuf, lbuf, &errorMsg);
            if(response)
            {
                chMtxLock(&mtxTerminalLock);
                _tPutui(response);
                _tPutc(' ');
                if(errorMsg)
                    _tPuts(errorMsg);
                else
                    switch(response)
                    {
                        case 200: _tPuts("OK"); break;
                        case 400: _tPuts("COMMAND NOT FOUND"); break;
                        case 500: _tPuts("INTERNAL ERROR"); break;
                    }
                _tPutLn();
                chMtxUnlock();
            }
        }
    }

    return 0;
}


void tInit(int echo)
{
    sdStart(&TSD, NULL);

    tEcho = echo;
    chThdCreateStatic(waThdTerminal, sizeof(waThdTerminal), NORMALPRIO-1, ThdTerminal, NULL);
}


