#include <msp430x14x.h>
#include <stdlib.h>
#include <time.h>
#include "LCD.h"

// #define znak_sam

//---------------- zmienne globalne ----------------
unsigned int time = 0;
unsigned int result = 0;
unsigned int screen[4][16];

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
    screen[1][1] = 2;
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

void refreshFrame()
{
}

unsigned int game()
{
    result = 0;
    clearScreen();
    while (1)
    {
        generateObstacle();
        refreshFrame();
    }

    return result;
}

//------------------- przerwania -------------------
void timer()
{
    if (++time > 60)
        time = 0;
}