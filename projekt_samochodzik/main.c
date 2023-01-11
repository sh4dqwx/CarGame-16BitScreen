#include <msp430x14x.h>
#include "lcd.h"
#include "portyLcd.h"
#include "menu.h"
#include "game.h"

//---------------- zmienne globalne ----------------
unsigned int i = 0;

void InitClock(void)
{
  BCSCTL1 |= XTS;

  do
  {
    IFG1 &= ~OFIFG;
    for (i = 0xFF; i > 0; i--)
      ;
  } while ((IFG1 & OFIFG) == OFIFG);

  BCSCTL1 |= DIVA_1;
  BCSCTL2 |= SELM0 | SELM1;

  TACTL = TASSEL_1 + MC_1 + ID_3;
  CCTL0 = CCIE;
  CCR0 = 50000;

  _EINT();
}

//------------------ main program ------------------
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;

  InitPortsLcd();
  InitLCD();
  clearDisplay();
  InitClock();

  menu();
}

//------------------- przerwania -------------------
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A(void)
{
  timer();
}