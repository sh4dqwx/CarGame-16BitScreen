#include <msp430x14x.h>
#include "LCD.h"
#include "game.h"
#include "portyLcd.h"

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
unsigned int userCounter = 0;
User userList[10];
//-------------------- funkcje ---------------------

void showOptions()
{
    clearDisplay();
    SEND_TEXT("      MENU");
    gotoSecondLine();
    SEND_TEXT(" Start   Wyniki ");
}

void printUsers(int index)
{
    clearDisplay();
    for(int i = index ; (i < index + 2) && (i < userCounter); i++)
    {
        if(i == 1) gotoSecondLine();
        SEND_NUMBER(index + 1);
        SEND_CHAR(' ');
        SEND_TEXT(userList[i].name);
        SEND_TEXT(" - ");
        SEND_NUMBER(userList[i].result);
    }
}

void sort(unsigned char L, unsigned char R)
{
    if(R - L <= 1) return;
    unsigned char i=L, j=R, wyn;
    do
    {
        do i++ while(i<=R && userList[i].result < userList[L].result);
        do j-- while(userList[j].result > userList[L].result);
        if(i<j)
        {
            User tmp = userList[i];
            userList[i] = userList[j];
            userList[j] = tmp;
        }
    } while(i < j);

    User tmp = userList[L];
    userList[L] = userList[j];
    userList[j] = tmp;

    sort(L, j); sort(j+1, R);
}

void results()
{
    sort();
    printUsers(0);
    int chooseIndex = 0;
    unsigned int pressed = 1;
    while(1)
    {
        if ((B1) == 0 && pressed == 0)
        {
            pressed = 1;
            if (--chooseIndex < 0) chooseIndex = 0;
            printUsers(chooseIndex);
        }
        else if ((B2) == 0 && pressed == 0)
        {
            pressed = 1;
            if (++chooseIndex >= userCounter) chooseIndex = userCounter - 1;
            printUsers(chooseIndex);
        }
        else if ((B1) != 0 && (B2) != 0)
            pressed = 0;

        if ((B4) == 0) break;
    }

}

void createUser()
{
    if(userCounter >= 9)
    {
        clearDisplay();
        SEND_TEXT("BYKU, LIMIT!");
        while(1)
        {
          if ((B4) == 0) return;
        }
    }
    unsigned int pressed = 1;
    clearDisplay();
    char nick[3] = "A";
    unsigned int index = 0;
    SEND_TEXT("PODAJ NICK: ");
    while(1)
    {
        SEND_CMD(DD_RAM_ADDR + 12);
        SEND_TEXT(nick);

        if ((B1) == 0 && pressed == 0)
        {
            pressed = 1;
            if(++nick[index] > 'Z') nick[index] = 'A';

        }
        else if ((B2) == 0 && pressed == 0)
        {
            pressed = 1;
            if(--nick[index] < 'A') nick[index] = 'Z';
        }
        else if ((B3) == 0 && pressed == 0)
        {
            pressed = 1;
            if(++index > 2) break;
            nick[index] = 'A';
        }
        else if ((B1) != 0 && (B2) != 0 && (B3) != 0)
            pressed = 0;

        if ((B4) == 0) return;

        Delay(1000);
    }

    for(int i = 0; i < 3; i++)
        userList[userCounter].name[i] = nick[i];

    userCounter++;
}

void initGame()
{
    if(userCounter == 0)
    {
        createUser();
        if(userCounter == 0)
            return;

        userList[0].result = game();
        return;
    }

    clearDisplay();
    SEND_TEXT("WYBIERZ GRACZA");
    unsigned int pressed = 1;
    int chooseIndex = 0;
    gotoSecondLine();
    SEND_TEXT(userList[chooseIndex].name);
    while(1)
    {
        if ((B1) == 0 && pressed == 0)
        {
            pressed = 1;
            clearDisplay();
            SEND_TEXT("WYBIERZ GRACZA");
            gotoSecondLine();
            if (++chooseIndex > userCounter) chooseIndex = 0;
            if(chooseIndex == userCounter)
            {
                SEND_TEXT("NOWY GRACZ");
            }
            SEND_TEXT(userList[chooseIndex].name);
        }
        if ((B2) == 0 && pressed == 0)
        {
            pressed = 1;
            clearDisplay();
            SEND_TEXT("WYBIERZ GRACZA");
            gotoSecondLine();
            if (--chooseIndex < 0) chooseIndex = userCounter;
            if(chooseIndex == userCounter)
            {
                SEND_TEXT("NOWY GRACZ");
            }
            SEND_TEXT(userList[chooseIndex].name);
        }
        else if ((B1) != 0 && (B2) != 0)
            pressed = 0;

        if ((B3) == 0)
            break;

        Delay(1000);
    }
    if (chooseIndex == userCounter) createUser();
    userList[chooseIndex].result = game();
}

void menu()
{
    unsigned int pressed = 1;
    showOptions();
    while (1)
    {
        if ((B1) == 0 && pressed == 0)
        {
            pressed = 1;
            initGame();
            showOptions();
        }
        else if ((B2) == 0  && pressed == 0)
        {
            pressed = 1;
            results();
            showOptions();
        }
        else if((B1) != 0 && (B2) != 0)
            pressed = 0;

        Delay(1000);
    }
}
