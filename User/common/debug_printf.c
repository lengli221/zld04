#include "includes.h"

/* 
** retarget the C library printf function to the USART 
*/
int fputc(int ch, FILE *f){
    usart_data_transmit(USART2, (uint8_t)ch);
    while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
    return ch;
}


