#include "mbed.h"

DigitalOut Enable_(D3);
DigitalOut M0(D4);
DigitalOut M1(D5);
DigitalOut M2(D6);
DigitalOut STEP(D7);
DigitalOut Dir(D8);
DigitalIn btn(D12);
DigitalOut LED(LED3);
int MaxTurn = 200;
int StepDelay = 500;
void Set_StepMode(int mode);

int main()
{
    int i, j;
Enable_=1;
    while (1)
    {
        if(btn == 1)
        {
        int mode = 1;
        Enable_=0;
        for (i = 0; i <= 8; i++)
        {
            wait(3);
            LED=1;
            Dir = 0;
            Set_StepMode(mode);
            for (j = 0; j <= MaxTurn; j++)
            {
                STEP=1;
                wait_us(StepDelay);
                STEP=0;
                wait_us(StepDelay);
            }
            Dir = 1;
            wait_ms(500);
            for (j = 0; j <= MaxTurn; j++)
            {
                STEP=1;
                wait_us(StepDelay);
                STEP=0;
                wait_us(StepDelay);
            }
            mode = mode * 2;
            LED=0;
            
        }
        Enable_=1;
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