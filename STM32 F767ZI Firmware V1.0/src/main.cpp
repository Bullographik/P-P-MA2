#include <mbed.h>
#include <math.h>
#include <DRV8825.h>
//#define DEBUG
//#define JOY

const int MANMODELEDTPSON = 200;
const int MANMODELEDTPSOFF = 800;
const int MinStepDelay = 170;  // µs
const int MaxStepDelay = 2000; //µs
const float JoyDeadZone = 0.05;

DigitalIn btnToggle(PC_6);
DigitalIn btnManCtrl(USER_BUTTON);
DigitalOut ledManCtrl(LED1);
PinName ENABLE_(D14), M0(D13), M1(D12), M2(D11), STEP(D10), DIRECTION(D9);
AnalogIn Yy(A0);
AnalogIn Xx(A1);
Serial pc(USBTX, USBRX);
DRV8825 Mot(ENABLE_, M0, M1, M2, STEP, DIRECTION, MinStepDelay);

#ifdef JOY
Timer Joy;
#endif
Timer LedTimer, btn, btnResTps;
Timer SerialCom, StepTimer;

bool manMode = false, LedTimertgl = false, LedTgl = false;
int RES;
int xDIR, yDIR;
int mvmtX, mvmtY;

int main()
{ //SETUP

    float X = 0;
    float Y = 0;
    int ToggleMode = 0;
    Mot.setDestination(200, 1, 1, 0);
    btn.start(); //Start the debounce timer for the buttons
    btnResTps.start();
    StepTimer.start();
    pc.baud(11520);
#ifdef JOY
    Joy.start();
#endif
    // LOOP

    while (1)
    {
#ifdef DEBUG
        pc.printf("Button value = %d \n", btnToggle.read());
#endif

        if (btnManCtrl.read() == 1 && btn.read_ms() >= 500) // Toggle Manual flag when btnToggle is on
        {
            btn.reset(); // Restet the debounce timer
            manMode = !manMode;
            ledManCtrl = 0;
            if (manMode)
            {
                pc.printf("Manual Mode Toggle \n");
                Mot.enable(0);
            }
            else
            {
                pc.printf("AutoMode \n");
                Mot.enable(1);
            }
        }
#ifdef DEBUG
        pc.printf("Time 1 is = %d \n", LedTimer.read_ms());
#endif
        if (LedTimertgl == false) // Start the Led timer if not.
        {
            LedTimer.start();
            LedTimertgl = true;
        }

        /****************************************         Manual Control          *****************************************************/

        if (manMode == true)
        {
            if (LedTimer.read_ms() >= MANMODELEDTPSON && LedTgl == false)
            {
#ifdef DEBUG
                pc.printf("LED OFF \n");
                wait_ms(500);
#endif
                ledManCtrl.write(0);
                LedTgl = true;
            }
            else if (LedTimer.read_ms() >= MANMODELEDTPSOFF && LedTgl == true)
            {
                ledManCtrl.write(1);
                LedTimer.reset();
                LedTgl = false;
#ifdef DEBUG
                pc.printf("LED ON \n");
                wait_ms(500);
#endif
            }
            X = 0.5 - round(Xx.read() * 10000) / 10000;  // Stabilize the data from the ADC
            Y = 0.52 - round(Yy.read() * 10000) / 10000; // Same
            mvmtX = (1 - abs(X) * 2) * (MaxStepDelay);   // Calcul the Tps between each step
            if (mvmtX < MinStepDelay)                    // If delay <800µs set to 500µs
                mvmtX = MinStepDelay;
            if (abs(X) <= JoyDeadZone) // Manage a stable 0
            {
                mvmtX = 0;
            }
            mvmtY = (1 - abs(Y) * 2) * (MaxStepDelay); // Same as before
            if (mvmtY < MinStepDelay)
                mvmtY = MinStepDelay;
            if (abs(Y) <= JoyDeadZone)
            {
                mvmtY = 0;
            }

            if (X > 0) // Manage the direction sent to the Driver
            {
                xDIR = 0;
            }
            else
            {
                xDIR = 1;
            }
            if (Y > 0)
            {
                yDIR = 0;
            }
            else
            {
                yDIR = 1;
            }

            // Resolution toggling when you push on the joy button
            if (btnToggle.read() == 1 && btnResTps.read_ms() >= 500)
            {
                if (ToggleMode >= 6)
                {
                    ToggleMode = 0;
                }
                ToggleMode++;
                btnResTps.reset();
            }
            switch (ToggleMode)
            {
            case 1:
                RES = 1;
                break;

            case 2:
                RES = 2;
                break;

            case 3:
                RES = 4;
                break;

            case 4:
                RES = 8;
                break;

            case 5:
                RES = 16;
                break;

            case 6:
                RES = 32;
                break;
            }

            //********************************************************************************
            //Motor Mouvement
            Mot.setResolution(RES);
            if (mvmtX != 0)
                Mot.freeMove(StepTimer.read_us(), mvmtX, xDIR);

#ifdef JOY
            if (Joy.read_ms() > 100)
            {
                pc.printf(" x = %d xDIR=%d       y = %d  yDIR=%d  ************** RES = %d \n", mvmtX, xDIR, mvmtY, yDIR, RES);
                Joy.reset();
            }
#endif
        }
        else
        {
           if (Mot.stepToGo() > 0)
            {
                 Mot.enable(0);
                Mot.update(StepTimer.read_us());
               // pc.printf("%d \n",Mot.stepToGo());
            }
            else    
            {
                //pc.printf("%d \n",Mot.stepToGo());
                Mot.enable(1);
                Mot.setDestination(6400, 32, 1, 200);
               // pc.printf("%d \n",Mot.stepToGo());
                wait(2);
            }
            
        }
    }
}