#include <msp430x14x.h>
#include "LCD.h"
#include "game.h"

//---------------- zmienne globalne ----------------
unsigned int choice = 0;

//-------------------- funkcje ---------------------
void menu()
{
    clearDisplay();
    SEND_TEXT("      MENU");
    SEND_TEXT("Sta ");
    SEND_TEXT("Wyn  ");
    SEND_TEXT("Wyb ");
    SEND_TEXT("Utw");
}