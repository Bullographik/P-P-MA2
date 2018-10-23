#include <mbed.h>
#include <math.h>

//#define DEBUG
#define JOY

const int MANMODELEDTPSON = 200;
const int MANMODELEDTPSOFF = 800;
const int MinStepDelay = 500;   // µs
const int MaxStepDelay = 5000; //µs
const float JoyDeadZone = 0.01;

DigitalIn btnToggle(PC_6);
DigitalIn btnManCtrl(USER_BUTTON);
DigitalOut ledManCtrl(LED1);
AnalogIn Yy(A0);
AnalogIn Xx(A1);
Serial pc(USBTX, USBRX);

#ifdef JOY
Timer Joy;
#endif
Timer LedTimer, btn;
Timer SerialCom;

bool manMode = false, LedTimertgl = false, LedTgl = false;

int main()
{ //SETUP

    float X = 0;
    float Y = 0;
    float mvmtX, mvmtY;
    int xDIR,yDIR;



    btn.start(); //Start the debounce timer for the buttons
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

        if (btnManCtrl.read() == 1 && btn.read() >= 1) // Toggle Manual flag when btnToggle is on
        {
            btn.reset(); // Restet the debounce timer
            manMode = !manMode;
            ledManCtrl=0;
            if (manMode)
                pc.printf("Manual Mode Toggle \n");
            else
                pc.printf("AutoMode \n");
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
            X = 0.5 - round(Xx.read() * 10000) / 10000; // Stabilize the data from the ADC
            Y = 0.52 - round(Yy.read() * 10000) / 10000; // Same
            mvmtX = (1 - abs(X) * 2) * (MaxStepDelay); // Calcul the Tps between each step
            if (mvmtX < 800) // If delay <800µs set to 500µs
                mvmtX = MinStepDelay;
            if (abs(X) <= 0.01)// Manage a stable 0
            {
                mvmtX = 0;
            }
            mvmtY = (1 - abs(Y) * 2) * (MaxStepDelay); // Same as before
            if (mvmtY < 800)
                mvmtY = MinStepDelay;
            if (abs(Y) <= 0.01)
            {
                mvmtY = 0;
            }


            if(X>0) // Manage the direction sent to the Driver
            {
                xDIR=0;
            }
            else
            {
                xDIR=1;
            }
            if(Y>0)
            {
                yDIR=0;
            }
            else
            {
                yDIR=1;
            }

            // Resolution manage when you push on the joy button
            if()

            #ifdef JOY
            if (Joy.read_ms() > 100)
            {
                pc.printf(" x = %f xDIR=%d       y = %f  yDIR=%d \n", mvmtX,xDIR ,mvmtY, yDIR);
                Joy.reset();
            }
#endif
        }
        //else{}
    }
}