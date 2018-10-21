#include "DRV8825.h"

DRV8825::DRV8825(PinName Enable, PinName M0, PinName M1, PinName M2, PinName STEP, PinName DIR, int minStepDelay) : _Enable(Enable), _M0(M0), _M1(M1), _M2(M2), _STEP(STEP), _DIR(DIR)
{

    _minStepDelay = minStepDelay;
    _Enable = 1;
    _M0 = 0;
    _M1 = 0;
    _M2 = 0;
}

//***********************************************************************************
void DRV8825::setDestination(int numStep, int resolution, int direction, int stepDelay)
{
    _DIR = direction;
    _numStep = numStep * 2;
    setResolution(resolution);
    if (stepDelay < _minStepDelay)
        _stepDelay = _minStepDelay;
    else
        _stepDelay = stepDelay;
}

void DRV8825::setResolution(int _resolution)
{

    switch (_resolution)
    {
    case 1:
        _M0 = 0;
        _M1 = 0;
        _M2 = 0;
        break;
    case 2:
        _M0 = 1;
        _M1 = 0;
        _M2 = 0;
        break;
    case 4:
        _M0 = 0;
        _M1 = 1;
        _M2 = 0;
        break;
    case 8:
        _M0 = 1;
        _M1 = 1;
        _M2 = 0;
        break;
    case 16:
        _M0 = 0;
        _M1 = 0;
        _M2 = 1;
        break;
    case 32:
        _M0 = 1;
        _M1 = 0;
        _M2 = 1;
        break;
    }
}
//************************************************************************************
void DRV8825::update(int currentMillis){
if(currentMillis>=_Lastmillis+_stepDelay){
    doStep();
    _numStep--;
    _Lastmillis=currentMillis;
}
}

void DRV8825::doStep(){
_STEP= !_STEP;
}

//************************************************************************************