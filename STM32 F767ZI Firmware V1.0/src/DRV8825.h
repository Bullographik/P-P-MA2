
#ifndef MBED_DRV8825_H
#define MBED_DRV8825_H

#include "mbed.h"

class DRV8825
{
  public:
    //************************************************************************************************************//
    /* Initialisation of stepper motor controler.
    * @param pin Enable = Enable Pin of the DRV8825
    * @param pin M0 = M0 Pin of the DRV8825
    * @param pin M1 = M1 Pin of the DRV8825
    * @param pin M2 = M2 Pin of the DRV8825
    * @param pin STEP = STEP Pin of the DRV8825
    * @param pin DIR = DIR Pin of the DRV8825
    * @param minDelayStep is the minimum delay needed for the stepper motor to not unhook in milli-seconds
    */
    DRV8825(PinName Enable, PinName M0, PinName M1, PinName M2, PinName STEP, PinName DIR, int minStepDelay);

    //************************************************************************************************************//
    /* Set the number of step you want to do with the resolution define
    *@param numStep = number of step you want to do with the resolution you input. 
                Ex: 3200 step at a resolution of 32 is equal de 1000 step with a resolution of 1 
    *@param resolution = (1,2,4,8,16,32) it's equal to the step fraction you want to apply
    *@param direction = (0,1) it's direction you want to move
    *@param stepDelay = delay between step you want
    * 
    */
    void setDestination(int numStep, int resolution, int direction, int stepDelay);
    //************************************************************************************************************//
    /*To use in order to update the driver at each loop of the main
    * currentMillis is time in milli-seconds you have with the main timer 
    */
    void update(int currentMillis);
    //************************************************************************************************************//
    // return true if the is not fisnihed
    bool moveComplete();

    //************************************************************************************************************//
    // Set the step resolution of the DRV8825
    void setResolution(int resolution);
    //************************************************************************************************************//

  private:
    DigitalOut _Enable, _M0, _M1, _M2, _STEP, _DIR;
    int _minStepDelay;
    int _numStep;
    int _stepDelay;
    int _Lastmillis;
    

    void doStep();
};

#endif