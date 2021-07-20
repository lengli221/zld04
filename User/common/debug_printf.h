#ifndef __DEBUG_PRINTF_H_
#define __DEBUG_PRINTF_H_

#include <stdio.h>

/* 
** retarget the C library printf function to the USART 
*/
int fputc(int ch, FILE *f);

#endif

