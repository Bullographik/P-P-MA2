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
    btn.start();
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
            btn.reset();
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
        if (LedTimertgl == false) // Start the timer if not.
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
            X = 0.5 - round(Xx.read() * 10000) / 10000;
            Y = 0.52 - round(Yy.read() * 10000) / 10000;
            mvmtX = (1 - abs(X) * 2) * (MaxStepDelay);
            if (mvmtX < 800)
                mvmtX = 500;
            if (abs(X) <= 0.01)
            {
                mvmtX = 0;
            }
            mvmtY = (1 - abs(Y) * 2) * (MaxStepDelay);
            if (mvmtY < 800)
                mvmtY = 500;
            if (abs(Y) <= 0.05)
            {
                mvmtY = 0;
            }
#ifdef JOY
            if (Joy.read_ms() > 100)
            {
                pc.printf(" x = %f       y = %f \n", mvmtX, mvmtY);
                Joy.reset();
            }

#endif
        }
        //else{}
    }
}