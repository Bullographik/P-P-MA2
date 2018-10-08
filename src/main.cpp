#include "mbed.h"

DigitalOut Enable_(D3);
DigitalOut M0(D4);
DigitalOut M1(D5);
DigitalOut M2(D6);
DigitalOut STEP(D7);
DigitalOut Dir(D8);
DigitalIn btn(D12);
DigitalOut LED(LED3);

void StepMode(int mode);

int main()
{
    int i, j;

    while (1)
    {
        if(btn == 1)
        {
        int mode = 1;
        Enable_=1;
        for (i = 0; i <= 8; i++)
        {
            LED=1;
            Dir = 0;
            for (j = 0; j <= 200; j++)
            {
                STEP=1;
                wait_us(20);
                STEP=0;
            }
            Dir = 1;
            for (j = 0; j <= 200; j++)
            {
                STEP=1;
                wait_us(200);
                STEP=0;
            }
            mode = mode * 2;
            LED=0;
            wait(3);
        }
        Enable_=0;
        LED=0;
        }

    }
}

void Set_StepMode(int mode)
{
    switch (mode)
    {
    case 1:
        M0 = 0;
        M1 = 0;
        M2 = 0;
        break;
    case 2:
        M0 = 1;
        M1 = 0;
        M2 = 0;
        break;
    case 4:
        M0 = 0;
        M1 = 1;
        M2 = 0;
        break;
    case 8:
        M0 = 1;
        M1 = 1;
        M2 = 0;
        break;
    case 16:
        M0 = 0;
        M1 = 0;
        M2 = 1;
        break;
    case 32:
        M0 = 1;
        M1 = 0;
        M2 = 1;
        break;
    }
}