#include <mbed.h>

DigitalIn btn(PC_6);
AnalogIn Xx(A0);
AnalogIn Yy(A1);
Serial pc(USBTX, USBRX);

int main()
{
    float X = 0;
    float Y = 0;
    // put your setup code here, to run once:
    pc.baud(11520);
    while (1)
    {
        X = Xx.read();
        Y = Yy.read();
        pc.printf(" x = %f       y = %f \n", X, Y);
        wait_ms(100);
        // put your main code here, to run repeatedly:
    }
}