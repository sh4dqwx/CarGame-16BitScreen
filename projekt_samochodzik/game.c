#include <msp430x14x.h>
#include "LCD.h"

//---------------- zmienne globalne ----------------
unsigned int result = 0;
unsigned int time = 0;

//-------------------- funkcje ---------------------
unsigned int game()
{
    return result;
}

//------------------- przerwania -------------------
void timer()
{
    if (++time > 60)
        time = 0;
}