#include <msp430x14x.h>
#include "LCD.h"
#include "game.h"

#define B1 BIT4 &P4IN
#define B2 BIT5 &P4IN
#define B3 BIT6 &P4IN
#define B4 BIT7 &P4IN

//---------------- zmienne globalne ----------------
struct User
{
    char name[3];
    unsigned int result;
} typedef User;

unsigned int choice = 0;
User userList[10];
//-------------------- funkcje ---------------------

void showOptions()
{
    clearDisplay();
    SEND_TEXT("      MENU");
    SEND_TEXT("Sta ");
    SEND_TEXT("Wyn  ");
    SEND_TEXT("Utw");
}

void results()
{
}

void createUser()
{
}

void menu()
{
    while (1)
    {
        showOptions();

        if (B1 == 0)
            game();
        else if (B2 == 0)
            results();
        else if (B3 == 0)
            createUser();
    }
}