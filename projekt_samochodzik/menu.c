#include <msp430x14x.h>
#include "LCD.h"
#include "game.h"

#define B1 BIT4&P4IN
#define B2 BIT5&P4IN
#define B3 BIT6&P4IN
#define B4 BIT7&P4IN

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
    gotoSecondLine();
    SEND_TEXT("Sta ");
    SEND_TEXT("Wyn ");
    SEND_TEXT("Utw");
}

void results()
{
}

void createUser()
{
    /*char player1[3];
    int index = 0;
    int litera = 129; // Z-154
    while (1)
    {
        if (~B1)
        {
            litera++;
            if (litera == 154)
                litera = 129;
        }
        if (~B2)
            index++;
        for (int i = 0; i < 3; i++)
            char[index] = (char)litera;
    }*/
}

void menu()
{
    while (1)
    {
        showOptions();

        if ((B1) == 0)
            game();
        else if ((B2) == 0)
            results();
        else if ((B3) == 0)
            createUser();
        
        Delay(1000);
    }
}
