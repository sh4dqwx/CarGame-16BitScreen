#include <msp430x14x.h>
#include <stdlib.h>
#include <time.h>
#include "LCD.h"
#include "portyLcd.h"

/**
 * \brief Zdefiniowanie zmiennej carUp na liczbe 8
 * Znak, ktory przechowuje samochodz na gornej czesci linii
 */
#define carUp 8
/**
 * \brief Zdefiniowanie zmiennej carDown na liczbe 9
 * Znak, ktory przechowuje samochod na dolnej czesci linii
 */
#define carDown 9
/**
 * \brief Zdefiniowanie zmiennej obsUp na liczbe 10
 * Znak, ktory przechowuje przeszkode na gornej czesci linii
 */
#define obsUp 10
/**
 * \brief Zdefiniowanie zmiennej obsDown na liczbe 11
 * Znak, ktory przechowuje przeszkode na dolnej czesci linii
 */
#define obsDown 11
/**
 * \brief Zdefiniowanie zmiennej obsBig na liczbe 12
 * Znak, ktory przechowuje przeszkode na calej szerokosci inii
 */
#define obsBig 12
/**
 * \brief Zdefiniowanie zmiennej carObs na liczbe 13
 * Znak, ktory przechowuje samochod na gornej czesci linii, a na dolnej czesci linii znajduje sie przeszkoda
 */
#define carObs 13
/**
 * \brief Zdefniowanie zmiennej obsCar na liczbe 14
 * Znak, ktory przechowuje przeszkode na gornej czesci linii, a na dolnej czesci linii znajduje sie samochod
 */
#define obsCar 14
/**
 * \brief Zdefiniowanie zmiennej Life na liczbe 15
 * Znak, ktory bedzie sie wyswietlał serce na cala linie
 */
#define life 15

/**
 * \brief Definiowanie przycisku B1
 *  B1 oznacza skrajny lewy przycisk
 */

#define B1 BIT4&P4IN
/**
 * \brief Definiowanie przycisku B2
 *  B2 oznacza kolejno drugi z lewej strony przycisk
 */
#define B2 BIT5&P4IN
/**
 * \brief Definiowanie przycisku B3
 *  B3 oznacza koljeno drugi z prawej strony przycisk
 */
#define B3 BIT6&P4IN
/**
 * \brief Definiowanie przycisku B4
 *  B5 oznacza skrajny prawy przycisk
 */
#define B4 BIT7&P4IN

//---------------- zmienne globalne ----------------
/**
 * \brief Definiowanie wyniku koncowego result
 */
unsigned int result = 0;
/**
 * \brief Definiowanie dwuwymiarowej tablicy wyswietlacza
 */
unsigned int screen[4][16];
/**
 * \brief Definiowanie zmiennej, ktora odlicza liczbe klatek do wygenerowania przeszkod
 */
unsigned int frame = 0;
/**
 * \brief Definiowanie zmiennej, ktora informuje czy gra jest właczona czy nie 
 */
unsigned int gameON = 0;
/**
 * \brief Definiowanie zmiennej ktora informuje czy nalezy odswiezyc ekran
 */
unsigned int screenChanged = 0;
/**
 * \brief Definiowanie zmiennej ktora odlicza sekunde aby wywolac konkretne funkcje
 */
unsigned int timeCounter = 0;
/**
 * \brief Definiowanie zmiennej odliczajaca czas do kolejnego przesuniecia ekranu
 */
unsigned int frameCounter = 0;
/**
 * \brief Definiowanie zmiennej predkosc gry
 */
unsigned int speed = 0;
/**
 * \brief Definiowanie zmiennej na ktora jest przypisana liczba zyc 
 */
unsigned int lives = 0;
/**
 * \brief Definiowanie zmiennej na ktora jest przypisana aktualna pozycja
 */
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
