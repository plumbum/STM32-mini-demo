#ifndef _THERMINAL_H_
#define _THERMINAL_H_

#include <ch.h>
#include <hal.h>

#ifndef EOLN
#define EOLN "\r\n"
#endif

/**
 * @file terminal.h
 * @brief Debug/configure command parser
 * @details
 *      
 * @addtogroup interface
 * @{
 */

/**
 * Init serial port hardware and run terminal thread
 */
void tInit(int echo);

/**
 * Put string direct to terminal
 * @param[in] str - output string
 */
void tPuts(const char* str);

/**
 * Put string direct to terminal. EOLN terminated
 * @param[in] str - output string
 */
void tPutsLn(const char* str);

/**
 * Get string terminated with '\\r' or '\\n' worm terminal
 * @param[out] str - output string
 * @param[in] maxlen - maximal length of input string
 */
int tGetLn(char* str, int maxlen);

/**
 * Direct put hex number to terminal
 */
void tPutHex(uint32_t val, int digits);

/**
 * Direct put unsigned integer decimal number to terminal
 */
void tPutui(uint32_t val);

/**
 * Direct put signed integer decimal number to terminal
 */
void tPuti(int32_t val);


unsigned int tParse(const char* ibuf, int lbuf, char** errorMsg);

/**
 * @}
 */

#endif /* _THERMINAL_H_ */

