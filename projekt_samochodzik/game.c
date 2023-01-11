#include <msp430x14x.h>
#include <stdlib.h>
#include <time.h>
#include "LCD.h"

#define carUp 8
#define carDown 9
#define obsUp 10
#define obsDown 11
#define obsBig 12

#define B1 BIT4 &P4IN
#define B2 BIT5 &P4IN

//---------------- zmienne globalne ----------------
unsigned int time = 0;
unsigned int result = 0;
unsigned int screen[4][16];
unsigned int position = 0;

//-------------------- funkcje ---------------------
void clearScreen();
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            screen[i][j] = 0;
        }
    }
    screen[1][2] = 2;
    position = 1;
}

void generateObstacle()
{
    int obstacles = 0;
    for (int i = 0; i < 4; i++)
    {
        int random = rand() % 2;
        screen[i][16] = random;
        if (random == 1)
            obstacles++;
    }
    if (obstacles == 4)
    {
        int random = rand() % 4;
        screen[random] = 0;
    }
}

void moveObstacle()
{
}

void refreshFrame()
{
    for (int i = 0; i < 4; i += 2)
    {
        for (int j = 0; j < 16; j++)
        {
            if (screen[i][j] == 0 && screen[i + 1][j] == 0)
                SEND_CHAR(" ");
            else if (screen[i][j] == 0 && screen[i + 1][j] == 1)
                SEND_CHAR(obsDown);
            else if (screen[i][j] == 0 && screen[i + 1][j] == 2)
                SEND_CHAR(carDown);
            else if (screen[i][j] == 2 && screen[i + 1][j] == 0)
                SEND_CHAR(carUp);
            else if (screen[i][j] == 1 && screen[i + 1][j] == 0)
                SEND_CHAR(obsUp);
            else if (screen[i][j] == 1 && screen[i + 1][j] == 1)
                SEND_CHAR(obsBig);
        }
        gotoSecondLine();
    }
}

void carGoVrrr(int direction)
{
    if (position + direction < 0 || position + direction > 3)
        return;
    screen[position][2] = 0;
    position += direction;
    screen[position][2] = 2;
}

unsigned int game()
{
    result = 0;
    clearScreen();
    while (1)
    {
        generateObstacle();
        refreshFrame();
        if (B1 == 0)
            carGoVrrr(-1);
        else if (B2 == 0)
            carGoVrrr(1);
    }

    return result;
}

//------------------- przerwania -------------------
void timer()
{
    if (++time > 60)
        time = 0;
}