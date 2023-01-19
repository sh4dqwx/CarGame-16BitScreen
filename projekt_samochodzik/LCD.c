#include <msp430x14x.h>
#include <string.h>
#include "portyLcd.h"
#include "LCD.h"

#define bitset(var, bitno) ((var) |= 1 << (bitno))
#define bitclr(var, bitno) ((var) &= ~(1 << (bitno)))

// void Delay(unsigned int a);
// void Delayx100us(unsigned char b);
// void _E(void);

void Delay(unsigned int a)
{
    int k;
    for (k = 0; k != a; ++k)
    {
        _NOP();
        _NOP();
        _NOP();
        _NOP();
    }
}

void Delayx100us(unsigned char b)
{
    int j;
    for (j = 0; j != b; ++j)
        Delay(_100us);
}

void clearDisplay()
{
    SEND_CMD(CLR_DISP);
    Delayx100us(10);
    SEND_CMD(DD_RAM_ADDR);
}

void gotoSecondLine()
{
    SEND_CMD(DD_RAM_ADDR2);
}

void _E(void)
{
    bitset(P2OUT, E); // toggle E for LCD
    Delay(_10us);
    bitclr(P2OUT, E);
}

void SEND_CHAR(unsigned char d)
{
    int temp;
    Delayx100us(5);  //.5ms
    temp = d & 0xf0; // get upper nibble
    LCD_Data &= 0x0f;
    LCD_Data |= temp;
    bitset(P2OUT, RS); // set LCD to data mode
    _E();              // toggle E for LCD
    temp = d & 0x0f;
    temp = temp << 4; // get down nibble
    LCD_Data &= 0x0f;
    LCD_Data |= temp;
    bitset(P2OUT, RS); // set LCD to data mode
    _E();              // toggle E for LCD
}

void SEND_TEXT(unsigned char text[])
{
    for (unsigned int i = 0; i < strlen(text); i++)
        SEND_CHAR(text[i]);
}

void SEND_NUMBER(int x)
{
    char c[5];
    unsigned char i = 0;
    while(x > 0)
    {
        c[i] = x%10 + '0';
        x/=10;
        i++;
    }
    for(int j = i-1; j >= 0; j--)
        SEND_CHAR(c[j]);
}

void MAKE_DEF_CHAR(unsigned char c[])
{
    for (unsigned char i = 0; i < 8; i++)
        SEND_CHAR(c[i]);
}


void SEND_CMD(unsigned char e)
{
    int temp;
    Delayx100us(10); // 10ms
    temp = e & 0xf0; // get upper nibble
    LCD_Data &= 0x0f;
    LCD_Data |= temp;  // send CMD to LCD
    bitclr(P2OUT, RS); // set LCD to CMD mode
    _E();              // toggle E for LCD
    temp = e & 0x0f;
    temp = temp << 4; // get down nibble
    LCD_Data &= 0x0f;
    LCD_Data |= temp;
    bitclr(P2OUT, RS); // set LCD to CMD mode
    _E();              // toggle E for LCD
}

void InitLCD(void)
{
    bitclr(P2OUT, RS);
    Delayx100us(250); // Delay 100ms
    Delayx100us(250);
    Delayx100us(250);
    Delayx100us(250);
    LCD_Data |= BIT4 | BIT5; // D7-D4 = 0011
    LCD_Data &= ~BIT6 & ~BIT7;
    _E();             // toggle E for LCD
    Delayx100us(100); // 10ms
    _E();             // toggle E for LCD
    Delayx100us(100); // 10ms
    _E();             // toggle E for LCD
    Delayx100us(100); // 10ms
    LCD_Data &= ~BIT4;
    _E(); // toggle E for LCD

    SEND_CMD(CG_RAM_ADDR);
    unsigned char chars[7][8] = {
        {31, 31, 31, 31, 0, 0, 0, 0},  // samochód góra (8)
        {0, 0, 0, 0, 31, 31, 31, 31},  // samochód dół (9)
        {15, 30, 30, 15, 0, 0, 0, 0},  // przeszkoda góra (10)
        {0, 0, 0, 0, 15, 30, 30, 15},  // przeszkoda dół (11)
        {7, 14, 30, 30, 30, 30, 14, 7}, // duża przeszkoda (12)
        {31, 31, 31, 31, 15, 30, 30, 15}, //samochod+przeszkoda (13)
        {15, 30, 30, 15, 31, 31, 31, 31} // przeszkoda+samochod (14)
    };
    MAKE_DEF_CHAR(chars[0]);
    MAKE_DEF_CHAR(chars[1]);
    MAKE_DEF_CHAR(chars[2]);
    MAKE_DEF_CHAR(chars[3]);
    MAKE_DEF_CHAR(chars[4]);
    MAKE_DEF_CHAR(chars[5]);
    MAKE_DEF_CHAR(chars[6]);
  
    SEND_CMD(DD_RAM_ADDR);
    SEND_CMD(DISP_ON);
    SEND_CMD(CLR_DISP);
    Delayx100us(250);
    Delayx100us(250);
    Delayx100us(250);
    Delayx100us(250);
}
