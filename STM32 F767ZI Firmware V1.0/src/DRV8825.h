/****************************************************************************************************************************
*****************************************************************************************************************************
Library orginaly design for the Pick and Place project at HelMo Gramme By Nicolas Rixhon - rixhon.nicolas@live.com
This library is subject to evolve this is an Alpha version

This library is design to control stepper motor with the DRV8825. It can be easily adapt to other controller.

Changelog :

Year            Version                               Change

2018            DRV8825 library V a0.1                Creation of the library


Actual version : DRV8825 library V a0.1 
********************************************************************************************************************************
********************************************************************************************************************************/

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
    *@param stepDelay = delay between step you want µs
    * 
    */
    void setDestination(int numStep, int resolution, int direction, int stepDelay);
    //************************************************************************************************************//
    /*To use in order to update the driver at each loop of the main
    * currentMillis is time in µ second you have with the main timer 
    */
    void update(unsigned long currentMillis);
    //************************************************************************************************************//
    // return true if the is not fisnihed
    bool moveComplete();

    //************************************************************************************************************//
    // Set the step resolution of the DRV8825
    void setResolution(int resolution);
    //************************************************************************************************************//
    //Enable the motor to move : 0 to move, 1 to stop
    void enable(int en);
    void freeMove(unsigned long currentMillis, int StepDelay, int direction);
    int stepToGo();
  private:
    DigitalOut _Enable, _M0, _M1, _M2, _STEP, _DIR;
    int _minStepDelay;
    int _numStep;
    int _stepDelay;
    unsigned long _Lastmillis;
    unsigned long _LastmillisStepCtrl;
    int _previousStepDelay;
    int _maxDtStep;
     int StepDelay_;

    void doStep();
};

#endif