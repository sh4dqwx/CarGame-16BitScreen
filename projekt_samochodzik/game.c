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
unsigned int result = 0;
unsigned int screen[4][16];
unsigned int position = 0;
unsigned int pressed = 0;
unsigned int frame = 0;
unsigned int gameON = 0;
unsigned int screenChanged = 0;

//-------------------- funkcje ---------------------
void generateObstacle()
{
    int obstacles = 0;
    for (int i = 0; i < 4; i++)
    {
        int random = rand() % 2;
        screen[i][15] = random;
        if (random == 1)
            obstacles++;
    }
    if (obstacles == 4)
    {
        int random = rand() % 4;
        screen[random][15] = 0;
    }
}

void initScreen()
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
    generateObstacle();
}

void moveObstacle()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 1; j < 16; j++)
        {
            if (screen[i][j] == 2 || screen[i][j] == 0)
                continue;

            screen[i][j - 1] = screen[i][j];
            screen[i][j] = 0;
        }
    }
}

void refreshFrame()
{
    clearDisplay();
    for (int i = 0; i < 4; i += 2)
    {
        for (int j = 0; j < 16; j++)
        {
            if (screen[i][j] == 0 && screen[i + 1][j] == 0)
                SEND_CHAR(' ');
            else if (screen[i][j] == 0 && screen[i + 1][j] == 2)
                SEND_CHAR(carDown);
            else if (screen[i][j] == 2 && screen[i + 1][j] == 0)
                SEND_CHAR(carUp);
            else if (screen[i][j] == 0 && screen[i + 1][j] == 1)
                SEND_CHAR(obsDown);
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
    screenChanged = 1;
}

unsigned int game()
{
    result = 0;
    screenChanged = 0;
    initScreen();
    clearDisplay();
    refreshFrame();
    gameON = 1;
    while (1)
    {
        if (screenChanged == 1)
        {
            refreshFrame();
            screenChanged = 0;
        }

        if ((B1) == 0 && pressed == 0)
        {
            pressed = 1;
            carGoVrrr(-1);
        }
        else if ((B2) == 0 && pressed == 0)
        {
            pressed = 1;
            carGoVrrr(1);
        }
        else if ((B1) != 0 && (B2) != 0)
            pressed = 0;
    }

    return result;
}

//------------------- przerwania -------------------
void timer()
{
    if (gameON == 0)
        return;

    if (frame++ >= 2)
    {
        generateObstacle();
        frame = 0;
    }
    moveObstacle();
    screenChanged = 1;
}
