#include <msp430x14x.h>
#include <stdlib.h>
#include <time.h>
#include "LCD.h"
#include "portyLcd.h"

#define carUp 8
#define carDown 9
#define obsUp 10
#define obsDown 11
#define obsBig 12
#define carObs 13
#define obsCar 14
#define life 15

#define B1 BIT4&P4IN
#define B2 BIT5&P4IN
#define B3 BIT6&P4IN
#define B4 BIT7&P4IN

//---------------- zmienne globalne ----------------
unsigned int result = 0;
unsigned int screen[4][16];
unsigned int frame = 0;
unsigned int gameON = 0;
unsigned int screenChanged = 0;
unsigned int timeCounter = 0;
unsigned int frameCounter = 0;
unsigned int speed = 0;
unsigned int lives = 0;
int position = 0;

//-------------------- funkcje ---------------------
void generateObstacle()
{
    int obstacles = 0;
    for (int i = 0; i < 4; i++)
    {
        int random = rand() % 2;
        screen[i][14] = random;
        if (random == 1)
            obstacles++;
    }
    if (obstacles == 4)
    {
        int random = rand() % 4;
        screen[random][14] = 0;
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
}

void moveObstacle()
{
    for (int i = 0; i < 4; i++)
    {
        screen[i][0] = 0;
        for (int j = 1; j < 16; j++)
        {
            if (screen[i][j] != 1)
                continue;
            if (screen[i][j - 1] == 2)
            {
                if(lives-- == 0) gameON = 0;
                return;
            }

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
        for (int j = 0; j < 15; j++)
        {
            if (screen[i][j] == 0 && screen[i + 1][j] == 0)
                SEND_CHAR(' ');
            else if (screen[i][j] == 0 && screen[i + 1][j] == 2)
                SEND_CHAR(carDown);
            else if (screen[i][j] == 2 && screen[i + 1][j] == 0)
                SEND_CHAR(carUp);
            else if (screen[i][j] == 2 && screen[i + 1][j] == 1)
                SEND_CHAR(carObs);
            else if (screen[i][j] == 1 && screen[i + 1][j] == 2)
                SEND_CHAR(obsCar);
            else if (screen[i][j] == 0 && screen[i + 1][j] == 1)
                SEND_CHAR(obsDown);
            else if (screen[i][j] == 1 && screen[i + 1][j] == 0)
                SEND_CHAR(obsUp);
            else if (screen[i][j] == 1 && screen[i + 1][j] == 1)
                SEND_CHAR(obsBig);
        }

        if(i == 0 && lives == 2) SEND_CHAR(life);
        if(i == 2 && lives != 0) SEND_CHAR(life);

        gotoSecondLine();
    }
}

void moveCar(int direction)
{
    if (position + direction < 0 || position + direction > 3)
        return;
    screen[position][2] = 0;
    position += direction;
    if(screen[position][2] == 1)
    {
        if(lives-- == 0) gameON = 0;
        return;
    }
    screen[position][2] = 2;
    screenChanged = 1;
}

void gameOver()
{
    clearDisplay();
    SEND_TEXT("GAME OVER");
    gotoSecondLine();
    SEND_TEXT("PUNKTY: ");
    SEND_NUMBER(result);
    while(1)
    {
      if ((B4) == 0) break;
    }
}

unsigned int game()
{
    unsigned int pressed = 1;
    result = 0;
    frame = 2;
    initScreen();
    refreshFrame();
    timeCounter = 0;
    speed = 800;
    lives = 2;
    gameON = 1;
    screenChanged = 0;
    while (1)
    {
        if(screenChanged == 1)
        {
            refreshFrame();
            screenChanged = 0;
        }

        if ((B1) == 0 && pressed == 0)
        {
            pressed = 1;
            moveCar(-1);
        }
        else if ((B2) == 0 && pressed == 0)
        {
            pressed = 1;
            moveCar(1);
        }
        else if ((B1) != 0 && (B2) != 0)
            pressed = 0;

        if(gameON == 0) break;

        Delay(1000);
    }
    gameOver();
    return result;
}

//------------------- przerwania -------------------
void timer()
{
    if (gameON == 0) return;

    if(timeCounter++ > 1000)
    {
        timeCounter = 0;
        if(speed > 300) speed-=10;
        result++;
    }

    if (frameCounter++ <= speed) return;
    frameCounter = 0;
    moveObstacle();
    if (frame++ >= 2)
    {
        generateObstacle();
        frame = 0;
    }
    screenChanged = 1;
}
